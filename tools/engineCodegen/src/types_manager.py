
class TypesCache:
    def __init__(self, type_names, paths):
        self.type_names = type_names
        self.paths = paths
        
    def get_path(self, type_name):
        id = self.type_names.index(type_name)
        if id < 0:
            return "None"
        
        return self.paths[id]
        
def cache_types(types_data):
    cache = dict()
    
    for type_data in types_data:
        data = type_data["data"]
        for type_name in data:
            cache[type_name] = data[type_name]

    type_names = []
    paths = []
    for type_name in cache:
        type_names.append(type_name)
        paths.append(cache[type_name])
    
    return TypesCache(type_names, paths)

def get_includes_list(cached_types : TypesCache, types_to_include : list[str]) -> list[str]:
    includes_list = []
    
    unique_types = list(set(types_to_include))
    unique_types.sort()
    created_includes = set() 
    for type_to_include in unique_types:
        path = cached_types.get_path(type_to_include)
        if path == "None" or path in created_includes:
            continue
        created_includes.add(path)
        includes_list.append(path)
        
    return includes_list
