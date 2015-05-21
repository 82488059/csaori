//setlocale//
#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma setlocale("japanese")
#endif
#endif
//setlocale end//

#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

#include <new>
#include <fstream>

#include "eject.h"

//////////WINDOWS DEFINE///////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
///////////////////////////////////////////////////

/*===============================================================
	����N���X����
===============================================================*/

/*---------------------------------------------------------------
	�C���X�^���X�쐬�icsaori_base����Ă΂��j
---------------------------------------------------------------*/
CSAORIBase* CreateInstance(void)
{
	return new CEjectPlugin();
}

/*---------------------------------------------------------------
	������(DllMain����)
---------------------------------------------------------------*/
CEjectPlugin::CEjectPlugin(void) : last_tick(0)
{
}

CEjectPlugin::~CEjectPlugin()
{
}

/*---------------------------------------------------------------
	������(DllMain�Ƃ͕�)
---------------------------------------------------------------*/
bool CEjectPlugin::load()
{
	return true;
}

bool CEjectPlugin::unload(void)
{
	return true;
}

void CEjectPlugin::exec(const CSAORIInput& in,CSAORIOutput& out)
{
	out.result_code = SAORIRESULT_NO_CONTENT;

	//--------------------------------------------------------
	if ( wcsicmp(in.id.c_str(),L"OnMenuExec") == 0 ) {
		::mciSendStringA("Set CDaudio Door Open",NULL,0,NULL);
		return;
	}

}
