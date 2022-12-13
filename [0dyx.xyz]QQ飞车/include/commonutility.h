#include "windows.h"
#include "math.h"

inline FLOAT fLength2D(const Vector3& v1,const Vector3& v2)
{
	float len;
	len = 0;

	len += (v1.x - v2.x) * (v1.x - v2.x);
	len += (v1.y - v2.y) * (v1.y - v2.y);

	len = sqrt(len);

	return len;
}

inline void GetPP2D(const Vector3& v1,const Vector3 v2,Vector3& v3,float dis)
{
	dis /= fLength2D(v1,v2);

	v3.x =  v1.x + dis * (v2.x - v1.x);
	v3.y =  v1.y + dis * (v2.y - v1.y);
	v3.z =  v1.z + dis * (v2.z - v1.z);
}

inline void WriteErrorLog(const char * errortext, ...)
{
	static char error[1024];
	va_list		argptr;

	va_start( argptr, errortext );
	vsprintf( error, errortext, argptr );
	va_end( argptr );

	FILE * file = NULL;
	file = fopen("helplog/error.log", "a");

	if (file) 
	{
		fwrite(error, sizeof(char), strlen(error) + 1, file);
		fclose(file);
	}
}
