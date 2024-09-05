import types_manager

import generator

from templite import Templite

import os
current_directory = os.path.dirname(os.path.abspath(__file__))

class TypeData:
    def __init__(self, name):
        self.name = name
        self.variables : list[str] = []

def get_list_of_variables(file_schema : dict) -> list[str]:
    
    names = []
    
    variables_data_list = file_schema.get("variables")
    if variables_data_list is None:
        return names
    
    for variable in variables_data_list:
        name = variable.get("name")
        if name is None:
            continue
        names.append(name)
    
    return names

def _get_type_names_from_types(schema : dict, prefix : list[str]) -> TypeData:
    names = []
    
    types = schema.get("types")
    if types is None:
        return names
    
    for type in types:
        name = type.get("name")
        if name is None:
            continue
        new_type = TypeData('::'.join(prefix) + "::" + name)
        new_type.variables = get_list_of_variables(type)
        names.append(new_type)
        prefix.append(name)
        
        names.extend(_get_type_names_from_types(type, prefix))
        
        prefix.pop()

    return names

def _get_type_names_from_namespaces(schema : dict, prefix : list[str]) -> TypeData:
    names = []
    
    namespaces = schema.get("namespaces")
    if namespaces is not None:
        for namespace in namespaces:
            name = namespace.get("name")
            if name is None:
                continue
            prefix.append(name)
            names.extend(_get_type_names_from_namespaces(namespace, prefix))
            names.extend(_get_type_names_from_types(namespace, prefix))
            prefix.pop()
    
    return names

def get_list_of_types(file_schema : dict) -> list[TypeData]:
    
    data_list = []
    
    data_list.extend(_get_type_names_from_types(file_schema, []))
    data_list.extend(_get_type_names_from_namespaces(file_schema, []))
    
    return data_list

def generate_impls(output_file_path : str, file_schema : dict, cache : types_manager.TypesCache):
    
    function_data_list = get_list_of_types(file_schema)
    if len(function_data_list) == 0:
        return    
    
    type_path = cache.get_path(function_data_list[0].name)
    if type_path is None:
        return
    
    output_directory = os.path.dirname(output_file_path)
    if not os.path.exists(output_directory):
        os.makedirs(output_directory)
    
    output_name = os.path.basename(output_file_path)
    
    parameters = {
        "function_data_list": function_data_list,
        "type_path": type_path,
    }

    t = Templite(filename=current_directory + "/genSchemas/typesImpl.gens")
    file_data = t.render(**parameters)
    with open("{}/{}".format(output_directory, output_name), 'w') as file:
        file.write(file_data)
