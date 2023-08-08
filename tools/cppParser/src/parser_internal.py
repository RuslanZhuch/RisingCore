
class ScopeData:
    def __init__(self):
        self.openIndices : list[int] = []
        self.closeIndices : list[int] = []
        
    def append(self, openIndex : int, closeIndex : int):
        self.openIndices.insert(0, openIndex)
        self.closeIndices.insert(0, closeIndex)
        
class TypeObject:
    def __init__(self, scope_entry_index : int):
        self.type_objects : list[TypeObject] = []
        self.scope_entry_index : int = scope_entry_index
        
    def set_type_objects(self, new_type_objects):
        self.type_objects = new_type_objects
        
class NamespaceObject:
    def __init__(self, scope_entry_index : int):
        self.namespace_objects : list[NamespaceObject] = []
        self.type_objects : list[TypeObject] = []
        self.scope_entry_index : int = scope_entry_index
        
    def set_type_objects(self, new_type_objects):
        self.type_objects = new_type_objects
        
class ScopeTreeNode:
    def __init__(self, scope_entry_id : int):
        self.entry = scope_entry_id
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

def get_scopes_data(data : str) -> ScopeData:
    
    entries = ScopeData()
    
    opened_brackets = []
    
    for id, symbol in enumerate(data):
        if symbol == "{":
            opened_brackets.append(id)
        elif symbol == "}":
            open_scope_index = opened_brackets.pop()
            close_scope_index = id
            #print("open_scope_index creation", open_scope_index)
            entries.append(open_scope_index, close_scope_index)
           
    return entries

def create_namespace_objects(namespace_entries : list[int], scrope_entries : ScopeData) -> list[NamespaceObject]:
    
    namespace_scope_indices = []
    
    for namespace_entry in namespace_entries:
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
            
        namespace_scope_indices.append(min_scope_entry_id)
        
    objects : list[NamespaceObject] = []
    
    for namespace_scope_index in namespace_scope_indices:
        if namespace_scope_index == -1:
            continue
        
        objects.append(NamespaceObject(namespace_scope_index))

    return objects

def create_type_objects(type_entries : list[int], scrope_entries : ScopeData) -> list[TypeObject]:
    
    type_scope_indices = []
    
    for type_entry in type_entries:
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
            
        type_scope_indices.append(min_scope_entry_id)
        
    objects : list[TypeObject] = []
    
    for namespace_scope_index in type_scope_indices:
        if namespace_scope_index == -1:
            continue
        
        objects.append(TypeObject(namespace_scope_index))

    return objects

def create_scopes_tree(scrope_entries : ScopeData) -> ScopeTreeNode: 
    root = ScopeTreeNode(-1)
    
    scopes_refs : list[list[int]] = []
    nodes : list[ScopeTreeNode] = []
    
    num_of_entries = len(scrope_entries.openIndices)
    
    for scope_index in range(0, num_of_entries):
        nodes.append(ScopeTreeNode(scope_index))
    
    inner_nodes = set()
    
    for scope_index in range(0, num_of_entries):
        scope_open_index = scrope_entries.openIndices[scope_index]
        scope_close_index = scrope_entries.closeIndices[scope_index]
        
        refs : list[int] = []
        node = nodes[scope_index]
        
        for inner_scope_index in range(0, num_of_entries):
            if inner_scope_index == scope_index:
                continue
            inner_scope_open_index = scrope_entries.openIndices[inner_scope_index]
            if inner_scope_open_index < scope_open_index or inner_scope_open_index > scope_close_index:
                continue
            
            inner_scope_close_index = scrope_entries.closeIndices[inner_scope_index]
            if inner_scope_close_index > scope_close_index:
                continue
            
            refs.append(inner_scope_index)
            node.scopes.append(nodes[inner_scope_index])
            inner_nodes.add(inner_scope_index)
            
        scopes_refs.append(refs)
    
    for scope_index in range(0, num_of_entries):
        if scope_index not in inner_nodes:
            root.scopes.append(nodes[scope_index])
            
    return root
    