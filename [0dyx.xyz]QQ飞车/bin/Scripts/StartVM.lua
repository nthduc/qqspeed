print(">>>>Start StartVM.lua >>>>")
print("Starting	LUA	Vitual Machine...")

-- some	libraries, set a user data fields using	a table
function set_userdata(target, source)
	for	x in source	do
		if type(source[x]) == "table" then
			set_userdata(target[x],	source[x])
		elseif type(x) == "number" then
			target[x-1]	= source[x]
		else 
			target[x] =	source[x]
		end
	end
end

copy_table = set_userdata


print("<<<<Finish startVM.LUA <<<<")
