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

        self.assertEqual(len(contexts_list), 10)
        self.assertEqual(contexts_list[0], "context10Inst1")
        self.assertEqual(contexts_list[1], "context1Inst1")
        self.assertEqual(contexts_list[2], "context2Inst1")
        self.assertEqual(contexts_list[3], "context3Inst1")
        self.assertEqual(contexts_list[4], "context4Inst1")
        self.assertEqual(contexts_list[5], "context5Inst1")
        self.assertEqual(contexts_list[6], "context6Inst1")
        self.assertEqual(contexts_list[7], "context7Inst1")
        self.assertEqual(contexts_list[8], "context8Inst1")
        self.assertEqual(contexts_list[9], "context9Inst1")

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

        print(contexts_list, "contexts_list")
        mask = structures.get_initial_deps_mask(structure, anchors_list, contexts_list)

        self.assertEqual(len(mask), 10)
        self.assertEqual(mask[0], 0)
        self.assertEqual(mask[1], 1)
        self.assertEqual(mask[2], 1)
        self.assertEqual(mask[3], 0)
        self.assertEqual(mask[4], 0)
        self.assertEqual(mask[5], 0)
        self.assertEqual(mask[6], 0)
        self.assertEqual(mask[7], 0)
        self.assertEqual(mask[8], 0)
        self.assertEqual(mask[9], 0)

    def test_get_executors_segmented(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        executors_mandatory_list, executors_opt_list = structures.get_executors_segmented(structure)
        self.assertEqual(len(executors_mandatory_list), 5)
        self.assertEqual(len(executors_opt_list), 3)

        self.assertEqual(executors_mandatory_list[0], "executor2")
        self.assertEqual(executors_mandatory_list[1], "executor4")
        self.assertEqual(executors_mandatory_list[2], "executor5")
        self.assertEqual(executors_mandatory_list[3], "executor6")
        self.assertEqual(executors_mandatory_list[4], "executor7")

        self.assertEqual(executors_opt_list[0], "executor1")
        self.assertEqual(executors_opt_list[1], "executor3")
        self.assertEqual(executors_opt_list[2], "executor8")

    def test_get_executors_dep_mask(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        contexts_list = structures.get_contexts_list(structure)

        masks_list = structures.get_executors_deps_mask(structure, contexts_list)

        self.assertEqual(len(masks_list), 8)
        self.assertEqual(masks_list[0].executor_name, "executor1")
        self.assertEqual(masks_list[0].mask, [0, 1, 0, 0, 0, 0, 0, 0, 0, 0])
        self.assertEqual(masks_list[1].executor_name, "executor2")
        self.assertEqual(masks_list[1].mask, [0, 0, 1, 0, 0, 0, 0, 0, 0, 0])
        self.assertEqual(masks_list[2].executor_name, "executor3")
        self.assertEqual(masks_list[2].mask, [0, 0, 0, 1, 0, 0, 0, 0, 0, 0])
        self.assertEqual(masks_list[3].executor_name, "executor4")
        self.assertEqual(masks_list[3].mask, [0, 0, 0, 0, 1, 0, 0, 0, 0, 0])
        self.assertEqual(masks_list[4].executor_name, "executor5")
        self.assertEqual(masks_list[4].mask, [0, 0, 0, 0, 0, 1, 0, 0, 0, 0])
        self.assertEqual(masks_list[5].executor_name, "executor6")
        self.assertEqual(masks_list[5].mask, [0, 0, 0, 0, 0, 0, 1, 0, 0, 0])
        self.assertEqual(masks_list[6].executor_name, "executor7")
        self.assertEqual(masks_list[6].mask, [0, 0, 0, 0, 0, 0, 0, 1, 0, 0])
        self.assertEqual(masks_list[7].executor_name, "executor8")
        self.assertEqual(masks_list[7].mask, [1, 0, 0, 0, 0, 0, 0, 0, 1, 0])
        
    def test_get_executors_inputs_contexts(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        data = structures.get_executors_inputs_contexts(structure)

        self.assertEqual(len(data), 8)

        self.assertEqual(list(data.keys())[0], "executor1")
        self.assertEqual(len(data[list(data.keys())[0]]), 1)
        self.assertEqual(data[list(data.keys())[0]][0].input_name, "context1Input")
        self.assertEqual(data[list(data.keys())[0]][0].instance_name, "context1Inst1")
        
        self.assertEqual(list(data.keys())[1], "executor2")
        self.assertEqual(len(data[list(data.keys())[1]]), 1)
        self.assertEqual(data[list(data.keys())[1]][0].input_name, "context2Input")
        self.assertEqual(data[list(data.keys())[1]][0].instance_name, "context2Inst1")
        
        self.assertEqual(list(data.keys())[2], "executor3")
        self.assertEqual(len(data[list(data.keys())[3]]), 1)
        self.assertEqual(data[list(data.keys())[2]][0].input_name, "context3Input")
        self.assertEqual(data[list(data.keys())[2]][0].instance_name, "context3Inst1")
        
        self.assertEqual(list(data.keys())[3], "executor4")
        self.assertEqual(len(data[list(data.keys())[3]]), 1)
        self.assertEqual(data[list(data.keys())[3]][0].input_name, "context4Input")
        self.assertEqual(data[list(data.keys())[3]][0].instance_name, "context4Inst1")
        
        self.assertEqual(list(data.keys())[4], "executor5")
        self.assertEqual(len(data[list(data.keys())[4]]), 1)
        self.assertEqual(data[list(data.keys())[4]][0].input_name, "context5Input")
        self.assertEqual(data[list(data.keys())[4]][0].instance_name, "context5Inst1")
        
        self.assertEqual(list(data.keys())[5], "executor6")
        self.assertEqual(len(data[list(data.keys())[5]]), 1)
        self.assertEqual(data[list(data.keys())[5]][0].input_name, "context6Input")
        self.assertEqual(data[list(data.keys())[5]][0].instance_name, "context6Inst1")
        
        self.assertEqual(list(data.keys())[6], "executor7")
        self.assertEqual(len(data[list(data.keys())[6]]), 1)
        self.assertEqual(data[list(data.keys())[6]][0].input_name, "context7Input")
        self.assertEqual(data[list(data.keys())[6]][0].instance_name, "context7Inst1")
        
        self.assertEqual(list(data.keys())[7], "executor8")
        self.assertEqual(len(data[list(data.keys())[7]]), 2)
        self.assertEqual(data[list(data.keys())[7]][0].input_name, "context8Input")
        self.assertEqual(data[list(data.keys())[7]][0].instance_name, "context8Inst1")
        self.assertEqual(data[list(data.keys())[7]][1].input_name, "context10Input")
        self.assertEqual(data[list(data.keys())[7]][1].instance_name, "context10Inst1")

    def test_get_executors_dep_contexts(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        contexts_list = structures.get_contexts_list(structure)

        data = structures.get_executors_deps_contexts(structure, contexts_list)

        self.assertEqual(len(data), 8)
        self.assertEqual(data[0].executor_name, "executor1")
        self.assertEqual(data[0].contexts_list, ["context3Inst1"])
        self.assertEqual(data[0].depenency_contexts_mask, "0x2")
        self.assertEqual(data[1].executor_name, "executor2")
        self.assertEqual(data[1].contexts_list, ["context4Inst1", "context6Inst1"])
        self.assertEqual(data[1].depenency_contexts_mask, "0x4")
        self.assertEqual(data[2].executor_name, "executor3")
        self.assertEqual(data[2].contexts_list, ["context4Inst1", "context9Inst1"])
        self.assertEqual(data[2].depenency_contexts_mask, "0x8")
        self.assertEqual(data[3].executor_name, "executor4")
        self.assertEqual(data[3].contexts_list, ["context5Inst1", "context6Inst1"])
        self.assertEqual(data[3].depenency_contexts_mask, "0x10")
        self.assertEqual(data[4].executor_name, "executor5")
        self.assertEqual(data[4].contexts_list, ["context8Inst1"])
        self.assertEqual(data[4].depenency_contexts_mask, "0x20")
        self.assertEqual(data[5].executor_name, "executor6")
        self.assertEqual(data[5].contexts_list, ["context7Inst1", "context9Inst1"])
        self.assertEqual(data[5].depenency_contexts_mask, "0x40")
        self.assertEqual(data[6].executor_name, "executor7")
        self.assertEqual(data[6].contexts_list, ["context2Inst1", "context8Inst1"])
        self.assertEqual(data[6].depenency_contexts_mask, "0x80")
        self.assertEqual(data[7].executor_name, "executor8")
        self.assertEqual(data[7].contexts_list, ["context1Inst1"])
        self.assertEqual(data[7].depenency_contexts_mask, "0x101")
        
    def test_get_executors_per_contexts(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        contexts_list = structures.get_contexts_list(structure)
        _, optional_executors_list = structures.get_executors_segmented(structure)

        executors_per_contexts = structures.get_executors_per_contexts(structure, contexts_list, optional_executors_list)

        self.assertEqual(len(executors_per_contexts), 9)
        self.assertEqual(executors_per_contexts[0].context_instance_name, "context1Inst1")
        self.assertEqual(executors_per_contexts[0].is_need_to_reset, False)
        self.assertEqual(len(executors_per_contexts[0].data), 1)
        self.assertEqual(len(executors_per_contexts[0].data_non_opt), 0)
        self.assertEqual(len(executors_per_contexts[0].data_opt), 1)
        self.assertEqual(executors_per_contexts[0].data_opt[0].executor_name, "executor8")
        self.assertEqual(executors_per_contexts[0].data_opt[0].context_target_name, "context1Output")
        self.assertEqual(executors_per_contexts[0].data_opt[0].optional_executor_id, 2)
        self.assertEqual(executors_per_contexts[0].data[0].executor_name, "executor8")
        self.assertEqual(executors_per_contexts[0].data[0].context_target_name, "context1Output")

        self.assertEqual(executors_per_contexts[1].context_instance_name, "context2Inst1")
        self.assertEqual(executors_per_contexts[1].is_need_to_reset, True)
        self.assertEqual(len(executors_per_contexts[1].data), 1)
        self.assertEqual(len(executors_per_contexts[1].data_non_opt), 1)
        self.assertEqual(len(executors_per_contexts[1].data_opt), 0)
        self.assertEqual(executors_per_contexts[1].data[0].executor_name, "executor7")
        self.assertEqual(executors_per_contexts[1].data[0].context_target_name, "context2Output")
        self.assertEqual(executors_per_contexts[1].data_non_opt[0].executor_name, "executor7")
        self.assertEqual(executors_per_contexts[1].data_non_opt[0].context_target_name, "context2Output")

        self.assertEqual(executors_per_contexts[2].context_instance_name, "context3Inst1")
        self.assertEqual(executors_per_contexts[2].is_need_to_reset, False)
        self.assertEqual(len(executors_per_contexts[2].data), 1)
        self.assertEqual(len(executors_per_contexts[2].data_non_opt), 0)
        self.assertEqual(len(executors_per_contexts[2].data_opt), 1)
        self.assertEqual(executors_per_contexts[2].data[0].executor_name, "executor1")
        self.assertEqual(executors_per_contexts[2].data[0].context_target_name, "context3Output")
        self.assertEqual(executors_per_contexts[2].data_opt[0].executor_name, "executor1")
        self.assertEqual(executors_per_contexts[2].data_opt[0].context_target_name, "context3Output")
        self.assertEqual(executors_per_contexts[2].data_opt[0].optional_executor_id, 0)

        self.assertEqual(executors_per_contexts[3].context_instance_name, "context4Inst1")
        self.assertEqual(executors_per_contexts[3].is_need_to_reset, False)
        self.assertEqual(len(executors_per_contexts[3].data), 2)
        self.assertEqual(len(executors_per_contexts[3].data_non_opt), 1)
        self.assertEqual(len(executors_per_contexts[3].data_opt), 1)
        self.assertEqual(executors_per_contexts[3].data[0].executor_name, "executor2")
        self.assertEqual(executors_per_contexts[3].data[0].context_target_name, "context4Output")
        self.assertEqual(executors_per_contexts[3].data[1].executor_name, "executor3")
        self.assertEqual(executors_per_contexts[3].data[1].context_target_name, "context4Output")
        self.assertEqual(executors_per_contexts[3].data_non_opt[0].executor_name, "executor2")
        self.assertEqual(executors_per_contexts[3].data_non_opt[0].context_target_name, "context4Output")
        self.assertEqual(executors_per_contexts[3].data_opt[0].executor_name, "executor3")
        self.assertEqual(executors_per_contexts[3].data_opt[0].context_target_name, "context4Output")
        self.assertEqual(executors_per_contexts[3].data_opt[0].optional_executor_id, 1)

        self.assertEqual(executors_per_contexts[4].context_instance_name, "context5Inst1")
        self.assertEqual(executors_per_contexts[4].is_need_to_reset, False)
        self.assertEqual(len(executors_per_contexts[4].data), 1)
        self.assertEqual(len(executors_per_contexts[4].data_non_opt), 1)
        self.assertEqual(len(executors_per_contexts[4].data_opt), 0)
        self.assertEqual(executors_per_contexts[4].data[0].executor_name, "executor4")
        self.assertEqual(executors_per_contexts[4].data[0].context_target_name, "context5Output")
        self.assertEqual(executors_per_contexts[4].data_non_opt[0].executor_name, "executor4")
        self.assertEqual(executors_per_contexts[4].data_non_opt[0].context_target_name, "context5Output")

        self.assertEqual(executors_per_contexts[5].context_instance_name, "context6Inst1")
        self.assertEqual(executors_per_contexts[5].is_need_to_reset, False)
        self.assertEqual(len(executors_per_contexts[5].data), 2)
        self.assertEqual(len(executors_per_contexts[5].data_non_opt), 2)
        self.assertEqual(len(executors_per_contexts[5].data_opt), 0)
        self.assertEqual(executors_per_contexts[5].data[0].executor_name, "executor2")
        self.assertEqual(executors_per_contexts[5].data[0].context_target_name, "context6Output")
        self.assertEqual(executors_per_contexts[5].data[1].executor_name, "executor4")
        self.assertEqual(executors_per_contexts[5].data[1].context_target_name, "context6Output")
        self.assertEqual(executors_per_contexts[5].data_non_opt[0].executor_name, "executor2")
        self.assertEqual(executors_per_contexts[5].data_non_opt[0].context_target_name, "context6Output")
        self.assertEqual(executors_per_contexts[5].data_non_opt[1].executor_name, "executor4")
        self.assertEqual(executors_per_contexts[5].data_non_opt[1].context_target_name, "context6Output")

        self.assertEqual(executors_per_contexts[6].context_instance_name, "context7Inst1")
        self.assertEqual(executors_per_contexts[6].is_need_to_reset, False)
        self.assertEqual(len(executors_per_contexts[6].data), 1)
        self.assertEqual(len(executors_per_contexts[6].data_non_opt), 1)
        self.assertEqual(len(executors_per_contexts[6].data_opt), 0)
        self.assertEqual(executors_per_contexts[6].data[0].executor_name, "executor6")
        self.assertEqual(executors_per_contexts[6].data[0].context_target_name, "context7Output")
        self.assertEqual(executors_per_contexts[6].data_non_opt[0].executor_name, "executor6")
        self.assertEqual(executors_per_contexts[6].data_non_opt[0].context_target_name, "context7Output")

        self.assertEqual(executors_per_contexts[7].context_instance_name, "context8Inst1")
        self.assertEqual(executors_per_contexts[7].is_need_to_reset, True)
        self.assertEqual(len(executors_per_contexts[7].data), 2)
        self.assertEqual(len(executors_per_contexts[7].data_non_opt), 2)
        self.assertEqual(len(executors_per_contexts[7].data_opt), 0)
        self.assertEqual(executors_per_contexts[7].data[0].executor_name, "executor5")
        self.assertEqual(executors_per_contexts[7].data[0].context_target_name, "context8Output")
        self.assertEqual(executors_per_contexts[7].data[1].executor_name, "executor7")
        self.assertEqual(executors_per_contexts[7].data[1].context_target_name, "context8Output")
        self.assertEqual(executors_per_contexts[7].data_non_opt[0].executor_name, "executor5")
        self.assertEqual(executors_per_contexts[7].data_non_opt[0].context_target_name, "context8Output")
        self.assertEqual(executors_per_contexts[7].data_non_opt[1].executor_name, "executor7")
        self.assertEqual(executors_per_contexts[7].data_non_opt[1].context_target_name, "context8Output")

        self.assertEqual(executors_per_contexts[8].context_instance_name, "context9Inst1")
        self.assertEqual(executors_per_contexts[8].is_need_to_reset, False)
        self.assertEqual(len(executors_per_contexts[8].data), 2)
        self.assertEqual(len(executors_per_contexts[8].data_non_opt), 1)
        self.assertEqual(len(executors_per_contexts[8].data_opt), 1)
        self.assertEqual(executors_per_contexts[8].data[0].executor_name, "executor3")
        self.assertEqual(executors_per_contexts[8].data[0].context_target_name, "context9Output")
        self.assertEqual(executors_per_contexts[8].data[1].executor_name, "executor6")
        self.assertEqual(executors_per_contexts[8].data[1].context_target_name, "context9Output")
        self.assertEqual(executors_per_contexts[8].data_non_opt[0].executor_name, "executor6")
        self.assertEqual(executors_per_contexts[8].data_non_opt[0].context_target_name, "context9Output")
        self.assertEqual(executors_per_contexts[8].data_opt[0].executor_name, "executor3")
        self.assertEqual(executors_per_contexts[8].data_opt[0].context_target_name, "context9Output")

    def test_get_number_of_parts(self):
        self.assertEqual(structures.compute_number_of_parts(1), 1)
        self.assertEqual(structures.compute_number_of_parts(32), 1)
        self.assertEqual(structures.compute_number_of_parts(64), 1)
        self.assertEqual(structures.compute_number_of_parts(65), 2)

    def test_convert_to_mask_1block(self):
        deps_mask = [1, 0, 0, 0, 0, 1, 1, 0]
        mask = structures.convert_to_mask(deps_mask)
        
        self.assertEqual(mask, "0x61")

    def test_convert_to_mask_2block(self):
        deps_mask = [1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0]
        mask = structures.convert_to_mask(deps_mask)
        self.assertEqual(mask, "0x3d3d3d3d,0x3d3d3d3d3d3d3d3d")

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

                    


