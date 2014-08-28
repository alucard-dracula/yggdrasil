
require "yggr_lua_base_type"

local function test_list_u64(param)

--[[
	local pair = yggr.lua.u32_pair(10, 20)
	print(pair)
	print("lua", param)
--]]
	local ret = yggr.lua.u64_list(param)
	ret:resize(ret:size() * 2)

	ret:push_back(yggr.lua.u64(100))
	ret:push_back(yggr.lua.u64(200))
	ret:pop_back()

	local num = yggr.lua.u64(0)
	local i = ret:begin_()
	local isize = ret:end_()


	while i ~= isize do
		i:set_val(num + 100)
		num = num + 1
		i:next()
	end


	local iter_s = ret:begin_()
	local iter_e = ret:end_()

	print(iter_s, iter_e)

	ret:reverse()
	while iter_s ~= iter_e do
		print("while", iter_s:get_val())
		iter_s:next()
	end

	local es = ret:begin_()
	local ee = ret:begin_()
	ee:next()
	ret:erase(es, ee)

	local tret = yggr.lua.u64_list(param)
	tret:swap(ret)
	ret:swap(tret)

	ret:sort(2)
	return ret
end


function calculate(param)
	return test_list_u64(param)
end

