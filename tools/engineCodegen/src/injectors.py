import json

from os import path
from os import walk
import ntpath

class Desc:
    def __init__(self):
        self.numOfInjections = 0
        self.segment_names : list[str] = []
        self.suspended_segment_names : list[str] = []

BEGIN_KEY = "// BEGIN "
END_KEY = "// END "

BEGIN_KEY_SUSPENDED = "/*" + BEGIN_KEY
END_KEY_SUSPENDED = "*/" + END_KEY

def __gather_segments(content : str, injection_list : list[int], segment_name_list : list[str], key_str : str):
    last_index = 0
    while True:
        begin_index = content.find(key_str, last_index)
        if begin_index == -1:
            return

        segment_name_end_index = content.find("\n", begin_index)
        if segment_name_end_index == -1:
            return

        last_index = segment_name_end_index
        injection_list.append(begin_index)
        segment_name = content[begin_index + len(key_str) : segment_name_end_index]
        segment_name_list.append(segment_name)

def load_content(file_path : str) -> str:
    file_data = open(file_path)
    content = file_data.read()
    file_data.close()

    return content

def gather_desc(content : str):

    injection_begin_list : list[int] = []
    injection_begin_segment_name_list : list[str] = []

    injection_end_list : list[int] = []
    injection_end_segment_name_list : list[str] = []

    __gather_segments(content, injection_begin_list, injection_begin_segment_name_list, BEGIN_KEY)
    __gather_segments(content, injection_end_list, injection_end_segment_name_list, BEGIN_KEY)

    desc = Desc()
    if injection_begin_segment_name_list == injection_end_segment_name_list:
        desc.numOfInjections = len(injection_end_segment_name_list)
        desc.segment_names = injection_begin_segment_name_list
        
    injection_begin_list : list[int] = []
    injection_begin_segment_name_list : list[str] = []

    injection_end_list : list[int] = []
    injection_end_segment_name_list : list[str] = []

    __gather_segments(content, injection_begin_list, injection_begin_segment_name_list, BEGIN_KEY_SUSPENDED)
    __gather_segments(content, injection_end_list, injection_end_segment_name_list, END_KEY_SUSPENDED)
    
    if injection_begin_segment_name_list == injection_end_segment_name_list:
        desc.suspended_segment_names = injection_begin_segment_name_list

    return desc

def suspend(content : str, segment_name : str):
    segment_begin = "{}{}".format(BEGIN_KEY, segment_name)
    begin_index = content.find(segment_begin)
    segment_end = "{}{}".format(END_KEY, segment_name)
    end_index = content.find(segment_end)

    new_content = content[:begin_index] + "/*" + content[begin_index:end_index] + "*/" + content[end_index:]

    return new_content

def restore(content : str, segment_name : str):
    segment_begin = "/*{}{}".format(BEGIN_KEY, segment_name)
    begin_index = content.find(segment_begin)
    segment_end = "*/{}{}".format(END_KEY, segment_name)
    end_index = content.find(segment_end)

    new_content = content[:begin_index] + content[begin_index + 2:end_index] + content[end_index + 2:]

    return new_content

def inject(content : str, segment_name : str, data_to_inject : str):
    segment_begin = "{}{}".format(BEGIN_KEY, segment_name)
    segment_end = "{}{}".format(END_KEY, segment_name)

    new_content = content + "\n{}\n{}\n{}\n".format(segment_begin, data_to_inject, segment_end)

    return new_content

def remove(content : str, segment_name : str):
    segment_begin = "\n{}{}".format(BEGIN_KEY, segment_name)
    begin_index = content.find(segment_begin)
    segment_end = "{}{}\n".format(END_KEY, segment_name)
    end_index = content.find(segment_end) + len(segment_end)

    new_content = content[:begin_index] + content[end_index:]

    return new_content
