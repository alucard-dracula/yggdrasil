-- ex_string.lua

require("include")

local function prv_splice(self, pattern, ...)
	local i, len, mode = ...
	i = i or 1
	len = len or self:len()

	rst_list = {}
	i = 1
	for w in self:sub(i, i+len):gmatch(pattern) do
		rst_list[i] = w
		i = i + 1
	end
	return rst_list
end

ex_string = nil

if string["splice"] == nil then
	string["splice"] = prv_splice
else
	print("error:", "string\'s member", "splice", "is exists")
end
