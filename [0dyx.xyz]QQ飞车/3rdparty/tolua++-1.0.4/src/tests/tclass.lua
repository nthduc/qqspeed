function hello()

	print("hello world")
end

rawset(Test.B, "hello", hello)

-- type convertion tests
print(Test.A)
assert(tolua.type(Test.A.last) == 'Test::Tst_A') -- first time the object is mapped
assert(tolua.type(Test.B.last) == 'Test::Tst_B') -- type convertion to specialized type
assert(tolua.type(Test.A.last) == 'Test::Tst_B') -- no convertion: obj already mapped as B


local a = Test.A:new()
assert(tolua.type(Test.A.last) == 'Test::Tst_A') -- no type convertion: same type
local b = Test.B:new()
assert(tolua.type(Test.A.last) == 'Test::Tst_B') -- no convertion: obj already mapped as B
local c = Test.luaC:new(0)
assert(tolua.type(Test.A.last) == 'Test::Tst_C') -- no convertion: obj already mapped as C
assert(tolua.type(Test.luaC.last) == 'Test::Tst_C') 

local aa = Test.A.AA:new()
local bb = Test.A.BB:new()
local xx = Test.create_aa()

-- method calling tests
assert(a:a() == 'A')
assert(b:a() == 'A')
assert(b:b() == 'B')
assert(c:a() == 'A')
assert(c:b() == 'B')
assert(c:c() == 'C')
assert(aa:aa() == 'AA')
assert(bb:aa() == bb:Base():aa())
assert(xx:aa() == 'AA')
assert(Test.is_aa(bb) == true)

-- test ownershipping handling
-- should delete objects: 6 7 8 9 10 (it may vary!)
_ = [[
local set = {}
for i=1,10 do
 local c = Test.luaC:new(i)
	if i>5 then
		tolua.takeownership(c)
	end
	set[i] = c
end
 
for i=1,5 do
 tolua.releaseownership(set[i])
end
--]]


e = Test.E(3)
ae = e.a

print("e is type "..tolua.type(e))
print("ae is type "..tolua.type(ae))

--e:delete()

b:hello()

print("Class test OK")
