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

def _generate_commands(handler):
    def cycle_body(handler):
        def exit_body(handler):
            generator.generate_line(handler, "return 0;")
        generator.generate_block(handler, "if (Dod::DataUtils::get(sApplicationContext.commands, 0) == 1)", exit_body)
        
    generator.generate_block(handler, "for (int32_t cmdId{}; cmdId < Dod::DataUtils::getNumFilledElements(sApplicationContext.commands); ++cmdId)", cycle_body)

def _get_unique_context_instances(shared_context_instances : list[contexts.ContextUsage]):
    context_names = [instance.context_name for instance in shared_context_instances]
    unique_instances = list(set(context_names))
    unique_instances.sort()
    return unique_instances

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
    
    pool_contexts_instances = runtime_data.get("poolContextsInstances")
    
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
        #contexts.generate_shared_usage(handler, context_data, 1, list(usage_context_names))
        #generator.generate_empty(handler)
        for data in executors_data:
            executor_desc_data = BlockData.ExecutorData()

            usage_list_raw = gather_shared_usage_for_executor(runtime_data, data["name"])
            usage_list = [ executors.SharedUsage(usage.shared_context_instance_name, usage.executor_context_name) for usage in usage_list_raw ]
            executor_desc_data.shared_setup_descs_list, executor_desc_data.executor_name = executors.get_shared_setup(data, usage_list, 1)
            block_data.executors_descs_list.append(executor_desc_data)
            #executors.gen_shared_setup(handler, data, usage_list, 1)
            #executors.gen_updates(handler, [data])
        #handler.newline(1)

        blocks_data_list.append(block_data)
        return blocks_data_list
    
    pool_id = 1
    for block_id, block_data in enumerate(schema_data):
        executors_in_block = block_data.get("executors")
        if executors_in_block is None:
            continue

        block_data = BlockData()

        block_data.pools_merge_descs_list = contexts.get_pools_merge(runtime_data, block_id)
        #contexts.generate_pools_merge(handler, runtime_data, block_id)
        #handler.newline(1)

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
        #contexts.generate_shared_usage(handler, context_data, pool_id, list(usage_context_names))
        #generator.generate_empty(handler)

        for executor_in_block in executors_in_block:
            for executor_data in executors_data:
                if executor_data["name"] == executor_in_block:
                    executor_desc_data = BlockData.ExecutorData()
                    usage_list_raw = gather_shared_usage_for_executor(runtime_data, executor_in_block)
                    usage_list = [ executors.SharedUsage(usage.shared_context_instance_name, usage.executor_context_name) for usage in usage_list_raw ]
                    #executors.gen_shared_setup(handler, executor_data, usage_list, pool_id)
                    #executors.gen_updates(handler, [executor_data])
                    
                    executor_desc_data.shared_setup_descs_list, executor_desc_data.executor_name = executors.get_shared_setup(executor_data, usage_list, pool_id)

                    block_data.executors_descs_list.append(executor_desc_data)
                    break

        blocks_data_list.append(block_data)
        pool_id += 1

    return blocks_data_list

def generate(target_path, executors_data, workspace_shared_contexts_file, loaded_contexts_data):
    
    workspace_context_data = loader.load_runtime_data(workspace_shared_contexts_file)
    #handler = generate_runtime_file(target_path)
    
    validated_shared_context_instances = _get_validated_shared_context_instaces(workspace_shared_contexts_file, loaded_contexts_data)

    structure = _get_structure_data(workspace_shared_contexts_file)
    structure_contexts_list = structures.get_contexts_list(structure)

    parameters = {
        "executors_data": executors_data,
        "unique_contexts_instances": _get_unique_context_instances(validated_shared_context_instances),
        "contexts_data": validated_shared_context_instances,
        "num_of_parts_in_deps_mask": structures.get_num_of_parts_for_deps_structure(len(structure_contexts_list)),
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

#
#
#    _generate_contexts_includes(handler, validated_shared_context_instances)
#    executors.gen_headers(handler, executors_data)
#    handler.newline(1)
#    
#    generator.generate_line(handler, "#include <dod/SharedContext.h>")
#    generator.generate_line(handler, "#include <engine/Timer.h>")
#    handler.newline(1)
#
#    def shared_contexts_init_body(namespace_handler):
#        contexts.generate_shared_init(namespace_handler, validated_shared_context_instances)
#    generator.generate_block(handler, "namespace", shared_contexts_init_body)
#    handler.newline(1)
#
#    def shared_executors_init_body(namespace_handler):
#        executors.gen_inits(handler, executors_data, workspace_context_data)
#    generator.generate_block(handler, "namespace", shared_executors_init_body)
#    handler.newline(1)
#
#    structure = _get_structure_data(workspace_shared_contexts_file)
#    structure_contexts_list = structures.get_contexts_list(structure)
#    structures.generate_deps_structure(handler, len(structure_contexts_list))
#    handler.newline(1)
#
#    def fill_function(self, handler):
#        contexts.generate_shared_load(handler, validated_shared_context_instances)
#        handler.newline(1)
#        
#        executors.gen_loads(handler, executors_data, workspace_context_data)
#    
#        def cycle_function(handler):
#            def impl(handler):
#                handler.newline(1)
#                
#                _generate_executors_update(handler, workspace_context_data, executors_data, validated_shared_context_instances)
#                
#                contexts.generate_pools_flush(handler, workspace_context_data)
#                contexts.generate_shared_flush(handler, workspace_context_data)
#                handler.newline(1)
#                
#                contexts.generate_shared_merge(handler, workspace_context_data)
#                handler.newline(1)
#                
#                executors.gen_flush(handler, executors_data)
#                handler.newline(1)
#            
#                _generate_commands(handler)
#                generator.generate_line(handler, "deltaTime = {};")
#                
#            generator.generate_block(handler, "if (deltaTime >= 1.f / 60.f)", impl)
#            handler.newline(1)
#            generator.generate_line(handler, "timer.tick();")
#            generator.generate_line(handler, "deltaTime += timer.getDeltaTime();")
#
#        
#        handler.newline(1)
#        
#        generator.generate_variable(handler, "Timer", "timer")
#        generator.generate_variable(handler, "float", "deltaTime", 0)
#        generator.generate_block(handler, "while(true)", cycle_function)
#        
#    generator.generate_function(handler, "main", fill_function, "int")
#    handler.close()
    