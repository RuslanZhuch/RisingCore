import generator
import executors
import contexts
import structures
import loader

from templite import Templite

import os
current_directory = os.path.dirname(os.path.abspath(__file__))

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

def _get_structure_data(workspace_shared_contexts_file) -> list[contexts.ContextUsage]:
    data = loader.load_file_data(workspace_shared_contexts_file)
    return data["structure"]

def _get_unique_context_instances(shared_context_instances : list[contexts.ContextUsage]):
    context_names = [instance.context_name for instance in shared_context_instances]
    unique_instances = list(set(context_names))
    unique_instances.sort()
    return unique_instances

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

class BlockData:
    class ExecutorData:
        def __init__(self):
            self.shared_setup_descs_list = []
            self.executor_name = ""
    def __init__(self):
        self.pools_merge_descs_list = []
        self.shared_usage_descs_list = []
        self.executors_descs_list = []

def _get_executors_update(runtime_data : dict[any], executors_data: list, context_data : list[contexts.ContextUsage]):
    blocks_data_list = []
    
    schema_data = runtime_data.get("schema")
    if schema_data is None:
        usage_context_names = list[str]()
        for executor_data in executors_data:
            usage_list_raw = gather_shared_usage_for_executor(runtime_data, executor_data["name"])
            for usage in usage_list_raw:
                if usage.shared_context_instance_name not in usage_context_names:
                    usage_context_names.append(usage.shared_context_instance_name)

        block_data = BlockData()

        block_data.shared_usage_decs_list = contexts.get_shared_usage(context_data, 1, list(usage_context_names))
        for data in executors_data:
            executor_desc_data = BlockData.ExecutorData()

            usage_list_raw = gather_shared_usage_for_executor(runtime_data, data["name"])
            usage_list = [ executors.SharedUsage(usage.shared_context_instance_name, usage.executor_context_name) for usage in usage_list_raw ]
            executor_desc_data.shared_setup_descs_list, executor_desc_data.executor_name = executors.get_shared_setup(data, usage_list, 1)
            block_data.executors_descs_list.append(executor_desc_data)

        blocks_data_list.append(block_data)
        return blocks_data_list
    
    pool_id = 1
    for block_id, block_data in enumerate(schema_data):
        executors_in_block = block_data.get("executors")
        if executors_in_block is None:
            continue

        block_data = BlockData()

        block_data.pools_merge_descs_list = contexts.get_pools_merge(runtime_data, block_id)

        usage_context_names = list[str]()
        for executor_in_block in executors_in_block:
            for executor_data in executors_data:
                if executor_data["name"] == executor_in_block:
                    usage_list_raw = gather_shared_usage_for_executor(runtime_data, executor_in_block)
                    for usage in usage_list_raw:
                        if usage.shared_context_instance_name not in usage_context_names:
                            usage_context_names.append(usage.shared_context_instance_name)
                    break
        block_data.shared_usage_decs_list = contexts.get_shared_usage(context_data, pool_id, list(usage_context_names))

        for executor_in_block in executors_in_block:
            for executor_data in executors_data:
                if executor_data["name"] == executor_in_block:
                    executor_desc_data = BlockData.ExecutorData()
                    usage_list_raw = gather_shared_usage_for_executor(runtime_data, executor_in_block)
                    usage_list = [ executors.SharedUsage(usage.shared_context_instance_name, usage.executor_context_name) for usage in usage_list_raw ]
                    
                    executor_desc_data.shared_setup_descs_list, executor_desc_data.executor_name = executors.get_shared_setup(executor_data, usage_list, pool_id)

                    block_data.executors_descs_list.append(executor_desc_data)
                    break

        blocks_data_list.append(block_data)
        pool_id += 1

    return blocks_data_list

def generate(target_path, executors_data, workspace_shared_contexts_file, loaded_contexts_data):
    
    workspace_context_data = loader.load_runtime_data(workspace_shared_contexts_file)
    
    validated_shared_context_instances = _get_validated_shared_context_instaces(workspace_shared_contexts_file, loaded_contexts_data)

    structure = _get_structure_data(workspace_shared_contexts_file)
    structure_contexts_list = structures.get_contexts_list(structure)
    anchors_list = structures.get_anchors_list(structure)
    executors_deps_masks = structures.get_executors_deps_mask(structure, structure_contexts_list)
    initial_context_states = structures.get_initial_deps_mask(structure, anchors_list, structure_contexts_list)
    initial_context_mask = structures.convert_to_mask(initial_context_states)
    executors_deps_descs_list =  [structures.get_deps_descs_for_executor(executor_deps_mask.executor_name, executor_deps_mask.mask) for executor_deps_mask in executors_deps_masks] 

    optional_executors = structures.get_optional_executors(structure)
    num_of_optiona_executors = structures.compute_number_of_parts(len(optional_executors))

    executors_per_contexts = structures.get_executors_per_contexts(structure, structure_contexts_list, optional_executors)

    parameters = {
        "executors_data": executors_data,
        "unique_contexts_instances": _get_unique_context_instances(validated_shared_context_instances),
        "contexts_data": validated_shared_context_instances,
        "num_of_parts_in_deps_mask": structures.compute_number_of_parts(len(structure_contexts_list)),
        "initial_deps_masks": executors_deps_descs_list,
        "initial_context_mask": initial_context_mask,
        "executors_per_contexts": executors_per_contexts,
        "num_of_optiona_executors": num_of_optiona_executors,
        "optional_executors": optional_executors,
        "executors_update_descs": _get_executors_update(workspace_context_data, executors_data, validated_shared_context_instances),
        "pools_flush_descs": contexts.get_pools_flush(workspace_context_data),
        "shared_flush_descs": contexts.get_shared_flush(workspace_context_data),
        "shared_merge_descs": contexts.get_shared_merge(workspace_context_data),
        "executors_flush": executors.get_flush(executors_data),
    }

    t = Templite(filename=current_directory + "/genSchemas/runtime.gens")
    file_data = t.render(**parameters)
    with open(target_path + "/runtime.cpp", 'w') as file:
        file.write(file_data)
