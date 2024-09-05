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

import utils

EXPECT_NUM_OF_EXECUTORS = 2

def load_executors():
    return executors.load("assets/executors")

def create_target_file():
    return generator.generate_file("dest", "gen.cpp")

class TestGenerators(unittest.TestCase):
    def test_generate_file(self):
        self.assertIsNotNone(generator.generate_file("dest", "file1.cpp"))
        open("dest/file1.cpp")
        self.assertIsNotNone(generator.generate_file("dest", "file2.cpp"))
        open("dest/file2.cpp")

    def test_check_file_exist(self):
        file_full_path = "dest/temp_file.cpp"
        if os.path.isfile(file_full_path):
            os.remove(file_full_path)
        
        self.assertFalse(generator.get_file_generated("dest", "temp_file.cpp"))
    
        self.assertIsNotNone(generator.generate_file("dest", "temp_file.cpp"))
        self.assertTrue(generator.get_file_generated("dest", "temp_file.cpp"))
                
    def test_write_content_to_file(self):
        content_1 = "Content 1 file\n"
        content_2 = "Content 2 file\n"

        if not generator.get_file_generated("dest", "temp_file.cpp"):
            generator.generate_file("dest", "temp_file.cpp")

        generator.write_content("dest", "temp_file.cpp", content_1)
        file = open("dest/temp_file.cpp")
        self.assertIsNotNone(file)
        self.assertEqual(file.read(), content_1)

        generator.write_content("dest", "temp_file.cpp", content_2)
        file = open("dest/temp_file.cpp", "r")
        self.assertIsNotNone(file)
        self.assertEqual(file.read(), content_2)
