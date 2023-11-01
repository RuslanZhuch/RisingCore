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
import injectors

import utils

data1_filename = "assets/injections/data.cpp"
data1_susp_func2_filename = "assets/expected/injections/dataSuspendedFunc2.cpp"

class TestInjectors(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
        
    def test_load_content(self):
        content = injectors.load_content(data1_filename)
        
        real_content_data = utils._try_load_file(data1_filename)
        self.assertEqual(content, real_content_data)

    def test_gather_desc(self):
        content = injectors.load_content(data1_filename)
        desc = injectors.gather_desc(content)

        self.assertEqual(desc.numOfInjections, 4)
        self.assertEqual(len(desc.offsets), 4)
        self.assertEqual(desc.offsets, [0, 0, 0, 0])
        self.assertEqual(desc.segment_names, [
            "func2",
            "func4",    
            "func5",
            "func7",
        ])
        
    def test_suspend_segment(self):
        content = injectors.load_content(data1_filename)
        desc = injectors.gather_desc(content)

        content = injectors.suspend(content, desc, "func2")

        desc = injectors.gather_desc(content)
        self.assertEqual(desc.numOfInjections, 4)
        self.assertEqual(len(desc.offsets), 4)
        self.assertEqual(desc.offsets, [0, 0, 0, 0])
        self.assertEqual(desc.segment_names, [
            "func2",
            "func4",    
            "func5",
            "func7",
        ])

        real_content_data = utils._try_load_file(data1_susp_func2_filename)
        self.assertEqual(content, real_content_data)

