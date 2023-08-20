import loader
import types_manager
import types_generator

import os
import contexts
import executors
import diff
import runtime

from os import path

import sys
sys.path.append("../../cppParser/src")

import parser

class ProjectDesc:
    def __init__(self):
        self.project_dest_folder = ""
        self.application_runtime_path = ""
        self.cache_folder = "",
        self.types_src_path = "",
        self.types_paths = []
        self.contexts_paths = []
        self.executors_paths = []

def _get_value(data, field):
    value = data.get(field)
    if value is not None:
        return value
    return ""

def _get_array(data, field):
    array_data = data.get(field)
    if array_data is not None:
        return array_data
    
    return []

def _generate_diff_for_contexts(cache_folder : str, context_paths : list[str]):
    diff_list = diff.generate_list(context_paths, cache_folder, [
        "initial",
        "capacity"
    ])

    diff.replace(cache_folder, diff_list)
    
    return diff_list

def _generate_diff_for_executors(cache_folder : str, executors_paths : list[str]):
    diff_list = diff.generate_list(executors_paths, cache_folder)

    diff.replace(cache_folder, diff_list)
    
    return diff_list

def _generate_diff_for_project_structure(cache_folder : str, file_path : str):
    file_name = path.basename(file_path)
    full_path = file_path
    full_path_prev = cache_folder + "/" + file_name
    
    if not diff.check_files_equals(full_path, full_path_prev):
        diff.replace(cache_folder, [
            file_path
        ])
        return True

    return False

def _generate_types_impls(types_cache : types_manager.TypesCache, cache_folder : str, project_dir : str, types_src_path : str):
    include_list = types_manager.get_includes_list(types_cache, types_cache.type_names)
    
    pathes_to_parse = []
    for path in include_list:
        full_path = types_src_path + path
        if not os.path.exists(full_path):
            continue
        pathes_to_parse.append(full_path)
        
    parsed_files = parser.parse_to_folder(pathes_to_parse, project_dir + "/parsed_types/")
    
    changed_types = diff.generate_list(parsed_files, cache_folder)
    diff.replace(cache_folder, changed_types)
    
    for parsed_file_path in changed_types:
        parsed_data = loader.load_file_data(parsed_file_path)
        file_name = os.path.basename(parsed_file_path)
        file_name_stripped = file_name.split(".")[0]
        output_path = "{}/generated_types/{}Gen.cpp".format(project_dir, file_name_stripped)
        types_generator.generate_impls(output_path, parsed_data, types_cache)

def load(project_desc_file):
    desc_file_data = loader.load_file_data(project_desc_file)
    
    project_desc = ProjectDesc()
    if desc_file_data is None:
        return project_desc
    
    project_desc.project_dest_folder = _get_value(desc_file_data, "project_dest_folder")
    project_desc.application_runtime_path = _get_value(desc_file_data, "runtime_path")
    project_desc.cache_folder = _get_value(desc_file_data, "cache_folder")
    project_desc.types_paths = _get_array(desc_file_data, "types_paths")
    project_desc.contexts_paths = _get_array(desc_file_data, "contexts_paths")
    project_desc.executors_paths = _get_array(desc_file_data, "executors_paths")
    project_desc.types_src_path = _get_value(desc_file_data, "types_src_path")
    
    return project_desc
    
def generate(project_path : str):
    project_desc = load(project_path)
    
    if project_desc is None:
        return
    
    target_path = project_desc.project_dest_folder
    if target_path == "":
        return
    
    if not os.path.exists(target_path):
        os.makedirs(target_path)
    
    contexts_target_path = target_path + "/contexts"
    if not os.path.exists(contexts_target_path):
        os.makedirs(contexts_target_path)
    
    executors_target_path = target_path + "/executors"
    if not os.path.exists(executors_target_path):
        os.makedirs(executors_target_path)
    
    cache_folder_contexts = project_desc.cache_folder + "/contexts"
    cache_folder_executors = project_desc.cache_folder + "/executors"
    cache_folder_types = project_desc.cache_folder + "/types"
    
    if not os.path.exists(cache_folder_contexts):
        os.makedirs(cache_folder_contexts)
    
    if not os.path.exists(cache_folder_executors):
        os.makedirs(cache_folder_executors)
        
    if not os.path.exists(cache_folder_types):
        os.makedirs(cache_folder_types)
        
    contexts_diff_list = _generate_diff_for_contexts(cache_folder_contexts, project_desc.contexts_paths)
    executors_diff_list = _generate_diff_for_executors(cache_folder_executors, project_desc.executors_paths)
    runtime_changed = _generate_diff_for_project_structure(project_desc.cache_folder, project_desc.application_runtime_path)
    project_structure_changed = _generate_diff_for_project_structure(project_desc.cache_folder, project_path)
    
    types_data = [loader.load_file_data(types_path) for types_path in project_desc.types_paths]
    types_cache = types_manager.cache_types(types_data)
    
    _generate_types_impls(types_cache, cache_folder_types, project_desc.project_dest_folder, project_desc.types_src_path)
    
    for context_file in project_desc.contexts_paths:
        if context_file not in contexts_diff_list:
            continue
        full_path = context_file
        contexts.generate_context_def(contexts_target_path, full_path, types_cache)
    
    for context_file in project_desc.contexts_paths:
        if context_file not in contexts_diff_list:
            continue
        full_path = context_file
        contexts.generate_context_impl(contexts_target_path, full_path)
    
    executor_full_paths = []
    executor_full_paths_changed = []
    for executor_path in project_desc.executors_paths:
        full_path = executor_path
        executor_full_paths.append(full_path)
        if executor_path not in executors_diff_list:
            continue
        executor_full_paths_changed.append(full_path)
    
    executors_data_changed = executors.load(executor_full_paths_changed)
    for data in executors_data_changed:
        executors.gen_header(executors_target_path, data)
        executors.gen_source(executors_target_path, data)
        executors.gen_implementation(executors_target_path, data)
    
    if project_structure_changed is False and runtime_changed is False:
        return
        
    application_context_path = project_desc.application_runtime_path
    
    contexts_full_paths = [context_path for context_path in project_desc.contexts_paths]
    contexts_data = contexts.load_contexts(contexts_full_paths)
    executors_data = executors.load(executor_full_paths)
    runtime.generate(target_path, executors_data, application_context_path, contexts_data)
    