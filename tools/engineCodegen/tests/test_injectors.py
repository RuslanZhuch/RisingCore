import unittest

import json

import os
import sys
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import injectors

import utils

data1_filename = "assets/injections/data.cpp"
data1_susp_func2_filename = "assets/injections/dataSuspendedFunc2.cpp"
data1_empty_filename = "assets/injections/dataEmpty.cpp"
exp_data1_susp_func2_filename = "assets/expected/injections/dataSuspendedFunc2.cpp"
exp_data1_injected_func8_filename = "assets/expected/injections/dataInjectedFunc8.cpp"
exp_data1_filename = "assets/expected/injections/data.cpp"
exp_data1_func1_filename = "assets/expected/injections/dataFunc1.cpp"
exp_data1_removed_funcs2_7_filename = "assets/expected/injections/dataRemovedFuncs7And2.cpp"

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
        self.assertEqual(desc.segment_names, [
            "func2",
            "func4",    
            "func5",
            "func7",
        ])
        self.assertEqual(desc.suspended_segment_names, [
        ])
        
    def test_suspend_segment(self):
        content = injectors.load_content(data1_filename)
        desc = injectors.gather_desc(content)

        content = injectors.suspend(content, "func2")

        desc = injectors.gather_desc(content)
        self.assertEqual(desc.numOfInjections, 4)
        self.assertEqual(desc.segment_names, [
            "func2",
            "func4",    
            "func5",
            "func7",
        ])
        self.assertEqual(desc.suspended_segment_names, [
            "func2",
        ])

        real_content_data = utils._try_load_file(exp_data1_susp_func2_filename)
        self.assertEqual(content, real_content_data)

    def test_restore_segment(self):
        content = injectors.load_content(data1_susp_func2_filename)
        desc = injectors.gather_desc(content)

        content = injectors.restore(content, "func2")

        desc = injectors.gather_desc(content)
        self.assertEqual(desc.numOfInjections, 4)
        self.assertEqual(desc.segment_names, [
            "func2",
            "func4",    
            "func5",
            "func7",
        ])
        self.assertEqual(desc.suspended_segment_names, [
        ])

        real_content_data = utils._try_load_file(exp_data1_filename)
        self.assertEqual(content, real_content_data)

    def test_inject_segment(self):
        content = injectors.load_content(data1_filename)
        desc = injectors.gather_desc(content)

        content = injectors.inject(content, "func8", "void func8()\n{\n\n}")

        desc = injectors.gather_desc(content)
        self.assertEqual(desc.numOfInjections, 5)
        self.assertEqual(desc.segment_names, [
            "func2",
            "func4",    
            "func5",
            "func7",
            "func8",
        ])
        self.assertEqual(desc.suspended_segment_names, [
        ])

        real_content_data = utils._try_load_file(exp_data1_injected_func8_filename)
        self.assertEqual(content, real_content_data)

    def test_inject_to_empty_segment(self):
        content = injectors.load_content(data1_empty_filename)
        desc = injectors.gather_desc(content)

        content = injectors.inject(content, "func1", "void func1()\n{\n\n}")

        desc = injectors.gather_desc(content)
        self.assertEqual(desc.numOfInjections, 1)
        self.assertEqual(desc.segment_names, [
            "func1",
        ])
        self.assertEqual(desc.suspended_segment_names, [
        ])

        real_content_data = utils._try_load_file(exp_data1_func1_filename)
        self.assertEqual(content, real_content_data)

    def test_remove_segment(self):
        content = injectors.load_content(data1_filename)
        desc = injectors.gather_desc(content)

        content = injectors.remove(content, "func2")
        content = injectors.remove(content, "func7")

        desc = injectors.gather_desc(content)
        self.assertEqual(desc.numOfInjections, 2)
        self.assertEqual(desc.segment_names, [
            "func4",
            "func5",
        ])
        self.assertEqual(desc.suspended_segment_names, [
        ])

        real_content_data = utils._try_load_file(exp_data1_removed_funcs2_7_filename)
        self.assertEqual(content, real_content_data)
