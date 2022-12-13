RegistInfo=
{
	{
		id="��ɫ";
		tabcover="res/uires/regist/rolebtn.gsa";
		cat=1;
		
		{
			id="������";
			itemid=10000;
		};
		{
			id="Сٻ";
			itemid=10001;
		};
		{
			id="С����";
			itemid=10012;
		};
	};
	{
		id="����";
		tabcover="res/uires/regist/colorbtn.gsa";
		cat=2;
	};
};

DummyRegistInfo=
{
	id="xxx";
	itemid=0;
};

function QueryRegistInfoCount(cat)
	for i=1,table.getn(RegistInfo),1 do
		local ri=RegistInfo[i];
		if ri.cat==cat then
			return table.getn(ri);
		end;
	end;
	return 0;
end;

function FindRegistInfo(cat,index)
	for i=1,table.getn(RegistInfo),1 do
		local ri=RegistInfo[i];
		if ri.cat==cat then
			return ri[index+1];
		end;
	end;
	return DummyRegistInfo;
end;