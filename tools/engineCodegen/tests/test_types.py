import unittest

from os import path
import sys
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import utils
import generator
import loader
import types_manager
import types_generator

class TestTyoes(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
    
    def test_load_types_data(self):
        types_default_file_data = loader.load_file_data("assets/workspace/types_default.json")
        types_additional_file_data = loader.load_file_data("assets/workspace/types_additional.json")
        
        types_cache = types_manager.cache_types([types_default_file_data, types_additional_file_data])
        self.assertEqual(len(types_cache.type_names), 6)
        self.assertEqual(len(types_cache.paths), 6)
        
        self.assertEqual(types_cache.get_path("int32_t"), "None")
        self.assertEqual(types_cache.get_path("float"), "None")
        self.assertEqual(types_cache.get_path("int64_t"), "None")
        self.assertEqual(types_cache.get_path("Engine::String"), "engine/String.h")
        self.assertEqual(types_cache.get_path("bool"), "None")
        self.assertEqual(types_cache.get_path("CryingCat"), "cats/Crying.h")
        
    def test_gen_types_includes(self):
        
        types_default_file_data = loader.load_file_data("assets/workspace/types_default.json")
        types_additional_file_data = loader.load_file_data("assets/workspace/types_additional.json")
        types_cache = types_manager.cache_types([types_default_file_data, types_additional_file_data])
        
        handler = generator.generate_file("dest", "gen_types_includes1.cpp")
        self.assertIsNotNone(handler)
        types_manager.gen_includes(handler, types_cache, ["float", "float", "bool", "CryingCat", "Engine::String", "int32_t", "CryingCat"])
        handler.close()
        
        utils.assert_files(self, "dest/gen_types_includes1.cpp", "assets/expected/gen_types_includes1.cpp")

    def test_get_list_of_types(self):
        
        type_schema_data = loader.load_file_data("assets/types/group1Deser.json")
        types = types_generator.get_list_of_types(type_schema_data)
        self.assertEqual(len(types), 1)
        self.assertEqual(types[0].name, "Types::Group1::Type1")
        self.assertEqual(len(types[0].variables), 2)
        self.assertEqual(types[0].variables[0], "x")
        self.assertEqual(types[0].variables[1], "y")
        
    def test_get_list_of_variables(self):
        
        type_schema_data = loader.load_file_data("assets/types/group1DeserType1.json")
        variables = types_generator.get_list_of_variables(type_schema_data)
        self.assertEqual(len(variables), 2)
        self.assertEqual(variables[0], "x")
        self.assertEqual(variables[1], "y")

    def test_gen_type_impl(self):
        
        types_file_data = loader.load_file_data("assets/types/types_generated.json")
        types_cache = types_manager.cache_types([types_file_data])
        
        type_schema_data = loader.load_file_data("assets/types/group1Deser.json")
        types_generator.generate_impls("dest/types/group1Deser.cpp", type_schema_data, types_cache)
        
        utils.assert_files(self, "dest/types/group1Deser.cpp", "assets/expected/types/group1Deser.cpp")
        