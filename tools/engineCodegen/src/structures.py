import loader
import generator
import injectors

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
