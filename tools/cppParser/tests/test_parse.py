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
    
    def test_parse_group_1(self):
        deser_data = parser.parse("assets/headers/group1.h")
        
        expected_file = open("assets/expected/group1Deser.json")
        expected_data = json.load(expected_file)
        
        self.assertEqual(deser_data, expected_data)
        
    def test_parse_multiple_files(self):
        files_list = [
            "assets/headers/group1.h"
        ]
        
        parsed_filenames = parser.parse_to_folder(files_list, "dest/parsed_1")
        
        self.assertEqual(len(parsed_filenames), 1)
        self.assertEqual(parsed_filenames[0], "dest/parsed_1/group1.json")
        
        parsed_file = open("dest/parsed_1/group1.json")
        parsed_data = json.load(parsed_file)
        
        expected_file = open("assets/expected/group1Deser.json")
        expected_data = json.load(expected_file)
        
        self.assertEqual(parsed_data, expected_data)
        