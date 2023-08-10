import re

class ScopeData:
    def __init__(self):
        self.openIndices : list[int] = []
        self.closeIndices : list[int] = []
        
    def append(self, openIndex : int, closeIndex : int):
        self.openIndices.insert(0, openIndex)
        self.closeIndices.insert(0, closeIndex)
        
class FeatureLink:
    def __init__(self, scope_entry_index : int, feature_name : str):
        self.scope_entry_index : int = scope_entry_index
        self.name = feature_name
        
class NamespaceLink(FeatureLink):
    pass

class TypeLink(FeatureLink):
    pass

class VariableData:
    def __init__(self, type_name : str, name : str):
        self.type_name = type_name
        self.name = name
        
class ScopeTreeNode:
    def __init__(self, scope_entry_id : int):
        self.entry = scope_entry_id
        self.feature : FeatureLink = None
        self.variables : list[VariableData] = []
        self.scopes : list[ScopeTreeNode] = []


def get_namespace_entries(data : str) -> list[int]:
    
    found_index : int = -1
    entries = []
    
    while True:
        found_index = data.find("namespace", found_index + 1)
        if found_index == -1:
            break
        entries.append(found_index)
        
    return entries

def get_types_entries(data : str) -> list[int]:
    
    found_index : int = -1
    entries = []
    
    while True:
        found_index = data.find("struct", found_index + 1)
        if found_index == -1:
            break
        entries.append(found_index)
        
    return entries

def get_namespace_name(data : str, namespace_entry : int):
    name_end = data.find("\n", namespace_entry)
    if name_end == -1:
        name_end = data.find("{", namespace_entry)
    if name_end == -1:
        return ""
    data_to_process = data[namespace_entry:name_end]
    keys = data_to_process.split()
    if len(keys) >= 2:
        return keys[1]
    return ""

def get_type_name(data : str, type_entry : int):
    return get_namespace_name(data, type_entry)

def get_scopes_data(data : str) -> ScopeData:
    
    entries = ScopeData()
    
    opened_brackets = []
    
    for id, symbol in enumerate(data):
        if symbol == "{":
            opened_brackets.append(id)
        elif symbol == "}":
            open_scope_index = opened_brackets.pop()
            close_scope_index = id
            entries.append(open_scope_index, close_scope_index)
           
    return entries

def create_namespace_links(namespace_entries : list[int], namespace_names : list[str], scrope_entries : ScopeData) -> list[NamespaceLink]:
    
    objects : list[NamespaceLink] = []
    
    for el_id, namespace_entry in enumerate(namespace_entries):
        min_scope_entry_id = -1
        min_distance = -1
        for id, scope_entry in enumerate(scrope_entries.openIndices):
            distance = scope_entry - namespace_entry
            if distance <= 0:
                continue
            
            if min_distance == -1:
                min_distance = distance
            
            if distance <= min_distance:
                min_distance = distance
                min_scope_entry_id = id
                
        if min_scope_entry_id == -1:
            continue
        
        objects.append(NamespaceLink(min_scope_entry_id, namespace_names[el_id]))
        
    return objects

def create_type_links(type_entries : list[int], type_names : list[str], scrope_entries : ScopeData) -> list[TypeLink]:
    
    objects : list[TypeLink] = []
    
    for el_id, type_entry in enumerate(type_entries):
        min_scope_entry_id = -1
        min_distance = -1
        for id, scope_entry in enumerate(scrope_entries.openIndices):
            distance = scope_entry - type_entry
            if distance <= 0:
                continue
            
            if min_distance == -1:
                min_distance = distance
            
            if distance <= min_distance:
                min_distance = distance
                min_scope_entry_id = id
            
        if min_scope_entry_id == -1:
            continue
        
        objects.append(TypeLink(min_scope_entry_id, type_names[el_id]))
        
    return objects

def find_variables_in_scope(data : str, scope_entry_id : int, scrope_entries : ScopeData) -> list[VariableData]:
    variables : list[VariableData] = []
    
    scope_open = scrope_entries.openIndices[scope_entry_id]
    scope_close = scrope_entries.closeIndices[scope_entry_id]
    
    scope_data = data[scope_open + 1:scope_close]
    
    fields = scope_data.split(";")
    
    for field in fields:
        if "(" in field or ")" in field:
            continue
        keys = [s.strip() for s in re.split("{|}| ", field)]
        keys = list(filter(lambda key : key != "", keys))
        if len(keys) < 2:
            continue
        variables.append(VariableData(keys[0], keys[1]))
    
    return variables

def create_scopes_tree(scrope_entries : ScopeData) -> ScopeTreeNode: 
    root = ScopeTreeNode(-1)
    
    scopes_refs : list[list[int]] = []
    nodes : list[ScopeTreeNode] = []
    
    num_of_entries = len(scrope_entries.openIndices)
    
    for scope_index in range(0, num_of_entries):
        nodes.append(ScopeTreeNode(scope_index))
    
    for scope_index in range(0, num_of_entries):
        scope_open_index = scrope_entries.openIndices[scope_index]
        scope_close_index = scrope_entries.closeIndices[scope_index]
        
        owners : list[int] = []
        node = nodes[scope_index]
        
        for outer_scope_index in range(0, num_of_entries):
            if outer_scope_index == scope_index:
                continue
            outer_scope_open_index = scrope_entries.openIndices[outer_scope_index]
            if outer_scope_open_index > scope_open_index or outer_scope_open_index > scope_close_index:
                continue
            
            outer_scope_close_index = scrope_entries.closeIndices[outer_scope_index]
            if outer_scope_close_index < scope_close_index:
                continue
            
            owners.append(outer_scope_index)
        
        if len(owners) >= 1:
            owners_dist = [scope_index - owner for owner in owners]
            min_dist = min(owners_dist)
            owner_id = owners_dist.index(min_dist)
            nodes[owner_id].scopes.append(node)
        else:
            root.scopes.append(node)
    
    return root

def populate_scopes_tree(scope_tree : ScopeTreeNode, namespace_links : list[NamespaceLink], type_links : list[TypeLink], variables : list[list[VariableData]]):
    
    def populate(scope_tree_node : ScopeTreeNode):
        scope_entry_id = scope_tree_node.entry
        for feature in namespace_links:
            if feature.scope_entry_index == scope_entry_id:
                scope_tree_node.feature = feature
                break
        for el_id, feature in enumerate(type_links):
            if feature.scope_entry_index == scope_entry_id:
                scope_tree_node.feature = feature
                scope_tree_node.variables = variables[el_id]
                break
            
        for node in scope_tree_node.scopes:
            populate(node)
            
    populate(scope_tree)

def create_structure(scope_tree : ScopeTreeNode) -> dict:
    
    structure = dict()
    
    def populate(scope_tree_node : ScopeTreeNode) -> dict:
        structure_node = dict()
        
        namespaces = []
        types = []
        
        if scope_tree_node.feature is not None:
            structure_node["name"] = scope_tree_node.feature.name
            
            variables = []
            for variable in scope_tree_node.variables:
                variables.append({
                    "name": variable.name,
                    "dataType": variable.type_name
                })
                
            if len(variables) > 0:
                structure_node["variables"] = variables
        
        for node in scope_tree_node.scopes:
            if type(node.feature) == NamespaceLink:
                namespaces.append(populate(node))
            elif type(node.feature) == TypeLink:
                types.append(populate(node))
                
        if len(namespaces) > 0:
            structure_node["namespaces"] = namespaces
        if len(types) > 0:
            structure_node["types"] = types

        return structure_node
    
    structure = populate(scope_tree)
    
    return structure
