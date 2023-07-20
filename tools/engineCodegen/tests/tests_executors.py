import unittest

import json

import os
import sys
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import executors
import generator
import runtime
import loader

import utils

EXPECT_NUM_OF_EXECUTORS = 3

def load_executors():
    return executors.load([
        "assets/executors/executor1.json",
        "assets/executors/executor2.json",
        "assets/executors/executor3.json"
    ])

def load_executors_for_class():
    return executors.load(["assets/executors/classGeneration/test1.json"])

def create_target_file():
    return runtime.generate_runtime_file("dest")

class TestExecutors(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
        
    def test_load_executors(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
    def test_get_executor_name(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        self.assertEqual(executors.get_name(executors_data[0]), "executor1")
        self.assertEqual(executors.get_name(executors_data[1]), "executor2")
        
    def test_load_shared_contexts_usage(self):
        workspace_data = loader.load_application_context_data("assets/workspace/ws_applicationContext.json")
        
        usage = executors.load_shared_context_usage(workspace_data)
        self.assertEqual(len(usage), 2)
        
        self.assertIsNotNone(usage["executor2"])
        self.assertEqual(len(usage["executor2"]), 4)
        self.assertEqual(usage["executor2"][0].shared_instance, "sharedInst1")
        self.assertEqual(usage["executor2"][0].executor_scontext, "shared1")
        self.assertEqual(usage["executor2"][1].shared_instance, "sharedInst2")
        self.assertEqual(usage["executor2"][1].executor_scontext, "shared2")
        self.assertEqual(usage["executor2"][2].shared_instance, "sharedInst3")
        self.assertEqual(usage["executor2"][2].executor_scontext, "shared3")
        self.assertEqual(usage["executor2"][3].shared_instance, "sharedInst4")
        self.assertEqual(usage["executor2"][3].executor_scontext, "shared4")
        
        self.assertIsNotNone(usage["executor3"])
        self.assertEqual(len(usage["executor3"]), 2)
        self.assertEqual(usage["executor3"][0].shared_instance, "sharedInst1")
        self.assertEqual(usage["executor3"][0].executor_scontext, "shared1_2")
        self.assertEqual(usage["executor3"][1].shared_instance, "sharedInst3")
        self.assertEqual(usage["executor3"][1].executor_scontext, "shared2_2")
        
    def test_gen_executor_1_shared_context_init(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = generator.generate_file("dest", "gen_executor_1_shared_context_init.cpp")
        self.assertIsNotNone(handler)
        
        workspace_data = loader.load_application_context_data("assets/workspace/ws_applicationContext.json")
        
        executors.gen_shared_context_init(handler, executors_data[0], workspace_data)
        handler.close()
        
        descriptor_file = open("dest/gen_executor_1_shared_context_init.cpp", "r")
        file_data = descriptor_file.read()
        self.assertEqual(file_data, "")
        
    def test_gen_executor_2_shared_context_init(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = generator.generate_file("dest", "gen_executor_2_shared_context_init.cpp")
        self.assertIsNotNone(handler)
        
        workspace_data = loader.load_application_context_data("assets/workspace/ws_applicationContext.json")
        
        executors.gen_shared_context_init(handler, executors_data[1], workspace_data)
        handler.close()
        
        utils.assert_files(self, "dest/gen_executor_2_shared_context_init.cpp", "assets/expected/gen_executor_2_shared_context_init.cpp")
                
    def test_gen_executor_headers(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = generator.generate_file("dest", "executors_headers.cpp")
        self.assertIsNotNone(handler)
        
        executors.gen_headers(handler, executors_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/executors_headers.cpp", "assets/expected/executors_headers.cpp")
                
    def test_gen_executor_init(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        workspace_data = loader.load_application_context_data("assets/workspace/ws_applicationContext.json")
        executors.gen_inits(handler, executors_data, workspace_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/runtime.cpp", "assets/expected/executorsGeneration.cpp")
        
    def test_gen_executor_update(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        executors.gen_updates(handler, executors_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/runtime.cpp", "assets/expected/executorsUpdates.cpp")
        
    def test_gen_executor_flush(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        executors.gen_flush(handler, executors_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/runtime.cpp", "assets/expected/executorsFlush.cpp")
        
    def test_gen_executor_1_body_flush(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = generator.generate_file("dest", "gen_executor_1_body_flush.cpp")
        self.assertIsNotNone(handler)
        
        executors.gen_body_flush(handler, executors_data[0])
        handler.close()
        
        descriptor_file = open("dest/gen_executor_1_body_flush.cpp", "r")
        file_data = descriptor_file.read()
        self.assertEqual(file_data, "")
        
    def test_gen_executor_2_body_flush(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = generator.generate_file("dest", "gen_executor_2_body_flush.cpp")
        self.assertIsNotNone(handler)
        
        executors.gen_body_flush(handler, executors_data[1])
        handler.close()
        
        utils.assert_files(self, "dest/gen_executor_2_body_flush.cpp", "assets/expected/gen_executor_2_body_flush.cpp")
        
#    def test_gen_executor_1_body_memory(self):
#        executors_data = load_executors()
#        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
#        
#        handler = generator.generate_file("dest", "gen_executor_1_body_memory.cpp")
#        self.assertIsNotNone(handler)
#        
#        executors.gen_body_memory(handler, executors_data[0])
#        handler.close()
#        
#        descriptor_file = open("dest/gen_executor_1_body_memory.cpp", "r")
#        file_data = descriptor_file.read()
#        self.assertEqual(file_data, "int32_t header{};\nthis->memory.allocate(2048);\n")
#        
#    def test_gen_executor_2_body_memory(self):
#        executors_data = load_executors()
#        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
#        
#        handler = generator.generate_file("dest", "gen_executor_2_body_memory.cpp")
#        self.assertIsNotNone(handler)
#        
#        executors.gen_body_memory(handler, executors_data[1])
#        handler.close()
#        
#        descriptor_file = open("dest/gen_executor_2_body_memory.cpp", "r")
#        file_data = descriptor_file.read()
#        self.assertEqual(file_data, "int32_t header{};\nthis->memory.allocate(4096);\n")
        
    def test_gen_executor_1_body_contexts_load(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = generator.generate_file("dest", "gen_executor_1_body_contexts_load.cpp")
        self.assertIsNotNone(handler)
        
        executors.gen_body_contexts_load(handler, executors_data[0])
        handler.close()
        
        descriptor_file = open("dest/gen_executor_1_body_contexts_load.cpp", "r")
        file_data = descriptor_file.read()
        self.assertEqual(file_data, "")
        
    def test_gen_executor_2_body_contexts_load(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = generator.generate_file("dest", "gen_executor_2_body_contexts_load.cpp")
        self.assertIsNotNone(handler)
        
        executors.gen_body_contexts_load(handler, executors_data[1])
        handler.close()
        
        utils.assert_files(self, "dest/gen_executor_2_body_contexts_load.cpp", "assets/expected/gen_executor_2_body_contexts_load.cpp")
    
    def test_gen_executor_1_body_updateImpl(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = generator.generate_file("dest", "gen_executor_1_body_update.cpp")
        self.assertIsNotNone(handler)
        
        executors.gen_body_update(handler, executors_data[0])
        handler.close()
        
        descriptor_file = open("dest/gen_executor_1_body_update.cpp", "r")
        file_data = descriptor_file.read()
        self.assertEqual(file_data, "this->updateImpl(dt);\n")
        
    def test_gen_executor_1_body_init(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = generator.generate_file("dest", "gen_executor_1_body_initImpl.cpp")
        self.assertIsNotNone(handler)
        
        executors.gen_body_init(handler, executors_data[0])
        handler.close()
        
        descriptor_file = open("dest/gen_executor_1_body_initImpl.cpp", "r")
        file_data = descriptor_file.read()
        self.assertEqual(file_data, "this->initImpl();\n")
        
    def test_gen_executor_1_contexts_decl(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = generator.generate_file("dest", "gen_executor_1_contexts_decl.cpp")
        self.assertIsNotNone(handler)
        
        def class_data(class_handler):
            executors.gen_contexts_decl(class_handler, executors_data[0])
            
        generator.generate_class(handler, "Test", class_data)
        handler.close()
        
        utils.assert_files(self, "dest/gen_executor_1_contexts_decl.cpp", "assets/expected/gen_executor_1_contexts_decl.cpp")
                
    def test_gen_executor_2_contexts_decl(self):
        executors_data = load_executors()
        self.assertEqual(len(executors_data), EXPECT_NUM_OF_EXECUTORS)
        
        handler = generator.generate_file("dest", "gen_executor_2_contexts_decl.cpp")
        self.assertIsNotNone(handler)
        
        def class_data(class_handler):
            executors.gen_contexts_decl(class_handler, executors_data[1])
            
        generator.generate_class(handler, "Test", class_data)
        handler.close()
        
        utils.assert_files(self, "dest/gen_executor_2_contexts_decl.cpp", "assets/expected/gen_executor_2_contexts_decl.cpp")
        
    def test_gen_executor_test1_header(self):
        executors_data = load_executors_for_class()
        self.assertEqual(len(executors_data), 1)
        
        executors.gen_header("dest", executors_data[0])
        
        utils.assert_files(self, "dest/Test1Executor.h", "assets/expected/Test1Executor.h")

    def test_gen_executor_test1_source(self):
        executors_data = load_executors_for_class()
        self.assertEqual(len(executors_data), 1)
        
        executors.gen_source("dest", executors_data[0])
        
        utils.assert_files(self, "dest/Test1Executor.cpp", "assets/expected/Test1Executor.cpp")
        
    def test_gen_executor_test1_impl(self):
        file_full_path = "dest/Test1ExecutorImpl.cpp"
        if os.path.isfile(file_full_path):
            os.remove(file_full_path)
        
        executors_data = load_executors_for_class()
        self.assertEqual(len(executors_data), 1)
        
        executors.gen_implementation("dest", executors_data[0])
        
        utils.assert_files(self, file_full_path, "assets/expected/Test1ExecutorImpl.cpp")
        
        file_modified = open("assets/expected/Test1ExecutorImplModified.cpp")
        file_modified_data = file_modified.read()
        
        file_to_write = open(file_full_path, "w")
        
        file_to_write.write(file_modified_data)
        
        file_to_write.close()
        
        file_updated = open(file_full_path)
        file_updated_data = file_updated.read()
        
        self.assertEqual(file_updated_data, file_modified_data)
        
        file_updated.close()
        
        executors.gen_implementation("dest", executors_data[0])
        utils.assert_files(self, file_full_path, "assets/expected/Test1ExecutorImplModified.cpp")
        