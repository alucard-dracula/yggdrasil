
import os
import sys
import platform

current_path = os.getcwd()
current_dir_name = os.path.basename(current_path)

var_arch = platform.architecture()[0]
var_prefix = sys.prefix

var_parent_path = os.path.dirname(current_path)
var_parent_dir_name = os.path.basename(var_parent_path)

var_exe_mark = "Release"
var_cl_mark = "vc143"

if var_arch == "64bit":
	var_arch_mark = "-x64"
	var_arch_num = "64"
else:
	var_arch_mark = "-x32"
	var_arch_num = ""

print(os.path.basename(os.path.dirname(current_path)))

if current_dir_name == 'python' and var_parent_dir_name == 'test':
	var_pyd_prefix = "/../../" + var_exe_mark + var_arch_mark + "/" + var_cl_mark
elif current_dir_name == 'test' and var_parent_dir_name == 'yggdrasil' :
	var_pyd_prefix = "/../" + var_exe_mark + var_arch_mark + "/" + var_cl_mark
else:
	var_pyd_prefix = ""

pyd_path = current_path + var_pyd_prefix

print("system: " + platform.system())

if platform.system() == "Windows":
	dll_path = "D:/third_part/boost/bin" + var_arch_num
elif platform.system() == "Linux" : 
	dll_path = os.environ['HOME'] + "/third_part/boost/lib" + var_arch_num
elif platform.system() == "Android" : 
	dll_path = current_path
elif platform.system() == "Darwin" : 
	dll_path = os.environ['HOME'] +  "/third_part/boost/lib"
else :
	dll_path = current_path

print("current_path: " + current_path)
print("pyd_search_path: " + pyd_path)
print("dll_search_path: " + dll_path)

if platform.system() == "Windows":
	os.add_dll_directory(dll_path)
	
sys.path.append(pyd_path)

from pyst import *

def enter(input):
	output = s_t(1+1)
	return output
