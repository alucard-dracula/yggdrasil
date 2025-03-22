
require "yggr_lua_base_type"

local function test_d64_cref(param)
	print("lua_test_d64 show", type(param), yggr.lua.typeid(param), param)

	print("lua_test_d64 negate", type(-param), yggr.lua.typeid(-param), -param)

	print("\n")
	print("lua_test_d64 +:", type(param + true), yggr.lua.typeid(param + true), param + true)
	print("lua_test_d64 +:", type(param + 10), yggr.lua.typeid(param + 10), param + 10)
	print("lua_test_d64 +:", type(10 + param), yggr.lua.typeid(10 + param), 10 + param)
	print("lua_test_d64 +:", type(param + yggr.lua.u64(10)), yggr.lua.typeid(param + yggr.lua.u64(10)), param + yggr.lua.u64(10))
	print("lua_test_d64 +:", type(param + yggr.lua.s64(10)), yggr.lua.typeid(param + yggr.lua.s64(10)), param + yggr.lua.s64(10))

	print("\n")
	print("lua_test_d64 -:", type(param - true), yggr.lua.typeid(param - true), param - true)
	print("lua_test_d64 -:", type(param - 10), yggr.lua.typeid(param - 10), param - 10)
	print("lua_test_d64 -:", type(10 - param), yggr.lua.typeid(10 - param), 10 - param)
	print("lua_test_d64 -:", type(param - yggr.lua.u64(10)), yggr.lua.typeid(param - yggr.lua.u64(10)), param - yggr.lua.u64(10))
	print("lua_test_d64 -:", type(param - yggr.lua.s64(10)), yggr.lua.typeid(param - yggr.lua.s64(10)), param - yggr.lua.s64(10))

	print("\n")
	print("lua_test_d64 *:", type(param * true), yggr.lua.typeid(param * true), param * true)
	print("lua_test_d64 *:", type(param * 10), yggr.lua.typeid(param * 10), param * 10)
	print("lua_test_d64 *:", type(10 * param), yggr.lua.typeid(10 * param), 10 * param)
	print("lua_test_d64 *:", type(param * yggr.lua.u64(10)), yggr.lua.typeid(param * yggr.lua.u64(10)), param * yggr.lua.u64(10))
	print("lua_test_d64 *:", type(param * yggr.lua.s64(10)), yggr.lua.typeid(param * yggr.lua.s64(10)), param * yggr.lua.s64(10))

	print("\n")
	print("lua_test_d64 /:", type(param / true), yggr.lua.typeid(param / true), param / true)
	print("lua_test_d64 /:", type(param / 10), yggr.lua.typeid(param / 10), param / 10)
	print("lua_test_d64 /:", type(10 / param), yggr.lua.typeid(10 / param), 10 / param)
	print("lua_test_d64 /:", type(param / yggr.lua.u64(10)), yggr.lua.typeid(param / yggr.lua.u64(10)), param / yggr.lua.u64(10))
	print("lua_test_d64 /:", type(param / yggr.lua.s64(10)), yggr.lua.typeid(param / yggr.lua.s64(10)), param / yggr.lua.s64(10))

	print("\n")
	print("lua_test_d64 ==:", type(param == 10), yggr.lua.typeid(param == 10), param == 10) -- false, because type(param) ~= type(10)
	print("lua_test_d64 ==:", type(10 == param), yggr.lua.typeid(10 == param), 10 == param) -- false, because type(param) ~= type(10)
	print("lua_test_d64 ==:", type(param == yggr.lua.u64(10)), yggr.lua.typeid(param == yggr.lua.u64(10)), param == yggr.lua.u64(10))
	print("lua_test_d64 ==:", type(param == yggr.lua.s64(10)), yggr.lua.typeid(param == yggr.lua.s64(10)), param == yggr.lua.s64(10))
	print("lua_test_d64 ==:", type(param == 100), yggr.lua.typeid(param == 100), param == 100) -- false, because type(param) ~= type(100)
	print("lua_test_d64 ==:", type(100 == param), yggr.lua.typeid(100 == param), 100 == param) -- false, because type(param) ~= type(100)
	print("lua_test_d64 ==:", type(param == yggr.lua.u64(100)), yggr.lua.typeid(param == yggr.lua.u64(100)), param == yggr.lua.u64(100))
	print("lua_test_d64 ==:", type(param == yggr.lua.s64(100)), yggr.lua.typeid(param == yggr.lua.s64(100)), param == yggr.lua.s64(100))

	print("\n")
	print("lua_test_d64 ~=:", type(param ~= 10), yggr.lua.typeid(param ~= 10), param ~= 10) -- true, because type(param) ~= type(10)
	print("lua_test_d64 ~=:", type(10 ~= param), yggr.lua.typeid(10 ~= param), 10 ~= param) -- true, because type(param) ~= type(10)
	print("lua_test_d64 ~=:", type(param ~= yggr.lua.u64(10)), yggr.lua.typeid(param ~= yggr.lua.u64(10)), param ~= yggr.lua.u64(10))
	print("lua_test_d64 ~=:", type(param ~= yggr.lua.s64(10)), yggr.lua.typeid(param ~= yggr.lua.s64(10)), param ~= yggr.lua.s64(10))
	print("lua_test_d64 ~=:", type(param ~= 100), yggr.lua.typeid(param ~= 100), param ~= 100) -- true, because type(param) ~= type(10)
	print("lua_test_d64 ~=:", type(100 ~= param), yggr.lua.typeid(100 ~= param), 100 ~= param) -- true, because type(param) ~= type(10)
	print("lua_test_d64 ~=:", type(param ~= yggr.lua.u64(100)), yggr.lua.typeid(param ~= yggr.lua.u64(100)), param ~= yggr.lua.u64(100))
	print("lua_test_d64 ~=:", type(param ~= yggr.lua.s64(100)), yggr.lua.typeid(param ~= yggr.lua.s64(100)), param ~= yggr.lua.s64(100))

	print("\n")
	--print("lua_test_d64 <:", type(param < 10), param < 10) -- can't compare userdata < number
	--print("lua_test_d64 <:", type(10 < param), 10 < param) -- can't compare userdata < number
	print("lua_test_d64 <:", type(param < yggr.lua.u64(10)), yggr.lua.typeid(param < yggr.lua.u64(10)), param < yggr.lua.u64(10))
	print("lua_test_d64 <:", type(param < yggr.lua.s64(10)), yggr.lua.typeid(param < yggr.lua.s64(10)), param < yggr.lua.s64(10))
	print("lua_test_d64 <:", type(param < yggr.lua.u64(100)), yggr.lua.typeid(param < yggr.lua.u64(100)), param < yggr.lua.u64(100))
	print("lua_test_d64 <:", type(param < yggr.lua.s64(100)), yggr.lua.typeid(param < yggr.lua.s64(100)), param < yggr.lua.s64(100))

	print("\n")
	print("lua_test_d64 <=:", type(param <= yggr.lua.u64(10)), yggr.lua.typeid(param <= yggr.lua.u64(10)), param <= yggr.lua.u64(10))
	print("lua_test_d64 <=:", type(param <= yggr.lua.s64(10)), yggr.lua.typeid(param <= yggr.lua.s64(10)), param <= yggr.lua.s64(10))
	print("lua_test_d64 <=:", type(param <= yggr.lua.u64(100)), yggr.lua.typeid(param <= yggr.lua.u64(100)), param <= yggr.lua.u64(100))
	print("lua_test_d64 <=:", type(param <= yggr.lua.s64(100)), yggr.lua.typeid(param <= yggr.lua.s64(100)), param <= yggr.lua.s64(100))
	print("lua_test_d64 <=:", type(param <= yggr.lua.u64(1000)), yggr.lua.typeid(param <= yggr.lua.u64(1000)), param <= yggr.lua.u64(1000))
	print("lua_test_d64 <=:", type(param <= yggr.lua.s64(1000)), yggr.lua.typeid(param <= yggr.lua.s64(1000)), param <= yggr.lua.s64(1000))

	print("\n")
	print("lua_test_d64 >:", type(param > yggr.lua.u64(10)), yggr.lua.typeid(param > yggr.lua.u64(10)), param > yggr.lua.u64(10))
	print("lua_test_d64 >:", type(param > yggr.lua.s64(10)), yggr.lua.typeid(param > yggr.lua.s64(10)), param > yggr.lua.s64(10))
	print("lua_test_d64 >:", type(param > yggr.lua.u64(100)), yggr.lua.typeid(param > yggr.lua.u64(100)), param > yggr.lua.u64(100))
	print("lua_test_d64 >:", type(param > yggr.lua.s64(100)), yggr.lua.typeid(param > yggr.lua.s64(100)), param > yggr.lua.s64(100))

	print("\n")
	print("lua_test_d64 >=:", type(param >= yggr.lua.u64(10)), yggr.lua.typeid(param >= yggr.lua.u64(10)), param >= yggr.lua.u64(10))
	print("lua_test_d64 >=:", type(param >= yggr.lua.s64(10)), yggr.lua.typeid(param >= yggr.lua.s64(10)), param >= yggr.lua.s64(10))
	print("lua_test_d64 >=:", type(param >= yggr.lua.u64(100)), yggr.lua.typeid(param >= yggr.lua.u64(100)), param >= yggr.lua.u64(100))
	print("lua_test_d64 >=:", type(param >= yggr.lua.s64(100)), yggr.lua.typeid(param >= yggr.lua.s64(100)), param >= yggr.lua.s64(100))
	print("lua_test_d64 >=:", type(param >= yggr.lua.u64(1000)), yggr.lua.typeid(param >= yggr.lua.u64(1000)), param >= yggr.lua.u64(1000))
	print("lua_test_d64 >=:", type(param >= yggr.lua.s64(1000)), yggr.lua.typeid(param >= yggr.lua.s64(1000)), param >= yggr.lua.s64(1000))

	print("\n")
	print("lua_test_d64 and:", type(param and true), yggr.lua.typeid(param and true), param and true)
	print("lua_test_d64 and:", type(true and param), yggr.lua.typeid(true and param), true and param)
	print("lua_test_d64 and:", type(param and 10), yggr.lua.typeid(param and 10), param and 10)
	print("lua_test_d64 and:", type(10 and param), yggr.lua.typeid(10 and param), 10 and param)
	print("lua_test_d64 and:", type(param and yggr.lua.u64(10)), yggr.lua.typeid(param and yggr.lua.u64(10)), param and yggr.lua.u64(10))
	print("lua_test_d64 and:", type(param and yggr.lua.s64(10)), yggr.lua.typeid(param and yggr.lua.s64(10)), param and yggr.lua.s64(10))
	print("lua_test_d64 and:", type(param and false), yggr.lua.typeid(param and false), param and false)
	print("lua_test_d64 and:", type(false and param), yggr.lua.typeid(false and param), false and param)
	print("lua_test_d64 and:", type(param and 0), yggr.lua.typeid(param and 0), param and 0)
	print("lua_test_d64 and:", type(0 and param), yggr.lua.typeid(0 and param), 0 and param)
	print("lua_test_d64 and:", type(param and yggr.lua.u64(0)), yggr.lua.typeid(param and yggr.lua.u64(0)), param and yggr.lua.u64(0))
	print("lua_test_d64 and:", type(param and yggr.lua.s64(0)), yggr.lua.typeid(param and yggr.lua.s64(0)), param and yggr.lua.s64(0))

	print("\n")
	print("lua_test_d64 or:", type(param or true), yggr.lua.typeid(param or true), param or true)
	print("lua_test_d64 or:", type(true or param), yggr.lua.typeid(true or param), true or param)
	print("lua_test_d64 or:", type(param or 10), yggr.lua.typeid(param or 10), param or 10)
	print("lua_test_d64 or:", type(10 or param), yggr.lua.typeid(10 or param), 10 or param)
	print("lua_test_d64 or:", type(param or yggr.lua.u64(10)), yggr.lua.typeid(param or yggr.lua.u64(10)), param or yggr.lua.u64(10))
	print("lua_test_d64 or:", type(param or yggr.lua.s64(10)), yggr.lua.typeid(param or yggr.lua.s64(10)), param or yggr.lua.s64(10))
	print("lua_test_d64 or:", type(param or false), yggr.lua.typeid(param or false), param or false)
	print("lua_test_d64 or:", type(false or param), yggr.lua.typeid(false or param), false or param)
	print("lua_test_d64 or:", type(param or 0), yggr.lua.typeid(param or 0), param or 0)
	print("lua_test_d64 or:", type(0 or param), yggr.lua.typeid(0 or param), 0 or param)
	print("lua_test_d64 or:", type(param or yggr.lua.u64(0)), yggr.lua.typeid(param or yggr.lua.u64(0)), param or yggr.lua.u64(0))
	print("lua_test_d64 or:", type(param or yggr.lua.s64(0)), yggr.lua.typeid(param or yggr.lua.s64(0)), param or yggr.lua.s64(0))

	print("\n")
	print("lua_test_d64 not:", type(not param), yggr.lua.typeid(not param), not param) --param is userdata not userdata == false
	print("lua_test_d64 not:", type(not 10), yggr.lua.typeid(not 10), not 10) --param is userdata not userdata == false
	print("lua_test_d64 not:", type(not 0), yggr.lua.typeid(not 0), not 0) --param is userdata not userdata == false
	print("lua_test_d64 not:", type(not true), yggr.lua.typeid(not true), not true) --param is userdata not userdata == false
	print("lua_test_d64 not:", type(not false), yggr.lua.typeid(not false), not false) --param is userdata not userdata == false
	print("lua_test_d64 not:", type(not yggr.lua.u64(0)), yggr.lua.typeid(not yggr.lua.u64(0)), not yggr.lua.u64(0)) --param is userdata not userdata == false
	print("lua_test_d64 not:", type(not yggr.lua.u64(1)), yggr.lua.typeid(not yggr.lua.u64(1)), not yggr.lua.u64(1)) --param is userdata not userdata == false

	print("\n")
	print("lua_test_d64 to_boolean:", type(yggr.lua.u64(0):to_boolean()), yggr.lua.typeid(yggr.lua.u64(0):to_boolean()), yggr.lua.u64(0):to_boolean())
	print("lua_test_d64 to_boolean:", type(yggr.lua.u64(1):to_boolean()), yggr.lua.typeid(yggr.lua.u64(1):to_boolean()), yggr.lua.u64(1):to_boolean())
	print("lua_test_d64 to_boolean:", type(yggr.lua.s64(0):to_boolean()), yggr.lua.typeid(yggr.lua.s64(0):to_boolean()), yggr.lua.s64(0):to_boolean())
	print("lua_test_d64 to_boolean:", type(yggr.lua.s64(1):to_boolean()), yggr.lua.typeid(yggr.lua.s64(1):to_boolean()), yggr.lua.s64(1):to_boolean())
	print("lua_test_d64 to_boolean:", type(param:to_boolean()), yggr.lua.typeid(param:to_boolean()), param:to_boolean())
	print("lua_test_d64 to_boolean:", type(not param:to_boolean()), yggr.lua.typeid(not param:to_boolean()), not param:to_boolean())

	-- cref not support swap
	--[[
	print("\n")
	local swap_val = yggr.lua.u64(2)
	param:swap(swap_val)
	print("lua_test_d64 swap:", type(param), yggr.lua.typeid(param), param)
	swap_val:swap(param)
	print("lua_test_d64 swap:", type(param), yggr.lua.typeid(param), param)
	--]]

	print("\n")
	print("lua_test_d64 get_value:", type(param:get_value()), yggr.lua.typeid(param:get_value()), param:get_value())
	local org_val = param:copy()
	print("lua_test_d64 org_val:", type(org_val), yggr.lua.typeid(org_val), org_val)

	-- cref not support set_value
	--[[
	print("lua_test_d64 set_value:", type(param:set_value(true)), yggr.lua.typeid(param:set_value(true)), param:set_value(true))
	print("lua_test_d64 set_value:", type(param:set_value(false)), yggr.lua.typeid(param:set_value(false)), param:set_value(false))
	print("lua_test_d64 set_value:", type(param:set_value(1)), yggr.lua.typeid(param:set_value(1)), param:set_value(1))
	print("lua_test_d64 set_value:",
			type(param:set_value(yggr.lua.s64(2))), yggr.lua.typeid(param:set_value(yggr.lua.s64(2))), param:set_value(yggr.lua.s64(2)))
	print("lua_test_d64 set_value:",
			type(param:set_value(org_val)), yggr.lua.typeid(param:set_value(org_val)), param:set_value(org_val))
	--]]

	print("\n")
	print("lua_test_d64 positive:", type(param:positive()), yggr.lua.typeid(param:positive()), param:positive())
	print("lua_test_d64 negate:", type(param:negate()), yggr.lua.typeid(param:negate()), param:negate())

	-- cref not support ++ --
	--[[
	print("\n")
	print("lua_test_d64 head_increment:",
			type(param:head_increment()), yggr.lua.typeid(param:head_increment()), param:head_increment())
	print("lua_test_d64 tail_increment:",
			type(param:tail_increment()), yggr.lua.typeid(param:tail_increment()), param:tail_increment())

	print("\n")
	print("lua_test_d64 head_decrement:",
			type(param:head_decrement()), yggr.lua.typeid(param:head_decrement()), param:head_decrement())
	print("lua_test_d64 tail_decrement:",
			type(param:tail_decrement()), yggr.lua.typeid(param:tail_decrement()), param:tail_decrement())
	--]]

	print("\n")
	print("lua_test_d64 plus:", type(param:plus(false)), yggr.lua.typeid(param:plus(false)), param:plus(false))
	print("lua_test_d64 plus:", type(param:plus(true)), yggr.lua.typeid(param:plus(true)), param:plus(true))
	print("lua_test_d64 plus:", type(param:plus(1)), yggr.lua.typeid(param:plus(1)), param:plus(1))
	print("lua_test_d64 plus:",
			type(param:plus(yggr.lua.u64(1))), yggr.lua.typeid(param:plus(yggr.lua.u64(1))), param:plus(yggr.lua.u64(1)) )
	print("lua_test_d64 plus:",
			type(param:plus(yggr.lua.s64(1))), yggr.lua.typeid(param:plus(yggr.lua.s64(1))), param:plus(yggr.lua.s64(1)) )

	-- cref not support +=
	--[[
	print("\n")
	print("lua_test_d64 plus_set:", type(param:plus_set(false)), yggr.lua.typeid(param:plus_set(false)), param:plus_set(false))
	print("lua_test_d64 plus_set:", type(param:plus_set(true)), yggr.lua.typeid(param:plus_set(true)), param:plus_set(true))
	print("lua_test_d64 plus_set:", type(param:plus_set(1)), yggr.lua.typeid(param:plus_set(1)), param:plus_set(1))
	print("lua_test_d64 plus_set:",
			type(param:plus_set(yggr.lua.u64(1))), yggr.lua.typeid(param:plus_set(yggr.lua.u64(1))), param:plus_set(yggr.lua.u64(1)) )
	print("lua_test_d64 plus_set:",
			type(param:plus_set(yggr.lua.s64(1))), yggr.lua.typeid(param:plus_set(yggr.lua.s64(1))), param:plus_set(yggr.lua.s64(1)) )
	--]]

	print("\n")
	print("lua_test_d64 minus:", type(param:minus(false)), yggr.lua.typeid(param:minus(false)), param:minus(false))
	print("lua_test_d64 minus:", type(param:minus(true)), yggr.lua.typeid(param:minus(true)), param:minus(true))
	print("lua_test_d64 minus:", type(param:minus(1)), yggr.lua.typeid(param:minus(1)), param:minus(1))
	print("lua_test_d64 minus:",
			type(param:minus(yggr.lua.u64(1))), yggr.lua.typeid(param:minus(yggr.lua.u64(1))), param:minus(yggr.lua.u64(1)) )
	print("lua_test_d64 minus:",
			type(param:minus(yggr.lua.s64(1))), yggr.lua.typeid(param:minus(yggr.lua.s64(1))), param:minus(yggr.lua.s64(1)) )

	-- cref not support -=
	--[[
	print("\n")
	print("lua_test_d64 minus_set:", type(param:minus_set(false)), yggr.lua.typeid(param:minus_set(false)), param:minus_set(false))
	print("lua_test_d64 minus_set:", type(param:minus_set(true)), yggr.lua.typeid(param:minus_set(true)), param:minus_set(true))
	print("lua_test_d64 minus_set:", type(param:minus_set(1)), yggr.lua.typeid(param:minus_set(1)), param:minus_set(1))
	print("lua_test_d64 minus_set:",
			type(param:minus_set(yggr.lua.u64(1))), yggr.lua.typeid(param:minus_set(yggr.lua.u64(1))), param:minus_set(yggr.lua.u64(1)) )
	print("lua_test_d64 minus_set:",
			type(param:minus_set(yggr.lua.s64(1))), yggr.lua.typeid(param:minus_set(yggr.lua.s64(1))), param:minus_set(yggr.lua.s64(1)) )
	--]]

	print("\n")
	print("lua_test_d64 multiplies:", type(param:multiplies(true)), yggr.lua.typeid(param:multiplies(true)), param:multiplies(true))
	print("lua_test_d64 multiplies:", type(param:multiplies(1)), yggr.lua.typeid(param:multiplies(1)), param:multiplies(1))
	print("lua_test_d64 multiplies:",
			type(param:multiplies(yggr.lua.u64(1))), yggr.lua.typeid(param:multiplies(yggr.lua.u64(1))), param:multiplies(yggr.lua.u64(1)) )
	print("lua_test_d64 multiplies:",
			type(param:multiplies(yggr.lua.s64(1))), yggr.lua.typeid(param:multiplies(yggr.lua.s64(1))), param:multiplies(yggr.lua.s64(1)) )

	-- cref not support *=
	--[[
	print("\n")
	print("lua_test_d64 multiplies_set:", type(param:multiplies_set(true)), yggr.lua.typeid(param:multiplies_set(true)), param:multiplies_set(true))
	print("lua_test_d64 multiplies_set:", type(param:multiplies_set(1)), yggr.lua.typeid(param:multiplies_set(1)), param:multiplies_set(1))
	print("lua_test_d64 multiplies_set:",
			type(param:multiplies_set(yggr.lua.u64(1))), yggr.lua.typeid(param:multiplies_set(yggr.lua.u64(1))), param:multiplies_set(yggr.lua.u64(1)) )
	print("lua_test_d64 multiplies_set:",
			type(param:multiplies_set(yggr.lua.s64(1))), yggr.lua.typeid(param:multiplies_set(yggr.lua.s64(1))), param:multiplies_set(yggr.lua.s64(1)) )
	--]]

	print("\n")
	print("lua_test_d64 divides:", type(param:divides(true)), yggr.lua.typeid(param:divides(true)), param:divides(true))
	print("lua_test_d64 divides:", type(param:divides(1)), yggr.lua.typeid(param:divides(1)), param:divides(1))
	print("lua_test_d64 divides:",
			type(param:divides(yggr.lua.u64(1))), yggr.lua.typeid(param:divides(yggr.lua.u64(1))), param:divides(yggr.lua.u64(1)) )
	print("lua_test_d64 divides:",
			type(param:divides(yggr.lua.s64(1))), yggr.lua.typeid(param:divides(yggr.lua.s64(1))), param:divides(yggr.lua.s64(1)) )

	-- cref not support /=
	--[[
	print("\n")
	print("lua_test_d64 divides_set:", type(param:divides_set(true)), yggr.lua.typeid(param:divides_set(true)), param:divides_set(true))
	print("lua_test_d64 divides_set:", type(param:divides_set(1)), yggr.lua.typeid(param:divides_set(1)), param:divides_set(1))
	print("lua_test_d64 divides_set:",
			type(param:divides_set(yggr.lua.u64(1))), yggr.lua.typeid(param:divides_set(yggr.lua.u64(1))), param:divides_set(yggr.lua.u64(1)) )
	print("lua_test_d64 divides_set:",
			type(param:divides_set(yggr.lua.s64(1))), yggr.lua.typeid(param:divides_set(yggr.lua.s64(1))), param:divides_set(yggr.lua.s64(1)) )
	--]]

	print("\n")
	print("lua_test_d64 modulus:", type(param:modulus(true)), yggr.lua.typeid(param:modulus(true)), param:modulus(true))
	print("lua_test_d64 modulus:", type(param:modulus(1)), yggr.lua.typeid(param:modulus(1)), param:modulus(1))
	print("lua_test_d64 modulus:",
			type(param:modulus(yggr.lua.u64(1))), yggr.lua.typeid(param:modulus(yggr.lua.u64(1))), param:modulus(yggr.lua.u64(1)) )
	print("lua_test_d64 modulus:",
			type(param:modulus(yggr.lua.s64(1))), yggr.lua.typeid(param:modulus(yggr.lua.s64(1))), param:modulus(yggr.lua.s64(1)) )

	-- cref not support %=
	--[[
	print("\n")
	print("lua_test_d64 modulus_set:", type(param:modulus_set(true)), yggr.lua.typeid(param:modulus_set(true)), param:modulus_set(true))
	print("lua_test_d64 modulus_set:", type(param:modulus_set(1)), yggr.lua.typeid(param:modulus_set(1)), param:modulus_set(1))
	print("lua_test_d64 modulus_set:",
			type(param:modulus_set(yggr.lua.u64(1))), yggr.lua.typeid(param:modulus_set(yggr.lua.u64(1))), param:modulus_set(yggr.lua.u64(1)) )
	print("lua_test_d64 modulus_set:",
			type(param:modulus_set(yggr.lua.s64(1))), yggr.lua.typeid(param:modulus_set(yggr.lua.s64(1))), param:modulus_set(yggr.lua.s64(1)) )
	--]]

	print("\n")
	--param:set_value(100)
	print("lua_test_d64 logic_and:", type(param:logic_and(true)), yggr.lua.typeid(param:logic_and(true)), param:logic_and(true))
	print("lua_test_d64 logic_and:", type(param:logic_and(false)), yggr.lua.typeid(param:logic_and(false)), param:logic_and(false))
	print("lua_test_d64 logic_and:", type(param:logic_and(1)), yggr.lua.typeid(param:logic_and(1)), param:logic_and(1))
	print("lua_test_d64 logic_and:", type(param:logic_and(0)), yggr.lua.typeid(param:logic_and(0)), param:logic_and(0))
	print("lua_test_d64 logic_and:",
			type(param:logic_and(yggr.lua.u64(1))), yggr.lua.typeid(param:logic_and(yggr.lua.u64(1))), param:logic_and(yggr.lua.u64(1)) )
	print("lua_test_d64 logic_and:",
			type(param:logic_and(yggr.lua.u64(0))), yggr.lua.typeid(param:logic_and(yggr.lua.u64(0))), param:logic_and(yggr.lua.u64(0)) )
	print("lua_test_d64 logic_and:",
			type(param:logic_and(yggr.lua.s64(1))), yggr.lua.typeid(param:logic_and(yggr.lua.s64(1))), param:logic_and(yggr.lua.s64(1)) )
	print("lua_test_d64 logic_and:",
			type(param:logic_and(yggr.lua.s64(0))), yggr.lua.typeid(param:logic_and(yggr.lua.s64(0))), param:logic_and(yggr.lua.s64(0)) )

	print("\n")
	print("lua_test_d64 logic_or:", type(param:logic_or(true)), yggr.lua.typeid(param:logic_or(true)), param:logic_or(true))
	print("lua_test_d64 logic_or:", type(param:logic_or(false)), yggr.lua.typeid(param:logic_or(false)), param:logic_or(false))
	print("lua_test_d64 logic_or:", type(param:logic_or(1)), yggr.lua.typeid(param:logic_or(1)), param:logic_or(1))
	print("lua_test_d64 logic_or:", type(param:logic_or(0)), yggr.lua.typeid(param:logic_or(0)), param:logic_or(0))
	print("lua_test_d64 logic_or:",
			type(param:logic_or(yggr.lua.u64(1))), yggr.lua.typeid(param:logic_or(yggr.lua.u64(1))), param:logic_or(yggr.lua.u64(1)) )
	print("lua_test_d64 logic_or:",
			type(param:logic_or(yggr.lua.u64(0))), yggr.lua.typeid(param:logic_or(yggr.lua.u64(0))), param:logic_or(yggr.lua.u64(0)) )
	print("lua_test_d64 logic_or:",
			type(param:logic_or(yggr.lua.s64(1))), yggr.lua.typeid(param:logic_or(yggr.lua.s64(1))), param:logic_or(yggr.lua.s64(1)) )
	print("lua_test_d64 logic_or:",
			type(param:logic_or(yggr.lua.s64(0))), yggr.lua.typeid(param:logic_or(yggr.lua.s64(0))), param:logic_or(yggr.lua.s64(0)) )

	print("\n")
	print("lua_test_d64 logic_not:", type(param:logic_not()), yggr.lua.typeid(param:logic_not()), param:logic_not())

	print("\n")
	--param:set_value(100)
	print("lua_test_d64 compare_eq:", type(param:compare_eq(true)), yggr.lua.typeid(param:compare_eq(true)), param:compare_eq(true))
	print("lua_test_d64 compare_eq:", type(param:compare_eq(false)), yggr.lua.typeid(param:compare_eq(false)), param:compare_eq(false))
	print("lua_test_d64 compare_eq:", type(param:compare_eq(100)), yggr.lua.typeid(param:compare_eq(100)), param:compare_eq(100))
	print("lua_test_d64 compare_eq:", type(param:compare_eq(0)), yggr.lua.typeid(param:compare_eq(0)), param:compare_eq(0))
	print("lua_test_d64 compare_eq:",
			type(param:compare_eq(yggr.lua.u64(100))), yggr.lua.typeid(param:compare_eq(yggr.lua.u64(100))), param:compare_eq(yggr.lua.u64(100)) )
	print("lua_test_d64 compare_eq:",
			type(param:compare_eq(yggr.lua.u64(0))), yggr.lua.typeid(param:compare_eq(yggr.lua.u64(0))), param:compare_eq(yggr.lua.u64(0)) )
	print("lua_test_d64 compare_eq:",
			type(param:compare_eq(yggr.lua.s64(100))), yggr.lua.typeid(param:compare_eq(yggr.lua.s64(100))), param:compare_eq(yggr.lua.s64(100)) )
	print("lua_test_d64 compare_eq:",
			type(param:compare_eq(yggr.lua.s64(0))), yggr.lua.typeid(param:compare_eq(yggr.lua.s64(0))), param:compare_eq(yggr.lua.s64(0)) )

	print("\n")
	--param:set_value(100)
	print("lua_test_d64 compare:", type(param:compare(true)), yggr.lua.typeid(param:compare(true)), param:compare(true))
	print("lua_test_d64 compare:", type(param:compare(false)), yggr.lua.typeid(param:compare(false)), param:compare(false))
	print("lua_test_d64 compare:", type(param:compare(101)), yggr.lua.typeid(param:compare(101)), param:compare(101))
	print("lua_test_d64 compare:", type(param:compare(100)), yggr.lua.typeid(param:compare(100)), param:compare(100))
	print("lua_test_d64 compare:", type(param:compare(0)), yggr.lua.typeid(param:compare(0)), param:compare(0))
	print("lua_test_d64 compare:",
			type(param:compare(yggr.lua.u64(101))), yggr.lua.typeid(param:compare(yggr.lua.u64(101))), param:compare(yggr.lua.u64(101)) )
	print("lua_test_d64 compare:",
			type(param:compare(yggr.lua.u64(100))), yggr.lua.typeid(param:compare(yggr.lua.u64(100))), param:compare(yggr.lua.u64(100)) )
	print("lua_test_d64 compare:",
			type(param:compare(yggr.lua.u64(0))), yggr.lua.typeid(param:compare(yggr.lua.u64(0))), param:compare(yggr.lua.u64(0)) )
	print("lua_test_d64 compare:",
			type(param:compare(yggr.lua.s64(101))), yggr.lua.typeid(param:compare(yggr.lua.s64(101))), param:compare(yggr.lua.s64(101)) )
	print("lua_test_d64 compare:",
			type(param:compare(yggr.lua.s64(100))), yggr.lua.typeid(param:compare(yggr.lua.s64(100))), param:compare(yggr.lua.s64(100)) )
	print("lua_test_d64 compare:",
			type(param:compare(yggr.lua.s64(0))), yggr.lua.typeid(param:compare(yggr.lua.s64(0))), param:compare(yggr.lua.s64(0)) )

	print("\n")
	print("lua_test_d64 bitwise_left_shift:", type(param:bitwise_left_shift(false)), yggr.lua.typeid(param:bitwise_left_shift(false)), param:bitwise_left_shift(false))
	print("lua_test_d64 bitwise_left_shift:", type(param:bitwise_left_shift(true)), yggr.lua.typeid(param:bitwise_left_shift(true)), param:bitwise_left_shift(true))
	print("lua_test_d64 bitwise_left_shift:", type(param:bitwise_left_shift(1)), yggr.lua.typeid(param:bitwise_left_shift(1)), param:bitwise_left_shift(1))
	print("lua_test_d64 bitwise_left_shift:",
			type(param:bitwise_left_shift(yggr.lua.u64(1))), yggr.lua.typeid(param:bitwise_left_shift(yggr.lua.u64(1))), param:bitwise_left_shift(yggr.lua.u64(1)) )
	print("lua_test_d64 bitwise_left_shift:",
			type(param:bitwise_left_shift(yggr.lua.s64(1))), yggr.lua.typeid(param:bitwise_left_shift(yggr.lua.s64(1))), param:bitwise_left_shift(yggr.lua.s64(1)) )

	-- cref not support <<=
	--[[
	print("\n")
	print("lua_test_d64 bitwise_left_shift_set:", type(param:bitwise_left_shift_set(false)), yggr.lua.typeid(param:bitwise_left_shift_set(false)), param:bitwise_left_shift_set(false))
	print("lua_test_d64 bitwise_left_shift_set:", type(param:bitwise_left_shift_set(true)), yggr.lua.typeid(param:bitwise_left_shift_set(true)), param:bitwise_left_shift_set(true))
	print("lua_test_d64 bitwise_left_shift_set:", type(param:bitwise_left_shift_set(1)), yggr.lua.typeid(param:bitwise_left_shift_set(1)), param:bitwise_left_shift_set(1))
	print("lua_test_d64 bitwise_left_shift_set:",
			type(param:bitwise_left_shift_set(yggr.lua.u64(1))), yggr.lua.typeid(param:bitwise_left_shift_set(yggr.lua.u64(1))), param:bitwise_left_shift_set(yggr.lua.u64(1)) )
	print("lua_test_d64 bitwise_left_shift_set:",
			type(param:bitwise_left_shift_set(yggr.lua.s64(1))), yggr.lua.typeid(param:bitwise_left_shift_set(yggr.lua.s64(1))), param:bitwise_left_shift_set(yggr.lua.s64(1)) )
	--]]

	print("\n")
	print("lua_test_d64 bitwise_right_shift:", type(param:bitwise_right_shift(false)), yggr.lua.typeid(param:bitwise_right_shift(false)), param:bitwise_right_shift(false))
	print("lua_test_d64 bitwise_right_shift:", type(param:bitwise_right_shift(true)), yggr.lua.typeid(param:bitwise_right_shift(true)), param:bitwise_right_shift(true))
	print("lua_test_d64 bitwise_right_shift:", type(param:bitwise_right_shift(1)), yggr.lua.typeid(param:bitwise_right_shift(1)), param:bitwise_right_shift(1))
	print("lua_test_d64 bitwise_right_shift:",
			type(param:bitwise_right_shift(yggr.lua.u64(1))), yggr.lua.typeid(param:bitwise_right_shift(yggr.lua.u64(1))), param:bitwise_right_shift(yggr.lua.u64(1)) )
	print("lua_test_d64 bitwise_right_shift:",
			type(param:bitwise_right_shift(yggr.lua.s64(1))), yggr.lua.typeid(param:bitwise_right_shift(yggr.lua.s64(1))), param:bitwise_right_shift(yggr.lua.s64(1)) )

	-- cref not support >>=
	--[[
	print("\n")
	print("lua_test_d64 bitwise_right_shift_set:", type(param:bitwise_right_shift_set(false)), yggr.lua.typeid(param:bitwise_right_shift_set(false)), param:bitwise_right_shift_set(false))
	print("lua_test_d64 bitwise_right_shift_set:", type(param:bitwise_right_shift_set(true)), yggr.lua.typeid(param:bitwise_right_shift_set(true)), param:bitwise_right_shift_set(true))
	print("lua_test_d64 bitwise_right_shift_set:", type(param:bitwise_right_shift_set(1)), yggr.lua.typeid(param:bitwise_right_shift_set(1)), param:bitwise_right_shift_set(1))
	print("lua_test_d64 bitwise_left_shift_set:",
			type(param:bitwise_right_shift_set(yggr.lua.u64(1))), yggr.lua.typeid(param:bitwise_right_shift_set(yggr.lua.u64(1))), param:bitwise_right_shift_set(yggr.lua.u64(1)) )
	print("lua_test_d64 bitwise_right_shift_set:",
			type(param:bitwise_right_shift_set(yggr.lua.s64(1))), yggr.lua.typeid(param:bitwise_right_shift_set(yggr.lua.s64(1))), param:bitwise_right_shift_set(yggr.lua.s64(1)) )
	--]]

	print("\n")
	print("lua_test_d64 bitwise_not:", type(param:bitwise_not()), yggr.lua.typeid(param:bitwise_not()), param:bitwise_not())

	print("\n")
	print("lua_test_d64 bitwise_and:", type(param:bitwise_and(false)), yggr.lua.typeid(param:bitwise_and(false)), param:bitwise_and(false))
	print("lua_test_d64 bitwise_and:", type(param:bitwise_and(true)), yggr.lua.typeid(param:bitwise_and(true)), param:bitwise_and(true))
	print("lua_test_d64 bitwise_and:", type(param:bitwise_and(1)), yggr.lua.typeid(param:bitwise_and(1)), param:bitwise_and(1))
	print("lua_test_d64 bitwise_and:",
			type(param:bitwise_and(yggr.lua.u64(1))), yggr.lua.typeid(param:bitwise_and(yggr.lua.u64(1))), param:bitwise_and(yggr.lua.u64(1)) )
	print("lua_test_d64 bitwise_and:",
			type(param:bitwise_and(yggr.lua.s64(1))), yggr.lua.typeid(param:bitwise_and(yggr.lua.s64(1))), param:bitwise_and(yggr.lua.s64(1)) )

	-- cref not support &=
	--[[
	print("\n")
	print("lua_test_d64 bitwise_and_set:", type(param:bitwise_and_set(false)), yggr.lua.typeid(param:bitwise_and_set(false)), param:bitwise_and_set(false))
	print("lua_test_d64 bitwise_and_set:", type(param:bitwise_and_set(true)), yggr.lua.typeid(param:bitwise_and_set(true)), param:bitwise_and_set(true))
	print("lua_test_d64 bitwise_and_set:", type(param:bitwise_and_set(1)), yggr.lua.typeid(param:bitwise_and_set(1)), param:bitwise_and_set(1))
	print("lua_test_d64 bitwise_and_set:",
			type(param:bitwise_and_set(yggr.lua.u64(1))), yggr.lua.typeid(param:bitwise_and_set(yggr.lua.u64(1))), param:bitwise_and_set(yggr.lua.u64(1)) )
	print("lua_test_d64 bitwise_and_set:",
			type(param:bitwise_and_set(yggr.lua.s64(1))), yggr.lua.typeid(param:bitwise_and_set(yggr.lua.s64(1))), param:bitwise_and_set(yggr.lua.s64(1)) )
	--]]

	print("\n")
	print("lua_test_d64 bitwise_or:", type(param:bitwise_or(false)), yggr.lua.typeid(param:bitwise_or(false)), param:bitwise_or(false))
	print("lua_test_d64 bitwise_or:", type(param:bitwise_or(true)), yggr.lua.typeid(param:bitwise_or(true)), param:bitwise_or(true))
	print("lua_test_d64 bitwise_or:", type(param:bitwise_or(1)), yggr.lua.typeid(param:bitwise_or(1)), param:bitwise_or(1))
	print("lua_test_d64 bitwise_or:",
			type(param:bitwise_or(yggr.lua.u64(1))), yggr.lua.typeid(param:bitwise_or(yggr.lua.u64(1))), param:bitwise_or(yggr.lua.u64(1)) )
	print("lua_test_d64 bitwise_or:",
			type(param:bitwise_or(yggr.lua.s64(1))), yggr.lua.typeid(param:bitwise_or(yggr.lua.s64(1))), param:bitwise_or(yggr.lua.s64(1)) )

	-- cref not support |=
	--[[
	print("\n")
	print("lua_test_d64 bitwise_or_set:", type(param:bitwise_or_set(false)), yggr.lua.typeid(param:bitwise_or_set(false)), param:bitwise_or_set(false))
	print("lua_test_d64 bitwise_or_set:", type(param:bitwise_or_set(true)), yggr.lua.typeid(param:bitwise_or_set(true)), param:bitwise_or_set(true))
	print("lua_test_d64 bitwise_or_set:", type(param:bitwise_or_set(1)), yggr.lua.typeid(param:bitwise_or_set(1)), param:bitwise_or_set(1))
	print("lua_test_d64 bitwise_or_set:",
			type(param:bitwise_or_set(yggr.lua.u64(1))), yggr.lua.typeid(param:bitwise_or_set(yggr.lua.u64(1))), param:bitwise_or_set(yggr.lua.u64(1)) )
	print("lua_test_d64 bitwise_or_set:",
			type(param:bitwise_or_set(yggr.lua.s64(1))), yggr.lua.typeid(param:bitwise_or_set(yggr.lua.s64(1))), param:bitwise_or_set(yggr.lua.s64(1)) )
	--]]

	print("\n")
	print("lua_test_d64 bitwise_xor:", type(param:bitwise_xor(false)), yggr.lua.typeid(param:bitwise_xor(false)), param:bitwise_xor(false))
	print("lua_test_d64 bitwise_xor:", type(param:bitwise_xor(true)), yggr.lua.typeid(param:bitwise_xor(true)), param:bitwise_xor(true))
	print("lua_test_d64 bitwise_xor:", type(param:bitwise_xor(1)), yggr.lua.typeid(param:bitwise_xor(1)), param:bitwise_xor(1))
	print("lua_test_d64 bitwise_xor:",
			type(param:bitwise_xor(yggr.lua.u64(1))), yggr.lua.typeid(param:bitwise_xor(yggr.lua.u64(1))), param:bitwise_xor(yggr.lua.u64(1)) )
	print("lua_test_d64 bitwise_xor:",
			type(param:bitwise_xor(yggr.lua.s64(1))), yggr.lua.typeid(param:bitwise_xor(yggr.lua.s64(1))), param:bitwise_xor(yggr.lua.s64(1)) )

	-- cref not support ^=
	--[[
	print("\n")
	print("lua_test_d64 bitwise_xor_set:", type(param:bitwise_xor_set(false)), yggr.lua.typeid(param:bitwise_xor_set(false)), param:bitwise_xor_set(false))
	print("lua_test_d64 bitwise_xor_set:", type(param:bitwise_xor_set(true)), yggr.lua.typeid(param:bitwise_xor_set(true)), param:bitwise_xor_set(true))
	print("lua_test_d64 bitwise_xor_set:", type(param:bitwise_xor_set(1)), yggr.lua.typeid(param:bitwise_xor_set(1)), param:bitwise_xor_set(1))
	print("lua_test_d64 bitwise_xor_set:",
			type(param:bitwise_xor_set(yggr.lua.u64(1))), yggr.lua.typeid(param:bitwise_xor_set(yggr.lua.u64(1))), param:bitwise_xor_set(yggr.lua.u64(1)) )
	print("lua_test_d64 bitwise_xor_set:",
			type(param:bitwise_xor_set(yggr.lua.s64(1))), yggr.lua.typeid(param:bitwise_xor_set(yggr.lua.s64(1))), param:bitwise_xor_set(yggr.lua.s64(1)) )
	--]]

end


function calculate(param)
	print("\n")
	test_d64_cref(param)
	--param:set_value(1000)
	return param + 100
end


