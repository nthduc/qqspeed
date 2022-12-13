GPSettings.nearDistance=10;
GPSettings.nearAngel=0.18;
GPSettings.farDistance=10;
GPSettings.farAngel=0.20;
GPSettings.exchangetime=1000;
GPSettings.aimernode=34;
GPSettings.aimednode=35;
GPSettings.aimboardx=1;
GPSettings.aimedboardx=1;
GPSettings.aimoffsetx=0.0;	--瞄准境中心点偏移值
GPSettings.aimoffsety=-0.6;
GPSettings.aimoffsetz=0.70;
GPSettings.aimedmex=0;	--身上显示被瞄准标志的位置
GPSettings.aimedmey=0.5;
GPSettings.aimedmez=0.4;
GPSettings.aimedoffsetx=0.0;	--瞄准目标中心点偏移值
GPSettings.aimedoffsety=0.0;
GPSettings.aimedoffsetz=0.4;
GPSettings.aimeddistance=-0.4;
GPSettings.aimrotspeed=1;
GPSettings.aimscalespeed=10;
GPSettings.gottenshownode=19;
GPSettings.throwanimation=0;
GPSettings.watchcameraspeed=0.4;
GPSettings.eatsound=123;
GPSettings.eatensound=122;
GPSettings.gottendelay=0.4;

--道路上放置的静态道具盒参数------------------
--道具点默认的物理碰撞球体半径
GPSettings.gpPropPointRadius=0.3;
--静态道具盒显示位置相对于地面浮起的高度
GPSettings.gpStaticBoxFloatToGroud=0.3;

--磁铁道具参数------------------
--磁铁固定加速力，直接作用于使用磁铁的玩家
GPSettings.gpMagnetAcceForce=20.0;
--被施用磁铁玩家所受力的调整系数，即被使用磁铁玩家受力=gpMagnetAcceForce*gpMagnetTargetModifyParam
GPSettings.gpMagnetTargetModifyParam=0.2;
--磁铁作用期间对侧向摩擦力的减弱系数，以加大侧滑效果，必须>0.1 且 <1.0
GPSettings.gpMagnetSlackSlipFriction=0.3;

--导弹道具参数------------------
--导弹飞行初始状态时相对于地面浮起的高度
GPSettings.gpMissileFloatToGround=1.2;
--导弹螺旋飞行运动时的初始半径
GPSettings.gpMissileHelixRadius=0.001;
--弹螺旋飞行总圈数
GPSettings.gpMissileHelixCircleCount=5;

--酷比炸弹道具参数------------------
--酷比炸弹飞行初始状态时相对于地面浮起的高度
GPSettings.gpFlyingFloatToGround=1.0;
--螺旋平面运动最大限制半径
GPSettings.gpFlyingHelixMaxRadius=2.1;
--螺旋平面运动最小限制半径，必须<=gpFlyingHelixMaxRadius
GPSettings.gpFlyingHelixMinRadius=1.0;
--螺旋平面单侧旋转最大弧度(参考:Pi=3.141592653589793)
GPSettings.gpFlyingHelixSideMaxAngle=1.7;
--螺旋平面单侧旋转最小弧度，必须<=gpFlyingHelixSideMaxAngle(参考:Pi=3.141592653589793)
GPSettings.gpFlyingHelixSideMinAngle=0.314;
--总的转折飞行的最大限制次数(必须是整数)，必须>=2.0
GPSettings.gpFlyingTurnMaxCount=4.0;
--总的转折飞行的最小限制次数(必须是整数)，必须>=2.0 同时 <=gpFlyingTurnMaxCount
GPSettings.gpFlyingTurnMinCount=2.0;