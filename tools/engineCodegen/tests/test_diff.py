import unittest

import os
from os import path
import sys
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import shutil

import utils
import loader
import diff

class TestDiff(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
    
    def test_check_file_changed(self):
        self.assertFalse(diff.check_files_equals("assets/diff/config4.json", "assets/diff/config4NotExist.json"))
        self.assertTrue(diff.check_files_equals("assets/diff/config4.json", "assets/diff/config4Same.json"))
        shutil.copyfile("assets/diff/config4DiffInitial.json", "assets/diff/config4Diff.json")
        self.assertFalse(diff.check_files_equals("assets/diff/config4.json", "assets/diff/config4Diff.json"))
            
    def test_json_structure_diff_1(self):
        data = loader.load_file_data("assets/diff/config1.json")
        data_same = loader.load_file_data("assets/diff/config1Same.json")
        
        self.assertTrue(diff.check_structure_is_same(data, data_same))
        
        data_structure_differs = loader.load_file_data("assets/diff/config1StructDiffers.json")
        self.assertFalse(diff.check_structure_is_same(data, data_structure_differs))
        
        data_structure_differs2 = loader.load_file_data("assets/diff/config1StructDiffers2.json")
        self.assertFalse(diff.check_structure_is_same(data, data_structure_differs2))
        
        data_structure_differs3 = loader.load_file_data("assets/diff/config1StructDiffers3.json")
        self.assertFalse(diff.check_structure_is_same(data, data_structure_differs3))
        
        data_structure_differs4 = loader.load_file_data("assets/diff/config1StructDiffers4.json")
        self.assertFalse(diff.check_structure_is_same(data, data_structure_differs4))
        
        data_structure_differs5 = loader.load_file_data("assets/diff/config1StructDiffers5.json")
        self.assertFalse(diff.check_structure_is_same(data, data_structure_differs5))
    
    def test_json_structure_diff_2(self):
        data = loader.load_file_data("assets/diff/config2.json")
        
        data_structure_differs = loader.load_file_data("assets/diff/config2StructDiffers.json")
        self.assertFalse(diff.check_structure_is_same(data, data_structure_differs))
        
        data_structure_differs2 = loader.load_file_data("assets/diff/config2StructDiffers2.json")
        self.assertFalse(diff.check_structure_is_same(data, data_structure_differs2))
    
    def test_json_structure_diff_3(self):
        data = loader.load_file_data("assets/diff/config3.json")
                
        data_structure_swapped = loader.load_file_data("assets/diff/config3Swapped.json")
        self.assertFalse(diff.check_structure_is_same(data, data_structure_swapped))
        
        data_structure_differs = loader.load_file_data("assets/diff/config3StructDiffers.json")
        self.assertFalse(diff.check_structure_is_same(data, data_structure_differs))
        
        data_structure_differs2 = loader.load_file_data("assets/diff/config3StructDiffers2.json")
        self.assertFalse(diff.check_structure_is_same(data, data_structure_differs2))
        
        data_structure_differs3 = loader.load_file_data("assets/diff/config3StructDiffers3.json")
        self.assertFalse(diff.check_structure_is_same(data, data_structure_differs3))
        
        data_structure_differs4 = loader.load_file_data("assets/diff/config3StructDiffers4.json")
        self.assertFalse(diff.check_structure_is_same(data, data_structure_differs4))
        
    def test_json_structure_diff_3_filters(self):
        data = loader.load_file_data("assets/diff/config3.json")
                
        data_structure_swapped = loader.load_file_data("assets/diff/config3Swapped.json")
        self.assertTrue(diff.check_structure_is_same(data, data_structure_swapped, ["dataType", "name"]))
        
        data_structure_differs = loader.load_file_data("assets/diff/config3StructDiffers.json")
        self.assertTrue(diff.check_structure_is_same(data, data_structure_differs, ["type"]))
        
        data_structure_differs2 = loader.load_file_data("assets/diff/config3StructDiffers2.json")
        self.assertTrue(diff.check_structure_is_same(data, data_structure_differs2, ["version"]))
        
        data_structure_differs3 = loader.load_file_data("assets/diff/config3StructDiffers3.json")
        self.assertTrue(diff.check_structure_is_same(data, data_structure_differs3, ["name"]))
        
        data_structure_differs4 = loader.load_file_data("assets/diff/config3StructDiffers4.json")
        self.assertTrue(diff.check_structure_is_same(data, data_structure_differs4, ["capacity"]))
        
    def test_json_structure_diff_5(self):
        data = loader.load_file_data("assets/diff/config5.json")
        dataSame = loader.load_file_data("assets/diff/config5Same.json")
        self.assertTrue(diff.check_structure_is_same(data, dataSame))
                
        data_structure_differs = loader.load_file_data("assets/diff/config5StructDiffers.json")
        self.assertFalse(diff.check_structure_is_same(data, data_structure_differs))
                
        data_structure_differs2 = loader.load_file_data("assets/diff/config5StructDiffers2.json")
        self.assertFalse(diff.check_structure_is_same(data, data_structure_differs2))
             
    def test_json_structure_diff_6(self):
        data = loader.load_file_data("assets/diff/config6.json")
        dataSame = loader.load_file_data("assets/diff/config6SameArray.json")
        self.assertTrue(diff.check_structure_is_same(data, dataSame, ["initial"]))

        data_structure_same_array_complex = loader.load_file_data("assets/diff/config6SameArrayComplex.json")
        self.assertTrue(diff.check_structure_is_same(data, data_structure_same_array_complex, ["initial"]))

        data_structure_same_complex = loader.load_file_data("assets/diff/config6SameComplex.json")
        self.assertTrue(diff.check_structure_is_same(data, data_structure_same_complex, ["someData"]))

        data_structure_differs_array_complex = loader.load_file_data("assets/diff/config6StructureDiffersArrayComplex.json")
        self.assertTrue(diff.check_structure_is_same(data, data_structure_differs_array_complex, ["initial"]))
        
        data_structure_differs_array_trivial = loader.load_file_data("assets/diff/config6StructureDiffersArrayTrivial.json")
        self.assertTrue(diff.check_structure_is_same(data, data_structure_differs_array_trivial, ["initial"]))
        
        data_structure_differs_complex = loader.load_file_data("assets/diff/config6StructureDiffersComplex.json")
        self.assertTrue(diff.check_structure_is_same(data, data_structure_differs_complex, ["someData"]))
        
    def test_diff_list_generator(self):
        current_files_folder = "assets/diff/current/"
        previous_files_folder = "assets/diff/prev/"
        current_files_list = [
            current_files_folder + "file1.json",
            current_files_folder + "file2.json",
            current_files_folder + "file3.json",
            current_files_folder + "file4.json",
        ]
        diff_list = diff.generate_list(current_files_list, previous_files_folder)
        
        self.assertEqual(diff_list, [
            current_files_folder + "file2.json",
            current_files_folder + "file4.json",
        ])
                
    def test_diff_list_generator_filtered(self):
        current_files_folder = "assets/diff/current/"
        previous_files_folder = "assets/diff/prev/"
        curr_files_list = [
            current_files_folder + "file1.json",
            current_files_folder + "file2.json",
            current_files_folder + "file3.json",
            current_files_folder + "file4.json",
        ]
        diff_list = diff.generate_list(curr_files_list, previous_files_folder, [
            "dataType",
            "name"
        ])
        
        self.assertEqual(diff_list, [
        ])
        
    def test_diff_list_empty_generator(self):
        current_files_folder = "assets/diff/current/"
        previous_files_folder = "assets/diff/prevEmpty/"
        curr_files_list = [
            current_files_folder + "file1.json",
            current_files_folder + "file2.json",
            current_files_folder + "file3.json",
            current_files_folder + "file4.json",
        ]
        diff_list = diff.generate_list(curr_files_list, previous_files_folder)
        
        self.assertEqual(diff_list, [
            current_files_folder + "file1.json",
            current_files_folder + "file2.json",
            current_files_folder + "file3.json",
            current_files_folder + "file4.json",
        ])
        
    def test_diff_replace_files(self):
        
        current_files_folder = "assets/diff/current"
        previous_files_folder = "dest/diff/prev"
        files_to_replace = [
            current_files_folder + "/file2.json",
            current_files_folder + "/file3.json",
        ]
        
        if not os.path.exists(previous_files_folder):
            os.makedirs(previous_files_folder)
                
        shutil.copyfile(current_files_folder + "/file1.json", previous_files_folder + "/file1.json")
        shutil.copyfile(current_files_folder + "/file2Initial.json", previous_files_folder + "/file2.json")
        shutil.copyfile(current_files_folder + "/file3Initial.json", previous_files_folder + "/file3.json")
        shutil.copyfile(current_files_folder + "/file4.json", previous_files_folder + "/file4.json")
        
        utils.assert_files(self, current_files_folder + "/file1.json", previous_files_folder + "/file1.json")
        utils.assert_files(self, current_files_folder + "/file2Initial.json", previous_files_folder + "/file2.json")
        utils.assert_files(self, current_files_folder + "/file3Initial.json", previous_files_folder + "/file3.json")
        utils.assert_files(self, current_files_folder + "/file4.json", previous_files_folder + "/file4.json")
        
        diff.replace(previous_files_folder, files_to_replace)
        
        utils.assert_files(self, current_files_folder + "/file1.json", previous_files_folder + "/file1.json")
        utils.assert_files(self, current_files_folder + "/file2.json", previous_files_folder + "/file2.json")
        utils.assert_files(self, current_files_folder + "/file3.json", previous_files_folder + "/file3.json")
        utils.assert_files(self, current_files_folder + "/file4.json", previous_files_folder + "/file4.json")
        