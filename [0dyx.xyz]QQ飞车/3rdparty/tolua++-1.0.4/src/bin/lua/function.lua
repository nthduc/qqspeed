-- tolua: function class
-- Written by Waldemar Celes
-- TeCGraf/PUC-Rio
-- Jul 1998
-- $Id: $

-- This code is free software; you can redistribute it and/or modify it.
-- The software provided hereunder is on an "as is" basis, and
-- the author has no obligation to provide maintenance, support, updates,
-- enhancements, or modifications.



-- Function class
-- Represents a function or a class method.
-- The following fields are stored:
--  mod  = type modifiers
--  type = type
--  ptr  = "*" or "&", if representing a pointer or a reference
--  name = name
--  lname = lua name
--  args  = list of argument declarations
--  const = if it is a method receiving a const "this".
classFunction = {
 mod = '',
 type = '',
 ptr = '',
 name = '',
 args = {n=0},
 const = '',
}
classFunction.__index = classFunction
setmetatable(classFunction,classFeature)

-- declare tags
function classFunction:decltype ()
 self.type = typevar(self.type)
 if strfind(self.mod,'const') then
	 self.type = 'const '..self.type
		self.mod = gsub(self.mod,'const%s*','')
	end
 local i=1
 while self.args[i] do
  self.args[i]:decltype()
  i = i+1
 end
end


-- Write binding function
-- Outputs C/C++ binding function.
function classFunction:supcode (local_constructor)
 local overload = strsub(self.cname,-2,-1) - 1  -- indicate overloaded func
 local nret = 0      -- number of returned values
 local class = self:inclass()
 local _,_,static = strfind(self.mod,'^%s*(static)')
 if class then
 	if local_constructor then
		output("/* method: new_local of class ",class," */")
	else
		output("/* method:",self.name," of class ",class," */")
	end
 else
  output("/* function:",self.name," */")
 end

 if local_constructor then
	 output("static int",self.cname.."_local","(lua_State* tolua_S)")
 else
	 output("static int",self.cname,"(lua_State* tolua_S)")
 end
 output("{")

 -- check types
	if overload < 0 then
	 output('#ifndef TOLUA_RELEASE\n')
	end
	output(' tolua_Error tolua_err;')
 output(' if (\n')
 -- check self
 local narg
 if class then narg=2 else narg=1 end
 if class then
		local func = 'tolua_isusertype'
		local type = self.parent.type
		if self.name=='new' or static~=nil then
			func = 'tolua_isusertable'
			type = self.parent.type
		end
		if self.const ~= '' then
			type = "const "..type
		end
		output('     !'..func..'(tolua_S,1,"'..type..'",0,&tolua_err) ||\n')
 end
 -- check args
 if self.args[1].type ~= 'void' then
  local i=1
  while self.args[i] do
   local btype = isbasic(self.args[i].type)
   if btype ~= 'value' and btype ~= 'state' then
    output('     !'..self.args[i]:outchecktype(narg)..' ||\n')
   end
   if btype ~= 'state' then
	   narg = narg+1
   end
   i = i+1
  end
 end
 -- check end of list
 output('     !tolua_isnoobj(tolua_S,'..narg..',&tolua_err)\n )')
	output('  goto tolua_lerror;')

 output(' else\n')
	if overload < 0 then
	 output('#endif\n')
	end
	output(' {')

 -- declare self, if the case
 local narg
 if class then narg=2 else narg=1 end
 if class and self.name~='new' and static==nil then
  output(' ',self.const,self.parent.type,'*','self = ')
  output('(',self.const,self.parent.type,'*) ')
  output('tolua_tousertype(tolua_S,1,0);')
 elseif static then
  _,_,self.mod = strfind(self.mod,'^%s*static%s%s*(.*)')
 end
 -- declare parameters
 if self.args[1].type ~= 'void' then
  local i=1
  while self.args[i] do
   self.args[i]:declare(narg)
   if isbasic(self.args[i].type) ~= "state" then
	   narg = narg+1
   end
   i = i+1
  end
 end

 -- check self
 if class and self.name~='new' and static==nil then
	 output('#ifndef TOLUA_RELEASE\n')
  output('  if (!self) tolua_error(tolua_S,"invalid \'self\' in function \''..self.name..'\'",NULL);');
	 output('#endif\n')
 end

 -- get array element values
 if class then narg=2 else narg=1 end
 if self.args[1].type ~= 'void' then
  local i=1
  while self.args[i] do
   self.args[i]:getarray(narg)
   narg = narg+1
   i = i+1
  end
 end

 -- call function
 if class and self.name=='delete' then
  output('  delete self;')
 elseif class and self.name == 'operator&[]' then
  if flags['1'] then -- for compatibility with tolua5 ?
	output('  self->operator[](',self.args[1].name,'-1) = ',self.args[2].name,';')
  else
    output('  self->operator[](',self.args[1].name,') = ',self.args[2].name,';')
  end
 else
  output('  {')
  if self.type ~= '' and self.type ~= 'void' then
   output('  ',self.mod,self.type,self.ptr,'tolua_ret = ')
   output('(',self.mod,self.type,self.ptr,') ')
  else
   output('  ')
  end
  if class and self.name=='new' then
   output('new',self.type,'(')
  elseif class and static then
   output(class..'::'..self.name,'(')
  elseif class then
   output('self->'..self.name,'(')
  else
   output(self.name,'(')
  end

  -- write parameters
  local i=1
  while self.args[i] do
   self.args[i]:passpar()
   i = i+1
   if self.args[i] then
    output(',')
   end
  end

  if class and self.name == 'operator[]' and flags['1'] then
	output('-1);')
  else
	output(');')
  end

  -- return values
  if self.type ~= '' and self.type ~= 'void' then
   nret = nret + 1
   local t,ct = isbasic(self.type)
   if t then
    output('   tolua_push'..t..'(tolua_S,(',ct,')tolua_ret);')
   else
			 t = self.type
			 new_t = string.gsub(t, "const%s+", "")
    if self.ptr == '' then
     output('   {')
     output('#ifdef __cplusplus\n')
     output('    void* tolua_obj = new',new_t,'(tolua_ret);')
     output('    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"',t,'");')
     output('#else\n')
     output('    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(',t,'));')
     output('    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"',t,'");')
     output('#endif\n')
     output('   }')
    elseif self.ptr == '&' then
     output('   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"',t,'");')
    else
    	if local_constructor then
	  output('   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"',t,'");')
    	else
		     output('   tolua_pushusertype(tolua_S,(void*)tolua_ret,"',t,'");')
	    end
    end
   end
  end
  local i=1
  while self.args[i] do
   nret = nret + self.args[i]:retvalue()
   i = i+1
  end
  output('  }')

  -- set array element values
  if class then narg=2 else narg=1 end
  if self.args[1].type ~= 'void' then
   local i=1
   while self.args[i] do
    self.args[i]:setarray(narg)
    narg = narg+1
    i = i+1
   end
  end

  -- free dynamically allocated array
  if self.args[1].type ~= 'void' then
   local i=1
   while self.args[i] do
    self.args[i]:freearray()
    i = i+1
   end
  end
 end

 output(' }')
 output(' return '..nret..';')

 -- call overloaded function or generate error
	if overload < 0 then

		output('#ifndef TOLUA_RELEASE\n')
		output('tolua_lerror:\n')
		output(' tolua_error(tolua_S,"#ferror in function \''..self.lname..'\'.",&tolua_err);')
		output(' return 0;')
		output('#endif\n')
	else
		local _local = ""
		if local_constructor then
			_local = "_local"
		end
		output('tolua_lerror:\n')
		output(' return '..strsub(self.cname,1,-3)..format("%02d",overload).._local..'(tolua_S);')
	end
 output('}')
 output('\n')

	-- recursive call to write local constructor
	if class and self.name=='new' and not local_constructor then

		self:supcode(1)
	end

end


-- register function
function classFunction:register ()
 output(' tolua_function(tolua_S,"'..self.lname..'",'..self.cname..');')
  if self.name == 'new' then
	  output(' tolua_function(tolua_S,"new_local",'..self.cname..'_local);')
	  output(' tolua_set_call_event(tolua_S,'..self.cname..'_local);')
  end
end

-- Print method
function classFunction:print (ident,close)
 print(ident.."Function{")
 print(ident.." mod  = '"..self.mod.."',")
 print(ident.." type = '"..self.type.."',")
 print(ident.." ptr  = '"..self.ptr.."',")
 print(ident.." name = '"..self.name.."',")
 print(ident.." lname = '"..self.lname.."',")
 print(ident.." const = '"..self.const.."',")
 print(ident.." cname = '"..self.cname.."',")
 print(ident.." lname = '"..self.lname.."',")
 print(ident.." args = {")
 local i=1
 while self.args[i] do
  self.args[i]:print(ident.."  ",",")
  i = i+1
 end
 print(ident.." }")
 print(ident.."}"..close)
end

-- check if it returns an object by value
function classFunction:requirecollection (t)
	local r = false
	if self.type ~= '' and not isbasic(self.type) and self.ptr=='' then
		local type = gsub(self.type,"%s*const%s*","")
	 t[type] = "tolua_collect_" .. clean_template(type)
	 r = true
	end
	local i=1
	while self.args[i] do
		r = self.args[i]:requirecollection(t) or r
		i = i+1
	end
	return r
end

-- determine lua function name overload
function classFunction:overload ()
 return self.parent:overload(self.lname)
end



-- Internal constructor
function _Function (t)
 setmetatable(t,classFunction)

 if t.const ~= 'const' and t.const ~= '' then
  error("#invalid 'const' specification")
 end

 append(t)
 if t:inclass() then
 --print ('t.name is '..t.name..', parent.name is '..t.parent.name)
  if string.gsub(t.name, "%b<>", "") == string.gsub(t.parent.original_name or t.parent.name, "%b<>", "") then
   t.name = 'new'
   t.lname = 'new'
   t.parent._new = true
   t.type = t.parent.name
   t.ptr = '*'
  elseif string.gsub(t.name, "%b<>", "") == '~'..string.gsub(t.parent.original_name or t.parent.name, "%b<>", "") then
   t.name = 'delete'
   t.lname = 'delete'
   t.parent._delete = true
  end
 end
 t.cname = t:cfuncname("tolua")..t:overload(t)
 return t
end

-- Constructor
-- Expects three strings: one representing the function declaration,
-- another representing the argument list, and the third representing
-- the "const" or empty string.
function Function (d,a,c)
 --local t = split(strsub(a,2,-2),',') -- eliminate braces
 --local t = split_params(strsub(a,2,-2))
 local i=1
 local l = {n=0}

 	a = string.gsub(a, "%s*([%(%)])%s*", "%1")
	local t,strip,last = strip_pars(strsub(a,2,-2));
	if strip then
		local ns = string.sub(strsub(a,1,-2), 1, -(string.len(last)+1))
		ns = string.gsub(ns, "%s*,%s*$", "")..')'
		Function(d, ns, c)
	end

 while t[i] do
  l.n = l.n+1
  l[l.n] = Declaration(t[i],'var',true)
  i = i+1
 end
 local f = Declaration(d,'func')
 f.args = l
 f.const = c
 return _Function(f)
end

function strip_pars(s)

	local t = split_c_tokens(s, ',')
	local strip = false
	local last

	for i=1,t.n do

		if not strip and param_object(t[i]) then
			strip = true
		end
		if strip then
			last = t[i]
			t[i] = string.gsub(t[i], "=.*$", "")
		end
	end

	return t,strip,last

end

function param_object(par) -- returns true if the parameter has an object as its default value

	if not string.find(par, '=') then return false end -- it has no default value

	if string.find(par, "%*") then -- it's a pointer with a default value

		if string.find(par, '=%s*new') then -- it's a pointer with an instance as default parameter.. is that valid?
			return true
		end
		return false -- default value is 'NULL' or something
	end


	if string.find(par, "[%(&]") then
		return true

	end -- it's a reference, or default value is a constructor call (most likely both if it's a reference)

	return false -- ?
end

function strip_last_arg(all_args, last_arg) -- strips the default value from the last argument

	local _,_,s_arg = string.find(last_arg, "^([^=]+)")
	last_arg = string.gsub(last_arg, "([%%%(%)])", "%%%1");
	all_args = string.gsub(all_args, "%s*,%s*"..last_arg.."%s*%)%s*$", ")")
	return all_args, s_arg
end

