
import loader
import types_manager
from templite import Templite

import os
current_directory = os.path.dirname(os.path.abspath(__file__))

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
    MERGE_TYPE_ASSIGN_NON_ZERO : int = 3
    
    class ObjectDataElement:
        def __init__(self, el_id: int, name, data_type, initial, merge_type: int):
            self.el_id = el_id
            self.name = name
            self.data_type = data_type
            self.initial = initial
            self.merge_type = merge_type

    class BufferDataElement:
        def __init__(self, el_id: int, name, data_type, capacity, merge_type: int):
            self.el_id = el_id
            self.name = name
            self.data_type = data_type
            self.capacity = capacity
            self.merge_type = merge_type
             
    class TableDataElement:
        def __init__(self, el_id: int, name : str, data_name_list : list[str], data_type_list : list[str], data_type_full_list : dict, capacity: int, merge_type: int, flags: list[str]):
            self.el_id = el_id
            self.name = name
            self.data_name_list = data_name_list
            self.data_type_list = data_type_list
            self.data_type_full_list = data_type_full_list
            self.capacity = capacity
            self.merge_type = merge_type
            self.flags = flags
             
    def __init__(self, objects_data : list[ObjectDataElement], buffers_data : list[BufferDataElement], tables_data : list[TableDataElement]):
        self.objects_data = objects_data
        self.buffers_data = buffers_data
        self.tables_data = tables_data

def _get_merge_type_for_objects(merge_policy : str):
    if merge_policy == "simple":
        return ContextData.MERGE_TYPE_ASSIGN
    elif merge_policy == "add":
        return ContextData.MERGE_TYPE_ADD
    elif merge_policy == "simple_nz":
        return ContextData.MERGE_TYPE_ASSIGN_NON_ZERO
    return ContextData.MERGE_TYPE_NONE

def _get_merge_type_for_buffers(merge_policy : str):
    if merge_policy == "disabled":
        return ContextData.MERGE_TYPE_NONE
    return ContextData.MERGE_TYPE_ASSIGN

def load_data(context_raw_data : dict):
    objects_data : list[ContextData.ObjectDataElement] = []
    buffers_data : list[ContextData.BufferDataElement] = []
    tables_data : list[ContextData.TableDataElement] = []
    el_id = 0
    for variable in context_raw_data["data"]:
        type = variable["type"]
        name = variable["name"]
        if type == "object":
            initial = variable.get("initial")
            merge_policy = variable.get("merge")
            merge_type = _get_merge_type_for_objects(merge_policy if merge_policy is not None else "") 
            data_type = variable["dataType"]
            objects_data.append(ContextData.ObjectDataElement(el_id, name, data_type, initial, merge_type))
        elif type == "dbbuffer":
            capacity = variable["capacity"]
            merge_policy = variable.get("merge")
            merge_type = _get_merge_type_for_buffers(merge_policy if merge_policy is not None else "") 
            data_type = variable["dataType"]
            buffers_data.append(ContextData.BufferDataElement(el_id, name, data_type, capacity, merge_type))
        elif type == "data":
            capacity = variable["capacity"]
            merge_policy = variable.get("merge")
            merge_type = _get_merge_type_for_buffers(merge_policy if merge_policy is not None else "") 
            data_type = dict()
            if variable.get("dataTypeList"):
                data_type = variable.get("dataTypeList")
            else:
                data_type["data"] = variable["dataType"]
            type_names = data_type.keys()
            types = data_type.values()
            flags = variable.get("flags")
            if flags is None:
                flags = []
            tables_data.append(ContextData.TableDataElement(el_id, name, type_names, types, data_type, capacity, merge_type, flags))
        else:
            continue
        
        el_id += 1
    
    return ContextData(objects_data, buffers_data, tables_data)

def generate_context_def(dest_path, context_file_path, types_cache):
    context_raw_data = loader.load_file_data(context_file_path)
    context_name = _to_class_name(loader.load_name(context_file_path))

    context_data = load_data(context_raw_data)
    list_of_types = []
    
    for object in context_data.objects_data:
        list_of_types.append(object.data_type)
        
    for buffer in context_data.buffers_data:
        list_of_types.append(buffer.data_type)
        
    for table in context_data.tables_data:
        list_of_types.extend(table.data_type_list)
     
    includes_list = types_manager.get_includes_list(types_cache, list_of_types)
    context_data = load_data(context_raw_data)

    objects_data = []
    for object in context_data.objects_data:
        initial = ""
        if object.initial is not None:
            initial = "{}"
        objects_data.append({
            "name": object.name,
            "type": object.data_type,
            "initial": initial
        })
    convertion_list_tables = [ "Dod::ImTable<{}>(context.{})".format(", ".join(table.data_type_list), table.name) for table in context_data.tables_data ]
    
    parameters = {
        "context_class_name": context_name,
        "include_files_list": includes_list,
        "tables_data_list": context_data.tables_data,
        "variables_data_list": objects_data,
        "conversions_list": convertion_list_tables
    }

    t = Templite(filename=current_directory + "/genSchemas/contextDef.gens")
    file_data = t.render(**parameters)
    with open("{}/{}Context.h".format(dest_path, context_name), 'w') as file:
        file.write(file_data)
    
def generate_context_impl(dest_path, context_file_path):
    context_raw_data = loader.load_file_data(context_file_path)
    context_name = _to_class_name(loader.load_name(context_file_path))
    context_data = load_data(context_raw_data)


    parameters = {
        "context_class_name": context_name,
        "context_data": context_data,
        "context_file_path": context_file_path,
        "ContextData": ContextData
    }

    t = Templite(filename=current_directory + "/genSchemas/contextImpl.gens")
    file_data = t.render(**parameters)
    with open("{}/{}Context.cpp".format(dest_path, context_name), 'w') as file:
        file.write(file_data)

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
    pool_contexts_instances = content.get("poolContextsInstances")
    if pool_contexts_instances is not None:
        for usage in pool_contexts_instances:
            usage_data.append(ContextUsage(usage["contextName"], usage["instanceName"]))
    return usage_data
    
def load_shared_context_to_flush(workspace_data):
    return workspace_data["sharedContextsInstancesToFlush"]
    
def get_shared_flush(workspace_data):
    flush_list = []
    flush_data = load_shared_context_to_flush(workspace_data)
    for context in flush_data:
        flush_list.append(context)

    return flush_list

def get_pools_flush(workspace_data):
    pools_flush_list = []
    pools = workspace_data.get("poolContextsInstances")
    if pools is None:
        return pools_flush_list
    flush_data = [pool["instanceName"] for pool in pools]
    for context in flush_data:
        pools_flush_list.append(context)

    return pools_flush_list
    
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

def load_pool_context_merge(workspace_data):
    output = dict()
    
    merge_data_full = workspace_data.get("poolContextsMerge")
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

class SharedUsageDesc:
    def __init__(self, pool_id: int, instance_name: str, context_name: str, data_instance_name: str):
        self.pool_id = pool_id
        self.instance_name = instance_name
        self.context_name = context_name
        self.data_instance_name = data_instance_name

def get_shared_usage(contexts_data : list[ContextUsage], pool_id : int, instance_names : list[str]):
    shared_usage_descs_list = []
    for instance_name in instance_names:
        for data in contexts_data:
            if data.instance_name != instance_name:
                continue
            shared_usage_descs_list.append(SharedUsageDesc(
                pool_id,
                instance_name,
                _to_class_name(data.context_name),
                data.instance_name
            ))
            break
    return shared_usage_descs_list

class SharedMergeDesc:
    def __init__(self, executor_name: str, executor_scontext: str, instance_name: str):
        self.executor_name = executor_name
        self.executor_scontext = executor_scontext
        self.instance_name = instance_name

def get_shared_merge(workspace_data):
    shared_merge_descs_list = []
    merge_data = load_shared_context_merge(workspace_data)
    if merge_data is None:
        return shared_merge_descs_list
    
    for instance_name in merge_data:
        merge_context_full = merge_data[instance_name]
        for merge_context in merge_context_full:
            executor_name = merge_context.executor_name
            executor_scontext = merge_context.executor_scontext
            shared_merge_descs_list.append(SharedMergeDesc(
                executor_name,
                _to_class_name(executor_scontext),
                instance_name
            ))
            
    return shared_merge_descs_list

class PoolsMergeDesc:
    def __init__(self, executor_name: str, executor_scontext: str, instance_name: str):
        self.executor_name = executor_name
        self.executor_scontext = executor_scontext
        self.instance_name = instance_name

def get_pools_merge(workspace_data : dict[any], block_id : int):
    pools_merge_descs_list = []

    merge_data = load_pool_context_merge(workspace_data)
    if merge_data is None:
        return pools_merge_descs_list
    
    schema = workspace_data.get("schema")
    if schema is None:
        return pools_merge_descs_list
    
    if len(schema) <= block_id or block_id < 0:
        return pools_merge_descs_list
    
    block = schema[block_id]
    wait_rules = block.get("waitPools")
    if wait_rules is None:
        return pools_merge_descs_list
    
    prev_block_executors = []
    if block_id > 0:
        prev_block = schema[block_id - 1]
        prev_block_executors.extend(prev_block["executors"])
    
    for instance_name in wait_rules:
        merge_context_full = merge_data.get(instance_name)
        if merge_context_full is None:
            continue
        for merge_context in merge_context_full:
            executor_name = merge_context.executor_name
            if executor_name not in prev_block_executors:
                continue
            executor_scontext = merge_context.executor_scontext
            pools_merge_descs_list.append(PoolsMergeDesc(
                executor_name,
                _to_class_name(executor_scontext),
                instance_name
            ))

    return pools_merge_descs_list

