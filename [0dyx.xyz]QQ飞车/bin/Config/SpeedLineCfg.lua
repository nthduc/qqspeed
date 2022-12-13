
	SpeedLineParam =
	{
	LineNum = 32; --线条数量
	nMaxLineForward = 2;--前进时每个方向显示的线条数(8个方向)
	};

	Forward = 
	{
	fThetaCone = 30.0;    --圆锥顶角
	fXCone = 50.0;        --圆锥顶点的X坐标
	fXCone1 = 32.0;       --速度线开始处的基准坐标
	fXCone2 = 15.0;       --速度线结束处的基准坐标
	fLineWidth = 1.50;     --速度线的宽度
	fOffsetY = -3.0;      --屏幕竖直方向上的偏移
	fSpeed = 5.0;				--飞行的速度
	fLifeSpan = 3.0;    --基准的生命周期
	fMinDisplaySpeed = 11.0; --大于此速度，显示速度线
	};
	
	Backward = 
	{
	fThetaCone = 60.0;    --圆锥顶角
	fXCone = 50.0;        --圆锥顶点的X坐标
	fXCone1 = 35.0;       --速度线开始处的基准坐标
	fXCone2 = 10.0;       --速度线结束处的基准坐标
	fLineWidth = 2.10;     --速度线的宽度
	fOffsetY = 16.0;      --屏幕竖直方向上的偏移
	fSpeed = 2.0;				--飞行的速度
	fLifeSpan = 3.40;    --基准的生命周期
	fMinDisplaySpeed = -10.0; --大于此速度，显示速度线
	};