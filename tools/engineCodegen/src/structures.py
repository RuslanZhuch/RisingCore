CHANNELS_PER_PART = 64

def _get_executors_struct(structure):
    return structure["executors"]

def _get_contexts_to_preserve(structure):
    return structure["contextsToPreserve"]

def _get_contexts_mapping(structure):
    data = structure.get("contextsMapping")
    if data is None:
        return {}
    return data

def _get_ext_contexts_mapping(structure):
    data = structure.get("externalContextsMapping")
    if data is None:
        return {}
    return data

def _get_ext_contexts_sources(structure):
    data = structure.get("externalContextsSources")
    if data is None:
        return {}
    return data

def get_contexts_list(structure):
    contexts_list = []
    contexts_set = set()

    def iterate_over_io(io_name: str):
        if io_name not in executor_io:
            return
        outputs = executor_io[io_name]
        for context in outputs:
            if context in contexts_set:
                continue
            contexts_set.add(context)
            contexts_list.append(context)

    for executor_name in _get_executors_struct(structure):
        executor_io = _get_executors_struct(structure)[executor_name]
        iterate_over_io("outputs")
        iterate_over_io("inputs")

    contexts_list.sort()
    return contexts_list

def get_contexts_types_list(structure):
    types = set()
    mapping = _get_contexts_mapping(structure)
    for instance_name in mapping:
        types.add(mapping[instance_name])
    
    mapping = _get_ext_contexts_mapping(structure)
    for instance_name in mapping:
        types.add(mapping[instance_name])

    data = list(types)
    data.sort()
    return data

class ContextDesc:
    def __init__(self, context_name: str, instance_name: str):
        self.context_name = context_name
        self.instance_name = instance_name

def get_contexts_descs_list(structure):
    mapping = _get_contexts_mapping(structure)
    contexts = [
        ContextDesc(mapping[instance_name], instance_name)
        for instance_name in sorted(mapping)
    ]

    return contexts

def get_ext_contexts_descs_list(structure):
    mapping = _get_ext_contexts_mapping(structure)
    contexts = [
        ContextDesc(mapping[instance_name], instance_name)
        for instance_name in sorted(mapping)
    ]

    return contexts

def get_executors_list(structure):
    return list(_get_executors_struct(structure).keys())

def get_anchors_list(structure):
    anchors_list = []
    is_anchor_key = "isAnchor"

    for executor_name in _get_executors_struct(structure):
        executor_io = _get_executors_struct(structure)[executor_name]

        if is_anchor_key in executor_io and executor_io[is_anchor_key] is True:
            anchors_list.append(executor_name)

    return anchors_list

class ExecutorInputsDesc:
    def __init__(self, input_name: str, instance_name: str):
        self.input_name = input_name
        self.instance_name = instance_name

def get_executors_inputs_contexts(structure):
    data = dict()
    for executor_name in _get_executors_struct(structure):
        executor_io = _get_executors_struct(structure)[executor_name]
        if "inputs" not in executor_io:
            continue
        descs_list = []
        inputs = executor_io["inputs"]
        for input in inputs:
            descs_list.append(ExecutorInputsDesc(
                input_name= inputs[input],
                instance_name= input
            ))
        data[executor_name] = descs_list

    return data

def get_executors_ext_inputs_contexts(structure):
    data = dict()
    for executor_name in _get_executors_struct(structure):
        executor_io = _get_executors_struct(structure)[executor_name]
        if "externalInputs" not in executor_io:
            continue
        descs_list = []
        inputs = executor_io["externalInputs"]
        for input in inputs:
            descs_list.append(ExecutorInputsDesc(
                input_name= inputs[input],
                instance_name= input
            ))
        data[executor_name] = descs_list

    return data

class ExecutorOutputDesc:
    def __init__(self, output_name: str, instance_name: str):
        self.output_name = output_name
        self.instance_name = instance_name

def get_executors_ext_outputs(structure):
    data = dict()
    outputs = _get_ext_contexts_sources(structure)
    for context_name in outputs:
        context_data = outputs[context_name]

        executor_name = context_data["executor"]
        if executor_name not in data:
            data[executor_name] = list()
        output_name = context_data["instance_name"]
        data[executor_name].append(ExecutorOutputDesc(
            output_name= output_name,
            instance_name= context_name
        ))

    return data

def get_executors_type_list(structure):
    data = set()

    for executor_name in _get_executors_struct(structure):
        executor_io = _get_executors_struct(structure)[executor_name]
        executor_type = executor_io["type"]
        data.add(executor_type)

    type_list = list(data)
    type_list.sort()
    return type_list

class ExecutorDesc:
    def __init__(self, executor_type: str, executor_name: str):
        self.executor_type = executor_type
        self.executor_name = executor_name

def get_executors_descs_list(structure):
    data = list()
    for executor_name in _get_executors_struct(structure):
        executor_io = _get_executors_struct(structure)[executor_name]
        executor_type = executor_io["type"]
        data.append(ExecutorDesc(
            executor_type=executor_type,
            executor_name=executor_name
        ))

    return data

def get_initial_deps_mask(structure, anchors_list : list[str], contexts_list : list[str]):
    mask = [0] * len(contexts_list)

    for executor_name in _get_executors_struct(structure):
        if executor_name not in anchors_list:
            continue
        executor_io = _get_executors_struct(structure)[executor_name]
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

class OptionalExecutorDesc:
    def __init__(self, executor_name: str, toggle_code: int):
        self.executor_name = executor_name
        self.toggle_code = toggle_code

def get_optional_executors_data(structure, optional_executors_list: list[str]):
    data = []
    executors_struct = _get_executors_struct(structure)

    for optional_executor in optional_executors_list:
        if optional_executor not in executors_struct:
            continue
        executor_data = executors_struct[optional_executor]
        if "toggleCode" not in executor_data:
            continue
        toggle_code = executor_data["toggleCode"]
        data.append(OptionalExecutorDesc(
            optional_executor,
            toggle_code
        ))

    return data

def get_executors_segmented(structure):
    mandatory_executors = []
    optional_executors = []

    for executor_name in _get_executors_struct(structure):
        executor_data = _get_executors_struct(structure)[executor_name]
        if "toggleCode" not in executor_data:
            mandatory_executors.append(executor_name)
            continue
        optional_executors.append(executor_name)

    return mandatory_executors, optional_executors

class ExecutorDepsContexts:
    def __init__(self, executor_name: str = "", contexts_list: list[str] = [], depenency_contexts_mask: str = ""):
        self.executor_name = executor_name
        self.contexts_list = contexts_list
        self.depenency_contexts_mask = depenency_contexts_mask

def get_executors_deps_contexts(structure, contexts_list : list[str]):
    data = []

    executors_names = list(_get_executors_struct(structure).keys())
    for executor_name in executors_names:
        executor_io = _get_executors_struct(structure)[executor_name]
        desc = ExecutorDepsContexts()
        desc.executor_name = executor_name
        if "outputs" in executor_io:
            inputs = executor_io["outputs"]
            desc.contexts_list = list(inputs.keys())
        if "inputs" in executor_io:
            inputs = executor_io["inputs"]
            mask = [0] * len(contexts_list)
            for input in inputs:
                el_id = contexts_list.index(input)
                mask[el_id] = 1
            desc.depenency_contexts_mask = convert_to_mask(mask)
        data.append(desc)

    return data

class ExecutorDepsMask:
    def __init__(self, executor_name: str, mask: list[int]):
        self.executor_name = executor_name
        self.mask = mask

def get_executors_deps_mask(structure, contexts_list : list[str]):
    masks = []

    executors_names = list(_get_executors_struct(structure).keys())
    for executor_name in executors_names:
        executor_io = _get_executors_struct(structure)[executor_name]
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

    def __init__(self, context_instance_name: str, context_el_id: int, is_need_to_reset: bool, data_list: list[ExecutorData], data_non_opt: list[ExecutorData], data_opt_list: list[ExecutorData]):
        self.context_instance_name = context_instance_name
        self.context_el_id = context_el_id
        self.is_need_to_reset = is_need_to_reset
        self.data = data_list
        self.data_non_opt = data_non_opt
        self.data_opt = data_opt_list

def get_executors_per_contexts(structure, contexts_list : list[str], optional_executors_list : list[str] = []):
    executors_per_contexts = []

    executors_names = list(_get_executors_struct(structure).keys())

    context_el_id = 0
    for context_instance in contexts_list:
        data = []
        data_non_opt = []
        data_opt = []
        is_need_to_reset = context_instance not in _get_contexts_to_preserve(structure)
        for executor_name in executors_names:
            executor_io = _get_executors_struct(structure)[executor_name]
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
            executors_per_contexts.append(ExecutorsPerContextDesc(context_instance, context_el_id, is_need_to_reset, data, data_non_opt, data_opt))

        context_el_id = context_el_id + 1

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
