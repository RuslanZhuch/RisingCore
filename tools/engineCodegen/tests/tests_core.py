import unittest

import sys
sys.path.append("../src")

import loader
import generator

class TestCore(unittest.TestCase):
    def test_load_data_executor1(self):
        data = loader.load_descriptor("assets/executors/executor1.json")
        self.assertEqual(data.get("whoIAm"), "Executor")
        self.assertEqual(data.get("version"), 1)
        self.assertEqual(data.get("name"), "executor1")
        
    def test_load_data_executor2(self):
        data = loader.load_descriptor("assets/executors/executor2.json")
        self.assertEqual(data.get("whoIAm"), "Executor_2")
        self.assertEqual(data.get("version"), 2)
        self.assertEqual(data.get("name"), "executor2")
        
    def test_load_data_from_folder(self):
        data = loader.load_descriptors([
            "assets/executors/executor1.json",
            "assets/executors/executor2.json",
            "assets/executors/executor3.json"
        ])
        self.assertEqual(len(data), 3)
        
        self.assertEqual(data[0].get("whoIAm"), "Executor")
        self.assertEqual(data[0].get("version"), 1)
        self.assertEqual(data[0].get("name"), "executor1")
        
        self.assertEqual(data[1].get("whoIAm"), "Executor_2")
        self.assertEqual(data[1].get("version"), 2)
        self.assertEqual(data[1].get("name"), "executor2")
        
        self.assertEqual(data[2].get("whoIAm"), "Executor_3")
        self.assertEqual(data[2].get("version"), 3)
        self.assertEqual(data[2].get("name"), "executor3")
        
    def test_load_data_shared_contexts_from_folder(self):
        data = loader.load_shared_contexts([
            "assets/contexts/sContext1.json",
            "assets/contexts/sContext2.json"
        ])
        self.assertEqual(len(data), 2)
        
        self.assertEqual(data[0].get("whoIAm"), "SContext")
        self.assertEqual(data[0].get("version"), 1)
        self.assertEqual(data[0].get("name"), "sContext1")
        
        self.assertEqual(data[1].get("whoIAm"), "SContext")
        self.assertEqual(data[1].get("version"), 2)
        self.assertEqual(data[1].get("name"), "sContext2")
        