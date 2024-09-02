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

class TestBaseGen(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
    
    def _test_gen(self, gen_logic, target_file_name: str, expected_file_name: str):
        gen_logic(target_file_name)
        utils.assert_files(self, target_file_name, expected_file_name)
