--[[
avatarscat=
{
	{
		id="��ɫ";
		tabcover="res/uires/avatarsel/role.gsa";
		cat=3;
	};
	{
		id="����";
		tabcover="res/uires/avatarsel/car.gsa";
		cat=2;
	};
	{
		id="װ������",
		tabcover = "res/uires/avatarsel/item.gsa",
		cat = 4;
	};
	{
		id="װ��";
		tabcover="res/uires/avatarsel/adorn.gsa";
		cat=1;
	};

	
};

avatars=
{
	{
		id="�羵";
		icon="res/uires/avatarsel/256/10004.gsa";
		cat=1;
		avatarid=10004;
	};
	{
		id="ī��";
		icon="res/uires/avatarsel/256/10005.gsa";
		cat=1;
		avatarid=10005;
	};
	{
		id="è��";
		icon="res/uires/avatarsel/256/10006.gsa";
		cat=1;
		avatarid=10006;
	};
	{
		id="�ö�";
		icon="res/uires/avatarsel/256/10007.gsa";
		cat=1;
		avatarid=10007;
	};
	{
		id="��ɫ����";
		icon="res/uires/avatarsel/512/10008.gsa";
		cat=1;
		avatarid=10008;
	};
	{
		id="QQ����";
		icon="res/uires/avatarsel/512/10009.gsa";
		cat=1;
		avatarid=10009;
	};
	{
		id="QQ����";
		icon="res/uires/avatarsel/512/10010.gsa";
		cat=1;
		avatarid=10010;
	};
	{
		id="Love����";
		icon="res/uires/avatarsel/512/10011.gsa";
		cat=1;
		avatarid=10011;
	};	
};
]]--
DummyMap=
{
	id="�Ҳ���װ��";
	icon="";
	cat=0;
	avatarid=0;
};

function QueryAvatarCount(cat)
	local count=0;
	--test----------------------
	if cat==1 then --��ʱֻ����װ��
		--local num,items=GetAvatarAllList(GetMyInfo().uin);
		count=GetAvatarAllList(GetMyInfo().uin).itemnum;
	end;
	--[[
	if cat == 0 then	--1��ʾȫ����ͼ
		for i=1,table.getn(avatars),1 do
			local t = avatars[i];
			if t.cat >= cat then
				count = count+1;
			end;
		end;
	end;
	if cat ~= 0 then
		for i=1,table.getn(avatars),1 do
			local t = avatars[i];
			if t.cat==cat then
				count = count+1;
			end;
		end;
	end;
	]]
	return count;
end;

function FetchAvatars(cat)
	local count=0;
	local newavatars={};
	if cat == 0 then
		for i=1,table.getn(avatars),1 do
			local t = avatars[i];
			if t.cat >= cat then
				count = count+1;
				table.insert(newavatars,t);
			end;
		end;
	end;
	if cat ~= 0 then
		--[[for i=1,table.getn(avatars),1 do
			local t = avatars[i];
			if t.cat==cat then
				count = count+1;
				table.insert(newavatars,t);
			end;
		end;
		]]
		local t;
		local num=GetAvatarAllList(GetMyInfo().uin).itemnum;
		for i=1,num,1 do
		t=FindAvatar(items[i].itemid);
		table.insert(newavatars,t);
		end;
	end;
	return newavatars, count;
end;


function FindAvatar(mapid)
	for i=1,table.getn(avatars),1 do
		local t = avatars[i];
		if t.avatarid==mapid then
			return t;
		end;
	end;	
	return DummyMap;
end;

function FindAvatarByCatIndex(cat, index)
	local count=0;
	local newavatars={};
	if cat == 1 then
		for i=1,table.getn(avatars),1 do
			local t = avatars[i];
			if t.cat >= cat then
				if index == count then
					return t;
				end;
				count = count+1;
			end;
		end;
	end;
	if cat ~= 1 then
		for i=1,table.getn(avatars),1 do
			local t = avatars[i];
			if t.cat==cat then
				if index == count then
					return t;
				end;
				count = count+1;
			end;
		end;	
	end;
	return DummyAvatar;
end;