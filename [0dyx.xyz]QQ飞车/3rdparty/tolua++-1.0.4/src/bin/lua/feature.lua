-- tolua: abstract feature class
-- Written by Waldemar Celes
-- TeCGraf/PUC-Rio
-- Jul 1998
-- $Id: $

-- This code is free software; you can redistribute it and/or modify it.
-- The software provided hereunder is on an "as is" basis, and
-- the author has no obligation to provide maintenance, support, updates,
-- enhancements, or modifications. 


-- Feature class
-- Represents the base class of all mapped feature.
classFeature = {
}
classFeature.__index = classFeature

-- write support code
function classFeature:supcode ()
end

-- output tag
function classFeature:decltype ()
end

-- register feature
function classFeature:register ()
end

-- translate verbatim
function classFeature:preamble ()
end

-- check if it is a variable
function classFeature:isvariable ()
 return false
end

-- checi if it requires collection
function classFeature:requirecollection (t)
 return false
end

-- build names
function classFeature:buildnames ()
 if self.name and self.name~='' then
  local n = split(self.name,'@')
  self.name = n[1]
  if not n[2] then
   n[2] = applyrenaming(n[1])
  end
  self.lname = n[2] or gsub(n[1],"%[.-%]","")
  self.original_name = self.name
  self.lname = clean_template(self.lname)
 end
 if not self.is_parameter then
	 self.name = getonlynamespace() .. self.name
 end
end

function clean_template(t)

	return string.gsub(t, "[<>:, %*]", "_")
end

-- check if feature is inside a container definition
-- it returns the container class name or nil.
function classFeature:incontainer (which)
 if self.parent then
  local parent = self.parent
  while parent do
   if parent.classtype == which then
    return parent.name
   end
   parent = parent.parent
  end
 end
 return nil
end

function classFeature:inclass ()
 return self:incontainer('class')
end

function classFeature:inmodule ()
 return self:incontainer('module')
end

function classFeature:innamespace ()
 return self:incontainer('namespace')
end

-- return C binding function name based on name
-- the client specifies a prefix
function classFeature:cfuncname (n)

 if self.parent then
  n = self.parent:cfuncname(n)
 end

  n = string.gsub(n..'_'.. (self.lname or self.name), "[<>:, \.%*]", "_")

  return n
end

