

local function test_s16_u16_s32_u32_s64_u64_pair_cref(s16u16_pair, s32u32_pair, s64u64_pair)
	print("test_pair show", type(s16u16_pair), yggr.lua.typeid(s16u16_pair), s16u16_pair)
	print("test_pair show", type(s32u32_pair), yggr.lua.typeid(s32u32_pair), s32u32_pair)
	print("test_pair show", type(s64u64_pair), yggr.lua.typeid(s64u64_pair), s64u64_pair)


	print("\n")
	print("test_pair get_first", type(s16u16_pair:get_first()), yggr.lua.typeid(s16u16_pair:get_first()), s16u16_pair:get_first())
	print("test_pair get_first", type(s32u32_pair:get_first()), yggr.lua.typeid(s32u32_pair:get_first()), s32u32_pair:get_first())
	print("test_pair get_first", type(s64u64_pair:get_first()), yggr.lua.typeid(s64u64_pair:get_first()), s64u64_pair:get_first())

	print("\n")
	print("test_pair get_second", type(s16u16_pair:get_second()), yggr.lua.typeid(s16u16_pair:get_second()), s16u16_pair:get_second())
	print("test_pair get_second", type(s32u32_pair:get_second()), yggr.lua.typeid(s32u32_pair:get_second()), s32u32_pair:get_second())
	print("test_pair get_second", type(s64u64_pair:get_second()), yggr.lua.typeid(s64u64_pair:get_second()), s64u64_pair:get_second())

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
	return test_s16_u16_s32_u32_s64_u64_pair_cref(s16u16_pair, s32u32_pair, s64u64_pair)
end




