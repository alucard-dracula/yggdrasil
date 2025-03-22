--create_cdt.lua

require "lua_cal_svr_procotol"

genner = nil

local function init()
	if genner == nil then
		math.randomseed(os.time())
		genner = math.random;
	end
end

function create_cdt()
	init()
	return cdt_procotol_type(genner(100), genner(100))
end


