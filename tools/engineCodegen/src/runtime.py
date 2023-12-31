import generator
import executors
import contexts
import loader

class ContextUsage:
    def __init__(self, executor_context_name : str, shared_context_instance_name : str):
        self.executor_context_name = executor_context_name
        self.shared_context_instance_name = shared_context_instance_name

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
        generator.generate_block(handler, "if (Dod::DataUtils::get(sApplicationContext.commands, 0) == 1)", exit_body)
        
    generator.generate_block(handler, "for (int32_t cmdId{}; cmdId < Dod::DataUtils::getNumFilledElements(sApplicationContext.commands); ++cmdId)", cycle_body)

def _generate_contexts_includes(handler, shared_context_instances : list[contexts.ContextUsage]):
    context_names = [instance.context_name for instance in shared_context_instances]
    unique_instances = list(set(context_names))
    unique_instances.sort()
    
    for instance in unique_instances:
        generator.generate_line(handler, "#include <Contexts/{}Context.h>".format(_to_class_name(instance)))
        
    if len(unique_instances) > 0:
        handler.newline(1)
        
def gather_shared_usage_for_executor(runtime_data : dict[any], executor_name : str):
    usage_list : list[ContextUsage] = []


    def populate_usage(usage_list, data_source : str):
        contexts_usage = runtime_data.get(data_source)
        if contexts_usage is None:
            return
        for usage in contexts_usage:
            if usage["executorName"] != executor_name:
                continue
            usage_list.append(ContextUsage(usage["executorSharedName"], usage["instanceName"]))        

    usage_list = []
    populate_usage(usage_list, "sharedContextsUsage")
    populate_usage(usage_list, "poolContextsUsage")

    return usage_list

def _generate_executors_update(handler, runtime_data : dict[any], executors_data: list, context_data : list[contexts.ContextUsage]):
    pool_contexts_instances = runtime_data.get("poolContextsInstances")
    
    schema_data = runtime_data.get("schema")
    if schema_data is None:
        usage_context_names = list[str]()
        for executor_data in executors_data:
            usage_list_raw = gather_shared_usage_for_executor(runtime_data, executor_data["name"])
            for usage in usage_list_raw:
                if usage.shared_context_instance_name not in usage_context_names:
                    usage_context_names.append(usage.shared_context_instance_name)

        contexts.generate_shared_usage(handler, context_data, 1, list(usage_context_names))
        generator.generate_empty(handler)
        for data in executors_data:

            usage_list_raw = gather_shared_usage_for_executor(runtime_data, data["name"])
            usage_list = [ executors.SharedUsage(usage.shared_context_instance_name, usage.executor_context_name) for usage in usage_list_raw ]
            executors.gen_shared_setup(handler, data, usage_list, 1)
            executors.gen_updates(handler, [data])
        handler.newline(1)
        return
    
    pool_id = 1
    for block_id, block_data in enumerate(schema_data):
        executors_in_block = block_data.get("executors")
        if executors_in_block is None:
            continue

        contexts.generate_pools_merge(handler, runtime_data, block_id)
        handler.newline(1)

        usage_context_names = list[str]()
        for executor_in_block in executors_in_block:
            for executor_data in executors_data:
                if executor_data["name"] == executor_in_block:
                    usage_list_raw = gather_shared_usage_for_executor(runtime_data, executor_in_block)
                    for usage in usage_list_raw:
                        if usage.shared_context_instance_name not in usage_context_names:
                            usage_context_names.append(usage.shared_context_instance_name)
                    break
        contexts.generate_shared_usage(handler, context_data, pool_id, list(usage_context_names))
        generator.generate_empty(handler)

        for executor_in_block in executors_in_block:
            for executor_data in executors_data:
                if executor_data["name"] == executor_in_block:
                    usage_list_raw = gather_shared_usage_for_executor(runtime_data, executor_in_block)
                    usage_list = [ executors.SharedUsage(usage.shared_context_instance_name, usage.executor_context_name) for usage in usage_list_raw ]
                    executors.gen_shared_setup(handler, executor_data, usage_list, pool_id)
                    executors.gen_updates(handler, [executor_data])
                    break

        handler.newline(1)
        pool_id += 1

def generate(target_path, executors_data, workspace_shared_contexts_file, loaded_contexts_data):
    
    workspace_context_data = loader.load_runtime_data(workspace_shared_contexts_file)
    handler = generate_runtime_file(target_path)
    
    validated_shared_context_instances = _get_validated_shared_context_instaces(workspace_shared_contexts_file, loaded_contexts_data)
    
    _generate_contexts_includes(handler, validated_shared_context_instances)
    executors.gen_headers(handler, executors_data)
    handler.newline(1)
    
    generator.generate_line(handler, "#include <dod/SharedContext.h>")
    generator.generate_line(handler, "#include <chrono>")
    handler.newline(1)
    
    def fill_function(self, handler):
        contexts.generate_shared_init(handler, validated_shared_context_instances)
        handler.newline(1)
        
        executors.gen_inits(handler, executors_data, workspace_context_data)
    
        def cycle_function(handler):
            generator.generate_line(handler, "const auto start{ std::chrono::high_resolution_clock::now() };")
            handler.newline(1)
            
            _generate_executors_update(handler, workspace_context_data, executors_data, validated_shared_context_instances)
            
            contexts.generate_pools_flush(handler, workspace_context_data)
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
    