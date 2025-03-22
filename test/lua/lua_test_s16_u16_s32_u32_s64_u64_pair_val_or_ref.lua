

local function test_s16_u16_s32_u32_s64_u64_pair_val_or_ref(s16u16_pair, s32u32_pair, s64u64_pair)
	print("test_pair show", type(s16u16_pair), yggr.lua.typeid(s16u16_pair), s16u16_pair)
	print("test_pair show", type(s32u32_pair), yggr.lua.typeid(s32u32_pair), s32u32_pair)
	print("test_pair show", type(s64u64_pair), yggr.lua.typeid(s64u64_pair), s64u64_pair)

	print("\n")
	print("test_pair construct", type(yggr.lua.s16_u16_pair_type()), yggr.lua.typeid(yggr.lua.s16_u16_pair_type()), yggr.lua.s16_u16_pair_type())
	print("test_pair construct", type(yggr.lua.s32_u32_pair_type()), yggr.lua.typeid(yggr.lua.s32_u32_pair_type()), yggr.lua.s32_u32_pair_type())
	print("test_pair construct", type(yggr.lua.s64_u64_pair_type()), yggr.lua.typeid(yggr.lua.s64_u64_pair_type()), yggr.lua.s64_u64_pair_type())

	print("\n")
	print("test_pair construct",
			type(yggr.lua.s16_u16_pair_type(1, 1)),
			yggr.lua.typeid(yggr.lua.s16_u16_pair_type(1, 1)),
			yggr.lua.s16_u16_pair_type(1, 1))
	print("test_pair construct",
			type(yggr.lua.s32_u32_pair_type(2, 2)),
			yggr.lua.typeid(yggr.lua.s32_u32_pair_type(2, 2)),
			yggr.lua.s32_u32_pair_type(2, 2))
	print("test_pair construct",
			type(yggr.lua.s64_u64_pair_type(3, 3)),
			yggr.lua.typeid(yggr.lua.s64_u64_pair_type(3, 3)),
			yggr.lua.s64_u64_pair_type(3, 3))

	print("\n")
	print("test_pair construct",
			type(yggr.lua.s16_u16_pair_type(s16u16_pair)),
			yggr.lua.typeid(yggr.lua.s16_u16_pair_type(s16u16_pair)),
			yggr.lua.s16_u16_pair_type(s16u16_pair))
	print("test_pair construct",
			type(yggr.lua.s32_u32_pair_type(s32u32_pair)),
			yggr.lua.typeid(yggr.lua.s32_u32_pair_type(s32u32_pair)),
			yggr.lua.s32_u32_pair_type(s32u32_pair))
	print("test_pair construct",
			type(yggr.lua.s64_u64_pair_type(s64u64_pair)),
			yggr.lua.typeid(yggr.lua.s64_u64_pair_type(s64u64_pair)),
			yggr.lua.s64_u64_pair_type(s64u64_pair))

	print("\n")
	print("test_pair construct",
			type(yggr.lua.s16_u16_pair_type(s32u32_pair)),
			yggr.lua.typeid(yggr.lua.s16_u16_pair_type(s32u32_pair)),
			yggr.lua.s16_u16_pair_type(s32u32_pair))
	print("test_pair construct",
			type(yggr.lua.s32_u32_pair_type(s64u64_pair)),
			yggr.lua.typeid(yggr.lua.s32_u32_pair_type(s64u64_pair)),
			yggr.lua.s32_u32_pair_type(s64u64_pair))
	print("test_pair construct",
			type(yggr.lua.s64_u64_pair_type(s16u16_pair)),
			yggr.lua.typeid(yggr.lua.s64_u64_pair_type(s16u16_pair)),
			yggr.lua.s64_u64_pair_type(s16u16_pair))

	print("\n")
	local set_tmp = s16u16_pair:copy()
	set_tmp = yggr.lua.s16_u16_pair_type(1, 1)
	print("test_pair operator=", type(set_tmp), yggr.lua.typeid(set_tmp), set_tmp)

	set_tmp = s32u32_pair:copy()
	set_tmp = yggr.lua.s32_u32_pair_type(1, 1)
	print("test_pair operator=", type(set_tmp), yggr.lua.typeid(set_tmp), set_tmp)

	set_tmp = s64u64_pair:copy()
	set_tmp = yggr.lua.s64_u64_pair_type(1, 1)
	print("test_pair operator=", type(set_tmp), yggr.lua.typeid(set_tmp), set_tmp)

	print("\n")
	print("test_pair get_first", type(s16u16_pair:get_first()), yggr.lua.typeid(s16u16_pair:get_first()), s16u16_pair:get_first())
	print("test_pair get_first", type(s32u32_pair:get_first()), yggr.lua.typeid(s32u32_pair:get_first()), s32u32_pair:get_first())
	print("test_pair get_first", type(s64u64_pair:get_first()), yggr.lua.typeid(s64u64_pair:get_first()), s64u64_pair:get_first())

	print("\n")
	print("test_pair get_second", type(s16u16_pair:get_second()), yggr.lua.typeid(s16u16_pair:get_second()), s16u16_pair:get_second())
	print("test_pair get_second", type(s32u32_pair:get_second()), yggr.lua.typeid(s32u32_pair:get_second()), s32u32_pair:get_second())
	print("test_pair get_second", type(s64u64_pair:get_second()), yggr.lua.typeid(s64u64_pair:get_second()), s64u64_pair:get_second())

	print("\n")
	local set_first_tmp = s16u16_pair:copy()
	print("test_pair set_first", type(set_first_tmp:set_first(1)), yggr.lua.typeid(set_first_tmp:set_first(1)), set_first_tmp:set_first(1))

	set_first_tmp = s32u32_pair:copy()
	print("test_pair set_first", type(set_first_tmp:set_first(1)), yggr.lua.typeid(set_first_tmp:set_first(1)), set_first_tmp:set_first(1))

	set_first_tmp = s64u64_pair:copy()
	print("test_pair set_first", type(set_first_tmp:set_first(1)), yggr.lua.typeid(set_first_tmp:set_first(1)), set_first_tmp:set_first(1))

	print("\n")
	local set_second_tmp = s16u16_pair:copy()
	print("test_pair set_second", type(set_second_tmp:set_second(1)), yggr.lua.typeid(set_second_tmp:set_second(1)), set_second_tmp:set_second(1))

	set_second_tmp = s32u32_pair:copy()
	print("test_pair set_second", type(set_second_tmp:set_second(1)), yggr.lua.typeid(set_second_tmp:set_second(1)), set_second_tmp:set_second(1))

	set_second_tmp = s64u64_pair:copy()
	print("test_pair set_second", type(set_second_tmp:set_second(1)), yggr.lua.typeid(set_second_tmp:set_second(1)), set_second_tmp:set_second(1))

	print("\n")
	print("test_pair compare_eq",
			type(s16u16_pair:compare_eq(yggr.lua.s16_u16_pair_type(16, 1016) )),
			yggr.lua.typeid(s16u16_pair:compare_eq(yggr.lua.s16_u16_pair_type(16, 1016) )),
			s16u16_pair:compare_eq(yggr.lua.s16_u16_pair_type(16, 1016) ))
	print("test_pair compare_eq",
			type(s32u32_pair:compare_eq(yggr.lua.s32_u32_pair_type(32, 1032) )),
			yggr.lua.typeid(s32u32_pair:compare_eq(yggr.lua.s32_u32_pair_type(32, 1032) )),
			s32u32_pair:compare_eq(yggr.lua.s32_u32_pair_type(32, 1032) ))
	print("test_pair compare_eq",
			type(s64u64_pair:compare_eq(yggr.lua.s64_u64_pair_type(64, 1064) )),
			yggr.lua.typeid(s64u64_pair:compare_eq(yggr.lua.s64_u64_pair_type(64, 1064) )),
			s64u64_pair:compare_eq(yggr.lua.s64_u64_pair_type(64, 1064) ))

	print("test_pair compare_eq",
			type(s16u16_pair:compare_eq(s32u32_pair )),
			yggr.lua.typeid(s16u16_pair:compare_eq(s32u32_pair )),
			s16u16_pair:compare_eq(s32u32_pair ))
	print("test_pair compare_eq",
			type(s32u32_pair:compare_eq(s64u64_pair )),
			yggr.lua.typeid(s32u32_pair:compare_eq(s64u64_pair )),
			s32u32_pair:compare_eq(s64u64_pair ))
	print("test_pair compare_eq",
			type(s64u64_pair:compare_eq(s16u16_pair )),
			yggr.lua.typeid(s64u64_pair:compare_eq(s16u16_pair )),
			s64u64_pair:compare_eq(s16u16_pair ))

	print("\n")
	print("test_pair compare",
			type(s16u16_pair:compare(yggr.lua.s16_u16_pair_type(16, 1016) )),
			yggr.lua.typeid(s16u16_pair:compare(yggr.lua.s16_u16_pair_type(16, 1016) )),
			s16u16_pair:compare(yggr.lua.s16_u16_pair_type(16, 1016) ))
	print("test_pair compare",
			type(s32u32_pair:compare(yggr.lua.s32_u32_pair_type(32, 1032) )),
			yggr.lua.typeid(s32u32_pair:compare(yggr.lua.s32_u32_pair_type(32, 1032) )),
			s32u32_pair:compare(yggr.lua.s32_u32_pair_type(32, 1032) ))
	print("test_pair compare",
			type(s64u64_pair:compare(yggr.lua.s64_u64_pair_type(64, 1064) )),
			yggr.lua.typeid(s64u64_pair:compare(yggr.lua.s64_u64_pair_type(64, 1064) )),
			s64u64_pair:compare(yggr.lua.s64_u64_pair_type(64, 1064) ))

	print("test_pair compare",
			type(s16u16_pair:compare(yggr.lua.s16_u16_pair_type(16, 1016 - 1) )),
			yggr.lua.typeid(s16u16_pair:compare(yggr.lua.s16_u16_pair_type(16, 1016 - 1) )),
			s16u16_pair:compare(yggr.lua.s16_u16_pair_type(16, 1016 - 1) ))
	print("test_pair compare",
			type(s32u32_pair:compare(yggr.lua.s32_u32_pair_type(32, 1032 - 1) )),
			yggr.lua.typeid(s32u32_pair:compare(yggr.lua.s32_u32_pair_type(32, 1032 - 1) )),
			s32u32_pair:compare(yggr.lua.s32_u32_pair_type(32, 1032 - 1) ))
	print("test_pair compare",
			type(s64u64_pair:compare(yggr.lua.s64_u64_pair_type(64, 1064 - 1) )),
			yggr.lua.typeid(s64u64_pair:compare(yggr.lua.s64_u64_pair_type(64, 1064 - 1) )),
			s64u64_pair:compare(yggr.lua.s64_u64_pair_type(64, 1064 - 1) ))

	print("test_pair compare",
			type(s16u16_pair:compare(yggr.lua.s16_u16_pair_type(16, 1016 + 1) )),
			yggr.lua.typeid(s16u16_pair:compare(yggr.lua.s16_u16_pair_type(16, 1016 + 1) )),
			s16u16_pair:compare(yggr.lua.s16_u16_pair_type(16, 1016 + 1) ))
	print("test_pair compare",
			type(s32u32_pair:compare(yggr.lua.s32_u32_pair_type(32, 1032 + 1) )),
			yggr.lua.typeid(s32u32_pair:compare(yggr.lua.s32_u32_pair_type(32, 1032 + 1) )),
			s32u32_pair:compare(yggr.lua.s32_u32_pair_type(32, 1032 + 1) ))
	print("test_pair compare",
			type(s64u64_pair:compare(yggr.lua.s64_u64_pair_type(64, 1064 + 1) )),
			yggr.lua.typeid(s64u64_pair:compare(yggr.lua.s64_u64_pair_type(64, 1064 + 1) )),
			s64u64_pair:compare(yggr.lua.s64_u64_pair_type(64, 1064 + 1) ))

	print("\n")
	print("test_pair compare",
			type(s16u16_pair:compare(s32u32_pair )),
			yggr.lua.typeid(s16u16_pair:compare(s32u32_pair )),
			s16u16_pair:compare(s32u32_pair ))
	print("test_pair compare",
			type(s32u32_pair:compare(s64u64_pair )),
			yggr.lua.typeid(s32u32_pair:compare(s64u64_pair )),
			s32u32_pair:compare(s64u64_pair ))
	print("test_pair compare",
			type(s64u64_pair:compare(s16u16_pair )),
			yggr.lua.typeid(s64u64_pair:compare(s16u16_pair )),
			s64u64_pair:compare(s16u16_pair ))

	print("\n")
	print("test_pair operator==",
			type( s16u16_pair == yggr.lua.s16_u16_pair_type(16, 1016) ),
			yggr.lua.typeid( s16u16_pair == yggr.lua.s16_u16_pair_type(16, 1016) ),
			s16u16_pair == yggr.lua.s16_u16_pair_type(16, 1016) )
	print("test_pair operator==",
			type(s32u32_pair == yggr.lua.s32_u32_pair_type(32, 1032) ),
			yggr.lua.typeid(s32u32_pair == yggr.lua.s32_u32_pair_type(32, 1032) ),
			s32u32_pair == yggr.lua.s32_u32_pair_type(32, 1032) )
	print("test_pair operator==",
			type(s64u64_pair == yggr.lua.s64_u64_pair_type(64, 1064) ),
			yggr.lua.typeid(s64u64_pair == yggr.lua.s64_u64_pair_type(64, 1064) ),
			s64u64_pair == yggr.lua.s64_u64_pair_type(64, 1064) )

	print("test_pair operator==",
			type(s16u16_pair == s32u32_pair),
			yggr.lua.typeid(s16u16_pair == s32u32_pair),
			s16u16_pair == s32u32_pair)
	print("test_pair operator==",
			type(s32u32_pair == s64u64_pair),
			yggr.lua.typeid(s32u32_pair == s64u64_pair),
			s32u32_pair == s64u64_pair)
	print("test_pair operator==",
			type(s64u64_pair == s16u16_pair),
			yggr.lua.typeid(s64u64_pair == s16u16_pair),
			s64u64_pair == s16u16_pair)

	print("\n")
	print("test_pair operator~=",
			type( s16u16_pair ~= yggr.lua.s16_u16_pair_type(16, 1016) ),
			yggr.lua.typeid( s16u16_pair ~= yggr.lua.s16_u16_pair_type(16, 1016) ),
			s16u16_pair ~= yggr.lua.s16_u16_pair_type(16, 1016) )
	print("test_pair operator~=",
			type(s32u32_pair ~= yggr.lua.s32_u32_pair_type(32, 1032) ),
			yggr.lua.typeid(s32u32_pair ~= yggr.lua.s32_u32_pair_type(32, 1032) ),
			s32u32_pair ~= yggr.lua.s32_u32_pair_type(32, 1032) )
	print("test_pair operator~=",
			type(s64u64_pair ~= yggr.lua.s64_u64_pair_type(64, 1064) ),
			yggr.lua.typeid(s64u64_pair ~= yggr.lua.s64_u64_pair_type(64, 1064) ),
			s64u64_pair ~= yggr.lua.s64_u64_pair_type(64, 1064) )

	print("test_pair operator~=",
			type(s16u16_pair ~= s32u32_pair),
			yggr.lua.typeid(s16u16_pair ~= s32u32_pair),
			s16u16_pair ~= s32u32_pair)
	print("test_pair operator~=",
			type(s32u32_pair ~= s64u64_pair),
			yggr.lua.typeid(s32u32_pair ~= s64u64_pair),
			s32u32_pair ~= s64u64_pair)
	print("test_pair operator~=",
			type(s64u64_pair ~= s16u16_pair),
			yggr.lua.typeid(s64u64_pair ~= s16u16_pair),
			s64u64_pair ~= s16u16_pair)

	print("\n")
	print("test_pair operator<",
			type( s16u16_pair < yggr.lua.s16_u16_pair_type(16, 1016) ),
			yggr.lua.typeid( s16u16_pair < yggr.lua.s16_u16_pair_type(16, 1016) ),
			s16u16_pair < yggr.lua.s16_u16_pair_type(16, 1016) )
	print("test_pair operator<",
			type(s32u32_pair < yggr.lua.s32_u32_pair_type(32, 1032) ),
			yggr.lua.typeid(s32u32_pair < yggr.lua.s32_u32_pair_type(32, 1032) ),
			s32u32_pair < yggr.lua.s32_u32_pair_type(32, 1032) )
	print("test_pair operator<",
			type(s64u64_pair < yggr.lua.s64_u64_pair_type(64, 1064) ),
			yggr.lua.typeid(s64u64_pair < yggr.lua.s64_u64_pair_type(64, 1064) ),
			s64u64_pair < yggr.lua.s64_u64_pair_type(64, 1064) )

	print("test_pair operator<",
			type(s16u16_pair < s32u32_pair),
			yggr.lua.typeid(s16u16_pair < s32u32_pair),
			s16u16_pair < s32u32_pair)
	print("test_pair operator<",
			type(s32u32_pair < s64u64_pair),
			yggr.lua.typeid(s32u32_pair < s64u64_pair),
			s32u32_pair < s64u64_pair)
	print("test_pair operator<",
			type(s64u64_pair < s16u16_pair),
			yggr.lua.typeid(s64u64_pair < s16u16_pair),
			s64u64_pair < s16u16_pair)

	print("\n")
	print("test_pair operator<=",
			type( s16u16_pair <= yggr.lua.s16_u16_pair_type(16, 1016) ),
			yggr.lua.typeid( s16u16_pair <= yggr.lua.s16_u16_pair_type(16, 1016) ),
			s16u16_pair <= yggr.lua.s16_u16_pair_type(16, 1016) )
	print("test_pair operator<=",
			type(s32u32_pair <= yggr.lua.s32_u32_pair_type(32, 1032) ),
			yggr.lua.typeid(s32u32_pair <= yggr.lua.s32_u32_pair_type(32, 1032) ),
			s32u32_pair <= yggr.lua.s32_u32_pair_type(32, 1032) )
	print("test_pair operator<=",
			type(s64u64_pair <= yggr.lua.s64_u64_pair_type(64, 1064) ),
			yggr.lua.typeid(s64u64_pair <= yggr.lua.s64_u64_pair_type(64, 1064) ),
			s64u64_pair <= yggr.lua.s64_u64_pair_type(64, 1064) )

	print("test_pair operator<=",
			type(s16u16_pair <= s32u32_pair),
			yggr.lua.typeid(s16u16_pair <= s32u32_pair),
			s16u16_pair <= s32u32_pair)
	print("test_pair operator<=",
			type(s32u32_pair <= s64u64_pair),
			yggr.lua.typeid(s32u32_pair <= s64u64_pair),
			s32u32_pair <= s64u64_pair)
	print("test_pair operator<=",
			type(s64u64_pair <= s16u16_pair),
			yggr.lua.typeid(s64u64_pair <= s16u16_pair),
			s64u64_pair <= s16u16_pair)

	print("\n")
	print("test_pair operator>",
			type( s16u16_pair > yggr.lua.s16_u16_pair_type(16, 1016) ),
			yggr.lua.typeid( s16u16_pair > yggr.lua.s16_u16_pair_type(16, 1016) ),
			s16u16_pair > yggr.lua.s16_u16_pair_type(16, 1016) )
	print("test_pair operator>",
			type(s32u32_pair > yggr.lua.s32_u32_pair_type(32, 1032) ),
			yggr.lua.typeid(s32u32_pair > yggr.lua.s32_u32_pair_type(32, 1032) ),
			s32u32_pair > yggr.lua.s32_u32_pair_type(32, 1032) )
	print("test_pair operator>",
			type(s64u64_pair > yggr.lua.s64_u64_pair_type(64, 1064) ),
			yggr.lua.typeid(s64u64_pair > yggr.lua.s64_u64_pair_type(64, 1064) ),
			s64u64_pair > yggr.lua.s64_u64_pair_type(64, 1064) )

	print("test_pair operator>",
			type(s16u16_pair > s32u32_pair),
			yggr.lua.typeid(s16u16_pair > s32u32_pair),
			s16u16_pair > s32u32_pair)
	print("test_pair operator>",
			type(s32u32_pair > s64u64_pair),
			yggr.lua.typeid(s32u32_pair > s64u64_pair),
			s32u32_pair > s64u64_pair)
	print("test_pair operator>",
			type(s64u64_pair > s16u16_pair),
			yggr.lua.typeid(s64u64_pair > s16u16_pair),
			s64u64_pair > s16u16_pair)

	print("\n")
	print("test_pair operator>=",
			type( s16u16_pair >= yggr.lua.s16_u16_pair_type(16, 1016) ),
			yggr.lua.typeid( s16u16_pair >= yggr.lua.s16_u16_pair_type(16, 1016) ),
			s16u16_pair >= yggr.lua.s16_u16_pair_type(16, 1016) )
	print("test_pair operator>=",
			type(s32u32_pair >= yggr.lua.s32_u32_pair_type(32, 1032) ),
			yggr.lua.typeid(s32u32_pair >= yggr.lua.s32_u32_pair_type(32, 1032) ),
			s32u32_pair >= yggr.lua.s32_u32_pair_type(32, 1032) )
	print("test_pair operator>=",
			type(s64u64_pair >= yggr.lua.s64_u64_pair_type(64, 1064) ),
			yggr.lua.typeid(s64u64_pair >= yggr.lua.s64_u64_pair_type(64, 1064) ),
			s64u64_pair >= yggr.lua.s64_u64_pair_type(64, 1064) )

	print("test_pair operator>=",
			type(s16u16_pair >= s32u32_pair),
			yggr.lua.typeid(s16u16_pair >= s32u32_pair),
			s16u16_pair >= s32u32_pair)
	print("test_pair operator>=",
			type(s32u32_pair >= s64u64_pair),
			yggr.lua.typeid(s32u32_pair >= s64u64_pair),
			s32u32_pair >= s64u64_pair)
	print("test_pair operator>=",
			type(s64u64_pair >= s16u16_pair),
			yggr.lua.typeid(s64u64_pair >= s16u16_pair),
			s64u64_pair >= s16u16_pair)


	return s64u64_pair
end


function calculate(s16u16_pair, s32u32_pair, s64u64_pair)
	print("\n")
	return test_s16_u16_s32_u32_s64_u64_pair_val_or_ref(s16u16_pair, s32u32_pair, s64u64_pair)
end




