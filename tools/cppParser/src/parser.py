import parser_internal

import json
from pathlib import Path
import os

def parse(file_name : str):
    file_data = open(file_name)
    working_data = file_data.read()
    file_data.close()
    
    scope_entries = parser_internal.get_scopes_data(working_data)
    
    scopes_tree = parser_internal.create_scopes_tree(scope_entries)
    
    namespace_entries = parser_internal.get_namespace_entries(working_data)
    namespace_names = [parser_internal.get_namespace_name(working_data, namespace_entry) for namespace_entry in namespace_entries]
    namespace_links = parser_internal.create_namespace_links(namespace_entries, namespace_names, scope_entries)
    
    type_entries = parser_internal.get_types_entries(working_data)
    type_names = [parser_internal.get_type_name(working_data, type_entry) for type_entry in type_entries]
    type_links = parser_internal.create_type_links(type_entries, type_names, scope_entries)
    
    types_variables = [parser_internal.find_variables_in_scope(working_data, type_link.scope_entry_index, scope_entries) for type_link in type_links ]
    
    parser_internal.populate_scopes_tree(scopes_tree, namespace_links, type_links, types_variables)

    return parser_internal.create_structure(scopes_tree)

def parse_to_folder(file_pathes : list[str], output_folder : str) -> list[str]:
    parsed_pathes = []
    
    for file_path in file_pathes:
        parsed_data = parse(file_path)
        
        file_name = Path(file_path).stem
        
        if not os.path.exists(output_folder):
            os.makedirs(output_folder)
            
        output_file = output_folder + "/" + file_name + ".json"
        with open(output_file, "w") as outfile:
            json.dump(parsed_data, outfile)
            parsed_pathes.append(output_file)
            
    return parsed_pathes
            