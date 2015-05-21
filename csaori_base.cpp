//setlocale//
#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma setlocale("japanese")
#endif
#endif
//setlocale end//

/*
 * csaori_base.cpp
 */

#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#pragma warning( disable : 4996 )
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <locale>
#include <process.h>

#include "csaori_base.h"

//////////WINDOWS DEFINE///////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
///////////////////////////////////////////////////

//global CSAORIBase object
CSAORIBase* pSaori;
HINSTANCE g_hModule;

extern CSAORIBase* CreateInstance(void);

//------------------------------------------------------------------------------
// Utility functions
//------------------------------------------------------------------------------
namespace SAORI_FUNC{
	template<> string_t numToString<unsigned char>(unsigned char num){
		char_t buf[32];
		swprintf(buf,L"%u",num);
		return buf;
	}
	template<> string_t numToString<unsigned int>(unsigned int num){
		char_t buf[32];
		swprintf(buf,L"%u",num);
		return buf;
	}
	template<> string_t numToString<unsigned long>(unsigned long num){
		char_t buf[32];
		swprintf(buf,L"%u",num);
		return buf;
	}

	std::string UnicodeToMultiByte(const wchar_t *Source, unsigned int CodePage, DWORD Flags)
	{
	  if (int Len = ::WideCharToMultiByte(CodePage, Flags, Source, (int)wcslen(Source), NULL, 0, NULL, NULL)) {
		std::vector<char> Dest(Len);
		if (Len = ::WideCharToMultiByte(CodePage, Flags, Source, (int)wcslen(Source), &Dest[0], static_cast<int>(Dest.size()), NULL, NULL)) {
		  return std::string(Dest.begin(), Dest.begin() + Len);
		}
	  }
	  return "";
	}


	std::wstring MultiByteToUnicode(const char* Source, unsigned int CodePage, DWORD Flags)
	{
	  if (int Len = ::MultiByteToWideChar(CodePage, Flags, Source, (int)strlen(Source), NULL, 0)) {
		std::vector<wchar_t> Dest(Len);
		if (Len = ::MultiByteToWideChar(CodePage, 0, Source, (int)strlen(Source), &Dest[0], static_cast<int>(Dest.size()))) {
		  return std::wstring(Dest.begin(), Dest.begin() + Len);
		}
	  }
	  return L"";
	}

	std::wstring CodePagetoString(unsigned int cset){
		switch(cset){
			case CP_SJIS:
				return L"Shift_JIS";
			case CP_ISO2022JP:
				return L"ISO-2022-JP";
			case CP_EUCJP:
				return L"EUC-JP";
			case CP_UTF8:
				return L"UTF-8";
		}
		return L"unknown charset";
	}

	UINT StringtoCodePage(const char *str)
	{
		if ( str && *str ) {
			if ( strnicmp(str,"shift_jis",9) == 0 ) {
				return CP_SJIS;
			}
			if ( strnicmp(str,"x-sjis",6) == 0 ) {
				return CP_SJIS;
			}
			if ( strnicmp(str,"iso-2022-jp",11) == 0 ) {
				return CP_ISO2022JP;
			}
			if ( strnicmp(str,"euc-jp",6) == 0 ) {
				return CP_EUCJP;
			}
			if ( strnicmp(str,"x-euc-jp",8) == 0 ) {
				return CP_EUCJP;
			}
			if ( strnicmp(str,"utf-8",5) == 0 ) {
				return CP_UTF8;
			}
		}
		return CP_SJIS;
	}

	string_t getResultString(int rc)
	{
		char_t	*p;
		
		switch(rc) {
			case 200:
				p = L"OK"; break;
			case 204:
				p = L"No Content"; break;
			case 210:
				p = L"Break"; break;		// SSTP Break (SSTP session aborted)
			case 300:
				p = L"Communicate"; break; // (obsolete)
			case 311:
				p = L"Not Enough"; break; // SHIORI/2.4 TEACH...need more info.
			case 312:
				p = L"Advice"; break; // SHIORI/2.4 TEACH...drop recent reference
			case 400:
				p = L"Bad Request"; break;
			case 401:
				p = L"Unauthorized"; break;
			case 403:
				p = L"Forbidden"; break;
			case 404:
				p = L"Not Found"; break;
			case 405:
				p = L"Method Not Allowed"; break;
			case 406:
				p = L"Not Acceptable"; break;
			case 408:
				p = L"Request Timeout"; break;
			case 409:
				p = L"Conflict"; break;
			case 420:
				p = L"Refuse"; break;		// SSTP refused by ghost
			case 500:
				p = L"Internal Server Error"; break;
			case 501:
				p = L"Not Implemented"; break;
			case 503:
				p = L"Service Unavailable"; break;
			case 510:
				p = L"Not Local IP"; break;	// SSTP from Non-Local IP
			case 511:
				p = L"In Black List"; break;	// SSTP from "Black-Listed" IP
			case 512:
				p = L"Invisible"; break;	// SSTP inivisible (not proceed)
			default:
				p = L"Unknown Error";
		}
		
		return string_t(p);
	}

	// cut one line
	// return ... first position of next line or string::npos if not found.

	string_t::size_type getLine(string_t &sl, const string_t &src, string_t::size_type tpos)
	{
		string_t::size_type len = src.size();
		if (tpos == string_t::npos || tpos >= len) {
			sl = L"";
			return string_t::npos;
		}
		string_t::size_type  ppos = src.find_first_of(L"\r\n", tpos);
		if (ppos == string_t::npos) {
			sl = src.substr(tpos);
			return ppos;
		}
		sl = src.substr(tpos, ppos - tpos);
		ppos++;
		if (ppos >= len) return string_t::npos;
		if (src[ppos - 1] == '\r' && src[ppos] == '\n') {
			ppos++;
			if (ppos >= len) return string_t::npos;
		}
		
		return ppos;
	}

	string_t replaceAll(string_t string, const string_t &find, const string_t &replace) {
		string_t::size_type found = std::string::npos;
		while((found = string.find(find, found)) != std::string::npos) {
			string.replace(found, find.length(), replace);
			found += replace.size();
		}
		return string;
	}

	std::string replaceAll(std::string string, const std::string &find, const std::string &replace) {
		std::string::size_type found = std::string::npos;
		while((found = string.find(find, found)) != std::string::npos) {
			string.replace(found, find.length(), replace);
			found += replace.size();
		}
		return string;
	}

	typedef struct tagMessageBoxInfo {
		void *hwnd;
		string_t message;
		string_t title;
		unsigned int flags;
	} MessageBoxInfo;

	static void __cdecl AsyncMessageBoxProc(void *p)
	{
		MessageBoxInfo *pInf = reinterpret_cast<MessageBoxInfo*>(p);
		::MessageBoxW((HWND)pInf->hwnd,pInf->message.c_str(),pInf->title.c_str(),pInf->flags);
		delete pInf;
	}

	void AsyncMessageBox(void *hwnd,char_t *message,char_t *title,unsigned int flags)
	{
		MessageBoxInfo *pMsg = new MessageBoxInfo;
		pMsg->hwnd = hwnd;
		pMsg->title = title ? title : L"";
		pMsg->message = message ? message : L"";
		pMsg->flags = flags;

		_beginthread(AsyncMessageBoxProc,0,pMsg);
	}

	CCriticalSection::CCriticalSection(void) : init(false)
	{
		//Vista�ȍ~�̃��������[�N�ƃ}���`�X���b�h�̃p�t�H�[�}���X�΍�
		typedef BOOL (WINAPI *FInitializeCriticalSectionEx)(LPCRITICAL_SECTION lpCriticalSection,DWORD dwSpinCount,DWORD Flags);
		typedef BOOL (WINAPI *FInitializeCriticalSectionAndSpinCount)(LPCRITICAL_SECTION lpCriticalSection,DWORD dwSpinCoun);

		static FInitializeCriticalSectionEx SInitializeCriticalSectionEx = NULL;
		static FInitializeCriticalSectionAndSpinCount SInitializeCriticalSectionAndSpinCount = NULL;
		static bool init = false;

		if ( ! init ) {
			OSVERSIONINFO inf;
			inf.dwOSVersionInfoSize = sizeof(inf);
			::GetVersionEx(&inf);

			init = true;
			if ( inf.dwMajorVersion >= 6 ) {
				SInitializeCriticalSectionEx = reinterpret_cast<FInitializeCriticalSectionEx>(
					::GetProcAddress(::GetModuleHandleA("kernel32"),"InitializeCriticalSectionEx") );
			}

			if ( inf.dwMajorVersion >= 5 ) {
				SInitializeCriticalSectionAndSpinCount = reinterpret_cast<FInitializeCriticalSectionAndSpinCount>(
					::GetProcAddress(::GetModuleHandleA("kernel32"),"InitializeCriticalSectionAndSpinCount") );
			}
		}

		BOOL result;
		if ( SInitializeCriticalSectionEx ) {
			result = SInitializeCriticalSectionEx(&c,1500,0x01000000 /*CRITICAL_SECTION_NO_DEBUG_INFO*/);
		}
		else if ( SInitializeCriticalSectionAndSpinCount ) {
			result = SInitializeCriticalSectionAndSpinCount(&c,1500);
		}
		else {
			::InitializeCriticalSection(&c);
			result = 1;
		}
		init = (result != 0);
	}
	CCriticalSection::~CCriticalSection()
	{
		if ( init ) {
			::DeleteCriticalSection(&c);
		}
	}
	void CCriticalSection::Enter(void)
	{
		if ( init ) {
			::EnterCriticalSection(&c);
		}
	}
	void CCriticalSection::Leave(void)
	{
		if ( init ) {
			::LeaveCriticalSection(&c);
		}
	}

}

//------------------------------------------------------------------------------
// CSAORIInput
//------------------------------------------------------------------------------
bool CSAORIInput::parseString(const string_t &src)
{
	const string_t& atag = base.s_saori_argument();
	const string_t::size_type catag = atag.length();
	
	string_t::size_type pos = 0, nextpos;
	string_t::size_type ts;
	std::vector<string_t> _arg;
	std::map<string_t, string_t> _opt;
	string_t _cmd;
	string_t _id;
	
	string_t sl;
	
	pos = SAORI_FUNC::getLine(sl, src, pos);
	ts = sl.find(base.s_saori_def());
	if (ts == string_t::npos) return false;
	_cmd = sl.substr(0, ts);
	
	string_t k, v;
	int argc = 0;
	
	while(1) {
		sl = L"";
		nextpos = SAORI_FUNC::getLine(sl, src, pos);
		ts = sl.find(L": ");
		if (ts != string_t::npos) {
			k = sl.substr(0, ts);
			v = sl.substr(ts + 2);
			const char_t *pk = k.c_str();
			// _argument[n] �� vector (_args) �ɐς�
			if (k.size() > catag && _wcsnicmp(pk, atag.c_str(), catag) == 0) {
				int ord = _wtoi(pk + catag);
				if (ord > 0 || (ord == 0 && k[catag] == L'0')) {
					if (argc <= ord) {
						// _argument[n] �����Ԃɓ����ĂȂ��ꍇ�� 
						// ���Ԃ��ɖ��߂Ă��� 
						for(int i=(int)(_arg.size()); i<=ord; i++) {
							_arg.push_back(L"");
						}
					}
					//_arg.push_back(v);
					_arg[ord]=v;
					argc = (int)(_arg.size());
				}
			}
			// ID: ��id��
			else if ( wcsicmp(k.c_str(),L"ID") == 0 ) {
				_id = v;
			}
			// ����ȊO�̗v�f�� map (_opt) �ɐς� 
			else {
				if (ts > 0) {
					//�悭����w�b�_�̑啶������������
					if ( wcsicmp(k.c_str(),L"securitylevel") == 0 ) {
						k = L"SecurityLevel";
					}
					else if ( wcsicmp(k.c_str(),L"sender") == 0 ) {
						k = L"Sender";
					}
					else if ( wcsicmp(k.c_str(),L"charset") == 0 ) {
						k = L"Charset";
					}

					std::map<string_t,string_t>::iterator i;
					i = _opt.find(k);
					if (i == _opt.end()) {
						_opt.insert(std::pair<string_t,string_t>(k, v));
					}
					else _opt[k] = v;
				}
			}
		}
		if (nextpos == string_t::npos) break;
		pos = nextpos;
	}
	cmd=_cmd;
	args=_arg;
	opts=_opt;
	id=_id;
	return true;
}

//------------------------------------------------------------------------------
//CSAORIOutput
//------------------------------------------------------------------------------
void CSAORIOutput::setResultEmpty()
{
	if ( result_code == SAORIRESULT_OK || result_code == SAORIRESULT_FORCE_OK ) {
		result_code = SAORIRESULT_NO_CONTENT;
		result.erase();
		values.clear();
	}
}

string_t CSAORIOutput::toString()
{
	if ( result_code == SAORIRESULT_FORCE_OK ) {
		result_code = SAORIRESULT_OK;
	}
	//No Content�Ɏ����␳
	else if ( result.empty() && values.empty() && opts.empty() && result_code == SAORIRESULT_OK ) {
		result_code = SAORIRESULT_NO_CONTENT;
	}

	string_t rcstr = SAORI_FUNC::getResultString(result_code);
	
	wchar_t tmptxt[32];
	swprintf(tmptxt,L"%d",result_code);

	std::wstring dest;
	dest += base.s_saori_version();
	dest += L" ";
	dest += tmptxt + std::wstring(L" ") + std::wstring(rcstr) + L"\r\n";

	dest += L"Charset: " + SAORI_FUNC::CodePagetoString(codepage) + L"\r\n";
	
//	if (!result.empty()) { //�󕶎���ł����ʂ͕Ԃ��ׂ�
	dest += base.s_saori_result() + L": " + result + L"\r\n";
//	}
	if (!opts.empty()) {
		std::map<string_t,string_t>::iterator i;
		for(i=opts.begin(); i != opts.end(); i++) {
			dest += i->first + L": " + i->second + L"\r\n";
		}
	}
	if (!values.empty()) {
		int i, n = (int)(values.size());
		string_t tmp;
		for(i=0; i<n; i++) {
			swprintf(tmptxt,L"%d",i);

			dest += base.s_saori_value();
			dest += tmptxt;
			dest += std::wstring(L": ");

			tmp = values[i];
			std::string::size_type nPos = 0;
			while((nPos = tmp.find(L"\r\n", nPos)) != std::string::npos){
				tmp.replace(nPos, 2 , L"\1");
			}
			while((nPos = tmp.find(L"\r", nPos)) != std::string::npos){
				tmp.replace(nPos, 2 , L"\1");
			}
			while((nPos = tmp.find(L"\n", nPos)) != std::string::npos){
				tmp.replace(nPos, 2 , L"\1");
			}

			dest += tmp + L"\r\n";
		}
	}
	
	dest += L"\r\n";
	return dest;
}

//------------------------------------------------------------------------------
//CSAORIBase
//------------------------------------------------------------------------------
std::string CSAORIBase::request(const std::string &rq_tmp)
{
	call_id += 1;
	if ( call_id >= 0x7fffffffU ) {
		call_id = 1;
	}

	std::string cmd;
	CSAORIInput* pIn;
	CSAORIOutput* pOut;

	//�����HRequest���������ɕϊ�
	std::string tmp=rq_tmp;
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);

	//Charset����
	unsigned int cp = CP_SJIS;
	std::string::size_type count = tmp.find("\ncharset: ");
	if ( count != std::string::npos ) {
		cp = SAORI_FUNC::StringtoCodePage(tmp.c_str() + count + 10);
	}

	//�ϊ�
	string_t rq=SAORI_FUNC::MultiByteToUnicode(rq_tmp,cp);

	//��͏����J�n
	pIn=new CSAORIInput(*this);
	pIn->codepage=cp;
	pIn->opts[L"SecurityLevel"] = L"Local";
	bool result=pIn->parseString(rq);
	
	//pOut������
	pOut=new CSAORIOutput(*this);
	pOut->codepage=pIn->codepage;
	pOut->result=L"";
	pOut->result_code=SAORIRESULT_INTERNAL_SERVER_ERROR;

	if(result==false){
		pOut->result_code=SAORIRESULT_INTERNAL_SERVER_ERROR;
	}else{
		if (pIn->cmd == L"GET Version") {
			pOut->result_code=SAORIRESULT_FORCE_OK;
		}
		else if ( wcsicmp(pIn->cmd.c_str(),L"EXECUTE") == 0 || wcsicmp(pIn->cmd.c_str(),L"GET") == 0 || wcsicmp(pIn->cmd.c_str(),L"NOTIFY") == 0 ) {
			string_t sec = pIn->opts[L"SecurityLevel"];

			exec_before(*pIn,*pOut);
			if ( sec.empty() || wcsicmp(sec.c_str(),L"local") == 0 ) {
				exec(*pIn,*pOut);
			}
			else {
				if ( ! exec_insecure(*pIn,*pOut) ) {
					pOut->result_code=SAORIRESULT_INTERNAL_SERVER_ERROR;
				}
			}
			exec_after(*pIn,*pOut);

			if ( wcsicmp(pIn->cmd.c_str(),L"NOTIFY") == 0 ) {
				pOut->setResultEmpty();
			}
		}
		else {
			pOut->result_code=SAORIRESULT_BAD_REQUEST;
		}
	}
	string_t res_wstr=pOut->toString();
	std::string res_str=SAORI_FUNC::UnicodeToMultiByte(res_wstr,pOut->codepage);
	delete pIn;
	delete pOut;
	return res_str;
}

void CSAORIBase::setModulePath(const std::string &str){
	module_path=SAORI_FUNC::MultiByteToUnicode(str);
}

void CSAORIBase::setModuleHandle(HANDLE hMod){
	module_handle=hMod;
}

string_t CSAORIBase::checkAndModifyPathW(const string_t &p)
{
	string_t filepath = p;

	if ( p.size() >= 3 ) {
#ifdef _WINDOWS
		//���ϐ��W�J�p����R�[�h
		string_t::size_type len=filepath.size() < MAX_PATH ? MAX_PATH : filepath.size();
		len *= 2;

		{
			void *pBuf = malloc(len+1);
#ifdef _UNICODE
			std::string::size_type realLen = ::ExpandEnvironmentStrings(filepath.c_str(),(wchar_t*)pBuf,(DWORD)len);
#else
			std::string filepatha = SAORI_FUNC::UnicodeToMultiByte(p);
			std::string::size_type realLen = ::ExpandEnvironmentStrings(filepatha.c_str(),(char*)pBuf,(DWORD)len);
#endif
			if ( realLen > len ) {
				free(pBuf);
				pBuf = malloc(realLen+1);
#ifdef _UNICODE
				realLen = ::ExpandEnvironmentStrings(filepath.c_str(),(wchar_t*)pBuf,(DWORD)realLen);
#else
				realLen = ::ExpandEnvironmentStrings(filepatha.c_str(),(char*)pBuf,(DWORD)realLen);
#endif
			}

			if ( realLen ) {
#ifdef _UNICODE
				filepath = (wchar_t*)pBuf;
#else
				filepatha = (char*)pBuf;
				filepath = SAORI_FUNC::MultiByteToUnicode(filepatha);
#endif
			}
			free(pBuf);
		}

		//Windows ��΃p�X
		if ( wcsncmp(filepath.c_str(),L"\\\\",2) == 0 || wcsncmp(filepath.c_str()+1,L":\\",2) == 0 ||
			wcsncmp(filepath.c_str(),L"//",2) == 0 || wcsncmp(filepath.c_str()+1,L":/",2) == 0 ) {
			return filepath;
		}
#else
		//Unix
		if ( filepath[0] == L'/' ) {
			return filepath;
		}
#endif
	}
	string_t fullpath = module_path;
#ifdef _WINDOWS
	if ( fullpath[fullpath.size()-1] != L'\\' ) {
		fullpath += L"\\";
	}
#else
	if ( fullpath[fullpath.size()-1] != L'/' ) {
		fullpath += L"/";
	}
#endif
	fullpath += filepath;
	return fullpath;
}



std::string CSAORIBase::checkAndModifyPath(const std::string &p)
{
	std::string filepath = p;

	if ( p.size() >= 3 ) {
#ifdef _WINDOWS
		//���ϐ��W�J�p����R�[�h
		std::string::size_type len = filepath.size() < MAX_PATH ? MAX_PATH : filepath.size();
		len *= 2;

		{
			void *pBuf = malloc(len+1);
#ifdef _UNICODE
			string_t filepathw = SAORI_FUNC::MultiByteToUnicode(p);
			std::string::size_type realLen = ::ExpandEnvironmentStrings(filepathw.c_str(),(wchar_t*)pBuf,(DWORD)len);
#else
			std::string::size_type realLen = ::ExpandEnvironmentStrings(filepath.c_str(),(char*)pBuf,(DWORD)len);
#endif
			if ( realLen > len ) {
				free(pBuf);
				pBuf = malloc(realLen+1);
#ifdef _UNICODE
				realLen = ::ExpandEnvironmentStrings(filepathw.c_str(),(wchar_t*)pBuf,(DWORD)realLen);
#else
				realLen = ::ExpandEnvironmentStrings(filepath.c_str(),(char*)pBuf,(DWORD)realLen);
#endif
			}

			if ( realLen ) {
#ifdef _UNICODE
				filepathw = (wchar_t*)pBuf;
				filepath = SAORI_FUNC::UnicodeToMultiByte(filepathw);
#else
				filepath = (char*)pBuf;
#endif
			}
			free(pBuf);
		}

		//Windows ��΃p�X
		if ( strncmp(filepath.c_str(),"\\\\",2) == 0 || strncmp(filepath.c_str()+1,":\\",2) == 0 ||
			strncmp(filepath.c_str(),"//",2) == 0 || strncmp(filepath.c_str()+1,":/",2) == 0 ) {
			return filepath;
		}
#else
		//Unix
		if ( filepath[0] == '/' ) {
			return filepath;
		}
#endif
	}
	std::string fullpath = SAORI_FUNC::UnicodeToMultiByte(module_path);
#ifdef _WINDOWS
	if ( fullpath[fullpath.size()-1] != '\\' ) {
		fullpath += "\\";
	}
#else
	if ( fullpath[fullpath.size()-1] != '/' ) {
		fullpath += "/";
	}
#endif
	fullpath += filepath;
	return fullpath;
}

//------------------------------------------------------------------------------
//SAORI INTERFACES
//------------------------------------------------------------------------------
BOOL APIENTRY DllMain(
	HANDLE hModule,
	DWORD  ul_reason_for_call, 
	LPVOID lpReserved
){
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = (HINSTANCE)hModule;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		unload();
		break;
	}
    return TRUE;
}

SAORIAPI
HGLOBAL
SAORICDECL 
request(HGLOBAL h, long *len)
{
	std::string rq((char *)h, *len);
	GlobalFree(h);
	
	std::string re;

#ifndef _DEBUG
	try {
#endif
		re = pSaori->request(rq);
#ifndef _DEBUG
	}
	catch(...) {
		re = SAORI_FUNC::UnicodeToMultiByte(pSaori->s_saori_version() + string_t(L" 500 Internal Server Error\r\n\r\n"));
	}
#endif

	*len = (long)(re.size());
	h = GlobalAlloc(GMEM_FIXED, *len+1);
	if (h) {
		memcpy(h,re.c_str(),*len+1);
	}
	
	return h;
}

SAORIAPI
BOOL
SAORICDECL
load(HGLOBAL h, long len)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(829);
#endif
	if(pSaori!=NULL){
		unload();
	}
 	pSaori = CreateInstance();
	if (h) {
		std::string mpath;
		mpath.assign((char*)h,len);
		GlobalFree(h);
		pSaori->setModulePath(mpath);
		pSaori->setModuleHandle(g_hModule);
	}

	BOOL re;

#ifndef _DEBUG
	try {
#endif
		re = pSaori->load();
#ifndef _DEBUG
	}
	catch(...) {
		re = FALSE;
	}
#endif

	return re;
}

SAORIAPI
BOOL
SAORICDECL
unload()
{
	if(pSaori==NULL){
		return TRUE;
	}

	BOOL re;

#ifndef _DEBUG
	try {
#endif
		re=pSaori->unload();
#ifndef _DEBUG
	}
	catch(...) {
		re = 0;
	}
#endif
	delete pSaori;
	pSaori=NULL;

	return re;
}
