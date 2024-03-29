import unittest

import json

from os import path
import sys
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import generator
import contexts
import loader
import types_manager

import utils

def load_shared_contexts():
    return contexts.load_contexts([
        "assets/contexts/sContext1.json",
        "assets/contexts/sContext2.json"
    ])

class TestContexts(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
    
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
        
    def test_gen_context1_buffer_decls(self):
        context_raw_data = loader.load_file_data("assets/contexts/lContext1.json")
        
        handler = generator.generate_file("dest", "gen_lContext1_buffer_decls.cpp")
        self.assertIsNotNone(handler)
        context_data = contexts.load_data(context_raw_data)
        contexts.generate_context_buffer_decls(handler, context_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext1_buffer_decls.cpp", "assets/expected/gen_lContext1_buffer_decls.cpp")
        

    def test_gen_context1_data(self):
        context_raw_data = loader.load_file_data("assets/contexts/lContext1.json")
        
        handler = generator.generate_file("dest", "gen_lContext1_data.cpp")
        self.assertIsNotNone(handler)
        context_data = contexts.load_data(context_raw_data)
        contexts.generate_context_data(handler, context_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext1_data.cpp", "assets/expected/gen_lContext1_data.cpp")
        
    def test_gen_context1_def(self):
        types_file_data = loader.load_file_data("assets/project1/types_contexts_data.json")
        types_cache = types_manager.cache_types([types_file_data])
        
        contexts.generate_context_def("dest", "assets/contexts/lContext1.json", types_cache)
        utils.assert_files(self, "dest/lContext1Context.h", "assets/expected/lContext1Context.h")
        
    def test_gen_context2_def(self):
        types_file_data = loader.load_file_data("assets/project1/types_contexts_data.json")
        types_cache = types_manager.cache_types([types_file_data])
        
        contexts.generate_context_def("dest", "assets/contexts/lContext2.json", types_cache)
        utils.assert_files(self, "dest/lContext2Context.h", "assets/expected/lContext2Context.h")
        
    def test_gen_context1_load(self):
        file_path = "assets/contexts/lContext1.json"
        context_raw_data = loader.load_file_data(file_path)
        
        handler = generator.generate_file("dest", "gen_lContext1_load.cpp")
        self.assertIsNotNone(handler)
        contexts.generate_context_load(handler, context_raw_data, file_path)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext1_load.cpp", "assets/expected/gen_lContext1_load.cpp")
          
    def test_gen_context2_load(self):
        file_path = "assets/contexts/lContext2.json"
        context_raw_data = loader.load_file_data(file_path)
        
        handler = generator.generate_file("dest", "gen_lContext2_load.cpp")
        self.assertIsNotNone(handler)
        contexts.generate_context_load(handler, context_raw_data, file_path)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext2_load.cpp", "assets/expected/gen_lContext2_load.cpp")
      
    def test_gen_context1_reset(self):
        file_path = "assets/contexts/lContext1.json"
        context_raw_data = loader.load_file_data(file_path)
        
        handler = generator.generate_file("dest", "gen_lContext1_reset.cpp")
        self.assertIsNotNone(handler)
        contexts.generate_context_reset(handler, context_raw_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext1_reset.cpp", "assets/expected/gen_lContext1_reset.cpp")
          
    def test_gen_context1_merge(self):
        file_path = "assets/contexts/lContext1.json"
        context_raw_data = loader.load_file_data(file_path)
        
        handler = generator.generate_file("dest", "gen_lContext1_merge.cpp")
        self.assertIsNotNone(handler)
        contexts.generate_context_merge(handler, context_raw_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext1_merge.cpp", "assets/expected/gen_lContext1_merge.cpp")
          
    def test_gen_context4_merge(self):
        file_path = "assets/contexts/lContext4.json"
        context_raw_data = loader.load_file_data(file_path)
        
        handler = generator.generate_file("dest", "gen_lContext4_merge.cpp")
        self.assertIsNotNone(handler)
        contexts.generate_context_merge(handler, context_raw_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext4_merge.cpp", "assets/expected/gen_lContext4_merge.cpp")
          
    def test_gen_context5_merge(self):
        file_path = "assets/contexts/lContext5.json"
        context_raw_data = loader.load_file_data(file_path)
        
        handler = generator.generate_file("dest", "gen_lContext5_merge.cpp")
        self.assertIsNotNone(handler)
        contexts.generate_context_merge(handler, context_raw_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext5_merge.cpp", "assets/expected/gen_lContext5_merge.cpp")
          
    def test_gen_context6_data(self):
        context_raw_data = loader.load_file_data("assets/contexts/lContext6.json")
        
        handler = generator.generate_file("dest", "gen_lContext6_data.cpp")
        self.assertIsNotNone(handler)
    
        context_data = contexts.load_data(context_raw_data)
        contexts.generate_context_data(handler, context_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext6_data.cpp", "assets/expected/gen_lContext6_data.cpp")
        
    def test_gen_context6_data_convertion(self):
        context_raw_data = loader.load_file_data("assets/contexts/lContext6.json")
        
        handler = generator.generate_file("dest", "gen_lContext6_data_convertion.cpp")
        self.assertIsNotNone(handler)
    
        context_data = contexts.load_data(context_raw_data)
        contexts.generate_data_converter(handler, context_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext6_data_convertion.cpp", "assets/expected/gen_lContext6_data_convertion.cpp")
        
    def test_gen_context6_def(self):
        types_file_data = loader.load_file_data("assets/project1/types_contexts_data.json")
        types_cache = types_manager.cache_types([types_file_data])
        
        contexts.generate_context_def("dest", "assets/contexts/lContext6.json", types_cache)
        utils.assert_files(self, "dest/lContext6Context.h", "assets/expected/lContext6Context.h")

    def test_gen_context6_reset(self):
        file_path = "assets/contexts/lContext6.json"
        context_raw_data = loader.load_file_data(file_path)
        
        handler = generator.generate_file("dest", "gen_lContext6_reset.cpp")
        self.assertIsNotNone(handler)
        contexts.generate_context_reset(handler, context_raw_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext6_reset.cpp", "assets/expected/gen_lContext6_reset.cpp")
          
    def test_gen_context6_merge(self):
        file_path = "assets/contexts/lContext6.json"
        context_raw_data = loader.load_file_data(file_path)
        
        handler = generator.generate_file("dest", "gen_lContext6_merge.cpp")
        self.assertIsNotNone(handler)
        contexts.generate_context_merge(handler, context_raw_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext6_merge.cpp", "assets/expected/gen_lContext6_merge.cpp")
        
    def test_gen_context6_load(self):
        file_path = "assets/contexts/lContext6.json"
        context_raw_data = loader.load_file_data(file_path)
        
        handler = generator.generate_file("dest", "gen_lContext6_load.cpp")
        self.assertIsNotNone(handler)
        contexts.generate_context_load(handler, context_raw_data, file_path)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext6_load.cpp", "assets/expected/gen_lContext6_load.cpp")
        
    def test_gen_context6_getters(self):
        file_path = "assets/contexts/lContext6.json"
        context_raw_data = loader.load_file_data(file_path)
        
        handler = generator.generate_file("dest", "gen_lContext6_getters.cpp")
        self.assertIsNotNone(handler)
        context_data = contexts.load_data(context_raw_data)
        contexts.generate_context_getters(handler, context_data, "LContext6")
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext6_getters.cpp", "assets/expected/gen_lContext6_getters.cpp")
      
    def test_gen_context6_setters(self):
        file_path = "assets/contexts/lContext6.json"
        context_raw_data = loader.load_file_data(file_path)
        
        handler = generator.generate_file("dest", "gen_lContext6_setters.cpp")
        self.assertIsNotNone(handler)
        context_data = contexts.load_data(context_raw_data)
        contexts.generate_context_setters(handler, context_data, "LContext6")
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext6_setters.cpp", "assets/expected/gen_lContext6_setters.cpp")
      
    def test_gen_context6_impl(self):
        contexts.generate_context_impl("dest", "assets/contexts/lContext6.json")
        utils.assert_files(self, "dest/lContext6Context.cpp", "assets/expected/lContext6Context.cpp")
        
    def test_gen_context8_load(self):
        file_path = "assets/contexts/lContext8.json"
        context_raw_data = loader.load_file_data(file_path)
        
        handler = generator.generate_file("dest", "gen_lContext8_load.cpp")
        self.assertIsNotNone(handler)
        contexts.generate_context_load(handler, context_raw_data, file_path)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_lContext8_load.cpp", "assets/expected/gen_lContext8_load.cpp")
      
    def test_gen_context1_impl(self):
        contexts.generate_context_impl("dest", "assets/contexts/lContext1.json")
        utils.assert_files(self, "dest/lContext1Context.cpp", "assets/expected/lContext1Context.cpp")
        
    def test_gen_context2_impl(self):
        contexts.generate_context_impl("dest", "assets/contexts/lContext2.json")
        utils.assert_files(self, "dest/lContext2Context.cpp", "assets/expected/lContext2Context.cpp")
        
    def test_load_shared_context_instancies(self):
        data = contexts.load_shared_context_instances("assets/project1/ws_runtime.json")
        self.assertEqual(len(data), 5)
        
        self.assertEqual(data[0].context_name, "sContext1")
        self.assertEqual(data[0].instance_name, "sharedInst1")
        
        self.assertEqual(data[1].context_name, "sContext1")
        self.assertEqual(data[1].instance_name, "sharedInst2")
        
        self.assertEqual(data[2].context_name, "sContext2")
        self.assertEqual(data[2].instance_name, "sharedInst3")
        
        self.assertEqual(data[3].context_name, "sContext2")
        self.assertEqual(data[3].instance_name, "sharedInst4")
        
        self.assertEqual(data[4].context_name, "sContext3")
        self.assertEqual(data[4].instance_name, "sharedInst5")
        
    def test_get_valid_shared_context_instancies(self):
        contexts_data = load_shared_contexts()
        instances_data = contexts.load_shared_context_instances("assets/project1/ws_runtime.json")
        
        validated_instances = contexts.get_validated_context_instances(contexts_data, instances_data)
        self.assertEqual(len(validated_instances), 4)
        self.assertEqual(validated_instances[0].context_name, "sContext1")
        self.assertEqual(validated_instances[0].instance_name, "sharedInst1")
        
        self.assertEqual(validated_instances[1].context_name, "sContext1")
        self.assertEqual(validated_instances[1].instance_name, "sharedInst2")
        
        self.assertEqual(validated_instances[2].context_name, "sContext2")
        self.assertEqual(validated_instances[2].instance_name, "sharedInst3")
        
        self.assertEqual(validated_instances[3].context_name, "sContext2")
        self.assertEqual(validated_instances[3].instance_name, "sharedInst4")
        
    def test_load_pool_shared_context_instancies(self):
        data = contexts.load_shared_context_instances("assets/project3/ws_runtime.json")
        self.assertEqual(len(data), 3)
        
        self.assertEqual(data[0].context_name, "sContext1")
        self.assertEqual(data[0].instance_name, "poolInst1")
        
        self.assertEqual(data[1].context_name, "sContext1")
        self.assertEqual(data[1].instance_name, "poolInst2")
        
        self.assertEqual(data[2].context_name, "sContext2")
        self.assertEqual(data[2].instance_name, "poolInst3")
        
    def test_generate_shared_contexts_init(self):
        instances_data = contexts.load_shared_context_instances("assets/project1/ws_runtime.json")
        handler = generator.generate_file("dest", "gen_scontexts_init.cpp")
        self.assertIsNotNone(handler)
        contexts.generate_shared_init(handler, instances_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_scontexts_init.cpp", "assets/expected/gen_scontexts_init.cpp")
        
    def test_generate_shared_contexts_usage(self):
        instances_data = contexts.load_shared_context_instances("assets/project3/ws_runtime.json")
        handler = generator.generate_file("dest", "gen_scontexts_usage.cpp")
        self.assertIsNotNone(handler)
        pool_id = 1
        contexts.generate_shared_usage(handler, instances_data, pool_id, [
            "poolInst1",
            "poolInst2",
            "poolInst3"
        ])
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_scontexts_usage.cpp", "assets/expected/gen_scontexts_usage.cpp")
        
    def test_get_shared_contexts_flush(self):
        workspace_data = loader.load_runtime_data("assets/project1/ws_runtime.json")
        flush_data = contexts.load_shared_context_to_flush(workspace_data)
        self.assertEqual(len(flush_data), 3)
        self.assertEqual(flush_data[0], "sharedInst1")
        self.assertEqual(flush_data[1], "sharedInst2")
        self.assertEqual(flush_data[2], "sharedInst4")
        
    def test_generate_shared_contexts_flush(self):
        handler = generator.generate_file("dest", "gen_shared_contexts_flush.cpp")
        self.assertIsNotNone(handler)
        workspace_data = loader.load_runtime_data("assets/project1/ws_runtime.json")
        contexts.generate_shared_flush(handler, workspace_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_shared_contexts_flush.cpp", "assets/expected/gen_shared_contexts_flush.cpp")
       
    def test_generate_pools_contexts_flush(self):
        handler = generator.generate_file("dest", "gen_pools_contexts_flush.cpp")
        self.assertIsNotNone(handler)
        workspace_data = loader.load_runtime_data("assets/project3/ws_runtime.json")
        contexts.generate_pools_flush(handler, workspace_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_pools_contexts_flush.cpp", "assets/expected/gen_pools_contexts_flush.cpp")
              
    def test_load_shared_contexts_merge(self):
        workspace_data = loader.load_runtime_data("assets/project1/ws_runtime.json")
        
        merge_data = contexts.load_shared_context_merge(workspace_data)
        self.assertEqual(len(merge_data), 2)
        
        self.assertIsNotNone(merge_data["sharedInst1"])
        self.assertEqual(len(merge_data["sharedInst1"]), 2)
        self.assertEqual(merge_data["sharedInst1"][0].executor_name, "executor2")
        self.assertEqual(merge_data["sharedInst1"][0].executor_scontext, "target1")
        self.assertEqual(merge_data["sharedInst1"][1].executor_name, "executor2")
        self.assertEqual(merge_data["sharedInst1"][1].executor_scontext, "target2")
        
        self.assertIsNotNone(merge_data["sharedInst3"])
        self.assertEqual(len(merge_data["sharedInst3"]), 1)
        self.assertEqual(merge_data["sharedInst3"][0].executor_name, "executor3")
        self.assertEqual(merge_data["sharedInst3"][0].executor_scontext, "shared3")
                        
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
                        
    def test_generate_shared_contexts_merge(self):
        handler = generator.generate_file("dest", "gen_shared_contexts_merge.cpp")
        self.assertIsNotNone(handler)
        workspace_data = loader.load_runtime_data("assets/project1/ws_runtime.json")
        contexts.generate_shared_merge(handler, workspace_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_shared_contexts_merge.cpp", "assets/expected/gen_shared_contexts_merge.cpp")
                        
    def test_generate_shared_contexts_merge_with_pools(self):
        handler = generator.generate_file("dest", "gen_shared_contexts_merge_with_pools.cpp")
        self.assertIsNotNone(handler)
        workspace_data = loader.load_runtime_data("assets/project3/ws_runtime.json")
        contexts.generate_shared_merge(handler, workspace_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_shared_contexts_merge_with_pools.cpp", "assets/expected/gen_shared_contexts_merge_with_pools.cpp")

    def test_generate_pools_merge(self):
        handler = generator.generate_file("dest", "gen_pools_merge.cpp")
        self.assertIsNotNone(handler)
        workspace_data = loader.load_runtime_data("assets/project3/ws_runtime.json")
        contexts.generate_pools_merge(handler, workspace_data, 0)
        contexts.generate_pools_merge(handler, workspace_data, 1)
        contexts.generate_pools_merge(handler, workspace_data, 2)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_pools_merge.cpp", "assets/expected/gen_pools_merge.cpp")
       