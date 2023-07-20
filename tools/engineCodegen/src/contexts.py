
import loader
import generator
import types_manager

class SharedMerge:
    def __init__(self, executor_name, executor_scontext):
        self.executor_name = executor_name
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

def load_contexts(context_file_names : list[str]):
    return loader.load_shared_contexts(context_file_names)

class ContextData:
    MERGE_TYPE_NONE : int = 0
    MERGE_TYPE_ASSIGN : int = 1
    MERGE_TYPE_ADD : int = 2        
    
    class ObjectDataElement:
        def __init__(self, name, data_type, initial, merge_type: int):
             self.name = name
             self.data_type = data_type
             self.initial = initial
             self.merge_type = merge_type
             
    class BufferDataElement:
        def __init__(self, name, data_type, capacity):
             self.name = name
             self.data_type = data_type
             self.capacity = capacity
             
    def __init__(self, objects_data, buffers_data):
        self.objects_data = objects_data
        self.buffers_data = buffers_data

def _get_merge_type(merge_policy : str):
    if merge_policy == "simple":
        return ContextData.MERGE_TYPE_ASSIGN
    elif merge_policy == "add":
        return ContextData.MERGE_TYPE_ADD
    return ContextData.MERGE_TYPE_NONE

def load_data(context_raw_data):
    objects_data = []
    buffers_data = []
    for variable in context_raw_data["data"]:
        type = variable["type"]
        name = variable["name"]
        data_type = variable["dataType"]
        if type == "object":
            initial = variable.get("initial")
            merge_policy = variable.get("merge")
            merge_type = _get_merge_type(merge_policy if merge_policy is not None else "") 
            objects_data.append(ContextData.ObjectDataElement(name, data_type, initial, merge_type))
        elif type == "dbbuffer":
            capacity = variable["capacity"]
            buffers_data.append(ContextData.BufferDataElement(name, data_type, capacity))
    
    return ContextData(objects_data, buffers_data)

def generate_context_data(handler, context_raw_data):
    
    def struct_body(struct_handler):
        generator.generate_struct_method(struct_handler, "load", "void", [], False)
        generator.generate_struct_method(struct_handler, "reset", "void", [], False)
        generator.generate_struct_method(struct_handler, "merge", "void", ["const Data& other"], False)
        
        context_data = load_data(context_raw_data)
        
        for object in context_data.objects_data:
            initial = 0 if object.initial != 0 and object.initial is not None else object.initial
            generator.generate_struct_variable(struct_handler, object.data_type, object.name, initial)
            
        generator.generate_struct_variable(struct_handler, "Dod::MemPool", "memory", None)
        for buffer in context_data.buffers_data:
            type = "Dod::DBBuffer<{}>".format(buffer.data_type)
            generator.generate_struct_variable(struct_handler, type, buffer.name, None)
            
    generator.generate_struct(handler, "Data", struct_body)

def generate_context_def(dest_path, context_file_path, types_cache):
    context_raw_data = loader.load_file_data(context_file_path)
    context_name = _to_class_name(loader.load_name(context_file_path))
    handler = generator.generate_file(dest_path, "{}Context.h".format(context_name))
    
    generator.generate_line(handler, "#pragma once")
    generator.generate_empty(handler)
    
    context_data = load_data(context_raw_data)
    list_of_types = []
    
    for object in context_data.objects_data:
        list_of_types.append(object.data_type)
        
    for buffer in context_data.buffers_data:
        list_of_types.append(buffer.data_type)
        
    types_manager.gen_includes(handler, types_cache, list_of_types)
        
    #generator.generate_line(handler, "#include <{}>".format(buffer.data_type))
            
    generator.generate_line(handler, "#include <dod/Buffers.h>")
    generator.generate_line(handler, "#include <dod/MemPool.h>")
    generator.generate_empty(handler)
    
    def namespace_body(namespace_handler):
        generate_context_data(namespace_handler, context_raw_data)
    generator.generate_block(handler, "namespace Game::Context::{}".format(context_name), namespace_body)
    
def generate_context_impl(dest_path, context_file_path):
    context_raw_data = loader.load_file_data(context_file_path)
    context_name = _to_class_name(loader.load_name(context_file_path))
    handler = generator.generate_file(dest_path, "{}Context.cpp".format(context_name))
    
    generator.generate_line(handler, "#include \"{}Context.h\"".format(context_name))
    generator.generate_empty(handler)

    generator.generate_line(handler, "#include <dod/BufferUtils.h>")
    generator.generate_line(handler, "#include <engine/contextUtils.h>")
    generator.generate_empty(handler)
    
    def namespace_body(namespace_handler):
        generate_context_load(namespace_handler, context_raw_data, context_file_path)
        generate_context_reset(namespace_handler, context_raw_data)
        generate_context_merge(namespace_handler, context_raw_data)
        generator.generate_empty(handler)
    class_name = context_name
    generator.generate_block(handler, "namespace Game::Context::{}".format(class_name), namespace_body)
    
def generate_context_load(handler, context_raw_data, context_file_path):
    def struct_data(struct_handler):
        
        def load_body(self, handler):
            generator.generate_empty(handler)
            
            context_data = load_data(context_raw_data)
            total_elements = len(context_data.objects_data) + len(context_data.buffers_data)
            
            generator.generate_variable(handler, "const auto", "doc", "Engine::ContextUtils::loadFileDataRoot(\"{}\")".format(context_file_path))
            generator.generate_variable(handler, "const auto&", "inputDataOpt", "Engine::ContextUtils::gatherContextData(doc, {})".format(total_elements))
            generator.generate_empty(handler)
            
            def if_statement_body(handler):
                generator.generate_line(handler, "return;")
            generator.generate_block(handler, "if (!inputDataOpt.has_value())", if_statement_body)
            generator.generate_empty(handler)
            
            generator.generate_variable(handler, "const auto&", "loadingDataArray", "inputDataOpt.value()")
            generator.generate_empty(handler)
            
            element_id = 0
            for object in context_data.objects_data:
                variable_name = object.name
                generator.generate_line(handler, "Engine::ContextUtils::loadVariable(this->{}, loadingDataArray, {});".format(variable_name, element_id))
                element_id += 1
            generator.generate_empty(handler)
            
            buffer_id = element_id
            for buffer in context_data.buffers_data:
                buffer_name = buffer.name
                type = buffer.data_type
                generator.generate_line(handler, "const auto {}CapacityBytes{{ Engine::ContextUtils::getBufferCapacityBytes<{}>(loadingDataArray, {}) }};".format(buffer_name, type, buffer_id))
                buffer_id += 1
            generator.generate_empty(handler)                
                
            buffer_id = element_id
            generator.generate_line(handler, "int32_t needBytes{};")
            for buffer in context_data.buffers_data:
                buffer_name = buffer.name
                generator.generate_line(handler, "needBytes += {}CapacityBytes;".format(buffer_name))
                buffer_id += 1
            generator.generate_empty(handler)
            
            generator.generate_line(handler, "this->memory.allocate(needBytes);")
            generator.generate_line(handler, "int32_t header{};")
            generator.generate_empty(handler)
            
            buffer_id = element_id
            for buffer in context_data.buffers_data:
                buffer_name = buffer.name
                generator.generate_line(handler, "Engine::ContextUtils::loadBuffer(this->{0}, {0}CapacityBytes, this->memory, header);".format(buffer_name))
                buffer_id += 1
            generator.generate_empty(handler)
        
        generator.generate_struct_method(struct_handler, "load", "void", [], False, load_body)

    generator.generate_struct_impl(handler, "Data", struct_data)
    
def generate_context_reset(handler, context_raw_data):
    def struct_data(struct_handler):
        
        def load_body(self, handler):
            context_data = load_data(context_raw_data)
            for buffer in context_data.buffers_data:
                buffer_name = buffer.name
                generator.generate_line(handler, "Dod::BufferUtils::flush(this->{});".format(buffer_name))

        generator.generate_struct_method(struct_handler, "reset", "void", [], False, load_body)

    generator.generate_struct_impl(handler, "Data", struct_data)
    
def generate_context_merge(handler, context_raw_data):
    def struct_data(struct_handler):
        
        def load_body(self, handler):
            context_data = load_data(context_raw_data)
            for object in context_data.objects_data:
                if object.merge_type == ContextData.MERGE_TYPE_NONE:
                    continue
                object_name = object.name
                if object.merge_type == ContextData.MERGE_TYPE_ASSIGN:
                    generator.generate_line(handler, "this->{0} = other.{0};".format(object_name))
                elif object.merge_type == ContextData.MERGE_TYPE_ADD:
                    generator.generate_line(handler, "this->{0} += other.{0};".format(object_name))
            
            for buffer in context_data.buffers_data:
                buffer_name = buffer.name
                generator.generate_line(handler, "Dod::BufferUtils::append(this->{0}, Dod::BufferUtils::createImFromBuffer(other.{0}));".format(buffer_name))

        generator.generate_struct_method(struct_handler, "merge", "void", ["const Data& other"], False, load_body)

    generator.generate_struct_impl(handler, "Data", struct_data)
    
class ContextUsage:
    def __init__(self, context_name, instance_name):
        self.context_name = context_name
        self.instance_name = instance_name
        
    def __lt__(self, other):
        return self.context_name < other.context_name

def load_shared_context_instances(file) -> list[ContextUsage]:
    content = loader.load_shared_contexts_usage(file)
    
    usage_data = []
    for usage in content["sharedContextsInstances"]:
        usage_data.append(ContextUsage(usage["contextName"], usage["instanceName"]))
    return usage_data
    
def load_shared_context_to_flush(workspace_data):
    return workspace_data["sharedContextsInstancesToFlush"]
    
def generate_shared_flush(handler, workspace_data):
    flush_data = load_shared_context_to_flush(workspace_data)
    for context in flush_data:
        generator.generate_line(handler, "Dod::SharedContext::flush(&{}Context);".format(context))
    
def load_shared_context_merge(workspace_data):    
    output = dict()
    
    merge_data_full = workspace_data.get("sharedContextsMerge")
    if merge_data_full is None:
        return output
    
    for element in merge_data_full:
        executor_name = element["executorName"]
        shared_instance = element["instanceName"]
        executor_context = element["executorContextName"]
        if output.get(shared_instance) is None:
            output[shared_instance] = []    
        output[shared_instance].append(SharedMerge(executor_name, executor_context))
        
    return output
    
def get_validated_context_instances(contexts_data, shared_contexts_instances) -> list[ContextUsage]:
    loaded_names = [el.get("name") for el in contexts_data]
    
    validated = []
    for instance in shared_contexts_instances:
        if loaded_names.count(instance.context_name) == 0:
            continue
        validated.append(instance)
        
    return validated
    
def generate_shared_init(handler, contexts_data):
    for instance in contexts_data:
        class_name = _to_class_name(instance.context_name)
        generator.generate_line(handler, "Dod::SharedContext::Controller<Game::Context::{}::Data> {}Context;".format(class_name, instance.instance_name))
    
def generate_shared_merge(handler, workspace_data):
    merge_data = load_shared_context_merge(workspace_data)
    if merge_data is None:
        return
    
    for instance_name in merge_data:
        merge_context_full = merge_data[instance_name]
        for merge_context in merge_context_full:
            executor_name = merge_context.executor_name
            executor_scontext = merge_context.executor_scontext
            generator.generate_line(handler, "Dod::SharedContext::merge(&{}Context, {}.{}Context);".format(instance_name, executor_name, executor_scontext))
