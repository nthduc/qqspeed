-- tolua: class class
-- Written by Waldemar Celes
-- TeCGraf/PUC-Rio
-- Jul 1998
-- $Id: $

-- This code is free software; you can redistribute it and/or modify it.
-- The software provided hereunder is on an "as is" basis, and
-- the author has no obligation to provide maintenance, support, updates,
-- enhancements, or modifications.


-- Class class
-- Represents a class definition.
-- Stores the following fields:
--    name = class name
--    base = class base, if any (only single inheritance is supported)
--    {i}  = list of members
classClass = {
 classtype = 'class',
 name = '',
 base = '',
 type = '',
 btype = '',
 ctype = '',
}
classClass.__index = classClass
setmetatable(classClass,classContainer)


-- register class
function classClass:register ()
 push(self)
	if _collect[self.type] then
		output('#ifdef __cplusplus\n')
		output(' tolua_cclass(tolua_S,"'..self.lname..'","'..self.type..'","'..self.btype..'",'.._collect[self.type]..');')
		output('#else\n')
		output(' tolua_cclass(tolua_S,"'..self.lname..'","'..self.type..'","'..self.btype..'",NULL);')
		output('#endif\n')
	else
		output(' tolua_cclass(tolua_S,"'..self.lname..'","'..self.type..'","'..self.btype..'",NULL);')
	end
	if self.extra_bases then
		for k,base in ipairs(self.extra_bases) do
			-- not now
			--output(' tolua_addbase(tolua_S, "'..self.type..'", "'..base..'");')
		end
	end
	output(' tolua_beginmodule(tolua_S,"'..self.lname..'");')
 local i=1
 while self[i] do
  self[i]:register()
  i = i+1
 end
	output(' tolua_endmodule(tolua_S);')
	pop()
end

-- return collection requirement
function classClass:requirecollection (t)
 push(self)
	local r = false
 local i=1
 while self[i] do
  r = self[i]:requirecollection(t) or r
  i = i+1
 end
	pop()
	-- only class that exports destructor can be appropriately collected
	-- classes that export constructors need to have a collector
	if self._delete or self._new then
  --t[self.type] = "tolua_collect_" .. gsub(self.type,"::","_")
  t[self.type] = "tolua_collect_" .. clean_template(self.type)
		r = true
	end
 return r
end

-- output tags
function classClass:decltype ()
 push(self)
	self.type = regtype(self.original_name or self.name)
	self.btype = typevar(self.base)
	self.ctype = 'const '..self.type
	if self.extra_bases then
		for i=1,table.getn(self.extra_bases) do
			self.extra_bases[i] = typevar(self.extra_bases[i])
		end
	end
 local i=1
 while self[i] do
  self[i]:decltype()
  i = i+1
 end
	pop()
end


-- Print method
function classClass:print (ident,close)
 print(ident.."Class{")
 print(ident.." name = '"..self.name.."',")
 print(ident.." base = '"..self.base.."';")
 print(ident.." lname = '"..self.lname.."',")
 print(ident.." type = '"..self.type.."',")
 print(ident.." btype = '"..self.btype.."',")
 print(ident.." ctype = '"..self.ctype.."',")
 local i=1
 while self[i] do
  self[i]:print(ident.." ",",")
  i = i+1
 end
 print(ident.."}"..close)
end

-- Internal constructor
function _Class (t)
 setmetatable(t,classClass)
 t:buildnames()
 append(t)
 return t
end

-- Constructor
-- Expects the name, the base (array) and the body of the class.
function Class (n,p,b)

	if table.getn(p) > 1 then
		b = string.sub(b, 1, -2)
		for i=2,table.getn(p),1 do
			b = b.."\n tolua_inherits "..p[i].." __"..p[i].."__;\n"
		end
		b = b.."\n}"
	end

	-- check for template
	local t,_,T,I = string.find(b, "TEMPLATE_BIND%s*%(+%s*\"?([^\",]*)\"?%s*,%s*([^%)]*)%s*%)+")
	if t then

		-- remove quotes
		I = string.gsub(I, "\"", "")
		T = string.gsub(T, "\"", "")
		-- get type list
		local types = split(I, ",")
		-- remove TEMPLATE_BIND line
		local bs = string.gsub(b, "\n[^\n]*TEMPLATE_BIND[^\n]*\n", "\n")

		-- replace
		for i =1 , types.n do

			local Tl = split(T, " ")
			local Il = split(types[i], " ")
			local bI = bs
			local pI = {}
			for j = 1,Tl.n do
				Tl[j] = findtype(Tl[j]) or Tl[j]
				bI = string.gsub(bI, "([^_%w])"..Tl[j].."([^_%w])", "%1"..Il[j].."%2")
				if p then
					for i=1,table.getn(p) do
						pI[i] = string.gsub(p[i], "([^_%w]?)"..Tl[j].."([^_%w]?)", "%1"..Il[j].."%2")
					end
				end
			end
			local append = "<"..string.gsub(types[i], "%s+", ",")..">"
			append = string.gsub(append, ">>", "> >")
			for i=1,table.getn(pI) do
				pI[i] = string.gsub(pI[i], ">>", "> >")
			end
			bI = string.gsub(bI, ">>", "> >")

			Class(n..append, pI, bI)
		end
		return
	end
	local mbase
	if p then
		mbase = table.remove(p, 1)
		if not p[1] then p = nil end
	end
 local c = _Class(_Container{name=n, base=mbase, extra_bases=p})

 local ft = getnamespace(c.parent)..c.original_name
 append_global_type(ft, c)

 push(c)
 c:parse(strsub(b,2,strlen(b)-1)) -- eliminate braces
 pop()
end

