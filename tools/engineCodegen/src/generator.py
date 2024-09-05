import sys
import os
sys.path.append("../../thirdparty/code_generator/src")

from code_generation.core import code_generator

def generate_file(folder, file_name):
    return code_generator.CppFile(folder + "/" + file_name)

def get_file_generated(folder, file_name):
    path = folder + "/" + file_name
    return os.path.isfile(path)

def write_content(folder : str, file_name : str, content : str):
    file = open(folder + "/" + file_name, "w")
    file.write(content)
