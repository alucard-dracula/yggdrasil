-- 打印错误信息
local function __TRACKBACK__(errmsg)
    local track_text = debug.traceback(tostring(errmsg), 6)
    print("---------------------------------------- TRACKBACK ----------------------------------------")
    print(track_text, "LUA ERROR")
    print("---------------------------------------- TRACKBACK ----------------------------------------")
    local exception_text = "LUA EXCEPTION\n" .. track_text
    return false
end

--[[ 尝试调一个function 这个function可以带可变参数
如果被调用的函数有异常 返回false，退出此方法继续执行其他代码并打印出异常信息；]]
local function trycall(func, ...)
	local args = { ... }
	return xpcall(function() func(unpack(args)) end, __TRACKBACK__)
end
--测试代码：

local function test(param)
	print("message " .. param)
	print("message " .. nil)
end

trycall(test, "test trycall")



