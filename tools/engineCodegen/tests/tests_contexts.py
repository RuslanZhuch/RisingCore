import unittest

import json

from os import path
import sys
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import contexts
import loader
import types_manager

import gen_base_test
import utils

def load_shared_contexts():
    return contexts.load_contexts([
        "assets/contexts/sContext1.json",
        "assets/contexts/sContext2.json"
    ])

class TestContexts(gen_base_test.TestBaseGen):
    def test_load_context_data(self):
        context_raw_data = loader.load_file_data("assets/contexts/lContext1.json")
        context_data = contexts.load_data(context_raw_data)
        
        self.assertEqual(len(context_data.objects_data), 3)
        self.assertEqual(len(context_data.tables_data), 2)
        
        self.assertEqual(context_data.objects_data[0].name, "var1")
        self.assertEqual(context_data.objects_data[1].name, "var2")
        self.assertEqual(context_data.objects_data[2].name, "var3")
        
        self.assertEqual(context_data.objects_data[0].data_type, "int32_t")
        self.assertEqual(context_data.objects_data[1].data_type, "float")
        self.assertEqual(context_data.objects_data[2].data_type, "Types::Data::CustomData1")
        
        self.assertEqual(context_data.objects_data[0].initial, 1)
        self.assertEqual(context_data.objects_data[1].initial, 1.1)
        self.assertEqual(context_data.objects_data[2].initial, None)
        
        self.assertEqual(context_data.tables_data[0].name, "dbvar1")
        self.assertEqual(context_data.tables_data[1].name, "dbvar2")
        
#        self.assertEqual(context_data.tables_data[0].data_name_list, {"float"})
#        self.assertEqual(context_data.tables_data[1].data_name_list, ("int64_t",))
        
        self.assertEqual(context_data.tables_data[0].capacity, 512)
        self.assertEqual(context_data.tables_data[1].capacity, 1024)
        
    def test_gen_context1_def(self):
        def logic(gen_file_name):
            types_file_data = loader.load_file_data("assets/project1/types_contexts_data.json")
            types_cache = types_manager.cache_types([types_file_data])
            contexts.generate_context_def("dest", "assets/contexts/lContext1.json", types_cache)
        
        self._test_gen(logic, "dest/lContext1Context.h", "assets/expected/lContext1Context.h")
        
    def test_gen_context2_def(self):
        def logic(gen_file_name):
            types_file_data = loader.load_file_data("assets/project1/types_contexts_data.json")
            types_cache = types_manager.cache_types([types_file_data])
            contexts.generate_context_def("dest", "assets/contexts/lContext2.json", types_cache)
        
        self._test_gen(logic, "dest/lContext2Context.h", "assets/expected/lContext2Context.h")

    def test_gen_context6_def(self):
        def logic(gen_file_name):
            types_file_data = loader.load_file_data("assets/project1/types_contexts_data.json")
            types_cache = types_manager.cache_types([types_file_data])
            contexts.generate_context_def("dest", "assets/contexts/lContext6.json", types_cache)
        
        self._test_gen(logic, "dest/lContext6Context.h", "assets/expected/lContext6Context.h")

    def test_gen_context6_impl(self):
        contexts.generate_context_impl("dest", "assets/contexts/lContext6.json")
        utils.assert_files(self, "dest/lContext6Context.cpp", "assets/expected/lContext6Context.cpp")
        
    def test_gen_context1_impl(self):
        contexts.generate_context_impl("dest", "assets/contexts/lContext1.json")
        utils.assert_files(self, "dest/lContext1Context.cpp", "assets/expected/lContext1Context.cpp")
        
    def test_gen_context2_impl(self):
        contexts.generate_context_impl("dest", "assets/contexts/lContext2.json")
        utils.assert_files(self, "dest/lContext2Context.cpp", "assets/expected/lContext2Context.cpp")
        
    def test_load_pool_contexts_merge(self):
        workspace_data = loader.load_runtime_data("assets/project3/ws_runtime.json")
        
        merge_data = contexts.load_pool_context_merge(workspace_data)
        self.assertEqual(len(merge_data), 3)
        
        self.assertIsNotNone(merge_data["poolInst1"])
        self.assertEqual(len(merge_data["poolInst1"]), 1)
        self.assertEqual(merge_data["poolInst1"][0].executor_name, "executor1")
        self.assertEqual(merge_data["poolInst1"][0].executor_scontext, "pool1")
        
        self.assertIsNotNone(merge_data["poolInst2"])
        self.assertEqual(len(merge_data["poolInst2"]), 2)
        self.assertEqual(merge_data["poolInst2"][0].executor_name, "executor1")
        self.assertEqual(merge_data["poolInst2"][0].executor_scontext, "pool2")
        self.assertEqual(merge_data["poolInst2"][1].executor_name, "executor2")
        self.assertEqual(merge_data["poolInst2"][1].executor_scontext, "pool2")
        
        self.assertIsNotNone(merge_data["poolInst3"])
        self.assertEqual(len(merge_data["poolInst3"]), 1)
        self.assertEqual(merge_data["poolInst3"][0].executor_name, "executor4")
        self.assertEqual(merge_data["poolInst3"][0].executor_scontext, "pool3")
