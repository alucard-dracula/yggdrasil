
require "yggr_lua_base_type"

local function test_u32_reverse_iterator_val_or_ref_or_cref(iter_rbeg, iter_rend)
	print("test_riterator show", type(iter_rbeg), yggr.lua.typeid(iter_rbeg), iter_rbeg)
	print("test_riterator show", type(iter_rend), yggr.lua.typeid(iter_rend), iter_rend)

	print("\n")
	print("test_riterator <", type(iter_rbeg < iter_rend), yggr.lua.typeid(iter_rbeg < iter_rend), iter_rbeg < iter_rend)
	print("test_riterator <=", type(iter_rbeg <= iter_rend), yggr.lua.typeid(iter_rbeg <= iter_rend), iter_rbeg <= iter_rend)
	print("test_riterator >", type(iter_rbeg > iter_rend), yggr.lua.typeid(iter_rbeg > iter_rend), iter_rbeg > iter_rend)
	print("test_riterator >=", type(iter_rbeg >= iter_rend), yggr.lua.typeid(iter_rbeg >= iter_rend), iter_rbeg >= iter_rend)
	print("test_riterator ==", type(iter_rbeg == iter_rbeg), yggr.lua.typeid(iter_rbeg == iter_rbeg), iter_rbeg == iter_rbeg)
	print("test_riterator ==", type(iter_rbeg == iter_rend), yggr.lua.typeid(iter_rbeg == iter_rend), iter_rbeg == iter_rend)
	print("test_riterator ~=", type(iter_rbeg ~= iter_rbeg), yggr.lua.typeid(iter_rbeg ~= iter_rbeg), iter_rbeg ~= iter_rbeg)
	print("test_riterator ~=", type(iter_rbeg ~= iter_rend), yggr.lua.typeid(iter_rbeg ~= iter_rend), iter_rbeg ~= iter_rend)

	print("\n")
	print("test_riterator <",
			type(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) < iter_rend),
			yggr.lua.typeid(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) < iter_rend),
			yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) < iter_rend)
	print("test_riterator <=",
			type(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) <= iter_rend),
			yggr.lua.typeid(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) <= iter_rend),
			yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) <= iter_rend)
	print("test_riterator >",
			type(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) > iter_rend),
			yggr.lua.typeid(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) > iter_rend),
			yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) > iter_rend)
	print("test_riterator >=",
			type(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) >= iter_rend),
			yggr.lua.typeid(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) >= iter_rend),
			yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) >= iter_rend)
	print("test_riterator ==",
			type(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) == iter_rbeg),
			yggr.lua.typeid(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) == iter_rbeg),
			yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) == iter_rbeg)
	print("test_riterator ==",
			type(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) == iter_rend),
			yggr.lua.typeid(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) == iter_rend),
			yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) == iter_rend)
	print("test_riterator ~=",
			type(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) ~= iter_rbeg),
			yggr.lua.typeid(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) ~= iter_rbeg),
			yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) ~= iter_rbeg)
	print("test_riterator ~=",
			type(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) ~= iter_rend),
			yggr.lua.typeid(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) ~= iter_rend),
			yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg) ~= iter_rend)

	print("\n")
	print("test_riterator <",
			type(iter_rbeg < yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)),
			yggr.lua.typeid(iter_rbeg < yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)),
			iter_rbeg < yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))
	print("test_riterator <=",
			type(iter_rbeg <= yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)),
			yggr.lua.typeid(iter_rbeg <= yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)),
			iter_rbeg <= yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))
	print("test_riterator >",
			type(iter_rbeg > yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)),
			yggr.lua.typeid(iter_rbeg > yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)),
			iter_rbeg > yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))
	print("test_riterator >=",
			type(iter_rbeg >= yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)),
			yggr.lua.typeid(iter_rbeg >= yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)),
			iter_rbeg >= yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))
	print("test_riterator ==",
			type(iter_rbeg == yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg)),
			yggr.lua.typeid(iter_rbeg == yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg)),
			iter_rbeg == yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg))
	print("test_riterator ==",
			type(iter_rbeg == yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)),
			yggr.lua.typeid(iter_rbeg == yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)),
			iter_rbeg == yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))
	print("test_riterator ~=",
			type(iter_rbeg ~= yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg)),
			yggr.lua.typeid(iter_rbeg ~= yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg)),
			iter_rbeg ~= yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg))
	print("test_riterator ~=",
			type(iter_rbeg ~= yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)),
			yggr.lua.typeid(iter_rbeg ~= yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)),
			iter_rbeg ~= yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))

	print("\n")
	print("test_riterator +", type(iter_rbeg + 1), yggr.lua.typeid(iter_rbeg + 1), iter_rbeg + 1, (iter_rbeg + 1):get_value())
	print("test_riterator +", type(1 + iter_rbeg), yggr.lua.typeid(1 + iter_rbeg), 1 + iter_rbeg, (1 + iter_rbeg):get_value())
	print("test_riterator +", type(1 + iter_rbeg + 1), yggr.lua.typeid(1 + iter_rbeg + 1), 1 + iter_rbeg + 1, (1 + iter_rbeg + 1):get_value())


	print("\n")
	print("test_riterator +",
			type(iter_rbeg + yggr.lua.s64(1)), yggr.lua.typeid(iter_rbeg + yggr.lua.s64(1)),
			iter_rbeg + yggr.lua.s64(1), (iter_rbeg + yggr.lua.s64(1)):get_value())
	print("test_riterator +",
			type(yggr.lua.s64(1) + iter_rbeg), yggr.lua.typeid(yggr.lua.s64(1) + iter_rbeg),
			yggr.lua.s64(1) + iter_rbeg, (yggr.lua.s64(1) + iter_rbeg):get_value())
	print("test_riterator +",
			type(yggr.lua.s64(1) + iter_rbeg + yggr.lua.s64(1)), yggr.lua.typeid(yggr.lua.s64(1) + iter_rbeg + yggr.lua.s64(1)),
			yggr.lua.s64(1) + iter_rbeg + yggr.lua.s64(1), (yggr.lua.s64(1) + iter_rbeg + yggr.lua.s64(1)):get_value())

	print("\n")
	print("test_riterator +",
			type(iter_rbeg + yggr.lua.u64(1)), yggr.lua.typeid(iter_rbeg + yggr.lua.u64(1)),
			iter_rbeg + yggr.lua.u64(1), (iter_rbeg + yggr.lua.u64(1)):get_value())
	print("test_riterator +",
			type(yggr.lua.u64(1) + iter_rbeg), yggr.lua.typeid(yggr.lua.u64(1) + iter_rbeg),
			yggr.lua.u64(1) + iter_rbeg, (yggr.lua.u64(1) + iter_rbeg):get_value())
	print("test_riterator +",
			type(yggr.lua.u64(1) + iter_rbeg + yggr.lua.u64(1)), yggr.lua.typeid(yggr.lua.u64(1) + iter_rbeg + yggr.lua.u64(1)),
			yggr.lua.u64(1) + iter_rbeg + yggr.lua.u64(1), (yggr.lua.u64(1) + iter_rbeg + yggr.lua.u64(1)):get_value())

	print("\n")
	print("test_riterator -", type(iter_rend - 1), yggr.lua.typeid(iter_rend - 1), iter_rend - 1, (iter_rend - 1):get_value())
	print("test_riterator -",
			type(iter_rend - yggr.lua.s64(1)), yggr.lua.typeid(iter_rend - yggr.lua.s64(1)),
			iter_rend - yggr.lua.s64(1), (iter_rend - yggr.lua.s64(1)):get_value())
	print("test_riterator -",
			type(iter_rend - yggr.lua.u64(1)), yggr.lua.typeid(iter_rend - yggr.lua.u64(1)),
			iter_rend - yggr.lua.u64(1), (iter_rend - yggr.lua.u64(1)):get_value())
	print("test_riterator -",
			type(iter_rend - iter_rbeg), yggr.lua.typeid(iter_rend - iter_rbeg),
			iter_rend - iter_rbeg)
	print("test_riterator -",
			type(iter_rend - yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg)),
			yggr.lua.typeid(iter_rend - yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg)),
			iter_rend - yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg))
	print("test_riterator -",
			type(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend) - iter_rbeg),
			yggr.lua.typeid(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend) - iter_rbeg),
			yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend) - iter_rbeg)

	print("\n")
	print("test_riterator get_value", type(iter_rbeg:get_value()), yggr.lua.typeid(iter_rbeg:get_value()), iter_rbeg:get_value())
	print("test_riterator get_value", type(iter_rbeg:get_value(1)), yggr.lua.typeid(iter_rbeg:get_value(1)), iter_rbeg:get_value(1))
	print("test_riterator get_value",
			type(iter_rbeg:get_value(yggr.lua.s64(1))), yggr.lua.typeid(iter_rbeg:get_value(yggr.lua.s64(1))),
			iter_rbeg:get_value(yggr.lua.s64(1)))
	print("test_riterator get_value",
			type(iter_rbeg:get_value(yggr.lua.u64(1))), yggr.lua.typeid(iter_rbeg:get_value(yggr.lua.u64(1))),
			iter_rbeg:get_value(yggr.lua.u64(1)))

	print("\n")
	print("test_riterator set_value",
			type(iter_rbeg:set_value(10)), yggr.lua.typeid(iter_rbeg:set_value(10)),
			iter_rbeg:set_value(10), iter_rbeg:set_value(10):get_value())
	print("test_riterator set_value",
			type(iter_rbeg:set_value(1)), yggr.lua.typeid(iter_rbeg:set_value(1)),
			iter_rbeg:set_value(1), iter_rbeg:set_value(1):get_value())

	print("test_riterator set_value",
			type(iter_rbeg:set_value(1, 20)), yggr.lua.typeid(iter_rbeg:set_value(1, 20)),
			iter_rbeg:set_value(1, 20), iter_rbeg:set_value(1, 20):get_value(1))
	print("test_riterator set_value",
			type(iter_rbeg:set_value(1, 2)), yggr.lua.typeid(iter_rbeg:set_value(1, 2)),
			iter_rbeg:set_value(1, 2), iter_rbeg:set_value(1, 2):get_value(1))

	print("test_riterator set_value",
			type(iter_rbeg:set_value(yggr.lua.s64(1), 20)), yggr.lua.typeid(iter_rbeg:set_value(yggr.lua.s64(1), 20)),
			iter_rbeg:set_value(yggr.lua.s64(1), 20), iter_rbeg:set_value(yggr.lua.s64(1), 20):get_value(1))
	print("test_riterator set_value",
			type(iter_rbeg:set_value(yggr.lua.s64(1), 2)), yggr.lua.typeid(iter_rbeg:set_value(yggr.lua.s64(1), 2)),
			iter_rbeg:set_value(yggr.lua.s64(1), 2), iter_rbeg:set_value(yggr.lua.s64(1), 2):get_value(1))

	print("test_riterator set_value",
			type(iter_rbeg:set_value(yggr.lua.u64(1), 20)), yggr.lua.typeid(iter_rbeg:set_value(yggr.lua.u64(1), 20)),
			iter_rbeg:set_value(yggr.lua.u64(1), 20), iter_rbeg:set_value(yggr.lua.u64(1), 20):get_value(1))
	print("test_riterator set_value",
			type(iter_rbeg:set_value(yggr.lua.u64(1), 2)), yggr.lua.typeid(iter_rbeg:set_value(yggr.lua.u64(1), 2)),
			iter_rbeg:set_value(yggr.lua.u64(1), 2), iter_rbeg:set_value(yggr.lua.u64(1), 2):get_value(1))

	print("\n")
	local tmp_iter_beg = iter_rbeg:copy()
	print("test_riterator head_increment",
			type(tmp_iter_beg:head_increment()), yggr.lua.typeid(tmp_iter_beg:head_increment()),
			tmp_iter_beg:head_increment(), (tmp_iter_beg:head_increment()):get_value())

	print("\n")
	tmp_iter_beg = iter_rbeg:copy()
	print("test_riterator tail_increment",
			type(tmp_iter_beg:tail_increment()), yggr.lua.typeid(tmp_iter_beg:tail_increment()),
			tmp_iter_beg:tail_increment(), (tmp_iter_beg:tail_increment()):get_value())

	print("\n")
	local tmp_iter_end = iter_rend:copy()
	print("test_riterator head_decrement",
			type(tmp_iter_end:head_decrement()), yggr.lua.typeid(tmp_iter_end:head_decrement()),
			tmp_iter_end:head_decrement(), (tmp_iter_end:head_decrement()):get_value())

	print("\n")
	tmp_iter_end = iter_rend:copy()
	print("test_riterator tail_decrement",
			type(tmp_iter_end:tail_decrement()), yggr.lua.typeid(tmp_iter_end:tail_decrement()),
			tmp_iter_end:tail_decrement(), (tmp_iter_end:tail_decrement()):get_value())

	print("\n")
	print("test_riterator plus", type(iter_rbeg:plus(1)), yggr.lua.typeid(iter_rbeg:plus(1)), iter_rbeg:plus(1), (iter_rbeg:plus(1)):get_value())
	print("test_riterator plus",
			type(iter_rbeg:plus(yggr.lua.s64(1))), yggr.lua.typeid(iter_rbeg:plus(yggr.lua.s64(1))),
			iter_rbeg:plus(yggr.lua.s64(1)), (iter_rbeg:plus(yggr.lua.s64(1))):get_value())
	print("test_riterator plus",
			type(iter_rbeg:plus(yggr.lua.u64(1))), yggr.lua.typeid(iter_rbeg:plus(yggr.lua.u64(1))),
			iter_rbeg:plus(yggr.lua.u64(1)), (iter_rbeg:plus(yggr.lua.u64(1))):get_value())

	print("\n")
	tmp_iter_beg = iter_rbeg:copy()
	print("test_riterator plus_set",
			type(tmp_iter_beg:plus_set(1)), yggr.lua.typeid(tmp_iter_beg:plus_set(1)),
			tmp_iter_beg:plus_set(1), (tmp_iter_beg:plus_set(1)):get_value())
	tmp_iter_beg = iter_rbeg:copy()
	print("test_riterator plus_set",
			type(tmp_iter_beg:plus_set(yggr.lua.s64(1))), yggr.lua.typeid(tmp_iter_beg:plus_set(yggr.lua.s64(1))),
			tmp_iter_beg:plus_set(yggr.lua.s64(1)), (tmp_iter_beg:plus_set(yggr.lua.s64(1))):get_value())
	tmp_iter_beg = iter_rbeg:copy()
	print("test_riterator plus_set",
			type(tmp_iter_beg:plus_set(yggr.lua.u64(1))), yggr.lua.typeid(tmp_iter_beg:plus_set(yggr.lua.u64(1))),
			tmp_iter_beg:plus_set(yggr.lua.u64(1)), (tmp_iter_beg:plus_set(yggr.lua.u64(1))):get_value())

	print("\n")
	print("test_iterator advance_copy",
			type(yggr.lua.advance_copy(iter_rbeg, 1)),
			yggr.lua.typeid(yggr.lua.advance_copy(iter_rbeg, 1)),
			yggr.lua.advance_copy(iter_rbeg, 1),
			(yggr.lua.advance_copy(iter_rbeg, 1)):get_value())
	print("test_iterator advance_copy",
			type(yggr.lua.advance_copy(iter_rbeg, yggr.lua.s64(1))),
			yggr.lua.typeid(yggr.lua.advance_copy(iter_rbeg, yggr.lua.s64(1))),
			yggr.lua.advance_copy(iter_rbeg, yggr.lua.s64(1)),
			(yggr.lua.advance_copy(iter_rbeg, yggr.lua.s64(1))):get_value())
	print("test_iterator advance_copy",
			type(yggr.lua.advance_copy(iter_rbeg, yggr.lua.u64(1))),
			yggr.lua.typeid(yggr.lua.advance_copy(iter_rbeg, yggr.lua.u64(1))),
			yggr.lua.advance_copy(iter_rbeg, yggr.lua.u64(1)),
			(yggr.lua.advance_copy(iter_rbeg, yggr.lua.u64(1))):get_value())

	print("\n")
	tmp_iter_beg = iter_rbeg:copy()
	yggr.lua.advance(tmp_iter_beg, 1)
	print("test_iterator advance",
			type(tmp_iter_beg),
			yggr.lua.typeid(tmp_iter_beg),
			tmp_iter_beg,
			(tmp_iter_beg):get_value())
	tmp_iter_beg = iter_rbeg:copy()
	yggr.lua.advance(tmp_iter_beg, yggr.lua.s64(1))
	print("test_iterator advance",
			type(tmp_iter_beg),
			yggr.lua.typeid(tmp_iter_beg),
			tmp_iter_beg,
			(tmp_iter_beg):get_value())
	tmp_iter_beg = iter_rbeg:copy()
	yggr.lua.advance(tmp_iter_beg, yggr.lua.u64(1))
	print("test_iterator advance",
			type(tmp_iter_beg),
			yggr.lua.typeid(tmp_iter_beg),
			tmp_iter_beg,
			(tmp_iter_beg):get_value())

	print("\n")
	print("test_riterator minus",
			type(iter_rend:minus(1)),
			yggr.lua.typeid(iter_rend:minus(1)),
			iter_rend:minus(1), (iter_rend:minus(1)):get_value())
	print("test_riterator minus",
			type(iter_rend:minus(yggr.lua.s64(1))), yggr.lua.typeid(iter_rend:minus(yggr.lua.s64(1))),
			iter_rend:minus(yggr.lua.s64(1)), (iter_rend:minus(yggr.lua.s64(1))):get_value())
	print("test_riterator minus",
			type(iter_rend:minus(yggr.lua.u64(1))), yggr.lua.typeid(iter_rend:minus(yggr.lua.u64(1))),
			iter_rend:minus(yggr.lua.u64(1)), (iter_rend:minus(yggr.lua.u64(1))):get_value())
	print("test_riterator minus",
			type(iter_rend:minus(iter_rbeg)), yggr.lua.typeid(iter_rend:minus(iter_rbeg)),
			iter_rend:minus(iter_rbeg))
	print("test_riterator minus",
			type(iter_rend:minus(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg))),
			yggr.lua.typeid(iter_rend:minus(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg))),
			iter_rend:minus(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg)))

	print("\n")
	print("test_riterator wraped_minus",
			type(iter_rend:wraped_minus(iter_rbeg)), yggr.lua.typeid(iter_rend:wraped_minus(iter_rbeg)),
			iter_rend:wraped_minus(iter_rbeg))
	print("test_riterator wraped_minus",
			type(iter_rend:wraped_minus(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg))),
			yggr.lua.typeid(iter_rend:wraped_minus(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg))),
			iter_rend:wraped_minus(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg)))

	print("\n")
	print("test_iterator distance",
			type(yggr.lua.distance(iter_rbeg, iter_rend)),
			yggr.lua.typeid(yggr.lua.distance(iter_rbeg, iter_rend)),
			yggr.lua.distance(iter_rbeg, iter_rend))
	print("test_iterator distance",
			type(yggr.lua.distance(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg), iter_rend)),
			yggr.lua.typeid(yggr.lua.distance(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg), iter_rend)),
			yggr.lua.distance(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg), iter_rend))
	print("test_iterator distance",
			type(yggr.lua.distance(iter_rbeg, yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))),
			yggr.lua.typeid(yggr.lua.distance(iter_rbeg, yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))),
			yggr.lua.distance(iter_rbeg, yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)))
	print("test_iterator distance",
			type(yggr.lua.distance(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg), yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))),
			yggr.lua.typeid(yggr.lua.distance(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg), yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))),
			yggr.lua.distance(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg), yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)))

	print("\n")
	print("test_iterator wraped_distance",
			type(yggr.lua.wraped_distance(iter_rbeg, iter_rend)),
			yggr.lua.typeid(yggr.lua.wraped_distance(iter_rbeg, iter_rend)),
			yggr.lua.wraped_distance(iter_rbeg, iter_rend))
	print("test_iterator wraped_distance",
			type(yggr.lua.wraped_distance(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg), iter_rend)),
			yggr.lua.typeid(yggr.lua.wraped_distance(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg), iter_rend)),
			yggr.lua.wraped_distance(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg), iter_rend))
	print("test_iterator wraped_distance",
			type(yggr.lua.wraped_distance(iter_rbeg, yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))),
			yggr.lua.typeid(yggr.lua.wraped_distance(iter_rbeg, yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))),
			yggr.lua.wraped_distance(iter_rbeg, yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)))
	print("test_iterator wraped_distance",
			type(yggr.lua.wraped_distance(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg), yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))),
			yggr.lua.typeid(yggr.lua.wraped_distance(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg), yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))),
			yggr.lua.wraped_distance(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg), yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)))


	print("\n")
	tmp_iter_end = iter_rend:copy()
	print("test_riterator minus_set",
			type(tmp_iter_end:minus_set(1)), yggr.lua.typeid(tmp_iter_end:minus_set(1)),
			tmp_iter_end:minus_set(1), (tmp_iter_end:minus_set(1)):get_value())
	tmp_iter_end = iter_rend:copy()
	print("test_riterator minus_set",
			type(tmp_iter_end:minus_set(yggr.lua.s64(1))), yggr.lua.typeid(tmp_iter_end:minus_set(yggr.lua.s64(1))),
			tmp_iter_end:minus_set(yggr.lua.s64(1)), (tmp_iter_end:minus_set(yggr.lua.s64(1))):get_value())
	tmp_iter_end = iter_rend:copy()
	print("test_riterator minus_set",
			type(tmp_iter_end:minus_set(yggr.lua.u64(1))), yggr.lua.typeid(tmp_iter_end:minus_set(yggr.lua.u64(1))),
			tmp_iter_end:minus_set(yggr.lua.u64(1)), (tmp_iter_end:minus_set(yggr.lua.u64(1))):get_value())

	print("\n")
	print("test_riterator compare_eq",
			type(iter_rbeg:compare_eq(iter_rbeg)), yggr.lua.typeid(iter_rbeg:compare_eq(iter_rbeg)),
			iter_rbeg:compare_eq(iter_rbeg) )
	print("test_riterator compare_eq",
			type(iter_rbeg:compare_eq(iter_rend)), yggr.lua.typeid(iter_rbeg:compare_eq(iter_rend)),
			iter_rbeg:compare_eq(iter_rend) )

	print("test_riterator compare_eq",
			type(iter_rbeg:compare_eq(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg))),
			yggr.lua.typeid(iter_rbeg:compare_eq(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg))),
			iter_rbeg:compare_eq(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg)) )
	print("test_riterator compare_eq",
			type(iter_rbeg:compare_eq(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))),
			yggr.lua.typeid(iter_rbeg:compare_eq(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))),
			iter_rbeg:compare_eq(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)) )

	print("\n")
	print("test_riterator compare",
			type(iter_rbeg:compare(iter_rbeg)), yggr.lua.typeid(iter_rbeg:compare(iter_rbeg)),
			iter_rbeg:compare(iter_rbeg) )
	print("test_riterator compare",
			type(iter_rbeg:compare(iter_rend)), yggr.lua.typeid(iter_rbeg:compare(iter_rend)),
			iter_rbeg:compare(iter_rend) )
	print("test_riterator compare",
			type(iter_rend:compare(iter_rbeg)), yggr.lua.typeid(iter_rend:compare(iter_rbeg)),
			iter_rend:compare(iter_rbeg) )
	print("test_riterator compare",
			type(iter_rend:compare(iter_rend)), yggr.lua.typeid(iter_rend:compare(iter_rend)),
			iter_rend:compare(iter_rend) )

	print("\n")
	print("test_riterator compare",
			type(iter_rbeg:compare(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg))),
			yggr.lua.typeid(iter_rbeg:compare(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg))),
			iter_rbeg:compare(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg)) )
	print("test_riterator compare",
			type(iter_rbeg:compare(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))),
			yggr.lua.typeid(iter_rbeg:compare(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))),
			iter_rbeg:compare(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)) )
	print("test_riterator compare",
			type(iter_rend:compare(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg))),
			yggr.lua.typeid(iter_rend:compare(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg))),
			iter_rend:compare(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rbeg)) )
	print("test_riterator compare",
			type(iter_rend:compare(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))),
			yggr.lua.typeid(iter_rend:compare(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend))),
			iter_rend:compare(yggr.lua.u32_vector_const_reverse_iterator_type(iter_rend)) )

	print("\n")
	tmp_iter_beg = iter_rbeg:copy()
	tmp_iter_end = iter_rend:copy()

	while tmp_iter_beg ~= tmp_iter_end do
		print("test_riterator next", tmp_iter_beg:get_value())
		tmp_iter_beg:next()
	end

	print("\n")
	tmp_iter_beg = iter_rbeg:copy()
	tmp_iter_end = iter_rend:copy()

	while tmp_iter_end ~= tmp_iter_beg do
		tmp_iter_end:prev()
		print("test_riterator prev", tmp_iter_end:get_value())
	end

	print("\n")
	tmp_iter_beg = iter_rbeg:copy()
	tmp_iter_end = iter_rend:copy()

	while tmp_iter_beg ~= tmp_iter_end do
		print("test_iterator g_next", tmp_iter_beg:get_value())
		tmp_iter_beg = yggr.lua.next(tmp_iter_beg)
	end

	print("\n")
	tmp_iter_beg = iter_rbeg:copy()
	tmp_iter_end = iter_rend:copy()

	while tmp_iter_end ~= tmp_iter_beg do
		tmp_iter_end = yggr.lua.prev(tmp_iter_end)
		print("test_iterator g_prev", tmp_iter_end:get_value())
	end


	print("\n")
	tmp_iter_beg = iter_rbeg:copy()
	tmp_iter_end = iter_rend:copy()

	yggr.lua.sort_aesc(tmp_iter_beg, tmp_iter_end)

	while tmp_iter_beg ~= tmp_iter_end do
		print("test_riterator sort_aesc", tmp_iter_beg:get_value())
		tmp_iter_beg:next()
	end

	print("\n")
	tmp_iter_beg = iter_rbeg:copy()
	tmp_iter_end = iter_rend:copy()

	yggr.lua.sort_desc(tmp_iter_beg, tmp_iter_end)

	while tmp_iter_beg ~= tmp_iter_end do
		print("test_riterator sort_desc", tmp_iter_beg:get_value())
		tmp_iter_beg:next()
	end

	print("\n")
	tmp_iter_beg = iter_rbeg:copy()
	tmp_iter_end = iter_rend:copy()

	yggr.lua.reverse(tmp_iter_beg, tmp_iter_end)

	while tmp_iter_beg ~= tmp_iter_end do
		print("test_riterator reverse", tmp_iter_beg:get_value())
		tmp_iter_beg:next()
	end

	print("\n")
	tmp_iter_beg = iter_rbeg:copy()
	tmp_iter_end = iter_rend:copy()

	yggr.lua.shuffle(tmp_iter_beg, tmp_iter_end)

	while tmp_iter_beg ~= tmp_iter_end do
		print("test_riterator shuffle", tmp_iter_beg:get_value())
		tmp_iter_beg:next()
	end

	print("\n")
	local iter_hit = yggr.lua.find(iter_rbeg, iter_rend, 3)
	print("test_iterator find", type(iter_hit), yggr.lua.typeid(iter_hit), iter_hit, iter_hit:get_value() == 3)

	print("\n")
	local new_end = yggr.lua.remove(iter_rbeg, iter_rend, 3)
	print("test_riterator remove", type(new_end), yggr.lua.typeid(new_end), new_end)
	tmp_iter_beg = iter_rbeg:copy()
	tmp_iter_end = iter_rend:copy()
	while tmp_iter_beg ~= tmp_iter_end do
		print("test_riterator remove", tmp_iter_beg:get_value())
		tmp_iter_beg:next()
	end

	return iter_rbeg:get_value()
end


function calculate(iter_rbeg, iter_rend)
	print("\n")
	return test_u32_reverse_iterator_val_or_ref_or_cref(iter_rbeg, iter_rend)
end


