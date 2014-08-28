
require "yggr_lua_base_type"

local function test_vector_u32(param)
--[[
	print(10, yggr.lua.u64(100))

	print(10 + yggr.lua.u64(100))
	print(10 - yggr.lua.u64(100))
	print(10 * yggr.lua.u64(100))
	print(10 / yggr.lua.u64(100))

--]]

	print("lua", param)
	local ret = yggr.lua.u32_vector(param)
	ret:resize(ret:size() * 2)

	ret:push_back(100)
	ret:push_back(200)
	ret:pop_back()

	local i = yggr.lua.u64(0)
	local isize = ret:size()


	while i < isize do
		ret:set_val_of_index(i, 100 + i)
		i = i + 1
	end

	local iter_s = yggr.lua.u32_vector_iterator(ret:begin_())
	local iter_e = yggr.lua.u32_vector_iterator(ret:end_())

	print(iter_s, iter_e)

	--ret:reverse()
	--ret:reverse(yggr.lua.u64(1), yggr.lua.u64(ret:size() - 1))
	ret:random_shuffle(yggr.lua.u64(1), yggr.lua.u64(ret:size() - 1))
	while iter_s ~= iter_e do
		print("while", iter_s:get_val())
		iter_s:next()
	end

	ret:erase(yggr.lua.u64(0), yggr.lua.u64(1))

	local tret = yggr.lua.u32_vector(param)
	tret:swap(ret)
	ret:swap(tret)

	print("value = ", ret:get_val_of_index(yggr.lua.u64(10)))
	print("idx = ", ret:get_index_of_val(110))
	ret:sort(2)

	return ret
end

function calculate(param)
	return test_vector_u32(param)
end

