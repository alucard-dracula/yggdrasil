-- class_env.lua

require("include")
require("ex_string")

local metatable_head = "metatab_"
local member_var_head = "member_"
local class_t_name = "class_info"
local namespace_t_name = "namespace_info"
local class_type_name = "__typename"

local members_list_name = "__members_list"
local supper_name = "supper"
local parent_name = "__parent"

local class_init_foo_name = "__init"

local class_name_fomat = "[_*%a*]+[_*%a*%w*]*"


local operator_tab =
{
	["+"] = "__add",
	["-"] = "__sub",
	["*"] = "__mul",
	["/"] = "__div",
	["^"] = "__pow",
	[".."] = "__concat",
	["_"] = "__unm",
	["()"] = "__call"
}

local base_type_name =
{
	["number"] = "number",
	["string"] = "string",
	["function"] = "function",
	["nil"] = "nil"
}

local function prv_is_std_name(name)
	return name == name:gmatch("(" .. class_name_fomat .. ")")
end

local function prv_ctype(obj)
	return base_type_name[type(obj)] or rawget(obj, class_type_name)
end

_G.ctype = prv_ctype


local function prv__index(tab, key)
	local members = rawget(tab, members_list_name)
	local parent = rawget(tab, parent_name)
	local var = rawget(members, key)

	if var == nil then
		if parent == nil then
			print("prv__index error:", "type", ctype(tab), tab, "member", key, "not exists")
			return nil
		end
	end

	return var or parent[key]
end

local function prv__newindex(tab, key, val)
	local members = rawget(tab, members_list_name)
	local parent = rawget(tab, parent_name)
	local var = rawget(members, key)

	if ctype(var) == "function" then
		if ctype(val) == "function" then
			print("prv__newindex error:", "cannot set", "type", ctype(tab), "member function " .. key)
		end
		return
	end

	if var ~= nil then
		if type(var) == type(val) then
			rawset(members, key, val)
		else
			print("prv__newindex error:", "type", ctype(tab), "set member " .. key .. " cannot convert type " .. type(val) .. " to " .. type(var))
		end
		return
	end

	if parent == nil then
		print("prv__newindex error:", "type", ctype(tab), "member", key, "not exists")
		return
	end

	parent[key] = val
end

local function prv_def_end(self)
	return self.upper
end

local function prv_new(self, params)
	local obj = {}

	local parent = self.parent

	obj[parent_name] = parent and parent:new(params.parent)


	obj.__members_list = {supper = obj.__parent}

	obj.__typename = self.full_space_name


	for i, v in pairs(self.members_list) do
		obj.__members_list[v[1]] = v[2]
	end


	obj.__this_type = self

	obj.static = self.static_members_list


	setmetatable(obj, self.access_rule)

	if params == nil then
		return obj
	end


	for i, v in pairs(params) do
		if i ~= "parent" and ctype(obj[i]) == ctype(v) then
			obj[i] = v
		end
	end

	return obj
end

local function prv_get_member(self, name)
	local member_var_name = member_var_head .. name

	return self.members_list[member_var_name] or rawget(self.static_members_list.__members_list, name)
end


local function prv_static_member(self, name, def_val)
	if name == "self" or name == supper_name then
		print("error:", "type", self.space_name, "static_member", name, "cannot use" .. name)
	end

	local tvar = self.static_members_list.__members_list

	if self:get_member(name) ~= nil then
		print("error:", "type", self.space_name, "static member", name, "is exists")
		return nil
	end

	rawset(tvar, name, def_val)
	return self
end


local function prv_member(self, name, def_val)
	if name == "self" or name == supper_name then
		print("error:", "type", self.space_name, name, "cannot use" .. name)
	end

	local member_var_name = member_var_head .. name

	if self:get_member(name) ~= nil then
		print("error:", "type", self.space_name, "member", name, "is exists")
		return nil
	end

	self.members_list[member_var_name] = {name, def_val}
	return self
end

local function prv_operator(self, op, def_val)
	local opname = operator_tab[op]
	if opname == nil then
		print("error:", "type", self.space_name, op, "is not in operator table")
		return nil
	end

	if self.access_rule[opname] then
		print("error:", "type", self.space_name, "operator", op, "is exists")
		return nil
	end

	self.access_rule[opname] = def_val
	return self
end


local function prv_externs(self, name)
	if self.parent ~= nil then
		print("error:", self.space_name, "has parent", self.__parent)
		return nil
	end

	if self.space_name == name then
		print("error:", self.space_name, "extern self")
		return nil
	end

	if self.parent ~= nil then
		print("error:", self.space_name, "class", name, "is exists")
		return nil
	end

	local space_name_list = name:splice(class_name_fomat)
	local space = 1
	local now_space = self.upper

	while now_space do
		if now_space[space_name_list[space]] ~= nil then
			break
		end
		now_space = now_space.upper
	end

	if now_space == nil then
		print("error:", self.space_name, "class", name, "is not exists")
		return nil
	end

	for i = 1, #space_name_list, 1 do
		now_space = now_space[space_name_list[i]]
		if now_space == nil then
			print("error:", self.space_name, "class", name, "is not exists")
			return nil
		end
	end

	for i, v in pairs(now_space.access_rule) do
		if self.access_rule[i] == nil then
			self.access_rule[i] = v
		end
	end

	self.parent = now_space
	rawset(self.static_members_list, parent_name, now_space.static_members_list)
	return self
end

local function prv_class(self, name)
	if prv_is_std_name(name) then
		print("error", self.space_name, "invalid namespace name", name)
	end

	if self.name ~= nil then
		print("error:", self.space_name, "class", name, "exists")
		return nil;
	end

	self[name] = {}

	local obj = self[name]

	obj.upper = self

	obj.space_name = name
	obj.full_space_name = self.full_space_name .. "." .. name
	obj.__typename = class_t_name

	obj.class = prv_class
	obj.new = prv_new

	obj.member = prv_member
	obj.static_member = prv_static_member
	obj.operator = prv_operator

	obj.externs = prv_externs

	obj.def_end = prv_def_end

	obj.get_member = prv_get_member
	obj.get_obj_member = prv_get_obj_member

	--obj.parent = nil

	obj.members_list = {}
	obj.static_members_list = { __members_list = {} }

	-- metatab config
	obj.access_rule = {__index = prv__index, __newindex = prv__newindex}

	setmetatable(obj.static_members_list, obj.access_rule)

	return obj
end

local function prv_namespace(self, name)
	if prv_is_std_name(name) then
		print("error", "invalid namespace name", name)
	end

	if self.name ~= nil then
		print("error:", "namespace", name, "exists")
		return nil
	end

	self[name] = {}
	local obj = self[name]
	obj.__typename = namespace_t_name

	obj.upper = self
	obj.space_name = name
	obj.full_space_name = (self.full_space_name and self.full_space_name .. "." or "") .. name
	obj.namespace = prv_namespace
	obj.class = prv_class
	obj.def_end = prv_def_end

	return obj
end

local function prv_g_namespace(name)
	local space = _G[name] or nil
	if space ~= nil then
		print("error:", "namespace", name, "is exists")
		return nil
	end
	return prv_namespace(_G, name)

end

class_env = nil
_G.namespace = prv_g_namespace

