import json

from os import path
from os import walk
import ntpath

def _path_leaf(path):
    head, tail = ntpath.split(path)
    return tail or ntpath.basename(head)

def load_name(file_path):
    return path.splitext(_path_leaf(file_path))[0]

def load_descriptor(file_path):
    descriptor_file = open(file_path)
    data = json.load(descriptor_file)
    data["name"] = path.splitext(_path_leaf(file_path))[0]
    return data

def load_descriptors(paths : list[str]):
    data = []
    
    for path in paths:
        data.append(load_descriptor(path))
    
    return data

def _load_shared_context(file_path):
    descriptor_file = open(file_path)
    data = json.load(descriptor_file)
    data["name"] = path.splitext(_path_leaf(file_path))[0]
    return data

def load_shared_contexts(paths : list[str]):
    data = []
    
    for path in paths:
        data.append(load_descriptor(path))
    
#    for (dir_path, dir_names, file_names) in walk(folder_path):
#        for file_name in file_names:
#            if dir_path == folder_path:
#                data.append(_load_shared_context(dir_path + "/" + file_name))
#            
    return data

def load_shared_contexts_usage(file_path):
    descriptor_file = open(file_path)
    data = json.load(descriptor_file)
    return data

def load_application_context_data(file_path):
    descriptor_file = open(file_path)
    data = json.load(descriptor_file)
    return data

def load_file_data(file_path):
    descriptor_file = open(file_path)
    data = json.load(descriptor_file)
    return data
