import unittest

import json

from os import path
import sys
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import runtime

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
        
        runtime.generate("dest", executors_data, "assets/workspace/ws_applicationContext.json", contexts_data)
        
        utils.assert_files(self, "dest/runtime.cpp", "assets/expected/runtime.cpp")
