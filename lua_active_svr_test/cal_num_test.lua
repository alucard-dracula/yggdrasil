
require "lua_cal_svr_procotol"

function cal(params)
	return rst_procotol_type(params:get_num1() + params:get_num2())
end

