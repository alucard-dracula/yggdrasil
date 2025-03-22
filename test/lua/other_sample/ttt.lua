

local function foo(la)
	local len = la:len()
	return len
end


foo("10")
g_a = 100
len = 100
print(g_a)

a = foo
print(type(a))
print(a("100"))

k = 0
if k == 0 then
	print("1", k)
else
	print("2", k)
end


dict = {["a"] = 1, ["b"] = 2, ["c"] = 3}
print(dict.a)
print(dict["a"])
array = {1, 2, 3}

print(array[1], #array)

function swap(a, b)
	return b, a
end

x = 100
y = 200

--x, y = swap(x, y)
x, y = y, x

print("xy", x, y)

g_foo = foo

print(g_foo("100"))
print(_G.g_foo("100"))
print(_G["g_foo"]("100"))

print("------------------------")
for k, v in pairs(_G) do
	print(k, v)
end
