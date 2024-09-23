import loader
import generator
import injectors
from templite import Templite

import os
current_directory = os.path.dirname(os.path.abspath(__file__))

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

class ExternalContextDesc:
    def __init__(self, type: str, name: str):
        self.context_type = type
        self.context_name = name

def load_external_contexts(executors_data):
    output = list()
    
    contexts = executors_data.get("contextsExternal")
    if contexts is None:
        return output
    
    for context in contexts:
        context_type = context["type"]
        for instance_name in context["list"]:
            output.append(ExternalContextDesc(
                type= context_type,
                name= instance_name
            ))

    return output

class SharedSetupDesc:
    def __init__(self, executor_scontext: str, pool_index: int, shared_instance: str):
        self.executor_scontext = executor_scontext
        self.pool_index = pool_index
        self.shared_instance = shared_instance

def get_shared_setup(executor_data, setup_desc_list : list[SharedUsage], pool_index : int):
    shared_setup_descs_list = []

    executor_name = get_name(executor_data)
    
    for desc in setup_desc_list:
        shared_setup_descs_list.append(SharedSetupDesc(
            desc.executor_scontext, 
            pool_index, 
            desc.shared_instance
        ))

    return shared_setup_descs_list, executor_name
        
def get_flush(executors_data):
    flush_descs_list = []
    for data in executors_data:
        name = get_name(data)
        flush_descs_list.append(name)

    return flush_descs_list

def gen_header(folder, executor_data):
    executor_name = get_name(executor_data)
    class_name = _to_class_name(executor_name)
    file_name = class_name + "Executor.h"

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

    external_contexts = load_external_contexts(executor_data)

    parameters = {
        "contexts_list_unique": contexts_list_unique,
        "class_name": class_name,
        "contexts_write_to": contexts_write_to,
        "executor_data": executor_data,
        "external_contexts": external_contexts,
    }

    t = Templite(filename=current_directory + "/genSchemas/exeHeader.gens")
    file_data = t.render(**parameters)
    with open(folder + "/" + file_name, 'w') as file:
        file.write(file_data)

def gen_source(folder, executor_data):
    executor_name = get_name(executor_data)
    class_name = _to_class_name(executor_name)
    file_name_source = class_name + "Executor.cpp"

    external_contexts = load_external_contexts(executor_data)

    parameters = {
        "class_name": class_name,
        "executor_data": executor_data,
        "external_contexts": external_contexts,
    }

    t = Templite(filename=current_directory + "/genSchemas/exeSource.gens")
    file_data = t.render(**parameters)
    with open(folder + "/" + file_name_source, 'w') as file:
        file.write(file_data)

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
    
    parameters = {
        "class_name": class_name,
        "executor_data": executor_data,
        "file_name_header": file_name_header,
    }

    t = Templite(filename=current_directory + "/genSchemas/exeImpl.gens")
    file_data = t.render(**parameters)
    with open(folder + "/" + file_name_source, 'w') as file:
        file.write(file_data)
