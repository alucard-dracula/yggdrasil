
require "lua_hello_dll"
function calculate(param)
	b = param + 100
	param = param + 200
	--print(param, b)
	return b
end
