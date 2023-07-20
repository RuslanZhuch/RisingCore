import generator
import executors
import contexts
import loader

def _to_double_camel_case(text):
    s = text.replace("-", " ").replace("_", " ")
    s = s.split()
    if len(text) <= 1:
        return text
    if len(s) <= 1:
        return text[0].capitalize() + text[1:]
    out = ''.join(i.capitalize() for i in s[:])
    return out

def _to_class_name(name):
    return _to_double_camel_case(name)

def generate_runtime_file(folder):
    return generator.generate_file(folder, 'runtime.cpp')

def _get_validated_shared_context_instaces(workspace_shared_contexts_file, contexts_data) -> list[contexts.ContextUsage]:
    instances_data = contexts.load_shared_context_instances(workspace_shared_contexts_file)
    validated_instances = contexts.get_validated_context_instances(contexts_data, instances_data)
    return validated_instances

def _generate_commands(handler):
    def cycle_body(handler):
        def exit_body(handler):
            generator.generate_line(handler, "return 0;")
        generator.generate_block(handler, "if (Dod::BufferUtils::get(sApplicationContext.context.commands, 0) == 1)", exit_body)
        
    generator.generate_block(handler, "for (int32_t cmdId{}; cmdId < Dod::BufferUtils::getNumFilledElements(sApplicationContext.context.commands); ++cmdId)", cycle_body)

def _generate_contexts_includes(handler, shared_context_instances : list[contexts.ContextUsage]):
    context_names = [instance.context_name for instance in shared_context_instances]
    unique_instances = list(set(context_names))
    unique_instances.sort()
    
    for instance in unique_instances:
        generator.generate_line(handler, "#include <Contexts/{}Context.h>".format(_to_class_name(instance)))
        
    if len(unique_instances) > 0:
        handler.newline(1)
        
def generate(target_path, executors_data, workspace_shared_contexts_file, loaded_contexts_data):
    
    workspace_context_data = loader.load_application_context_data(workspace_shared_contexts_file)
    handler = generate_runtime_file(target_path)
    
    validated_shared_context_instances = _get_validated_shared_context_instaces(workspace_shared_contexts_file, loaded_contexts_data)
    
    _generate_contexts_includes(handler, validated_shared_context_instances)
    executors.gen_headers(handler, executors_data)
    handler.newline(1)
    
    generator.generate_line(handler, "#include <dod/SharedContext.h>")
    generator.generate_line(handler, "#include <dod/BufferUtils.h>")
    generator.generate_line(handler, "#include <chrono>")
    handler.newline(1)
    
    def fill_function(self, handler):
        contexts.generate_shared_init(handler, validated_shared_context_instances)
        handler.newline(1)
        
        executors.gen_inits(handler, executors_data, workspace_context_data)
    
        def cycle_function(handler):
            generator.generate_line(handler, "const auto start{ std::chrono::high_resolution_clock::now() };")
            handler.newline(1)
            
            executors.gen_updates(handler, executors_data)
            handler.newline(1)
            
            contexts.generate_shared_flush(handler, workspace_context_data)
            handler.newline(1)
            
            contexts.generate_shared_merge(handler, workspace_context_data)
            handler.newline(1)
            
            executors.gen_flush(handler, executors_data)
            handler.newline(1)
        
            _generate_commands(handler)
            handler.newline(1)
            
            generator.generate_line(handler, "const auto end{ std::chrono::high_resolution_clock::now() };")
            generator.generate_line(handler, "deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;")
        
        handler.newline(1)
        
        generator.generate_variable(handler, "float", "deltaTime", 0)
        generator.generate_block(handler, "while(true)", cycle_function)
        
    generator.generate_function(handler, "main", fill_function, "int")
    handler.close()
    