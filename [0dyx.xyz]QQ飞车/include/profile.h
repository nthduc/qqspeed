
#ifndef __PROFILE_H__
#define __PROFILE_H__

namespace Profile
{
	void BeginFrame();
	void Begin( const char *name );
	void End();
	void DumpToBuffer( char *buffer, int maxbuf );

	class CAutoRecord
	{
	public:
		__forceinline CAutoRecord( const char *name )
		{
			Begin(name);
		}

		__forceinline ~CAutoRecord()
		{
			End();
		}
	};
}//namespace Profile


#endif //__PROFILE_H__