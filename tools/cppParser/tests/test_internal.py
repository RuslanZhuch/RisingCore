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
        
    def test_namespace_name_1(self):
        data = "namespace Name1 \n {   \n"
        namespace_name = parser_internal.get_namespace_name(data, 0)
        self.assertEqual(namespace_name, "Name1")
        
    def test_namespace_name_2(self):
        data = "namespace Name2 {   \n"
        namespace_name = parser_internal.get_namespace_name(data, 0)
        self.assertEqual(namespace_name, "Name2")
        
    def test_namespace_name_3(self):
        data = "namespace Name3 {"
        namespace_name = parser_internal.get_namespace_name(data, 0)
        self.assertEqual(namespace_name, "Name3")
        
    def test_type_name_1(self):
        data = "struct Type1 \n {   \n"
        namespace_name = parser_internal.get_type_name(data, 0)
        self.assertEqual(namespace_name, "Type1")
        
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
        
    def test_outer_namespace_links_group_1(self):
        working_file = open("assets/headers/group1.h")
        working_data = working_file.read()
        working_file.close()
        
        namespace_entries = parser_internal.get_namespace_entries(working_data)
        scope_entries = parser_internal.get_scopes_data(working_data)
        
        namespace_names = [parser_internal.get_namespace_name(working_data, namespace_entry) for namespace_entry in namespace_entries]
        
        namespace_links = parser_internal.create_namespace_links(namespace_entries, namespace_names, scope_entries)
        self.assertEqual(len(namespace_links), 1)
        self.assertEqual(namespace_links[0].scope_entry_index, 0)
        self.assertEqual(namespace_links[0].name, "Types::Group1")
        
    def test_outer_type_links_group_1(self):
        working_file = open("assets/headers/group1.h")
        working_data = working_file.read()
        working_file.close()
        
        type_entries = parser_internal.get_types_entries(working_data)
        scope_entries = parser_internal.get_scopes_data(working_data)

        type_names = [parser_internal.get_type_name(working_data, type_entry) for type_entry in type_entries]
        
        type_links = parser_internal.create_type_links(type_entries, type_names, scope_entries)
        self.assertEqual(len(type_links), 1)
        self.assertEqual(type_links[0].scope_entry_index, 1)
        self.assertEqual(type_links[0].name, "Type1")
        
    def test_find_varibles_in_scope_group_1(self):
        working_file = open("assets/headers/group1.h")
        working_data = working_file.read()
        working_file.close()
        
        scope_entries = parser_internal.get_scopes_data(working_data)
        
        variables_in_type = parser_internal.find_variables_in_scope(working_data, 1, scope_entries)
        
        self.assertEqual(len(variables_in_type), 2)
        self.assertEqual(variables_in_type[0].type_name, "int")
        self.assertEqual(variables_in_type[0].name, "x")
        self.assertEqual(variables_in_type[1].type_name, "float")
        self.assertEqual(variables_in_type[1].name, "y")

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

    def test_populate_scope_tree_group_1(self):
        working_file = open("assets/headers/group1.h")
        working_data = working_file.read()
        working_file.close()
        
        scope_entries = parser_internal.get_scopes_data(working_data)
        
        scopes_tree = parser_internal.create_scopes_tree(scope_entries)
        
        namespace_entries = parser_internal.get_namespace_entries(working_data)
        namespace_names = [parser_internal.get_namespace_name(working_data, namespace_entry) for namespace_entry in namespace_entries]
        namespace_links = parser_internal.create_namespace_links(namespace_entries, namespace_names, scope_entries)
        
        type_entries = parser_internal.get_types_entries(working_data)
        type_names = [parser_internal.get_type_name(working_data, type_entry) for type_entry in type_entries]
        type_links = parser_internal.create_type_links(type_entries, type_names, scope_entries)
        
        types_variables = [parser_internal.find_variables_in_scope(working_data, type_link.scope_entry_index, scope_entries) for type_link in type_links ]
        
        parser_internal.populate_scopes_tree(scopes_tree, namespace_links, type_links, types_variables)
        self.assertEqual(len(scopes_tree.scopes), 1)
        self.assertEqual(scopes_tree.entry, -1) 
        
        namespace_scope = scopes_tree.scopes[0]
        self.assertEqual(len(namespace_scope.scopes), 1)
        self.assertEqual(namespace_scope.entry, 0)
        self.assertEqual(namespace_scope.feature, namespace_links[0])
        
        type_scope = namespace_scope.scopes[0]
        self.assertEqual(len(type_scope.scopes), 2)
        self.assertEqual(type_scope.entry, 1)
        self.assertEqual(type_scope.feature, type_links[0])
        self.assertEqual(len(type_scope.variables), 2)
        self.assertEqual(type_scope.variables[0].name, "x")
        self.assertEqual(type_scope.variables[0].type_name, "int")
        self.assertEqual(type_scope.variables[1].name, "y")
        self.assertEqual(type_scope.variables[1].type_name, "float")
        
        var1_scope = type_scope.scopes[0]
        self.assertEqual(len(var1_scope.scopes), 0)
        self.assertEqual(var1_scope.entry, 2)
        
        var2_scope = type_scope.scopes[1]
        self.assertEqual(len(var2_scope.scopes), 0)
        self.assertEqual(var2_scope.entry, 3)

    def test_create_structure_group_1(self):
        working_file = open("assets/headers/group1.h")
        working_data = working_file.read()
        working_file.close()
        
        scope_entries = parser_internal.get_scopes_data(working_data)
        
        scopes_tree = parser_internal.create_scopes_tree(scope_entries)
        
        namespace_entries = parser_internal.get_namespace_entries(working_data)
        namespace_names = [parser_internal.get_namespace_name(working_data, namespace_entry) for namespace_entry in namespace_entries]
        namespace_links = parser_internal.create_namespace_links(namespace_entries, namespace_names, scope_entries)
        
        type_entries = parser_internal.get_types_entries(working_data)
        type_names = [parser_internal.get_type_name(working_data, type_entry) for type_entry in type_entries]
        type_links = parser_internal.create_type_links(type_entries, type_names, scope_entries)
        
        types_variables = [parser_internal.find_variables_in_scope(working_data, type_link.scope_entry_index, scope_entries) for type_link in type_links ]
        
        parser_internal.populate_scopes_tree(scopes_tree, namespace_links, type_links, types_variables)
        structure = parser_internal.create_structure(scopes_tree)

        self.assertIsNotNone(structure.get("namespaces"))
        self.assertEquals(len(structure["namespaces"]), 1)
        
        self.assertIsNotNone(structure["namespaces"][0].get("name"))
        self.assertEquals(structure["namespaces"][0]["name"], "Types::Group1")
        self.assertIsNotNone(structure["namespaces"][0].get("types"))
        self.assertEquals(len(structure["namespaces"][0]["types"]), 1)
        
        self.assertIsNotNone(structure["namespaces"][0]["types"][0].get("name"))
        self.assertEquals(structure["namespaces"][0]["types"][0]["name"], "Type1")
        self.assertIsNotNone(structure["namespaces"][0]["types"][0].get("variables"))
        self.assertIsNotNone(structure["namespaces"][0]["types"][0]["variables"][0].get("dataType"))
        self.assertEquals(structure["namespaces"][0]["types"][0]["variables"][0]["dataType"], "int")
        self.assertIsNotNone(structure["namespaces"][0]["types"][0]["variables"][0].get("name"))
        self.assertEquals(structure["namespaces"][0]["types"][0]["variables"][0]["name"], "x")
        
        self.assertIsNotNone(structure["namespaces"][0]["types"][0]["variables"][1].get("dataType"))
        self.assertEquals(structure["namespaces"][0]["types"][0]["variables"][1]["dataType"], "float")
        self.assertIsNotNone(structure["namespaces"][0]["types"][0]["variables"][1].get("name"))
        self.assertEquals(structure["namespaces"][0]["types"][0]["variables"][1]["name"], "y")
        