--yggr_lua_test1.lua

require("include_class_env")


print("--------------------------class_def_test---------------------")

namespace("aaa")
	:namespace("bbb")
		:class("ccc")
			:member("ma", 100)

			:member("mfoo1",
					function(self, a, b)
						return a+b
					end
					)

			:static_member("static_mfoo1",
					function(left, right)
						return left + right
					end
					)

			:static_member("sma", 1000)

			:operator("+",
					function(self, right)
						local obj = self.__this_type:new({ma = (self.ma + right.ma)})
						return obj;
					end
					)

			:member("mb", 200)

		:def_end()

		:class("ddd"):externs("ccc")
			:member("da", 500)
			:member("set_ma",
					function (self, a)
						self.supper.ma = a
						self.supper.mfoo1 = a -- error not set function
					end
					)
		:def_end()

		:namespace("eee")
			:namespace("fff")

			:def_end()
		:def_end()
	:def_end()
:def_end()

print("------------------new_test-----------------------")

test_class_ccc = aaa.bbb.ccc:new({ ma = 100})
test_class_ddd = aaa.bbb.ddd:new({ da = 200, ma = 300 })

print((test_class_ccc + test_class_ddd).ma)

print("------------------out_test-----------------------")
print("test", test_class_ccc.ma)
print("test", test_class_ccc.mb)
print("test", test_class_ccc.da)
print("test", test_class_ccc:mfoo1(10, 20))
print("test", test_class_ccc.static.static_mfoo1(20, 20))
print("test", test_class_ccc.static.sma)

test_class_ddd.ma = 2000
print("test", test_class_ddd.ma)
print("test", test_class_ddd.mb)
print("test", test_class_ddd.da)
print("test", test_class_ddd:mfoo1(10, 20))
print("test", test_class_ddd.static.static_mfoo1(20, 20))
print("test", test_class_ddd.static.sma)

print("------------------operator_test-----------------------")

test_class_ccc2 = test_class_ccc + test_class_ddd
print("test",ctype(test_class_ccc2), test_class_ccc2.ma)

test_class_ddd2 = test_class_ddd + test_class_ccc
print("test", ctype(test_class_ddd2), test_class_ddd2.ma)

test_class_ccc3 = test_class_ccc + test_class_ccc
print("test",ctype(test_class_ccc3), test_class_ccc3.ma)

test_class_ddd3 = test_class_ccc + test_class_ddd
print("test",ctype(test_class_ddd3), test_class_ddd3.ma)


print("------------------other_test-----------------------")

print("test", ctype(test_class_ccc), test_class_ccc.supper)
print("test", ctype(test_class_ddd), test_class_ddd.supper)

test_class_ddd:set_ma(99999)
test_class_ddd:set_ma("888888")
test_class_ddd:set_ma(function ()end)
print(test_class_ddd.ma)
