import unittest

from os import path
import sys
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import utils
import parser

import json

class TestParse(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
    
    def test_deser_group_1(self):
        deser_data = parser.parse("assets/headers/group1.h")
        
        expected_file = open("assets/expected/group1Deser.json")
        expected_data = json.load(expected_file)
        
        self.assertEqual(deser_data, expected_data)
        