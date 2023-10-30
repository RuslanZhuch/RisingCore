import unittest

import json

from os import path
import sys
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import os
import runtime

import shutil
import utils
import contexts
import loader
import executors

class TestRuntime(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
        
    def test_generate_runtime_file(self):
        handler = runtime.generate_runtime_file("dest")
        self.assertIsNotNone(handler)
        descriptor_file = open("dest/runtime.cpp")
                
    def test_gather_runtime_usage_for_executor(self):
        runtime_data = loader.load_runtime_data("assets/project1/ws_runtime.json")
        shared_usage_list = runtime.gather_shared_usage_for_executor(runtime_data, "executor2")

        self.assertEqual(len(shared_usage_list), 4)

        self.assertEqual(shared_usage_list[0].executor_context_name, "shared1")
        self.assertEqual(shared_usage_list[0].shared_context_instance_name, "sharedInst1")

        self.assertEqual(shared_usage_list[1].executor_context_name, "shared2")
        self.assertEqual(shared_usage_list[1].shared_context_instance_name, "sharedInst2")

        self.assertEqual(shared_usage_list[2].executor_context_name, "shared3")
        self.assertEqual(shared_usage_list[2].shared_context_instance_name, "sharedInst3")

        self.assertEqual(shared_usage_list[3].executor_context_name, "shared4")
        self.assertEqual(shared_usage_list[3].shared_context_instance_name, "sharedInst4")
      
    def test_gather_runtime_usage_for_executor_with_pools(self):
        runtime_data = loader.load_runtime_data("assets/project3/ws_runtime.json")
        shared_usage_list = runtime.gather_shared_usage_for_executor(runtime_data, "executor4")

        self.assertEqual(len(shared_usage_list), 2)

        self.assertEqual(shared_usage_list[0].executor_context_name, "pool2")
        self.assertEqual(shared_usage_list[0].shared_context_instance_name, "poolInst2")

        self.assertEqual(shared_usage_list[1].executor_context_name, "pool3")
        self.assertEqual(shared_usage_list[1].shared_context_instance_name, "poolInst3")

    def test_generate_runtime_function(self):        
        executors_data = executors.load([
            "assets/executors/executor1.json",
            "assets/executors/executor2.json",
            "assets/executors/executor3.json"
        ])
        contexts_file_list = [
            "assets/contexts/sContext1.json",
            "assets/contexts/sContext2.json"
        ]
        
        contexts_data = contexts.load_contexts(contexts_file_list)
        
        runtime.generate("dest", executors_data, "assets/project1/ws_runtime.json", contexts_data)
        
        utils.assert_files(self, "dest/runtime.cpp", "assets/expected/runtime.cpp")
                
    def test_generate_runtime_with_pools(self):
        if not os.path.exists('dest/projects/project3'):
            os.makedirs('dest/projects/project3')
             
        executors_data = executors.load([
            "assets/project3/executors/executor1.json",
            "assets/project3/executors/executor2.json",
            "assets/project3/executors/executor3.json",
            "assets/project3/executors/executor4.json",
            "assets/project3/executors/executor5.json",
            "assets/project3/executors/executor6.json"
        ])
        contexts_file_list = [
            "assets/project3/contexts/sContext1.json",
            "assets/project3/contexts/sContext2.json"
        ]
        
        contexts_data = contexts.load_contexts(contexts_file_list)
        
        runtime.generate("dest/projects/project3", executors_data, "assets/project3/ws_runtime.json", contexts_data)
        
        utils.assert_files(self, "dest/projects/project3/runtime.cpp", "assets/expected/project3/runtime.cpp")
