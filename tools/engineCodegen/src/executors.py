import loader
import generator
import injectors

class SharedUsage:
    def __init__(self, shared_instance, executor_scontext):
        self.shared_instance = shared_instance
        self.executor_scontext = executor_scontext

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

def load(paths : list):
    return loader.load_descriptors(paths)

def get_name(data):
    return data.get("name")

def load_shared_context_usage(workspace_data):
    output = dict()
    
    usage = workspace_data.get("sharedContextsUsage")
    if usage is None:
        return output
    
    for element in usage:
        executor_name = element["executorName"]
        shared_instance = element["instanceName"]
        executor_scontext = element["executorSharedName"]
        if output.get(executor_name) is None:
            output[executor_name] = []    
        output[executor_name].append(SharedUsage(shared_instance, executor_scontext))
        
    return output

def load_pool_context_usage(workspace_data):
    output = dict()
    
    usage = workspace_data.get("poolContextsUsage")
    if usage is None:
        return output
    
    for element in usage:
        executor_name = element["executorName"]
        shared_instance = element["instanceName"]
        executor_scontext = element["executorSharedName"]
        if output.get(executor_name) is None:
            output[executor_name] = []    
        output[executor_name].append(SharedUsage(shared_instance, executor_scontext))
        
    return output

def gen_shared_context_init(handler, executor_data, workspace_data):
    usage_full_data = load_shared_context_usage(workspace_data)
    pool_context_usage = load_pool_context_usage(workspace_data)
    if pool_context_usage is not None:
        usage_full_data.update(pool_context_usage)
    
    executor_name = get_name(executor_data)
    
    usage_data = usage_full_data.get(executor_name)
    if usage_data is None:
        return
    
    for usage in usage_data:
        generator.generate_line(handler, "{}.{}Context = &{}Context;".format(executor_name, usage.executor_scontext, usage.shared_instance))

def gen_headers(handler, executors_data):
    for data in executors_data:
        name = _to_class_name(get_name(data))
        generator.generate_line(handler, "#include <executors/{}Executor.h>".format(name))

def gen_inits(handler, executors_data, workspace_data):
    for data in executors_data:
        name = get_name(data)
        class_name = "Game::ExecutionBlock::" + _to_class_name(name)
        generator.generate_variable(handler, class_name, name)
        generator.generate_line(handler, name + ".loadContext();")
        #gen_shared_context_init(handler, data, workspace_data)
        generator.generate_line(handler, name + ".initiate();")
        
def gen_updates(handler, executors_data):
    for data in executors_data:
        name = get_name(data)
        generator.generate_line(handler, name + ".update(deltaTime);")

def gen_shared_setup(handler, executor_data, setup_desc_list : list[SharedUsage], pool_index : int):
    executor_name = get_name(executor_data)
    
    for desc in setup_desc_list:
        generator.generate_line(handler, executor_name + ".{}Context = computedP{}_{}Context;".format(desc.executor_scontext, pool_index, desc.shared_instance))
        
def gen_flush(handler, executors_data):
    for data in executors_data:
        name = get_name(data)
        generator.generate_line(handler, name + ".flushSharedLocalContexts();")
        
def gen_body_flush(handler, executor_data):
    contexts_to_flush = executor_data.get("contextsToFlush")
    if contexts_to_flush is None:
        return
    
    for flush_context in contexts_to_flush:
        generator.generate_line(handler, "this->{}Context.reset();".format(flush_context))
        
def gen_body_init(handler, executor_data):
    generator.generate_line(handler, "this->initImpl();")
  
def gen_body_contexts_load(handler, executor_data):
    contexts_local = executor_data.get("contextsLocal")
    if contexts_local is not None:
        for context in contexts_local:
            for element in context["list"]:
                generator.generate_line(handler, "this->{}Context.load();".format(element))
        generator.generate_empty(handler)
    
    contexts_write_to = executor_data.get("contextsWriteTo")
    if contexts_write_to is not None:
        for context in contexts_write_to:
            for element in context["list"]:
                generator.generate_line(handler, "this->{}Context.load();".format(element))
        
def gen_body_update(handler, executor_data):   
    generator.generate_line(handler, "this->updateImpl(dt);")
        
def gen_contexts_decl(handler, executor_data):   
    contexts_local = executor_data.get("contextsLocal")
    if contexts_local is not None:
        for context in contexts_local:
            class_name = "Context::{}::Data".format(_to_class_name(context["type"]))
            for element in context["list"]:
                field_name = "{}Context".format(element)
                generator.generate_class_variable(handler, class_name, field_name)
    
    contexts_shared = executor_data.get("contextsShared")
    if contexts_shared is not None:
        for context in contexts_shared:
            class_name = "Context::{}::CData".format(_to_class_name(context["type"]))
            for element in context["list"]:
                field_name = "{}Context".format(element)
                generator.generate_class_variable(handler, class_name, field_name)
        
def gen_header(folder, executor_data):
    executor_name = get_name(executor_data)
    class_name = _to_class_name(executor_name)
    file_name = class_name + "Executor.h"
    handler = generator.generate_file(folder, file_name)
    
    generator.generate_line(handler, "#pragma once")
    generator.generate_empty(handler)
    
    contexts_list = []
            
    contexts_local = executor_data.get("contextsLocal")
    if contexts_local is not None:
        for context in contexts_local:
            contexts_list.append(_to_class_name(context["type"]))
            
    contexts_write_to = executor_data.get("contextsWriteTo")
    if contexts_write_to is not None:
        for context in contexts_write_to:
            contexts_list.append(_to_class_name(context["type"]))
    
    contexts_shared = executor_data.get("contextsShared")
    if contexts_shared is not None:
        for context in contexts_shared:
            contexts_list.append(_to_class_name(context["type"]))
    
    contexts_list_unique = list(set(contexts_list))
    contexts_list_unique.sort()
    for context_name in contexts_list_unique:
        generator.generate_line(handler, "#include <Contexts/{}Context.h>".format(context_name))
            
    generator.generate_empty(handler)
    
    generator.generate_line(handler, "#include <dod/MemPool.h>")
    generator.generate_line(handler, "#include <dod/SharedContext.h>")
    generator.generate_empty(handler)
    
    def namespace_block_data(handler):
        def class_data(class_handler):
            generator.generate_class_public_method(class_handler, "loadContext", "void", [], False)
            generator.generate_class_public_method(class_handler, "initiate", "void", [], False)
            generator.generate_class_public_method(class_handler, "update", "void", ['float dt'], False)
            generator.generate_class_public_method(class_handler, "flushSharedLocalContexts", "void", [], False)
            generator.generate_class_private_method(class_handler, "initImpl", "void", [], False)
            generator.generate_class_private_method(class_handler, "updateImpl", "void", ['float dt'], False)
            
            contexts_write_to = executor_data.get("contextsWriteTo")
            if contexts_write_to is not None:
                for context in contexts_write_to:
                    context_type = _to_class_name(context["type"])
                    class_name = "Context::{}::Data".format(context_type)
                    for element in context["list"]:
                        method_name = "modify{}".format(_to_class_name(element))
                        argument = "{}&".format(class_name)
                        generator.generate_class_private_method(class_handler, method_name, "void", [argument], False)
            
            generator.generate_class_variable(class_handler, "Dod::MemPool", "memory")
            
            gen_contexts_decl(class_handler, executor_data)
                                    
        generator.generate_class(handler, class_name, class_data)
        
    generator.generate_line(handler, "#pragma warning(push)")
    generator.generate_line(handler, "#pragma warning(disable : 4625)")
    generator.generate_line(handler, "#pragma warning(disable : 4626)")
    generator.generate_empty(handler)
    
    generator.generate_block(handler, "namespace Game::ExecutionBlock", namespace_block_data)
    
    generator.generate_empty(handler)
    generator.generate_line(handler, "#pragma warning(pop)")
    
def gen_source(folder, executor_data):
    executor_name = get_name(executor_data)
    class_name = _to_class_name(executor_name)
    file_name_header = class_name + "Executor.h"
    file_name_source = class_name + "Executor.cpp"
    handler = generator.generate_file(folder, file_name_source)

    generator.generate_line(handler, "#include \"{}\"".format(file_name_header))
    generator.generate_empty(handler)
    
    def namespace_block_data(handler):
        def class_data(class_handler):
            def load_body(self, handler):
                gen_body_contexts_load(handler, executor_data)
            generator.generate_class_public_method(class_handler, "loadContext", "void", [], False, load_body)
            
            def init_body(self, handler):
                gen_body_init(handler, executor_data)
                
            generator.generate_class_public_method(class_handler, "initiate", "void", [], False, init_body)
            
            def update_body(self, handler):
                gen_body_update(handler, executor_data)
            generator.generate_class_public_method(class_handler, "update", "void", ['float dt'], False, update_body)
            
            def flush_body(self, handler):
                gen_body_flush(handler, executor_data)
            generator.generate_class_public_method(class_handler, "flushSharedLocalContexts", "void", [], False, flush_body)
                        
        generator.generate_class_impl(handler, class_name, class_data)
        generator.generate_empty(handler)
        
    generator.generate_block(handler, "namespace Game::ExecutionBlock", namespace_block_data)
    
def inject_modify_methods(folder, executor_data):
    executor_name = get_name(executor_data)
    class_name = _to_class_name(executor_name)
    file_name_source = "{}ExecutorImpl.cpp".format(class_name)

    if not generator.get_file_generated(folder, file_name_source):
        return
    
    full_path = "{}/{}".format(folder, file_name_source)
    content = injectors.load_content(full_path)
    initial_desc = injectors.gather_desc(content)

    contexts_names_list : list[str] = []
    contexts_types_list : list[str] = []
    contexts_write_to = executor_data.get("contextsWriteTo")
    if contexts_write_to is not None:
        for context in contexts_write_to:
            context_type = _to_class_name(context["type"])
            for element in context["list"]:
                contexts_names_list.append(element)
                contexts_types_list.append(context_type)

    def inject(context_id : int, content : str):
        context_name = contexts_names_list[context_id]
        context_type = contexts_types_list[context_id]

        data_to_inject = "void {}::modify{}([[maybe_unused]] Context::{}::Data& {}) noexcept\n{{\n\n}}".format(
            class_name, _to_class_name(context_name), context_type, context_name)
        
        return injectors.inject(content, context_name, data_to_inject)

    def restore(context_id : int, content : str):
        context_name = contexts_names_list[context_id]
        return injectors.restore(content, context_name)

    for index in range(0, len(contexts_names_list)):
        if contexts_names_list[index] in initial_desc.segment_names:
            continue
        if contexts_names_list[index] in initial_desc.suspended_segment_names:
            content = restore(index, content)
        else:
            content = inject(index, content)

    def suspend(context_id : int, content : str):
        context_name = initial_desc.segment_names[context_id]
        return injectors.suspend(content, context_name)

    for index in range(0, len(initial_desc.segment_names)):
        if initial_desc.segment_names[index] in contexts_names_list:
            continue
        if initial_desc.segment_names[index] in initial_desc.suspended_segment_names:
            continue
        content = suspend(index, content)
    
    generator.write_content(folder, file_name_source, content)

def gen_implementation(folder, executor_data):
    executor_name = get_name(executor_data)
    class_name = _to_class_name(executor_name)
    file_name_header = class_name + "Executor.h"
    file_name_source = class_name + "ExecutorImpl.cpp"
    
    if generator.get_file_generated(folder, file_name_source):
        return
    
    handler = generator.generate_file(folder, file_name_source)

    generator.generate_line(handler, "#include \"{}\"".format(file_name_header))
    generator.generate_empty(handler)
    generator.generate_line(handler, "using namespace Game::ExecutionBlock;")
    
    def class_data(class_handler):
        generator.generate_class_public_method(class_handler, "initImpl", "void", [], False)
        generator.generate_class_public_method(class_handler, "updateImpl", "void", ['[[maybe_unused]] float dt'], False)
    
    generator.generate_class_impl(handler, class_name, class_data)
    
