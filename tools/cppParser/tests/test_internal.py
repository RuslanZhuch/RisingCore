import unittest

from os import path
import sys
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import utils
import parser_internal

import json

class TestInternal(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
    
    def test_namespaces_group_1(self):
        working_file = open("assets/headers/group1.h")
        working_data = working_file.read()
        working_file.close()
        
        entries = parser_internal.get_namespace_entries(working_data)
        self.assertEqual(len(entries), 1)
        self.assertEqual(entries[0], 41)
        
    def test_types_group_1(self):
        working_file = open("assets/headers/group1.h")
        working_data = working_file.read()
        working_file.close()
        
        entries = parser_internal.get_types_entries(working_data)
        self.assertEqual(len(entries), 1)
        self.assertEqual(entries[0], 72)
        
    def test_scopes_group_1(self):
        working_file = open("assets/headers/group1.h")
        working_data = working_file.read()
        working_file.close()
        
        entries = parser_internal.get_scopes_data(working_data)
        self.assertEqual(len(entries.openIndices), 4)
        self.assertEqual(len(entries.closeIndices), 4)
        self.assertEqual(entries.openIndices[0], 65)
        self.assertGreater(entries.openIndices[1], entries.openIndices[0])
        self.assertGreater(entries.openIndices[2], entries.openIndices[1])
        self.assertGreater(entries.openIndices[2], entries.openIndices[3])
        self.assertGreater(entries.closeIndices[0], entries.closeIndices[1])
        self.assertGreater(entries.closeIndices[1], entries.closeIndices[2])
        self.assertGreater(entries.closeIndices[2], entries.closeIndices[3])
        
        self.assertEqual(entries.closeIndices[2] - entries.openIndices[2], 1)
        self.assertEqual(entries.closeIndices[3] - entries.openIndices[3], 1)
        
        print("entries.openIndices", entries.openIndices)
        print("entries.closeIndices", entries.closeIndices)
        
    def test_outer_namespace_object_group_1(self):
        working_file = open("assets/headers/group1.h")
        working_data = working_file.read()
        working_file.close()
        
        namespace_entries = parser_internal.get_namespace_entries(working_data)
        scope_entries = parser_internal.get_scopes_data(working_data)
        
        namespace_objects = parser_internal.create_namespace_objects(namespace_entries, scope_entries)
        self.assertEqual(len(namespace_objects), 1)
        self.assertEqual(namespace_objects[0].scope_entry_index, 0)
        
    def test_outer_type_object_group_1(self):
        working_file = open("assets/headers/group1.h")
        working_data = working_file.read()
        working_file.close()
        
        type_entries = parser_internal.get_types_entries(working_data)
        scope_entries = parser_internal.get_scopes_data(working_data)
        
        type_objects = parser_internal.create_type_objects(type_entries, scope_entries)
        self.assertEqual(len(type_objects), 1)
        self.assertEqual(type_objects[0].scope_entry_index, 1)
        
    def test_create_scropes_tree_group_1(self):
        working_file = open("assets/headers/group1.h")
        working_data = working_file.read()
        working_file.close()
        
        scope_entries = parser_internal.get_scopes_data(working_data)
        
        scopes_tree = parser_internal.create_scopes_tree(scope_entries)
        self.assertEqual(len(scopes_tree.scopes), 1)
        self.assertEqual(scopes_tree.entry, -1) 
        
        namespace_scope = scopes_tree.scopes[0]
        self.assertEqual(len(namespace_scope.scopes), 1)
        self.assertEqual(namespace_scope.entry, 0)
        
        type_scope = namespace_scope.scopes[0]
        self.assertEqual(len(type_scope.scopes), 2)
        self.assertEqual(type_scope.entry, 1)
        
        var1_scope = type_scope.scopes[0]
        self.assertEqual(len(var1_scope.scopes), 0)
        self.assertEqual(var1_scope.entry, 2)
        
        var2_scope = type_scope.scopes[1]
        self.assertEqual(len(var2_scope.scopes), 0)
        self.assertEqual(var2_scope.entry, 3)
        