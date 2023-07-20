import loader
import generator

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
    
    usage = workspace_data["sharedContextsUsage"]
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
        gen_shared_context_init(handler, data, workspace_data)
        generator.generate_line(handler, name + ".initiate();")
        
def gen_updates(handler, executors_data):
    for data in executors_data:
        name = get_name(data)
        generator.generate_line(handler, name + ".update(deltaTime);")
        
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
    
    contexts_write_to = executor_data.get("contextsWriteTo")
    if contexts_write_to is not None:
        for context in contexts_write_to:
            class_name = "Context::{}::Data".format(_to_class_name(context["type"]))
            for element in context["list"]:
                field_name = "{}Context".format(element)
                generator.generate_class_variable(handler, class_name, field_name)
    
    contexts_shared = executor_data.get("contextsShared")
    if contexts_shared is not None:
        for context in contexts_shared:
            class_name = "const Dod::SharedContext::Controller<Context::{}::Data>*".format(_to_class_name(context["type"]))
            for element in context["list"]:
                field_name = "{}Context".format(element)
                generator.generate_class_variable(handler, class_name, field_name, "nullptr")
        
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
            
            generator.generate_class_variable(class_handler, "Dod::MemPool", "memory")
            
            gen_contexts_decl(class_handler, executor_data)
                                    
        generator.generate_class(handler, class_name, class_data)
        
    generator.generate_block(handler, "namespace Game::ExecutionBlock", namespace_block_data)
    
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
    
    def namespace_block_data(handler):
        def class_data(class_handler):
            generator.generate_class_public_method(class_handler, "initImpl", "void", [], False)
            generator.generate_class_public_method(class_handler, "updateImpl", "void", ['float dt'], False)
       
        generator.generate_class_impl(handler, class_name, class_data)
        generator.generate_empty(handler)
        
    generator.generate_block(handler, "namespace Game::ExecutionBlock", namespace_block_data)
    