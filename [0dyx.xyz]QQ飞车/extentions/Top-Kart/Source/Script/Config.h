struct CameraSetting
{
	float fCameraHeight;
	float fCameraMaxDist;
	float fCameraMinDist;
	float fCameraFocusDist;
};

enum enCameraType
{
	enCameraFollow = 0,
	enCameraLookAt,
};

static CameraSetting gs_CameraSetting = 
{
	3.0f, 12.0f, 5.0f, 5.0f
};
