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
                
    def test_generate_line(self):
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        generator.generate_line(handler, "Generated line")
        
        handler.close()
        
        descriptor_file = open("dest/gen.cpp", "r")
        file_data = descriptor_file.read()
        
        self.assertEqual(file_data, "Generated line\n")
        
    def test_generate_empty_line(self):
        handler = generator.generate_file("dest", "gen_newLine.cpp")
        self.assertIsNotNone(handler)
        
        generator.generate_line(handler, "Generated line")
        generator.generate_empty(handler)
        generator.generate_line(handler, "Generated line2")
        generator.generate_empty(handler, 2)
        generator.generate_line(handler, "Generated line3")
        
        def block_data(handler):
            generator.generate_empty(handler)
            generator.generate_line(handler, "Generated line4")
            
        generator.generate_block(handler, "", block_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_newLine.cpp", "assets/expected/newLine.cpp")
                
    def test_generate_variable(self):
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        generator.generate_variable(handler, "Var_type", "var_name")
        
        handler.close()
        
        descriptor_file = open("dest/gen.cpp", "r")
        file_data = descriptor_file.read()
        
        self.assertEqual(file_data, "Var_type var_name;\n")
        
    def test_generate_variable_initial(self):
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        generator.generate_variable(handler, "Var_type", "var_name", 42)
        
        handler.close()
        
        descriptor_file = open("dest/gen.cpp", "r")
        file_data = descriptor_file.read()
        
        self.assertEqual(file_data, "Var_type var_name{ 42 };\n")
        
    def test_generate_variable_initial_zero(self):
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        generator.generate_variable(handler, "Var_type", "var_name", 0)
        
        handler.close()
        
        descriptor_file = open("dest/gen.cpp", "r")
        file_data = descriptor_file.read()
        
        self.assertEqual(file_data, "Var_type var_name{};\n")
                
    def test_generate_function(self):
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        
        def func_data(self, handler):
            generator.generate_line(handler, "Line1")
        
        generator.generate_function(handler, "func1", func_data)
        
        handler.close()
        
        descriptor_file = open("dest/gen.cpp", "r")
        file_data = descriptor_file.read()
        
        self.assertEqual(file_data, "void func1()\n{\n    Line1\n}\n")
        
    def test_generate_block(self):
        handler = create_target_file()
        self.assertIsNotNone(handler)
        
        
        def block_data(handler):
            generator.generate_line(handler, "Block line")
        
        generator.generate_block(handler, "block1", block_data)
        
        handler.close()
        
        descriptor_file = open("dest/gen.cpp", "r")
        file_data = descriptor_file.read()
        
        self.assertEqual(file_data, "block1\n{\n    Block line\n}\n")
        
    def test_generate_empty_class(self):
        handler = generator.generate_file("dest", "gen_empty_class.cpp")
        self.assertIsNotNone(handler)
        
        def class_data(class_handler):
            pass
        generator.generate_class(handler, "Test1", class_data)
        
        handler.close()
        
        utils.assert_files(self, "dest/gen_empty_class.cpp", "assets/expected/classDeclarationEmpty.cpp")
        
    def test_generate_struct_declaration(self):
        handler = generator.generate_file("dest", "gen_struct.cpp")
        self.assertIsNotNone(handler)
        
        def struct_data(struct_handler):
            generator.generate_struct_method(struct_handler, "method1", "float", [], True)
            generator.generate_struct_method(struct_handler, "method2", "void", ['float dt'], False)
            generator.generate_struct_method(struct_handler, "method3", "void", ['int32_t arg'], False, is_static = True)
            generator.generate_struct_variable(struct_handler, "int", "var1", 0)
            generator.generate_struct_variable(struct_handler, "Dod::MemPool", "memory")
            
        generator.generate_struct(handler, "Test2", struct_data)

        handler.close()
        
        utils.assert_files(self, "dest/gen_struct.cpp", "assets/expected/structDeclaration.cpp")
           
    def test_generate_struct_implementation(self):
        handler = generator.generate_file("dest", "gen_struct_impl.cpp")
        self.assertIsNotNone(handler)
        
        def class_data(class_handler):
            generator.generate_class_public_method(class_handler, "method1", "float", [], True)
            
            def pubm2_body(self, class_handler):
                generator.generate_line(class_handler, "some stuff here")
            generator.generate_class_public_method(class_handler, "method2", "void", ['float dt'], False, pubm2_body)
            
        generator.generate_struct_impl(handler, "Test3", class_data)

        handler.close()
        
        utils.assert_files(self, "dest/gen_struct_impl.cpp", "assets/expected/structImplementation.cpp")
        
    def test_generate_class_declaration(self):
        handler = generator.generate_file("dest", "gen_class.cpp")
        self.assertIsNotNone(handler)
        
        def class_data(class_handler):
            generator.generate_class_public_method(class_handler, "publicMethod1", "float", [], True)
            generator.generate_class_public_method(class_handler, "publicMethod2", "void", ['float dt'], False)
            generator.generate_class_private_method(class_handler, "privateMethod1", "int", ['int n'], True)
            generator.generate_class_variable(class_handler, "int", "var1", 0)
            generator.generate_class_variable(class_handler, "Dod::MemPool", "memory")
            
        generator.generate_class(handler, "Test2", class_data)

        handler.close()
        
        utils.assert_files(self, "dest/gen_class.cpp", "assets/expected/classDeclaration.cpp")
                
    def test_generate_class_implementation(self):
        handler = generator.generate_file("dest", "gen_class_impl.cpp")
        self.assertIsNotNone(handler)
        
        def class_data(class_handler):
            generator.generate_class_public_method(class_handler, "publicMethod1", "float", [], True)
            
            def pubm2_body(self, class_handler):
                generator.generate_line(class_handler, "some stuff here")
            generator.generate_class_public_method(class_handler, "publicMethod2", "void", ['float dt'], False, pubm2_body)
            
            def prm1_body(self, class_handler):
                generator.generate_line(class_handler, "some stuff here 2")
            generator.generate_class_private_method(class_handler, "privateMethod1", "int", ['int n'], True, prm1_body)
            
        generator.generate_class_impl(handler, "Test2", class_data)

        handler.close()
        
        utils.assert_files(self, "dest/gen_class_impl.cpp", "assets/expected/classImplementation.cpp")
        