import unittest

import os
from os import path
import sys
import time
 
from pathlib import Path
root_dir = Path.cwd()
sys.path.append(root_dir)

import sys
sys.path.append("../src")

import shutil

import utils
import project

def get_file_modification_time(filename : str):
    return path.getmtime(filename)
    
class TestProject(unittest.TestCase):
    def __init__(self, methodName: str = "runTest") -> None:
        super().__init__(methodName)
        self.maxDiff = None
    
    def test_decode_project1_file(self):
        project_desc = project.load("assets/workspace/project1.json")
        
        self.assertIsNotNone(project_desc)
        
        self.assertEqual(project_desc.project_dest_folder, "dest/projects/project1")
        
        self.assertEqual(project_desc.project_root_src_folder, "assets/workspace")
        self.assertEqual(project_desc.project_root_src_types, "assets/workspace")
        self.assertEqual(project_desc.project_contexts_src_folder, "assets/contexts")
        self.assertEqual(project_desc.project_executors_src_folder, "assets/executors")
        
        self.assertEqual(project_desc.application_context_path, "ws_applicationContext.json")
        
        self.assertEqual(len(project_desc.types_paths), 2)
        self.assertEqual(project_desc.types_paths[0], "types_default.json")
        self.assertEqual(project_desc.types_paths[1], "types_contexts_data.json")
        
        self.assertEqual(len(project_desc.contexts_paths), 5)
        self.assertEqual(project_desc.contexts_paths[0], "lContext1.json")
        self.assertEqual(project_desc.contexts_paths[1], "lContext2.json")
        self.assertEqual(project_desc.contexts_paths[2], "lContext3.json")
        self.assertEqual(project_desc.contexts_paths[3], "sContext1.json")
        self.assertEqual(project_desc.contexts_paths[4], "sContext2.json")
        
        self.assertEqual(len(project_desc.executors_paths), 3)
        self.assertEqual(project_desc.executors_paths[0], "executor1.json")
        self.assertEqual(project_desc.executors_paths[1], "executor2.json")
        self.assertEqual(project_desc.executors_paths[2], "executor3.json")
        
    def test_gen_project1(self):
        [f.unlink() for f in Path("assets/contexts/prev").glob("*") if f.is_file()] 
        [f.unlink() for f in Path("assets/executors/prev").glob("*") if f.is_file()] 
        
        project.generate("assets/workspace/project1.json")
        
        utils.assert_files(self, "dest/projects/project1/contexts/LContext1Context.h", "assets/expected/lContext1Context.h")
        utils.assert_files(self, "dest/projects/project1/contexts/LContext1Context.cpp", "assets/expected/lContext1Context.cpp")
        utils.assert_files(self, "dest/projects/project1/contexts/LContext2Context.h", "assets/expected/lContext2Context.h")
        utils.assert_files(self, "dest/projects/project1/contexts/LContext2Context.cpp", "assets/expected/lContext2Context.cpp")
        utils.assert_files(self, "dest/projects/project1/contexts/LContext3Context.h", "assets/expected/lContext3Context.h")
        utils.assert_files(self, "dest/projects/project1/contexts/LContext3Context.cpp", "assets/expected/lContext3Context.cpp")
        utils.assert_files(self, "dest/projects/project1/contexts/SContext1Context.h", "assets/expected/sContext1Context.h")
        utils.assert_files(self, "dest/projects/project1/contexts/SContext1Context.cpp", "assets/expected/sContext1Context.cpp")
        utils.assert_files(self, "dest/projects/project1/contexts/SContext2Context.h", "assets/expected/sContext2Context.h")
        utils.assert_files(self, "dest/projects/project1/contexts/SContext2Context.cpp", "assets/expected/sContext2Context.cpp")
        
        utils.assert_files(self, "dest/projects/project1/executors/Executor1Executor.h", "assets/expected/Executor1Executor.h")
        utils.assert_files(self, "dest/projects/project1/executors/Executor1Executor.cpp", "assets/expected/Executor1Executor.cpp")
        utils.assert_files(self, "dest/projects/project1/executors/Executor2Executor.h", "assets/expected/Executor2Executor.h")
        utils.assert_files(self, "dest/projects/project1/executors/Executor2Executor.cpp", "assets/expected/Executor2Executor.cpp")
        utils.assert_files(self, "dest/projects/project1/executors/Executor3Executor.h", "assets/expected/Executor3Executor.h")
        utils.assert_files(self, "dest/projects/project1/executors/Executor3Executor.cpp", "assets/expected/Executor3Executor.cpp")
        
        utils.assert_files(self, "dest/projects/project1/executors/Executor1ExecutorImpl.cpp", "assets/expected/Executor1ExecutorImpl.cpp")
        utils.assert_files(self, "dest/projects/project1/executors/Executor2ExecutorImpl.cpp", "assets/expected/Executor2ExecutorImpl.cpp")
        utils.assert_files(self, "dest/projects/project1/executors/Executor3ExecutorImpl.cpp", "assets/expected/Executor3ExecutorImpl.cpp")
        
        utils.assert_files(self, "dest/projects/project1/runtime.cpp", "assets/expected/runtime.cpp")
                
    def test_gen_project2_with_diff(self):
        [f.unlink() for f in Path("assets/project2/contexts/prev").glob("*") if f.is_file()] 
        [f.unlink() for f in Path("assets/project2/executors/prev").glob("*") if f.is_file()] 
        [f.unlink() for f in Path("assets/project2/prev").glob("*") if f.is_file()] 
                
        root_path = "assets/project2/"
        prev_root_path = root_path + "/prev"
        
        contexts_path = "assets/project2/contexts"
        prev_contexts_path = contexts_path + "/prev"
        if not os.path.exists(contexts_path):
            os.makedirs(contexts_path)
        if not os.path.exists(prev_contexts_path):
            os.makedirs(prev_contexts_path)
            
        executors_path = "assets/project2/executors"
        prev_executors_path = executors_path + "/prev"
        if not os.path.exists(executors_path):
            os.makedirs(executors_path)
        if not os.path.exists(prev_executors_path):
            os.makedirs(prev_executors_path)
        
        shutil.copyfile("assets/expected/project2/prev/lcontext2.json", prev_contexts_path + "/lcontext2.json")
        shutil.copyfile("assets/expected/project2/prev/executor1.json", prev_executors_path + "/executor1.json")
        
        shutil.copyfile("assets/expected/project2/contexts/lContext2Context.h", "dest/projects/project2/contexts/LContext2Context.h")
        shutil.copyfile("assets/expected/project2/contexts/lContext2Context.cpp", "dest/projects/project2/contexts/LContext2Context.cpp")
        shutil.copyfile("assets/expected/project2/executors/Executor1Executor.h", "dest/projects/project2/executors/Executor1Executor.h")
        shutil.copyfile("assets/expected/project2/executors/Executor1Executor.cpp", "dest/projects/project2/executors/Executor1Executor.cpp")
        
        prev_context_mod_time = get_file_modification_time(prev_contexts_path + "/lcontext2.json")
        prev_executor_mod_time = get_file_modification_time(prev_executors_path + "/executor1.json")
        
        prev_executor1h_mod_time = get_file_modification_time("dest/projects/project2/executors/Executor1Executor.h")
        prev_executor1cpp_mod_time = get_file_modification_time("dest/projects/project2/executors/Executor1Executor.cpp")
        
        prev_context2h_mod_time = get_file_modification_time("dest/projects/project2/contexts/LContext2Context.h")
        prev_context2cpp_mod_time = get_file_modification_time("dest/projects/project2/contexts/LContext2Context.cpp")
        
        project.generate("assets/project2/project2.json")
        
        utils.assert_files(self, "dest/projects/project2/contexts/LContext1Context.h", "assets/expected/project2/contexts/lContext1Context.h")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext1Context.cpp", "assets/expected/project2/contexts/lContext1Context.cpp")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext2Context.h", "assets/expected/project2/contexts/lContext2Context.h")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext2Context.cpp", "assets/expected/project2/contexts/lContext2Context.cpp")
        
        utils.assert_files(self, "dest/projects/project2/executors/Executor1Executor.h", "assets/expected/project2/executors/Executor1Executor.h")
        utils.assert_files(self, "dest/projects/project2/executors/Executor1Executor.cpp", "assets/expected/project2/executors/Executor1Executor.cpp")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2Executor.h", "assets/expected/project2/executors/Executor2Executor.h")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2Executor.cpp", "assets/expected/project2/executors/Executor2Executor.cpp")
        
        utils.assert_files(self, "dest/projects/project2/executors/Executor1ExecutorImpl.cpp", "assets/expected/project2/executors/Executor1ExecutorImpl.cpp")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2ExecutorImpl.cpp", "assets/expected/project2/executors/Executor2ExecutorImpl.cpp")
        
        utils.assert_files(self, "dest/projects/project2/runtime.cpp", "assets/expected/project2/runtime.cpp")
        
        self.assertEqual(prev_context_mod_time, get_file_modification_time(prev_contexts_path + "/lcontext2.json"))
        self.assertEqual(prev_executor_mod_time, get_file_modification_time(prev_executors_path + "/executor1.json"))
        self.assertNotEqual(prev_context_mod_time, get_file_modification_time(prev_contexts_path + "/lcontext1.json"))
        self.assertNotEqual(prev_executor_mod_time, get_file_modification_time(prev_executors_path + "/executor2.json"))
        
        self.assertEqual(prev_executor1h_mod_time, get_file_modification_time("dest/projects/project2/executors/Executor1Executor.h"))
        self.assertEqual(prev_executor1cpp_mod_time, get_file_modification_time("dest/projects/project2/executors/Executor1Executor.cpp"))
        self.assertEqual(prev_context2h_mod_time, get_file_modification_time("dest/projects/project2/contexts/LContext2Context.h"))
        self.assertEqual(prev_context2cpp_mod_time, get_file_modification_time("dest/projects/project2/contexts/LContext2Context.cpp"))
        
        max_initial_modification_time = max([
            prev_executor1h_mod_time, 
            prev_executor1cpp_mod_time,
            prev_context2h_mod_time,
            prev_context2cpp_mod_time
        ])
        
        self.assertGreater(get_file_modification_time("dest/projects/project2/executors/Executor2Executor.h"), max_initial_modification_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/executors/Executor2Executor.cpp"), max_initial_modification_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/contexts/LContext1Context.h"), max_initial_modification_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/contexts/LContext1Context.cpp"), max_initial_modification_time)
        
        self.assertGreater(get_file_modification_time("dest/projects/project2/runtime.cpp"), max_initial_modification_time)
        self.assertNotEqual(get_file_modification_time(prev_root_path + "/ws_applicationContext.json"), max_initial_modification_time)
   
    def test_gen_project2_with_diff_2(self):
        [f.unlink() for f in Path("assets/project2/contexts/prev").glob("*") if f.is_file()] 
        [f.unlink() for f in Path("assets/project2/executors/prev").glob("*") if f.is_file()] 
        [f.unlink() for f in Path("assets/project2/prev").glob("*") if f.is_file()] 
        
        root_path = "assets/project2/"
        prev_root_path = root_path + "/prev"
        
        contexts_path = "assets/project2/contexts"
        prev_contexts_path = contexts_path + "/prev"
        if not os.path.exists(contexts_path):
            os.makedirs(contexts_path)
        if not os.path.exists(prev_contexts_path):
            os.makedirs(prev_contexts_path)
            
        executors_path = "assets/project2/executors"
        prev_executors_path = executors_path + "/prev"
        if not os.path.exists(executors_path):
            os.makedirs(executors_path)
        if not os.path.exists(prev_executors_path):
            os.makedirs(prev_executors_path)
        
        shutil.copyfile("assets/expected/project2/prev/lcontext1.json", prev_contexts_path + "/lcontext1.json")
        shutil.copyfile("assets/expected/project2/prev/lContext2_modified.json", prev_contexts_path + "/lContext2.json")
        shutil.copyfile("assets/expected/project2/prev/executor1.json", prev_executors_path + "/executor1.json")
        shutil.copyfile("assets/expected/project2/prev/executor2.json", prev_executors_path + "/executor2.json")
        
        shutil.copyfile("assets/expected/project2/contexts/lContext1Context.h", "dest/projects/project2/contexts/LContext1Context.h")
        shutil.copyfile("assets/expected/project2/contexts/lContext1Context.cpp", "dest/projects/project2/contexts/LContext1Context.cpp")
        shutil.copyfile("assets/expected/project2/contexts/lContext2Context.h", "dest/projects/project2/contexts/LContext2Context.h")
        shutil.copyfile("assets/expected/project2/contexts/lContext2Context.cpp", "dest/projects/project2/contexts/LContext2Context.cpp")
        shutil.copyfile("assets/expected/project2/executors/Executor1Executor.h", "dest/projects/project2/executors/Executor1Executor.h")
        shutil.copyfile("assets/expected/project2/executors/Executor1Executor.cpp", "dest/projects/project2/executors/Executor1Executor.cpp")
        shutil.copyfile("assets/expected/project2/executors/Executor2Executor.h", "dest/projects/project2/executors/Executor2Executor.h")
        shutil.copyfile("assets/expected/project2/executors/Executor2Executor.cpp", "dest/projects/project2/executors/Executor2Executor.cpp")
        
        prev_context1_mod_time = get_file_modification_time(prev_contexts_path + "/lcontext1.json")
        prev_context2_mod_time = get_file_modification_time(prev_contexts_path + "/lcontext2.json")
        prev_executor1_mod_time = get_file_modification_time(prev_executors_path + "/executor1.json")
        prev_executor2_mod_time = get_file_modification_time(prev_executors_path + "/executor2.json")
        
        prev_executor1h_mod_time = get_file_modification_time("dest/projects/project2/executors/Executor1Executor.h")
        prev_executor1cpp_mod_time = get_file_modification_time("dest/projects/project2/executors/Executor1Executor.cpp")
        prev_executor2h_mod_time = get_file_modification_time("dest/projects/project2/executors/Executor2Executor.h")
        prev_executor2cpp_mod_time = get_file_modification_time("dest/projects/project2/executors/Executor2Executor.cpp")
        
        prev_context1h_mod_time = get_file_modification_time("dest/projects/project2/contexts/LContext1Context.h")
        prev_context1cpp_mod_time = get_file_modification_time("dest/projects/project2/contexts/LContext1Context.cpp")
        prev_context2h_mod_time = get_file_modification_time("dest/projects/project2/contexts/LContext2Context.h")
        prev_context2cpp_mod_time = get_file_modification_time("dest/projects/project2/contexts/LContext2Context.cpp")
        
        pre_generation_time = time.time()
        project.generate("assets/project2/project2.json")
        
        utils.assert_files(self, "dest/projects/project2/contexts/LContext1Context.h", "assets/expected/project2/contexts/lContext1Context.h")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext1Context.cpp", "assets/expected/project2/contexts/lContext1Context.cpp")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext2Context.h", "assets/expected/project2/contexts/lContext2Context.h")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext2Context.cpp", "assets/expected/project2/contexts/lContext2Context.cpp")
        
        utils.assert_files(self, "dest/projects/project2/executors/Executor1Executor.h", "assets/expected/project2/executors/Executor1Executor.h")
        utils.assert_files(self, "dest/projects/project2/executors/Executor1Executor.cpp", "assets/expected/project2/executors/Executor1Executor.cpp")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2Executor.h", "assets/expected/project2/executors/Executor2Executor.h")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2Executor.cpp", "assets/expected/project2/executors/Executor2Executor.cpp")
        
        utils.assert_files(self, "dest/projects/project2/executors/Executor1ExecutorImpl.cpp", "assets/expected/project2/executors/Executor1ExecutorImpl.cpp")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2ExecutorImpl.cpp", "assets/expected/project2/executors/Executor2ExecutorImpl.cpp")
        
        utils.assert_files(self, "dest/projects/project2/runtime.cpp", "assets/expected/project2/runtime.cpp")
        
        self.assertEqual(prev_executor1_mod_time, get_file_modification_time(prev_executors_path + "/executor1.json"))
        self.assertEqual(prev_executor2_mod_time, get_file_modification_time(prev_executors_path + "/executor2.json"))
        self.assertEqual(prev_context1_mod_time, get_file_modification_time(prev_contexts_path + "/lcontext1.json"))
        self.assertEqual(prev_context2_mod_time, get_file_modification_time(prev_contexts_path + "/lcontext2.json"))
        
        self.assertEqual(prev_executor1h_mod_time, get_file_modification_time("dest/projects/project2/executors/Executor1Executor.h"))
        self.assertEqual(prev_executor1cpp_mod_time, get_file_modification_time("dest/projects/project2/executors/Executor1Executor.cpp"))
        self.assertEqual(prev_context2h_mod_time, get_file_modification_time("dest/projects/project2/contexts/LContext2Context.h"))
        self.assertEqual(prev_context2cpp_mod_time, get_file_modification_time("dest/projects/project2/contexts/LContext2Context.cpp"))
                
        self.assertEqual(get_file_modification_time("dest/projects/project2/executors/Executor2Executor.h"), prev_executor2h_mod_time)
        self.assertEqual(get_file_modification_time("dest/projects/project2/executors/Executor2Executor.cpp"), prev_executor2cpp_mod_time)
        self.assertEqual(get_file_modification_time("dest/projects/project2/contexts/LContext1Context.h"), prev_context1h_mod_time)
        self.assertEqual(get_file_modification_time("dest/projects/project2/contexts/LContext1Context.cpp"), prev_context1cpp_mod_time)
        
        self.assertGreater(get_file_modification_time("dest/projects/project2/runtime.cpp"), pre_generation_time)
        self.assertNotEqual(get_file_modification_time(prev_root_path + "/ws_applicationContext.json"), pre_generation_time)

    def test_gen_project2_with_diff_workspace_structure(self):
        [f.unlink() for f in Path("assets/project2/contexts/prev").glob("*") if f.is_file()] 
        [f.unlink() for f in Path("assets/project2/executors/prev").glob("*") if f.is_file()] 
        [f.unlink() for f in Path("assets/project2/prev").glob("*") if f.is_file()] 
                
        root_path = "assets/project2/"
        prev_root_path = root_path + "/prev"
        if not os.path.exists(root_path):
            os.makedirs(root_path)
        if not os.path.exists(prev_root_path):
            os.makedirs(prev_root_path)
            
        shutil.copyfile("assets/expected/project2/prev/ws_applicationContext.json", prev_root_path + "/ws_applicationContext.json")
        shutil.copyfile("assets/expected/project2/prev/project2.json", prev_root_path + "/project2.json")
        
        prev_workspace_mod_time = get_file_modification_time(prev_root_path + "/ws_applicationContext.json")
        prev_project_mod_time = get_file_modification_time(prev_root_path + "/project2.json")
        prev_runtime_mod_time = get_file_modification_time("dest/projects/project2/runtime.cpp")
        
        project.generate("assets/project2/project2.json")
        
        utils.assert_files(self, "dest/projects/project2/contexts/LContext1Context.h", "assets/expected/project2/contexts/lContext1Context.h")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext1Context.cpp", "assets/expected/project2/contexts/lContext1Context.cpp")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext2Context.h", "assets/expected/project2/contexts/lContext2Context.h")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext2Context.cpp", "assets/expected/project2/contexts/lContext2Context.cpp")
        
        utils.assert_files(self, "dest/projects/project2/executors/Executor1Executor.h", "assets/expected/project2/executors/Executor1Executor.h")
        utils.assert_files(self, "dest/projects/project2/executors/Executor1Executor.cpp", "assets/expected/project2/executors/Executor1Executor.cpp")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2Executor.h", "assets/expected/project2/executors/Executor2Executor.h")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2Executor.cpp", "assets/expected/project2/executors/Executor2Executor.cpp")
        
        utils.assert_files(self, "dest/projects/project2/executors/Executor1ExecutorImpl.cpp", "assets/expected/project2/executors/Executor1ExecutorImpl.cpp")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2ExecutorImpl.cpp", "assets/expected/project2/executors/Executor2ExecutorImpl.cpp")
        
        utils.assert_files(self, "dest/projects/project2/runtime.cpp", "assets/expected/project2/runtime.cpp")
        
        self.assertEqual(prev_workspace_mod_time, get_file_modification_time(prev_root_path + "/ws_applicationContext.json"))
        self.assertEqual(prev_project_mod_time, get_file_modification_time(prev_root_path + "/project2.json"))
        
        self.assertEqual(prev_runtime_mod_time, get_file_modification_time("dest/projects/project2/runtime.cpp"))
        
        self.assertGreater(get_file_modification_time("dest/projects/project2/executors/Executor1Executor.h"), prev_runtime_mod_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/executors/Executor1Executor.cpp"), prev_runtime_mod_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/contexts/LContext2Context.h"), prev_runtime_mod_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/contexts/LContext2Context.cpp"), prev_runtime_mod_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/executors/Executor2Executor.h"), prev_runtime_mod_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/executors/Executor2Executor.cpp"), prev_runtime_mod_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/contexts/LContext1Context.h"), prev_runtime_mod_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/contexts/LContext1Context.cpp"), prev_runtime_mod_time)
        
    def test_gen_project2_with_no_prev_workspace_structure(self):
        [f.unlink() for f in Path("assets/project2/contexts/prev").glob("*") if f.is_file()] 
        [f.unlink() for f in Path("assets/project2/executors/prev").glob("*") if f.is_file()] 
        [f.unlink() for f in Path("assets/project2/prev").glob("*") if f.is_file()] 
                
        root_path = "assets/project2/"
        prev_root_path = root_path + "/prev"
        
        shutil.copyfile("assets/expected/project2/prev/project2.json", prev_root_path + "/project2.json")
        prev_project_mod_time = get_file_modification_time(prev_root_path + "/project2.json")
        
        pre_generation_time = time.time()
        
        project.generate("assets/project2/project2.json")
        
        utils.assert_files(self, "dest/projects/project2/contexts/LContext1Context.h", "assets/expected/project2/contexts/lContext1Context.h")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext1Context.cpp", "assets/expected/project2/contexts/lContext1Context.cpp")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext2Context.h", "assets/expected/project2/contexts/lContext2Context.h")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext2Context.cpp", "assets/expected/project2/contexts/lContext2Context.cpp")
        
        utils.assert_files(self, "dest/projects/project2/executors/Executor1Executor.h", "assets/expected/project2/executors/Executor1Executor.h")
        utils.assert_files(self, "dest/projects/project2/executors/Executor1Executor.cpp", "assets/expected/project2/executors/Executor1Executor.cpp")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2Executor.h", "assets/expected/project2/executors/Executor2Executor.h")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2Executor.cpp", "assets/expected/project2/executors/Executor2Executor.cpp")
        
        utils.assert_files(self, "dest/projects/project2/executors/Executor1ExecutorImpl.cpp", "assets/expected/project2/executors/Executor1ExecutorImpl.cpp")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2ExecutorImpl.cpp", "assets/expected/project2/executors/Executor2ExecutorImpl.cpp")
        
        utils.assert_files(self, "dest/projects/project2/runtime.cpp", "assets/expected/project2/runtime.cpp")
        
        self.assertGreater(get_file_modification_time("dest/projects/project2/runtime.cpp"), pre_generation_time)
        self.assertGreater(get_file_modification_time(prev_root_path + "/ws_applicationContext.json"), pre_generation_time)
        self.assertEqual(prev_project_mod_time, get_file_modification_time(prev_root_path + "/project2.json"))
        
        self.assertGreater(get_file_modification_time("dest/projects/project2/executors/Executor1Executor.h"), pre_generation_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/executors/Executor1Executor.cpp"), pre_generation_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/contexts/LContext2Context.h"), pre_generation_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/contexts/LContext2Context.cpp"), pre_generation_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/executors/Executor2Executor.h"), pre_generation_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/executors/Executor2Executor.cpp"), pre_generation_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/contexts/LContext1Context.h"), pre_generation_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/contexts/LContext1Context.cpp"), pre_generation_time)

    def test_gen_project2_with_diff_project_structure(self):
        [f.unlink() for f in Path("assets/project2/contexts/prev").glob("*") if f.is_file()] 
        [f.unlink() for f in Path("assets/project2/executors/prev").glob("*") if f.is_file()] 
        [f.unlink() for f in Path("assets/project2/prev").glob("*") if f.is_file()] 
                
        root_path = "assets/project2/"
        prev_root_path = root_path + "/prev"
        if not os.path.exists(root_path):
            os.makedirs(root_path)
        if not os.path.exists(prev_root_path):
            os.makedirs(prev_root_path)
            
        shutil.copyfile("assets/expected/project2/prev/ws_applicationContext.json", prev_root_path + "/ws_applicationContext.json")
        shutil.copyfile("assets/expected/project2/prev/project2Initial.json", prev_root_path + "/project2.json")
        
        prev_workspace_mod_time = get_file_modification_time(prev_root_path + "/ws_applicationContext.json")
        prev_project_mod_time = get_file_modification_time(prev_root_path + "/project2.json")
        prev_runtime_mod_time = get_file_modification_time("dest/projects/project2/runtime.cpp")
        
        project.generate("assets/project2/project2.json")
        
        utils.assert_files(self, "dest/projects/project2/contexts/LContext1Context.h", "assets/expected/project2/contexts/lContext1Context.h")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext1Context.cpp", "assets/expected/project2/contexts/lContext1Context.cpp")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext2Context.h", "assets/expected/project2/contexts/lContext2Context.h")
        utils.assert_files(self, "dest/projects/project2/contexts/LContext2Context.cpp", "assets/expected/project2/contexts/lContext2Context.cpp")
        
        utils.assert_files(self, "dest/projects/project2/executors/Executor1Executor.h", "assets/expected/project2/executors/Executor1Executor.h")
        utils.assert_files(self, "dest/projects/project2/executors/Executor1Executor.cpp", "assets/expected/project2/executors/Executor1Executor.cpp")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2Executor.h", "assets/expected/project2/executors/Executor2Executor.h")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2Executor.cpp", "assets/expected/project2/executors/Executor2Executor.cpp")
        
        utils.assert_files(self, "dest/projects/project2/executors/Executor1ExecutorImpl.cpp", "assets/expected/project2/executors/Executor1ExecutorImpl.cpp")
        utils.assert_files(self, "dest/projects/project2/executors/Executor2ExecutorImpl.cpp", "assets/expected/project2/executors/Executor2ExecutorImpl.cpp")
        
        utils.assert_files(self, "dest/projects/project2/runtime.cpp", "assets/expected/project2/runtime.cpp")
        
        self.assertEqual(prev_workspace_mod_time, get_file_modification_time(prev_root_path + "/ws_applicationContext.json"))
        self.assertGreater(get_file_modification_time(prev_root_path + "/project2.json"), prev_project_mod_time)
        
        self.assertGreater(get_file_modification_time("dest/projects/project2/runtime.cpp"), prev_runtime_mod_time)
        
        self.assertGreater(get_file_modification_time("dest/projects/project2/executors/Executor1Executor.h"), prev_runtime_mod_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/executors/Executor1Executor.cpp"), prev_runtime_mod_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/contexts/LContext2Context.h"), prev_runtime_mod_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/contexts/LContext2Context.cpp"), prev_runtime_mod_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/executors/Executor2Executor.h"), prev_runtime_mod_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/executors/Executor2Executor.cpp"), prev_runtime_mod_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/contexts/LContext1Context.h"), prev_runtime_mod_time)
        self.assertGreater(get_file_modification_time("dest/projects/project2/contexts/LContext1Context.cpp"), prev_runtime_mod_time)