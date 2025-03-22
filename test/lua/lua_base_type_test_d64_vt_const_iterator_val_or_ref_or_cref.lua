
require "yggr_lua_base_type"

local function test_d64_const_iterator_val_or_ref_or_cref(iter_beg, iter_end)
	print("test_iterator show", type(iter_beg), yggr.lua.typeid(iter_beg), iter_beg)
	print("test_iterator show", type(iter_end), yggr.lua.typeid(iter_end), iter_end)

	print("\n")
	print("test_iterator <", type(iter_beg < iter_end), yggr.lua.typeid(iter_beg < iter_end), iter_beg < iter_end)
	print("test_iterator <=", type(iter_beg <= iter_end), yggr.lua.typeid(iter_beg <= iter_end), iter_beg <= iter_end)
	print("test_iterator >", type(iter_beg > iter_end), yggr.lua.typeid(iter_beg > iter_end), iter_beg > iter_end)
	print("test_iterator >=", type(iter_beg >= iter_end), yggr.lua.typeid(iter_beg >= iter_end), iter_beg >= iter_end)
	print("test_iterator ==", type(iter_beg == iter_beg), yggr.lua.typeid(iter_beg == iter_beg), iter_beg == iter_beg)
	print("test_iterator ==", type(iter_beg == iter_end), yggr.lua.typeid(iter_beg == iter_end), iter_beg == iter_end)
	print("test_iterator ~=", type(iter_beg ~= iter_beg), yggr.lua.typeid(iter_beg ~= iter_beg), iter_beg ~= iter_beg)
	print("test_iterator ~=", type(iter_beg ~= iter_end), yggr.lua.typeid(iter_beg ~= iter_end), iter_beg ~= iter_end)

	print("\n")
	print("test_iterator +", type(iter_beg + 1), yggr.lua.typeid(iter_beg + 1), iter_beg + 1, (iter_beg + 1):get_value())
	print("test_iterator +", type(1 + iter_beg), yggr.lua.typeid(1 + iter_beg), 1 + iter_beg, (1 + iter_beg):get_value())
	print("test_iterator +", type(1 + iter_beg + 1), yggr.lua.typeid(1 + iter_beg + 1), 1 + iter_beg + 1, (1 + iter_beg + 1):get_value())

	print("\n")
	print("test_iterator +",
			type(iter_beg + yggr.lua.s64(1)), yggr.lua.typeid(iter_beg + yggr.lua.s64(1)),
			iter_beg + yggr.lua.s64(1), (iter_beg + yggr.lua.s64(1)):get_value())
	print("test_iterator +",
			type(yggr.lua.s64(1) + iter_beg), yggr.lua.typeid(yggr.lua.s64(1) + iter_beg),
			yggr.lua.s64(1) + iter_beg, (yggr.lua.s64(1) + iter_beg):get_value())
	print("test_iterator +",
			type(yggr.lua.s64(1) + iter_beg + yggr.lua.s64(1)), yggr.lua.typeid(yggr.lua.s64(1) + iter_beg + yggr.lua.s64(1)),
			yggr.lua.s64(1) + iter_beg + yggr.lua.s64(1), (yggr.lua.s64(1) + iter_beg + yggr.lua.s64(1)):get_value())

	print("\n")
	print("test_iterator +",
			type(iter_beg + yggr.lua.u64(1)), yggr.lua.typeid(iter_beg + yggr.lua.u64(1)),
			iter_beg + yggr.lua.u64(1), (iter_beg + yggr.lua.u64(1)):get_value())
	print("test_iterator +",
			type(yggr.lua.u64(1) + iter_beg), yggr.lua.typeid(yggr.lua.u64(1) + iter_beg),
			yggr.lua.u64(1) + iter_beg, (yggr.lua.u64(1) + iter_beg):get_value())
	print("test_iterator +",
			type(yggr.lua.u64(1) + iter_beg + yggr.lua.u64(1)), yggr.lua.typeid(yggr.lua.u64(1) + iter_beg + yggr.lua.u64(1)),
			yggr.lua.u64(1) + iter_beg + yggr.lua.u64(1), (yggr.lua.u64(1) + iter_beg + yggr.lua.u64(1)):get_value())


	print("\n")
	print("test_iterator -", type(iter_end - 1), yggr.lua.typeid(iter_end - 1), iter_end - 1, (iter_end - 1):get_value())
	print("test_iterator -",
			type(iter_end - yggr.lua.s64(1)), yggr.lua.typeid(iter_end - yggr.lua.s64(1)),
			iter_end - yggr.lua.s64(1), (iter_end - yggr.lua.s64(1)):get_value())
	print("test_iterator -",
			type(iter_end - yggr.lua.u64(1)), yggr.lua.typeid(iter_end - yggr.lua.u64(1)),
			iter_end - yggr.lua.u64(1), (iter_end - yggr.lua.u64(1)):get_value())
	print("test_iterator -",
			type(iter_end - iter_beg), yggr.lua.typeid(iter_end - iter_beg),
			iter_end - iter_beg)

	print("\n")
	print("test_iterator get_value", type(iter_beg:get_value()), yggr.lua.typeid(iter_beg:get_value()), iter_beg:get_value())
	print("test_iterator get_value", type(iter_beg:get_value(1)), yggr.lua.typeid(iter_beg:get_value(1)), iter_beg:get_value(1))
	print("test_iterator get_value",
			type(iter_beg:get_value(yggr.lua.s64(1))), yggr.lua.typeid(iter_beg:get_value(yggr.lua.s64(1))),
			iter_beg:get_value(yggr.lua.s64(1)))
	print("test_iterator get_value",
			type(iter_beg:get_value(yggr.lua.u64(1))), yggr.lua.typeid(iter_beg:get_value(yggr.lua.u64(1))),
			iter_beg:get_value(yggr.lua.u64(1)))

	print("\n")
	local tmp_iter_beg = iter_beg:copy()
	print("test_iterator head_increment",
			type(tmp_iter_beg:head_increment()), yggr.lua.typeid(tmp_iter_beg:head_increment()),
			tmp_iter_beg:head_increment(), (tmp_iter_beg:head_increment()):get_value())

	print("\n")
	tmp_iter_beg = iter_beg:copy()
	print("test_iterator tail_increment",
			type(tmp_iter_beg:tail_increment()), yggr.lua.typeid(tmp_iter_beg:tail_increment()),
			tmp_iter_beg:tail_increment(), (tmp_iter_beg:tail_increment()):get_value())

	print("\n")
	local tmp_iter_end = iter_end:copy()
	print("test_iterator head_decrement",
			type(tmp_iter_end:head_decrement()), yggr.lua.typeid(tmp_iter_end:head_decrement()),
			tmp_iter_end:head_decrement(), (tmp_iter_end:head_decrement()):get_value())

	print("\n")
	tmp_iter_end = iter_end:copy()
	print("test_iterator tail_decrement",
			type(tmp_iter_end:tail_decrement()), yggr.lua.typeid(tmp_iter_end:tail_decrement()),
			tmp_iter_end:tail_decrement(), (tmp_iter_end:tail_decrement()):get_value())

	print("\n")
	print("test_iterator plus", type(iter_beg:plus(1)), yggr.lua.typeid(iter_beg:plus(1)), iter_beg:plus(1), (iter_beg:plus(1)):get_value())
	print("test_iterator plus",
			type(iter_beg:plus(yggr.lua.s64(1))), yggr.lua.typeid(iter_beg:plus(yggr.lua.s64(1))),
			iter_beg:plus(yggr.lua.s64(1)), (iter_beg:plus(yggr.lua.s64(1))):get_value())
	print("test_iterator plus",
			type(iter_beg:plus(yggr.lua.u64(1))), yggr.lua.typeid(iter_beg:plus(yggr.lua.u64(1))),
			iter_beg:plus(yggr.lua.u64(1)), (iter_beg:plus(yggr.lua.u64(1))):get_value())

	print("\n")
	tmp_iter_beg = iter_beg:copy()
	print("test_iterator plus_set",
			type(tmp_iter_beg:plus_set(1)), yggr.lua.typeid(tmp_iter_beg:plus_set(1)),
			tmp_iter_beg:plus_set(1), (tmp_iter_beg:plus_set(1)):get_value())
	tmp_iter_beg = iter_beg:copy()
	print("test_iterator plus_set",
			type(tmp_iter_beg:plus_set(yggr.lua.s64(1))), yggr.lua.typeid(tmp_iter_beg:plus_set(yggr.lua.s64(1))),
			tmp_iter_beg:plus_set(yggr.lua.s64(1)), (tmp_iter_beg:plus_set(yggr.lua.s64(1))):get_value())
	tmp_iter_beg = iter_beg:copy()
	print("test_iterator plus_set",
			type(tmp_iter_beg:plus_set(yggr.lua.u64(1))), yggr.lua.typeid(tmp_iter_beg:plus_set(yggr.lua.u64(1))),
			tmp_iter_beg:plus_set(yggr.lua.u64(1)), (tmp_iter_beg:plus_set(yggr.lua.u64(1))):get_value())

	print("\n")
	print("test_iterator advance_copy",
			type(yggr.lua.advance_copy(iter_beg, 1)),
			yggr.lua.typeid(yggr.lua.advance_copy(iter_beg, 1)),
			yggr.lua.advance_copy(iter_beg, 1),
			(yggr.lua.advance_copy(iter_beg, 1)):get_value())
	print("test_iterator advance_copy",
			type(yggr.lua.advance_copy(iter_beg, yggr.lua.s64(1))),
			yggr.lua.typeid(yggr.lua.advance_copy(iter_beg, yggr.lua.s64(1))),
			yggr.lua.advance_copy(iter_beg, yggr.lua.s64(1)),
			(yggr.lua.advance_copy(iter_beg, yggr.lua.s64(1))):get_value())
	print("test_iterator advance_copy",
			type(yggr.lua.advance_copy(iter_beg, yggr.lua.u64(1))),
			yggr.lua.typeid(yggr.lua.advance_copy(iter_beg, yggr.lua.u64(1))),
			yggr.lua.advance_copy(iter_beg, yggr.lua.u64(1)),
			(yggr.lua.advance_copy(iter_beg, yggr.lua.u64(1))):get_value())

	print("\n")
	tmp_iter_beg = iter_beg:copy()
	yggr.lua.advance(tmp_iter_beg, 1)
	print("test_iterator advance",
			type(tmp_iter_beg),
			yggr.lua.typeid(tmp_iter_beg),
			tmp_iter_beg,
			(tmp_iter_beg):get_value())
	tmp_iter_beg = iter_beg:copy()
	yggr.lua.advance(tmp_iter_beg, yggr.lua.s64(1))
	print("test_iterator advance",
			type(tmp_iter_beg),
			yggr.lua.typeid(tmp_iter_beg),
			tmp_iter_beg,
			(tmp_iter_beg):get_value())
	tmp_iter_beg = iter_beg:copy()
	yggr.lua.advance(tmp_iter_beg, yggr.lua.u64(1))
	print("test_iterator advance",
			type(tmp_iter_beg),
			yggr.lua.typeid(tmp_iter_beg),
			tmp_iter_beg,
			(tmp_iter_beg):get_value())

	print("\n")
	print("test_iterator minus", type(iter_end:minus(1)), yggr.lua.typeid(iter_end:minus(1)), iter_end:minus(1), (iter_end:minus(1)):get_value())
	print("test_iterator minus",
			type(iter_end:minus(yggr.lua.s64(1))), yggr.lua.typeid(iter_end:minus(yggr.lua.s64(1))),
			iter_end:minus(yggr.lua.s64(1)), (iter_end:minus(yggr.lua.s64(1))):get_value())
	print("test_iterator minus",
			type(iter_end:minus(yggr.lua.u64(1))), yggr.lua.typeid(iter_end:minus(yggr.lua.u64(1))),
			iter_end:minus(yggr.lua.u64(1)), (iter_end:minus(yggr.lua.u64(1))):get_value())
	print("test_iterator minus",
			type(iter_end:minus(iter_beg)), yggr.lua.typeid(iter_end:minus(iter_beg)),
			iter_end:minus(iter_beg))

	print("\n")
	print("test_iterator wraped_minus",
			type(iter_end:wraped_minus(iter_beg)), yggr.lua.typeid(iter_end:wraped_minus(iter_beg)),
			iter_end:wraped_minus(iter_beg))

	print("\n")
	print("test_iterator distance",
			type(yggr.lua.distance(iter_beg, iter_end)),
			yggr.lua.typeid(yggr.lua.distance(iter_beg, iter_end)),
			yggr.lua.distance(iter_beg, iter_end))

	print("\n")
	print("test_iterator wraped_distance",
			type(yggr.lua.wraped_distance(iter_beg, iter_end)),
			yggr.lua.typeid(yggr.lua.wraped_distance(iter_beg, iter_end)),
			yggr.lua.wraped_distance(iter_beg, iter_end))

	print("\n")
	tmp_iter_end = iter_end:copy()
	print("test_iterator minus_set",
			type(tmp_iter_end:minus_set(1)), yggr.lua.typeid(tmp_iter_end:minus_set(1)),
			tmp_iter_end:minus_set(1), (tmp_iter_end:minus_set(1)):get_value())
	tmp_iter_end = iter_end:copy()
	print("test_iterator minus_set",
			type(tmp_iter_end:minus_set(yggr.lua.s64(1))), yggr.lua.typeid(tmp_iter_end:minus_set(yggr.lua.s64(1))),
			tmp_iter_end:minus_set(yggr.lua.s64(1)), (tmp_iter_end:minus_set(yggr.lua.s64(1))):get_value())
	tmp_iter_end = iter_end:copy()
	print("test_iterator minus_set",
			type(tmp_iter_end:minus_set(yggr.lua.u64(1))), yggr.lua.typeid(tmp_iter_end:minus_set(yggr.lua.u64(1))),
			tmp_iter_end:minus_set(yggr.lua.u64(1)), (tmp_iter_end:minus_set(yggr.lua.u64(1))):get_value())

	print("\n")
	print("test_iterator compare_eq",
			type(iter_beg:compare_eq(iter_beg)), yggr.lua.typeid(iter_beg:compare_eq(iter_beg)),
			iter_beg:compare_eq(iter_beg) )
	print("test_iterator compare_eq",
			type(iter_beg:compare_eq(iter_end)), yggr.lua.typeid(iter_beg:compare_eq(iter_end)),
			iter_beg:compare_eq(iter_end) )

	print("\n")
	print("test_iterator compare",
			type(iter_beg:compare(iter_beg)), yggr.lua.typeid(iter_beg:compare(iter_beg)),
			iter_beg:compare(iter_beg) )
	print("test_iterator compare",
			type(iter_beg:compare(iter_end)), yggr.lua.typeid(iter_beg:compare(iter_end)),
			iter_beg:compare(iter_end) )
	print("test_iterator compare",
			type(iter_end:compare(iter_beg)), yggr.lua.typeid(iter_end:compare(iter_beg)),
			iter_end:compare(iter_beg) )
	print("test_iterator compare",
			type(iter_end:compare(iter_end)), yggr.lua.typeid(iter_end:compare(iter_end)),
			iter_end:compare(iter_end) )

	print("\n")
	tmp_iter_beg = iter_beg:copy()
	tmp_iter_end = iter_end:copy()

	while tmp_iter_beg ~= tmp_iter_end do
		print("test_iterator next", tmp_iter_beg:get_value())
		tmp_iter_beg:next()
	end

	print("\n")
	tmp_iter_beg = iter_beg:copy()
	tmp_iter_end = iter_end:copy()

	while tmp_iter_end ~= tmp_iter_beg do
		tmp_iter_end:prev()
		print("test_iterator prev", tmp_iter_end:get_value())
	end

	print("\n")
	tmp_iter_beg = iter_beg:copy()
	tmp_iter_end = iter_end:copy()

	while tmp_iter_beg ~= tmp_iter_end do
		print("test_iterator g_next", tmp_iter_beg:get_value())
		tmp_iter_beg = yggr.lua.next(tmp_iter_beg)
	end

	print("\n")
	tmp_iter_beg = iter_beg:copy()
	tmp_iter_end = iter_end:copy()

	while tmp_iter_end ~= tmp_iter_beg do
		tmp_iter_end = yggr.lua.prev(tmp_iter_end)
		print("test_iterator g_prev", tmp_iter_end:get_value())
	end

	print("\n")
	local iter_hit = yggr.lua.find(iter_beg, iter_end, 3)
	print("test_iterator find", type(iter_hit), yggr.lua.typeid(iter_hit), iter_hit, iter_hit:get_value() == 3)


	return iter_beg:get_value()
end


function calculate(iter_beg, iter_end)
	print("\n")
	return test_d64_const_iterator_val_or_ref_or_cref(iter_beg, iter_end)
end


