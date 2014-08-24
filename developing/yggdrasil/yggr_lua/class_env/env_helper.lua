--basedef.lua

local append_g_foo_name = "append_g"

local function prv_show_members_list(obj)

	print("show", obj)
	print("obj type:", type(obj))
	for i, v in pairs(obj) do
		print("", i, v)
	end
end

local function prv_append_g(name, val)
	--print("call")
	if _G[name] ~= nil then
		print("error:", "_G\'s member", name, "is exists")
	else
		_G[name] = val
	end
	return prv_append_g
end

env_helper = nil
if _G[append_g_foo_name] == nil then
	_G[append_g_foo_name] = prv_append_g
else
	print("error:", "_G\'s member", append_g_foo_name, "is exists", "all system down")
end

append_g("show_members_list", prv_show_members_list)


