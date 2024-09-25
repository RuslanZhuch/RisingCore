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

def _get_structure_data(workspace_shared_contexts_file) -> list[contexts.ContextUsage]:
    data = loader.load_file_data(workspace_shared_contexts_file)
    return data["structure"]

class BlockData:
    class ExecutorData:
        def __init__(self):
            self.shared_setup_descs_list = []
            self.executor_name = ""
    def __init__(self):
        self.pools_merge_descs_list = []
        self.shared_usage_descs_list = []
        self.executors_descs_list = []

def generate(target_path, executors_data, workspace_shared_contexts_file, loaded_contexts_data):
    
    workspace_context_data = loader.load_runtime_data(workspace_shared_contexts_file)
    
    structure = _get_structure_data(workspace_shared_contexts_file)
    structure_contexts_list = structures.get_contexts_list(structure)
    anchors_list = structures.get_anchors_list(structure)
    executors_deps_masks = structures.get_executors_deps_mask(structure, structure_contexts_list)
    initial_context_states = structures.get_initial_deps_mask(structure, anchors_list, structure_contexts_list)
    initial_context_mask = structures.convert_to_mask(initial_context_states)
    executors_deps_descs_list =  [structures.get_deps_descs_for_executor(executor_deps_mask.executor_name, executor_deps_mask.mask) for executor_deps_mask in executors_deps_masks] 

    executors_deps_contexts = structures.get_executors_deps_contexts(structure, structure_contexts_list)

    mandatory_executors, optional_executors = structures.get_executors_segmented(structure)
    optional_executors_data = structures.get_optional_executors_data(structure, optional_executors)
    num_of_optional_executors = structures.compute_number_of_parts(len(optional_executors))

    executors_per_contexts = structures.get_executors_per_contexts(structure, structure_contexts_list, optional_executors)
    executors_inputs_context = structures.get_executors_inputs_contexts(structure)
    executors_ext_inputs_context = structures.get_executors_ext_inputs_contexts(structure)

    contexts_type_list = structures.get_contexts_types_list(structure)

    contexts_data = structures.get_contexts_descs_list(structure)
    ext_contexts_data = structures.get_ext_contexts_descs_list(structure)
    executors_list = structures.get_executors_list(structure)

    executors_ext_outputs = structures.get_executors_ext_outputs(structure)

    parameters = {
        "executors_data": executors_data,
        "executors_list": executors_list,
        "contexts_type_list": contexts_type_list,
        "contexts_data": contexts_data,
        "ext_contexts_data": ext_contexts_data,
        "executors_ext_outputs": executors_ext_outputs,
        "num_of_parts_in_deps_mask": structures.compute_number_of_parts(len(structure_contexts_list)),
        "initial_deps_masks": executors_deps_descs_list,
        "initial_context_mask": initial_context_mask,
        "executors_per_contexts": executors_per_contexts,
        "num_of_optional_executors": num_of_optional_executors,
        "mandatory_executors": mandatory_executors,
        "optional_executors": optional_executors,
        "optional_executors_data": optional_executors_data,
        "executors_deps_contexts": executors_deps_contexts,
        "executors_inputs_context": executors_inputs_context,
        "executors_ext_inputs_context": executors_ext_inputs_context,
        "executors_flush": executors.get_flush(executors_data),
    }

    t = Templite(filename=current_directory + "/genSchemas/runtime.gens")
    file_data = t.render(**parameters)
    with open(target_path + "/runtime.cpp", 'w') as file:
        file.write(file_data)
