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

        self.assertEqual(len(contexts_list), 11)
        self.assertEqual(contexts_list[0], "contextAInst1")
        self.assertEqual(contexts_list[1], "contextBInst1")
        self.assertEqual(contexts_list[2], "contextCInst1")
        self.assertEqual(contexts_list[3], "contextDInst1")
        self.assertEqual(contexts_list[4], "contextEInst1")
        self.assertEqual(contexts_list[5], "contextFInst1")
        self.assertEqual(contexts_list[6], "contextGInst1")
        self.assertEqual(contexts_list[7], "contextHInst1")
        self.assertEqual(contexts_list[8], "contextIInst1")
        self.assertEqual(contexts_list[9], "contextJInst1")
        self.assertEqual(contexts_list[10], "contextKInst1")

    def test_get_contexts_types_list(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        contexts_list = structures.get_contexts_types_list(structure)

        self.assertEqual(len(contexts_list), 9)
        self.assertEqual(contexts_list[0], "EContextA")
        self.assertEqual(contexts_list[1], "EContextB")
        self.assertEqual(contexts_list[2], "SContextA")
        self.assertEqual(contexts_list[3], "SContextB")
        self.assertEqual(contexts_list[4], "SContextC")
        self.assertEqual(contexts_list[5], "SContextD")
        self.assertEqual(contexts_list[6], "SContextE")
        self.assertEqual(contexts_list[7], "SContextF")
        self.assertEqual(contexts_list[8], "SContextG")

    def test_get_contexts_descs_list(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        contexts_list = structures.get_contexts_descs_list(structure)

        self.assertEqual(len(contexts_list), 11)
        self.assertEqual(contexts_list[0].context_name, "SContextA")
        self.assertEqual(contexts_list[0].instance_name, "contextAInst1")

        self.assertEqual(contexts_list[1].context_name, "SContextB")
        self.assertEqual(contexts_list[1].instance_name, "contextBInst1")

        self.assertEqual(contexts_list[2].context_name, "SContextC")
        self.assertEqual(contexts_list[2].instance_name, "contextCInst1")

        self.assertEqual(contexts_list[3].context_name, "SContextC")
        self.assertEqual(contexts_list[3].instance_name, "contextDInst1")

        self.assertEqual(contexts_list[4].context_name, "SContextD")
        self.assertEqual(contexts_list[4].instance_name, "contextEInst1")

        self.assertEqual(contexts_list[5].context_name, "SContextE")
        self.assertEqual(contexts_list[5].instance_name, "contextFInst1")

        self.assertEqual(contexts_list[6].context_name, "SContextE")
        self.assertEqual(contexts_list[6].instance_name, "contextGInst1")

        self.assertEqual(contexts_list[7].context_name, "SContextF")
        self.assertEqual(contexts_list[7].instance_name, "contextHInst1")

        self.assertEqual(contexts_list[8].context_name, "SContextF")
        self.assertEqual(contexts_list[8].instance_name, "contextIInst1")

        self.assertEqual(contexts_list[9].context_name, "SContextF")
        self.assertEqual(contexts_list[9].instance_name, "contextJInst1")

        self.assertEqual(contexts_list[10].context_name, "SContextG")
        self.assertEqual(contexts_list[10].instance_name, "contextKInst1")

    def test_get_ext_contexts_descs_list(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        contexts_list = structures.get_ext_contexts_descs_list(structure)

        self.assertEqual(len(contexts_list), 3)
        self.assertEqual(contexts_list[0].context_name, "EContextA")
        self.assertEqual(contexts_list[0].instance_name, "extContextAInst1")

        self.assertEqual(contexts_list[1].context_name, "EContextA")
        self.assertEqual(contexts_list[1].instance_name, "extContextAInst2")

        self.assertEqual(contexts_list[2].context_name, "EContextB")
        self.assertEqual(contexts_list[2].instance_name, "extContextBInst1")

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

        self.assertEqual(len(mask), 11)
        self.assertEqual(mask[0], 1)
        self.assertEqual(mask[1], 1)
        self.assertEqual(mask[2], 0)
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

    def test_get_optional_executors_data(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        _, executors_opt_list = structures.get_executors_segmented(structure)
        optional_executors_data = structures.get_optional_executors_data(structure, executors_opt_list)

        self.assertEqual(len(optional_executors_data), 3)
        self.assertEqual(optional_executors_data[0].executor_name, "executor1")
        self.assertEqual(optional_executors_data[0].toggle_code, 1)
        self.assertEqual(optional_executors_data[1].executor_name, "executor3")
        self.assertEqual(optional_executors_data[1].toggle_code, 3)
        self.assertEqual(optional_executors_data[2].executor_name, "executor8")
        self.assertEqual(optional_executors_data[2].toggle_code, 8)

    def test_get_executors_dep_mask(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        contexts_list = structures.get_contexts_list(structure)

        masks_list = structures.get_executors_deps_mask(structure, contexts_list)

        self.assertEqual(len(masks_list), 8)
        self.assertEqual(masks_list[0].executor_name, "executor1")
        self.assertEqual(masks_list[0].mask, [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
        self.assertEqual(masks_list[1].executor_name, "executor2")
        self.assertEqual(masks_list[1].mask, [0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0])
        self.assertEqual(masks_list[2].executor_name, "executor3")
        self.assertEqual(masks_list[2].mask, [0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0])
        self.assertEqual(masks_list[3].executor_name, "executor4")
        self.assertEqual(masks_list[3].mask, [0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0])
        self.assertEqual(masks_list[4].executor_name, "executor5")
        self.assertEqual(masks_list[4].mask, [0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0])
        self.assertEqual(masks_list[5].executor_name, "executor6")
        self.assertEqual(masks_list[5].mask, [0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0])
        self.assertEqual(masks_list[6].executor_name, "executor7")
        self.assertEqual(masks_list[6].mask, [0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0])
        self.assertEqual(masks_list[7].executor_name, "executor8")
        self.assertEqual(masks_list[7].mask, [0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0])

    def test_get_executors_types_list(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        types_list = structures.get_executors_type_list(structure)

        self.assertEqual(len(types_list), 6)
        self.assertEqual(types_list[0], "Executor1")
        self.assertEqual(types_list[1], "Executor2")
        self.assertEqual(types_list[2], "Executor3")
        self.assertEqual(types_list[3], "Executor4")
        self.assertEqual(types_list[4], "Executor5")
        self.assertEqual(types_list[5], "Executor6")


    def test_get_executors_descs_list(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        descs = structures.get_executors_descs_list(structure)

        self.assertEqual(len(descs), 8)
        self.assertEqual(descs[0].executor_type, "Executor1")
        self.assertEqual(descs[0].executor_name, "executor1")
        self.assertEqual(descs[1].executor_type, "Executor1")
        self.assertEqual(descs[1].executor_name, "executor2")
        self.assertEqual(descs[2].executor_type, "Executor2")
        self.assertEqual(descs[2].executor_name, "executor3")
        self.assertEqual(descs[3].executor_type, "Executor3")
        self.assertEqual(descs[3].executor_name, "executor4")
        self.assertEqual(descs[4].executor_type, "Executor3")
        self.assertEqual(descs[4].executor_name, "executor5")
        self.assertEqual(descs[5].executor_type, "Executor4")
        self.assertEqual(descs[5].executor_name, "executor6")
        self.assertEqual(descs[6].executor_type, "Executor5")
        self.assertEqual(descs[6].executor_name, "executor7")
        self.assertEqual(descs[7].executor_type, "Executor6")
        self.assertEqual(descs[7].executor_name, "executor8")

    def test_get_executors_inputs_contexts(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        data = structures.get_executors_inputs_contexts(structure)

        self.assertEqual(len(data), 8)

        self.assertEqual(list(data.keys())[0], "executor1")
        self.assertEqual(len(data[list(data.keys())[0]]), 1)
        self.assertEqual(data[list(data.keys())[0]][0].input_name, "context1Input")
        self.assertEqual(data[list(data.keys())[0]][0].instance_name, "contextAInst1")
        
        self.assertEqual(list(data.keys())[1], "executor2")
        self.assertEqual(len(data[list(data.keys())[1]]), 1)
        self.assertEqual(data[list(data.keys())[1]][0].input_name, "context2Input")
        self.assertEqual(data[list(data.keys())[1]][0].instance_name, "contextBInst1")
        
        self.assertEqual(list(data.keys())[2], "executor3")
        self.assertEqual(len(data[list(data.keys())[3]]), 1)
        self.assertEqual(data[list(data.keys())[2]][0].input_name, "context3Input")
        self.assertEqual(data[list(data.keys())[2]][0].instance_name, "contextCInst1")
        
        self.assertEqual(list(data.keys())[3], "executor4")
        self.assertEqual(len(data[list(data.keys())[3]]), 1)
        self.assertEqual(data[list(data.keys())[3]][0].input_name, "context4Input")
        self.assertEqual(data[list(data.keys())[3]][0].instance_name, "contextDInst1")
        
        self.assertEqual(list(data.keys())[4], "executor5")
        self.assertEqual(len(data[list(data.keys())[4]]), 1)
        self.assertEqual(data[list(data.keys())[4]][0].input_name, "context5Input")
        self.assertEqual(data[list(data.keys())[4]][0].instance_name, "contextEInst1")
        
        self.assertEqual(list(data.keys())[5], "executor6")
        self.assertEqual(len(data[list(data.keys())[5]]), 1)
        self.assertEqual(data[list(data.keys())[5]][0].input_name, "context6Input")
        self.assertEqual(data[list(data.keys())[5]][0].instance_name, "contextFInst1")
        
        self.assertEqual(list(data.keys())[6], "executor7")
        self.assertEqual(len(data[list(data.keys())[6]]), 1)
        self.assertEqual(data[list(data.keys())[6]][0].input_name, "context7Input")
        self.assertEqual(data[list(data.keys())[6]][0].instance_name, "contextGInst1")
        
        self.assertEqual(list(data.keys())[7], "executor8")
        self.assertEqual(len(data[list(data.keys())[7]]), 2)
        self.assertEqual(data[list(data.keys())[7]][0].input_name, "context8Input")
        self.assertEqual(data[list(data.keys())[7]][0].instance_name, "contextHInst1")
        self.assertEqual(data[list(data.keys())[7]][1].input_name, "context10Input")
        self.assertEqual(data[list(data.keys())[7]][1].instance_name, "contextJInst1")

    def test_get_executors_ext_inputs_contexts(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        data = structures.get_executors_ext_inputs_contexts(structure)

        self.assertEqual(len(data), 2)

        self.assertEqual(list(data.keys())[0], "executor6")
        self.assertEqual(len(data[list(data.keys())[0]]), 2)
        self.assertEqual(data[list(data.keys())[0]][0].input_name, "extInput1")
        self.assertEqual(data[list(data.keys())[0]][0].instance_name, "extContextAInst1")
        self.assertEqual(data[list(data.keys())[0]][1].input_name, "extInput2")
        self.assertEqual(data[list(data.keys())[0]][1].instance_name, "extContextBInst1")
        
        self.assertEqual(list(data.keys())[1], "executor8")
        self.assertEqual(len(data[list(data.keys())[1]]), 1)
        self.assertEqual(data[list(data.keys())[1]][0].input_name, "extInput1")
        self.assertEqual(data[list(data.keys())[1]][0].instance_name, "extContextAInst2")

    def test_get_executors_ext_outputs(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        data = structures.get_executors_ext_outputs(structure)
        self.assertEqual(len(data), 2)

        self.assertEqual(list(data.keys())[0], "executor1")
        self.assertEqual(len(data[list(data.keys())[0]]), 1)
        self.assertEqual(data[list(data.keys())[0]][0].output_name, "extOutput1")
        self.assertEqual(data[list(data.keys())[0]][0].instance_name, "extContextAInst1")
        self.assertEqual(len(data[list(data.keys())[1]]), 2)
        self.assertEqual(data[list(data.keys())[1]][0].output_name, "extOutput1")
        self.assertEqual(data[list(data.keys())[1]][0].instance_name, "extContextAInst2")
        self.assertEqual(data[list(data.keys())[1]][1].output_name, "extOutput2")
        self.assertEqual(data[list(data.keys())[1]][1].instance_name, "extContextBInst1")

    def test_get_executors_dep_contexts(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        contexts_list = structures.get_contexts_list(structure)

        data = structures.get_executors_deps_contexts(structure, contexts_list)

        self.assertEqual(len(data), 8)
        self.assertEqual(data[0].executor_name, "executor1")
        self.assertEqual(data[0].contexts_list, ["contextCInst1"])
        self.assertEqual(data[0].depenency_contexts_mask, "0x1")
        self.assertEqual(data[1].executor_name, "executor2")
        self.assertEqual(data[1].contexts_list, ["contextDInst1", "contextFInst1", "contextKInst1"])
        self.assertEqual(data[1].depenency_contexts_mask, "0x2")
        self.assertEqual(data[2].executor_name, "executor3")
        self.assertEqual(data[2].contexts_list, ["contextDInst1", "contextIInst1"])
        self.assertEqual(data[2].depenency_contexts_mask, "0x4")
        self.assertEqual(data[3].executor_name, "executor4")
        self.assertEqual(data[3].contexts_list, ["contextEInst1", "contextFInst1"])
        self.assertEqual(data[3].depenency_contexts_mask, "0x8")
        self.assertEqual(data[4].executor_name, "executor5")
        self.assertEqual(data[4].contexts_list, ["contextHInst1"])
        self.assertEqual(data[4].depenency_contexts_mask, "0x10")
        self.assertEqual(data[5].executor_name, "executor6")
        self.assertEqual(data[5].contexts_list, ["contextGInst1", "contextIInst1"])
        self.assertEqual(data[5].depenency_contexts_mask, "0x20")
        self.assertEqual(data[6].executor_name, "executor7")
        self.assertEqual(data[6].contexts_list, ["contextBInst1", "contextHInst1"])
        self.assertEqual(data[6].depenency_contexts_mask, "0x40")
        self.assertEqual(data[7].executor_name, "executor8")
        self.assertEqual(data[7].contexts_list, ["contextAInst1"])
        self.assertEqual(data[7].depenency_contexts_mask, "0x280")
        
    def test_get_executors_per_contexts(self):
        data = loader.load_file_data("assets/structures/dummy_structures.json")
        schema = data[1]
        structure = schema["structure"]

        contexts_list = structures.get_contexts_list(structure)
        _, optional_executors_list = structures.get_executors_segmented(structure)

        executors_per_contexts = structures.get_executors_per_contexts(structure, contexts_list, optional_executors_list)

        self.assertEqual(len(executors_per_contexts), 10)
        self.assertEqual(executors_per_contexts[0].context_instance_name, "contextAInst1")
        self.assertEqual(executors_per_contexts[0].is_need_to_reset, False)
        self.assertEqual(executors_per_contexts[0].context_el_id, 0)
        self.assertEqual(len(executors_per_contexts[0].data), 1)
        self.assertEqual(len(executors_per_contexts[0].data_non_opt), 0)
        self.assertEqual(len(executors_per_contexts[0].data_opt), 1)
        self.assertEqual(executors_per_contexts[0].data_opt[0].executor_name, "executor8")
        self.assertEqual(executors_per_contexts[0].data_opt[0].context_target_name, "context1Output")
        self.assertEqual(executors_per_contexts[0].data_opt[0].optional_executor_id, 2)
        self.assertEqual(executors_per_contexts[0].data[0].executor_name, "executor8")
        self.assertEqual(executors_per_contexts[0].data[0].context_target_name, "context1Output")

        self.assertEqual(executors_per_contexts[1].context_instance_name, "contextBInst1")
        self.assertEqual(executors_per_contexts[1].is_need_to_reset, True)
        self.assertEqual(executors_per_contexts[1].context_el_id, 1)
        self.assertEqual(len(executors_per_contexts[1].data), 1)
        self.assertEqual(len(executors_per_contexts[1].data_non_opt), 1)
        self.assertEqual(len(executors_per_contexts[1].data_opt), 0)
        self.assertEqual(executors_per_contexts[1].data[0].executor_name, "executor7")
        self.assertEqual(executors_per_contexts[1].data[0].context_target_name, "context2Output")
        self.assertEqual(executors_per_contexts[1].data_non_opt[0].executor_name, "executor7")
        self.assertEqual(executors_per_contexts[1].data_non_opt[0].context_target_name, "context2Output")

        self.assertEqual(executors_per_contexts[2].context_instance_name, "contextCInst1")
        self.assertEqual(executors_per_contexts[2].is_need_to_reset, False)
        self.assertEqual(executors_per_contexts[2].context_el_id, 2)
        self.assertEqual(len(executors_per_contexts[2].data), 1)
        self.assertEqual(len(executors_per_contexts[2].data_non_opt), 0)
        self.assertEqual(len(executors_per_contexts[2].data_opt), 1)
        self.assertEqual(executors_per_contexts[2].data[0].executor_name, "executor1")
        self.assertEqual(executors_per_contexts[2].data[0].context_target_name, "context3Output")
        self.assertEqual(executors_per_contexts[2].data_opt[0].executor_name, "executor1")
        self.assertEqual(executors_per_contexts[2].data_opt[0].context_target_name, "context3Output")
        self.assertEqual(executors_per_contexts[2].data_opt[0].optional_executor_id, 0)

        self.assertEqual(executors_per_contexts[3].context_instance_name, "contextDInst1")
        self.assertEqual(executors_per_contexts[3].is_need_to_reset, False)
        self.assertEqual(executors_per_contexts[3].context_el_id, 3)
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

        self.assertEqual(executors_per_contexts[4].context_instance_name, "contextEInst1")
        self.assertEqual(executors_per_contexts[4].is_need_to_reset, False)
        self.assertEqual(executors_per_contexts[4].context_el_id, 4)
        self.assertEqual(len(executors_per_contexts[4].data), 1)
        self.assertEqual(len(executors_per_contexts[4].data_non_opt), 1)
        self.assertEqual(len(executors_per_contexts[4].data_opt), 0)
        self.assertEqual(executors_per_contexts[4].data[0].executor_name, "executor4")
        self.assertEqual(executors_per_contexts[4].data[0].context_target_name, "context5Output")
        self.assertEqual(executors_per_contexts[4].data_non_opt[0].executor_name, "executor4")
        self.assertEqual(executors_per_contexts[4].data_non_opt[0].context_target_name, "context5Output")

        self.assertEqual(executors_per_contexts[5].context_instance_name, "contextFInst1")
        self.assertEqual(executors_per_contexts[5].is_need_to_reset, False)
        self.assertEqual(executors_per_contexts[5].context_el_id, 5)
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

        self.assertEqual(executors_per_contexts[6].context_instance_name, "contextGInst1")
        self.assertEqual(executors_per_contexts[6].is_need_to_reset, False)
        self.assertEqual(executors_per_contexts[6].context_el_id, 6)
        self.assertEqual(len(executors_per_contexts[6].data), 1)
        self.assertEqual(len(executors_per_contexts[6].data_non_opt), 1)
        self.assertEqual(len(executors_per_contexts[6].data_opt), 0)
        self.assertEqual(executors_per_contexts[6].data[0].executor_name, "executor6")
        self.assertEqual(executors_per_contexts[6].data[0].context_target_name, "context7Output")
        self.assertEqual(executors_per_contexts[6].data_non_opt[0].executor_name, "executor6")
        self.assertEqual(executors_per_contexts[6].data_non_opt[0].context_target_name, "context7Output")

        self.assertEqual(executors_per_contexts[7].context_instance_name, "contextHInst1")
        self.assertEqual(executors_per_contexts[7].is_need_to_reset, True)
        self.assertEqual(executors_per_contexts[7].context_el_id, 7)
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

        self.assertEqual(executors_per_contexts[8].context_instance_name, "contextIInst1")
        self.assertEqual(executors_per_contexts[8].is_need_to_reset, False)
        self.assertEqual(executors_per_contexts[8].context_el_id, 8)
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

        self.assertEqual(executors_per_contexts[9].context_instance_name, "contextKInst1")
        self.assertEqual(executors_per_contexts[9].is_need_to_reset, False)
        self.assertEqual(executors_per_contexts[9].context_el_id, 10)
        self.assertEqual(len(executors_per_contexts[9].data), 1)
        self.assertEqual(len(executors_per_contexts[9].data_non_opt), 1)
        self.assertEqual(len(executors_per_contexts[9].data_opt), 0)
        self.assertEqual(executors_per_contexts[9].data[0].executor_name, "executor2")
        self.assertEqual(executors_per_contexts[9].data[0].context_target_name, "context11Output")
        self.assertEqual(executors_per_contexts[9].data_non_opt[0].executor_name, "executor2")
        self.assertEqual(executors_per_contexts[9].data_non_opt[0].context_target_name, "context11Output")

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

                    


