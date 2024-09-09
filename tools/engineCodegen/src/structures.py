CHANNELS_PER_PART = 64

def get_contexts_list(structure):
    contexts_list = []
    contexts_set = set()

    for executor_name in structure:
        executor_io = structure[executor_name]
        if "inputs" not in executor_io:
            continue
        inputs = executor_io["inputs"]
        for input in inputs:
            if input in contexts_set:
                continue
            contexts_set.add(input)
            contexts_list.append(input)

    return contexts_list

def get_executors_list(structure):
    return list(structure.keys())

def get_optional_executors(structure):
    optional_executors = []

    for executor_name in structure:
        executor_data = structure[executor_name]
        if "isOptional" not in executor_data:
            continue
        if executor_data["isOptional"] is True:
            optional_executors.append(executor_name)

    return optional_executors

def get_anchors_list(structure):
    anchors_list = []
    is_anchor_key = "isAnchor"

    for executor_name in structure:
        executor_io = structure[executor_name]

        if is_anchor_key in executor_io and executor_io[is_anchor_key] is True:
            anchors_list.append(executor_name)

    return anchors_list

def get_initial_deps_mask(structure, anchors_list : list[str], contexts_list : list[str]):
    mask = [0] * len(contexts_list)

    for executor_name in structure:
        if executor_name not in anchors_list:
            continue
        executor_io = structure[executor_name]
        if "inputs" not in executor_io:
            continue
        inputs = executor_io["inputs"]
        for input in inputs:
            el_id = contexts_list.index(input)
            mask[el_id] = 1

    return mask

def convert_to_mask(deps_mask : list[int]):
    num_of_flags = len(deps_mask)
    num_of_parts = int((num_of_flags - 1) / CHANNELS_PER_PART) + 1
    num_of_virtual_flags_for_parts = num_of_parts * CHANNELS_PER_PART
    num_of_flags_to_append = num_of_virtual_flags_for_parts - num_of_flags
    deps_mask.extend([0]*num_of_flags_to_append)
    deps_mask.reverse()

    initials = []

    for partId in range(0, num_of_parts):
        mask = deps_mask[partId * CHANNELS_PER_PART : (partId + 1) * CHANNELS_PER_PART]
        binary_str = ''.join(map(str, mask))
        bitmask_int = int(binary_str, 2)
        hex_str = hex(bitmask_int)
        initials.append(hex_str)

    return ",".join(initials)

class ExecutorDepsMask:
    def __init__(self, executor_name: str, mask: list[int]):
        self.executor_name = executor_name
        self.mask = mask

def get_executors_deps_mask(structure, contexts_list : list[str]):
    masks = []

    executors_names = list(structure.keys())
    for executor_name in executors_names:
        executor_io = structure[executor_name]
        mask = [0] * len(contexts_list)
        if "inputs" in executor_io:
            inputs = executor_io["inputs"]
            for input in inputs:
                el_id = contexts_list.index(input)
                mask[el_id] = 1

        masks.append(ExecutorDepsMask(
            executor_name,
            mask
        ))

    return masks

class ExecutorsPerContextDesc:
    class ExecutorData:
        def __init__(self, executor_name: str, context_target_name: str):
            self.executor_name = executor_name
            self.context_target_name = context_target_name
    class ExecutorOptData:
        def __init__(self, executor_name: str, context_target_name: str, optional_executor_id: int):
            self.executor_name = executor_name
            self.context_target_name = context_target_name
            self.optional_executor_id = optional_executor_id

    def __init__(self, context_instance_name: str, data_list: list[ExecutorData], data_non_opt: list[ExecutorData], data_opt_list: list[ExecutorData]):
        self.context_instance_name = context_instance_name
        self.data = data_list
        self.data_non_opt = data_non_opt
        self.data_opt = data_opt_list

def get_executors_per_contexts(structure, contexts_list : list[str], optional_executors_list : list[str] = []):
    executors_per_contexts = []

    executors_names = list(structure.keys())
    for context_instance in contexts_list:
        data = []
        data_non_opt = []
        data_opt = []
        for executor_name in executors_names:
            executor_io = structure[executor_name]
            if "outputs" in executor_io:
                outputs = executor_io["outputs"]
                if context_instance in outputs:
                    data.append(ExecutorsPerContextDesc.ExecutorData(executor_name, outputs[context_instance]))
                    if executor_name in optional_executors_list:
                        id = optional_executors_list.index(executor_name)
                        data_opt.append(ExecutorsPerContextDesc.ExecutorOptData(executor_name, outputs[context_instance], id))
                    else:
                        data_non_opt.append(ExecutorsPerContextDesc.ExecutorData(executor_name, outputs[context_instance]))

        if len(data) > 0 or len(data_opt) > 0:
            executors_per_contexts.append(ExecutorsPerContextDesc(context_instance, data, data_non_opt, data_opt))

    return executors_per_contexts

def compute_number_of_parts(num_of_channels : int):
    return int((num_of_channels - 1) / CHANNELS_PER_PART) + 1

class ExecutorDepsDesc:
    def __init__(self, executor_name: str, initials: str):
        self.executor_name = executor_name
        self.initials = initials

def get_deps_descs_for_executor(executor_name : str, deps_mask : list[int]):
    num_of_flags = len(deps_mask)
    num_of_parts = int((num_of_flags - 1) / CHANNELS_PER_PART) + 1
    num_of_virtual_flags_for_parts = num_of_parts * CHANNELS_PER_PART
    num_of_flags_to_append = num_of_virtual_flags_for_parts - num_of_flags
    deps_mask.extend([0]*num_of_flags_to_append)
    deps_mask.reverse()

    initials = []

    for partId in range(0, num_of_parts):
        mask = deps_mask[partId * CHANNELS_PER_PART : (partId + 1) * CHANNELS_PER_PART]
        binary_str = ''.join(map(str, mask))
        bitmask_int = int(binary_str, 2)
        hex_str = hex(bitmask_int)
        initials.append(hex_str)

    return ExecutorDepsDesc(
        executor_name,
        ",".join(initials)
    )
