#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "csaori.h"

string_t replaceAll(string_t s, string_t f, string_t r) {
	unsigned int found = 0;
	while((found = s.find(f, found)) != std::string::npos) {
		s.replace(found, f.length(), r);
		found += r.size();
	}
	return s;
}

/*---------------------------------------------------------
	������
---------------------------------------------------------*/
bool CSAORI::load(){
	return true;
}

/*---------------------------------------------------------
	���
---------------------------------------------------------*/
bool CSAORI::unload(){
	return true;
}

/*---------------------------------------------------------
	���s
---------------------------------------------------------*/
void CSAORI::exec(const CSAORIInput& in,CSAORIOutput& out){
	string_t txt;
	out.result_code = SAORIRESULT_BAD_REQUEST;
	if ( in.args.size() < 1 ) { return; }
	if ( in.args[0].size() == 0 ) { return; }

	if ( ! ::OpenClipboard(NULL) ) {
		return;
	}

	//textcopy�d�l�ɒ����Ɂc
	out.result_code = SAORIRESULT_NO_CONTENT;
	if ( in.args.size() >= 2 ) {
		if ( wcstoul(in.args[1].c_str(),NULL,10) ) {
			out.result = in.args[0];
			out.result_code = SAORIRESULT_OK;
		}
	}

	txt = in.args[0];
	if ( in.args.size() >= 3 ) {
		txt = replaceAll(txt,in.args[2],L"\r\n");
	}

	::EmptyClipboard();

	//NT�ȏ�Ȃ�Unicode�Ή��A�łȂ����ACP�ɕϊ�
	OSVERSIONINFOEX osVer;
	ZeroMemory(&osVer,sizeof(osVer));
	osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx((OSVERSIONINFO*)&osVer);

	if ( osVer.dwPlatformId == VER_PLATFORM_WIN32_NT ) {
		HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,(txt.size() + 1) * sizeof(WORD));
		WORD *clData = (WORD*)::GlobalLock(hMem);
		wcscpy(clData,txt.c_str());
		::GlobalUnlock(hMem);

		::SetClipboardData(CF_UNICODETEXT,hMem); //hMem�͊J�����Ȃ��Ă悢
	}
	else {
		std::string astr = SAORI_FUNC::UnicodeToMultiByte(txt);

		HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,astr.size() + 1);
		char *clData = (char*)::GlobalLock(hMem);
		strcpy(clData,astr.c_str());
		::GlobalUnlock(hMem);

		::SetClipboardData(CF_TEXT,hMem); //hMem�͊J�����Ȃ��Ă悢
	}

	::CloseClipboard();
}

