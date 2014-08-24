--include.lua

--_G.package.path = _G.package.path .. ";./?.lua"
require("env_helper")

local g_index_tab_name = "__ex_g_index_tab"
local include_foo_name = "include"
local include_path_foo_name = "include_path"
local ext_name = ".lua"

-- 改成直接解析文件
local function prv_format_path(fpath)
	local str_len = fpath:len()
	local last_char = fpath:sub(str_len, str_len)
	local last_char = ((last_char ~= '/') and (last_char ~= '\\'))
						and "/?" .. ext_name or "?" .. ext_name

	return ";" .. fpath .. last_char
end

local function prv_include_path(fpath)
	local added_path = prv_format_path(fpath)
	_G.package.path = _G.package.path .. added_path
end

local function prv_include(fname)
	local info = pcall(require, fname) or (debug.traceback())
	if(info ~= true) then
		print(info)
		return false
	end
	return info
end

include = nil
append_g(include_path_foo_name, prv_include_path)(include_foo_name, prv_include)

