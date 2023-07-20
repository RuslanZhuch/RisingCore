
import loader

from os import path
import shutil

import filecmp

def _get_dict_keys(dict_data : any):
    keys = []
    if not isinstance(dict_data, dict):
        return keys
        
    for key in dict_data.keys():
        keys.append(key)
        value = dict_data[key]
        if isinstance(value, dict):
            keys.append(_get_dict_keys(value))
        elif isinstance(value, list):
            for el in value:
                keys.append(_get_dict_keys(el))
                
    return keys

def _get_dict_values(dict_data : dict, filter : list[str]):
    values = []
    if not isinstance(dict_data, dict):
        values.append(dict_data)
        return values
    
    for key in dict_data.keys():
        value = dict_data[key]
        if isinstance(value, dict):
            values.extend(_get_dict_values(value, filter))
        elif isinstance(value, list):
            for el in value:
                values.extend(_get_dict_values(el, filter))
        elif filter.count(key) <= 0:
            values.append(value)
            
    return values

def _get_exist_filenames(files_paths : list[str]):
    exist_names = []
    exist_paths = []
    for file_path in files_paths:
        if path.exists(file_path):
            exist_names.append(path.basename(file_path))
            exist_paths.append(file_path)
    
    return exist_names, exist_paths

def _find_element(buffer : list[any], element : any):
    index = 0
    for el in buffer:
        if el == element:
            return index
        index += 1
    
    return -1

def check_files_equals(file_path_left : str, file_path_right : str):
    if not path.isfile(file_path_left) or not path.isfile(file_path_right):
        return False
    return filecmp.cmp(file_path_left, file_path_right)

def check_structure_is_same(left_data : dict, right_data : dict, filter : list[str] = []):
    left_keys = _get_dict_keys(left_data)
    right_keys = _get_dict_keys(right_data)
    
    is_keys_struct_same = left_keys == right_keys
    if not is_keys_struct_same:
        return False

    left_values = _get_dict_values(left_data, filter)
    right_values = _get_dict_values(right_data, filter)
        
    return left_values == right_values

def generate_list(curr_states_folder : list[str], prev_states_folder : list[str], files_list : list[str], filter : list[str] = []):
    curr_file_paths = [curr_states_folder + "/" + file for file in files_list]
    prev_file_paths = [prev_states_folder + "/" + file for file in files_list]
    
    [curr_file_names, curr_file_paths] = _get_exist_filenames(curr_file_paths)
    [prev_file_names, prev_file_paths] = _get_exist_filenames(prev_file_paths)
    
    current_state_datas = [loader.load_file_data(file_name) for file_name in curr_file_paths]
    prev_state_datas = [loader.load_file_data(file_name) for file_name in prev_file_paths]
    
    diff_list = []
    for curr_file_index in range(0, len(current_state_datas)):
        prev_file_index = _find_element(prev_file_names, curr_file_names[curr_file_index])
        if prev_file_index == -1 or not check_structure_is_same(current_state_datas[curr_file_index], prev_state_datas[prev_file_index], filter):
            diff_list.append(files_list[curr_file_index])
        
    return diff_list

def replace(curr_states_folder : list[str], prev_states_folder : list[str], files_list : list[str]):
    
    for file_name in files_list:
        shutil.copyfile(curr_states_folder + "/" + file_name, prev_states_folder + "/" + file_name)
        