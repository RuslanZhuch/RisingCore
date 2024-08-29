import generator

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

        masks.append(mask)

    return masks

def get_executors_per_contexts(structure, contexts_list : list[str]):
    executors_per_contexts = []

    executors_names = list(structure.keys())
    for context in contexts_list:
        executors = []
        for executor_name in executors_names:
            executor_io = structure[executor_name]
            if "outputs" in executor_io:
                outputs = executor_io["outputs"]
                if context in outputs:
                    executors.append(executor_name)

        executors_per_contexts.append(executors)

    return executors_per_contexts

def generate_deps_structure(handler, num_of_channels : int):
    def struct_body_const(struct_handler):
        num_of_parts = int((num_of_channels - 1) / CHANNELS_PER_PART) + 1
        for partId in range(0, num_of_parts):
            generator.generate_struct_variable(struct_handler, "uint64_t", "part{}".format(partId + 1), 0)
            
    generator.generate_struct(handler, "DependenciesMask", struct_body_const)

def generate_deps_for_executor(handler, executor_name : str, deps_mask : list[int]):
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

    generator.generate_line(handler, "DependenciesMask {}DepsMask{{ {} }};".format(executor_name, ",".join(initials)))
