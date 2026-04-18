
import os
import sys

current_path = os.getcwd()
current_dir_name = os.path.basename(current_path)

var_prefix = sys.prefix

var_parent_path = os.path.dirname(current_path)
var_parent_dir_name = os.path.basename(var_parent_path)

var_exe_mark = "Debug"
var_cl_mark = "clang"

print(os.path.basename(os.path.dirname(current_path)))

pyd_path = current_path
dll_path = current_path

print("current_path: " + current_path)
print("pyd_search_path: " + pyd_path)
print("dll_search_path: " + dll_path)

sys.path.append(pyd_path)

from pyst import *

def enter(input):
	output = s_t(1+1)
	return output
