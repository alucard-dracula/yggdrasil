-- ��ӡ������Ϣ
local function __TRACKBACK__(errmsg)
    local track_text = debug.traceback(tostring(errmsg), 6)
    print("---------------------------------------- TRACKBACK ----------------------------------------")
    print(track_text, "LUA ERROR")
    print("---------------------------------------- TRACKBACK ----------------------------------------")
    local exception_text = "LUA EXCEPTION\n" .. track_text
    return false
end

--[[ ���Ե�һ��function ���function���Դ��ɱ����
��������õĺ������쳣 ����false���˳��˷�������ִ���������벢��ӡ���쳣��Ϣ��]]
local function trycall(func, ...)
	local args = { ... }
	return xpcall(function() func(unpack(args)) end, __TRACKBACK__)
end
--���Դ��룺

local function test(param)
	print("message " .. param)
	print("message " .. nil)
end

trycall(test, "test trycall")



