import unittest

import sys
sys.path.append("../src")
import loader
import structures

class TestCore(unittest.TestCase):
    def test_get_contexts_list(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        contexts_list = structures.get_contexts_list(structure)

        self.assertEqual(len(contexts_list), 8)
        self.assertEqual(contexts_list[0], "context1Inst1")
        self.assertEqual(contexts_list[1], "context2Inst1")
        self.assertEqual(contexts_list[2], "context3Inst1")
        self.assertEqual(contexts_list[3], "context4Inst1")
        self.assertEqual(contexts_list[4], "context5Inst1")
        self.assertEqual(contexts_list[5], "context6Inst1")
        self.assertEqual(contexts_list[6], "context7Inst1")
        self.assertEqual(contexts_list[7], "context8Inst1")

    def test_get_executers_list(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        executors_list = structures.get_executors_list(structure)

        self.assertEqual(len(executors_list), 8)
        self.assertEqual(executors_list[0], "executor1")
        self.assertEqual(executors_list[1], "executor2")
        self.assertEqual(executors_list[2], "executor3")
        self.assertEqual(executors_list[3], "executor4")
        self.assertEqual(executors_list[4], "executor5")
        self.assertEqual(executors_list[5], "executor6")
        self.assertEqual(executors_list[6], "executor7")
        self.assertEqual(executors_list[7], "executor8")

    def test_get_anchors_list(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        anchors_list = structures.get_anchors_list(structure)

        self.assertEqual(len(anchors_list), 2)
        self.assertEqual(anchors_list[0], "executor1")
        self.assertEqual(anchors_list[1], "executor2")

    def test_get_initial_dep_mask(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        contexts_list = structures.get_contexts_list(structure)
        anchors_list = structures.get_anchors_list(structure)

        mask = structures.get_initial_deps_mask(structure, anchors_list, contexts_list)

        self.assertEqual(len(mask), 8)
        self.assertEqual(mask[0], 1)
        self.assertEqual(mask[1], 1)
        self.assertEqual(mask[2], 0)
        self.assertEqual(mask[3], 0)
        self.assertEqual(mask[4], 0)
        self.assertEqual(mask[5], 0)
        self.assertEqual(mask[6], 0)
        self.assertEqual(mask[7], 0)
        
    def test_get_executors_dep_mask(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        contexts_list = structures.get_contexts_list(structure)

        masks_list = structures.get_executors_deps_mask(structure, contexts_list)

        self.assertEqual(len(masks_list), 8)
        self.assertEqual(masks_list[0], [1, 0, 0, 0, 0, 0, 0, 0])
        self.assertEqual(masks_list[1], [0, 1, 0, 0, 0, 0, 0, 0])
        self.assertEqual(masks_list[2], [0, 0, 1, 0, 0, 0, 0, 0])
        self.assertEqual(masks_list[3], [0, 0, 0, 1, 0, 0, 0, 0])
        self.assertEqual(masks_list[4], [0, 0, 0, 0, 1, 0, 0, 0])
        self.assertEqual(masks_list[5], [0, 0, 0, 0, 0, 1, 0, 0])
        self.assertEqual(masks_list[6], [0, 0, 0, 0, 0, 0, 1, 0])
        self.assertEqual(masks_list[7], [0, 0, 0, 0, 0, 0, 0, 1])
        
    def test_get_executors_per_contexts(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        contexts_list = structures.get_contexts_list(structure)

        executors_per_contexts = structures.get_executors_per_contexts(structure, contexts_list)

        self.assertEqual(len(executors_per_contexts), 8)
        self.assertEqual(executors_per_contexts[0], ["executor8"])
        self.assertEqual(executors_per_contexts[1], ["executor7"])
        self.assertEqual(executors_per_contexts[2], ["executor1"])
        self.assertEqual(executors_per_contexts[3], ["executor2", "executor3"])
        self.assertEqual(executors_per_contexts[4], ["executor4"])
        self.assertEqual(executors_per_contexts[5], ["executor2", "executor4"])
        self.assertEqual(executors_per_contexts[6], ["executor6"])
        self.assertEqual(executors_per_contexts[7], ["executor5"])

    def test_get_num_of_parts_for_deps_structure(self):
        self.assertEqual(structures.get_num_of_parts_for_deps_structure(1), 1)
        self.assertEqual(structures.get_num_of_parts_for_deps_structure(32), 1)
        self.assertEqual(structures.get_num_of_parts_for_deps_structure(64), 1)
        self.assertEqual(structures.get_num_of_parts_for_deps_structure(65), 2)

    def test_gen_deps_for_executor_1block(self):
        executor_name = "executor1"
        deps_mask = [1, 0, 0, 0, 0, 1, 1, 0]
        desc = structures.get_deps_descs_for_executor(executor_name, deps_mask)
        
        self.assertEqual(desc.executor_name, executor_name)
        self.assertEqual(desc.initials, "0x61")

    def test_gen_deps_for_executor_2block(self):
        executor_name = "executor1"
        deps_mask = [1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0]
        desc = structures.get_deps_descs_for_executor(executor_name, deps_mask)
        self.assertEqual(desc.executor_name, executor_name)
        self.assertEqual(desc.initials, "0x3d3d3d3d,0x3d3d3d3d3d3d3d3d")

    def test_create_structure_1exe(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[0]
        executors = schema["executors"]
        structure = schema["structure"]

        inputs_deps = {
            "context2Inst1": ["executor1"]
        }
        output_deps = {
            "context1Inst1": ["executor1"]
        }

        for executor_name in executors:
            executor_io = structure[executor_name]
            inputs = executor_io["inputs"]
            for input_name in inputs:
                inputs_deps = inputs_deps[input_name]
                for dep in inputs_deps:
                    if dep == executor_name:
                        continue

                    


