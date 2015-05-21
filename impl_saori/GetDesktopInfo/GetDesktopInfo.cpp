#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <commctrl.h>
#include <shlobj.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <process.h>
#include <comdef.h>

#include "csaori.h"

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
	����free
---------------------------------------------------------*/
class CAutoFree {
private:
	CAutoFree() { }

	void *m_pBuf;
public:
	CAutoFree(void* pBuf) : m_pBuf(pBuf) { }
	~CAutoFree() { if ( m_pBuf ) { free(m_pBuf); } }

	operator void*() { return m_pBuf; }
};

/*---------------------------------------------------------
	���s
---------------------------------------------------------*/
static HWND GetDesktopListView(void)
{
	HWND hWnd = ::FindWindow("Progman", "Program Manager");  
	if ( hWnd ) {
		hWnd = ::FindWindowEx(hWnd, NULL, "SHELLDLL_DefView", NULL);
		if ( hWnd ) {
			hWnd = ::FindWindowEx(hWnd, NULL, "SysListView32", NULL);
		}
	}
	return hWnd;
}

#define BUFFER_SIZE 4096

HWND g_hWnd = NULL;

class WindowIconInfo {
public:
	unsigned int m_index;
	string_t m_name;
	POINT m_pt;
};

static void __cdecl EmptyTrashThread(void*);
static void QueryTrash(DWORD &size,DWORD &item);

static unsigned int GetDesktopIconInfoListW4(std::vector<WindowIconInfo> &vec,CSAORIOutput& out);
static unsigned int GetDesktopIconInfoListW5(std::vector<WindowIconInfo> &vec,CSAORIOutput& out);

void CSAORI::exec(const CSAORIInput& in,CSAORIOutput& out)
{
	out.result_code = SAORIRESULT_BAD_REQUEST;

	if ( in.args.size() < 0 ) { return; }

	if ( wcsicmp(in.args[0].c_str(),L"hwnd") == 0 ) {
		if ( in.args.size() >= 2 ) {
			g_hWnd = reinterpret_cast<HWND>(wcstoul(in.args[1].c_str(),NULL,10));
			out.result_code = SAORIRESULT_OK;
		}
		return;
	}
	else if ( wcsicmp(in.args[0].c_str(),L"get") == 0 ) {
		out.result_code = SAORIRESULT_INTERNAL_SERVER_ERROR;

		//�����Ώۂƌ�������
		string_t find_string;
		if ( in.args.size() >= 2 ) {
			find_string = in.args[1];
			if ( wcsnicmp(find_string.c_str(),L"system:",7) == 0 ) {
				int nFolder = CSIDL_PERSONAL;
				const char_t *str = find_string.c_str()+7;
				if ( wcsicmp(str,L"bitbucket") == 0 || wcsicmp(str,L"trash") == 0 ) {
					nFolder = CSIDL_BITBUCKET;
				}
				else if ( wcsicmp(str,L"internet") == 0 ) {
					nFolder = CSIDL_INTERNET;
				}
				else if ( wcsicmp(str,L"drives") == 0 || wcsicmp(str,L"computer") == 0 || wcsicmp(str,L"mycomputer") == 0 ) {
					nFolder = CSIDL_DRIVES;
				}

				LPITEMIDLIST pidl;

				IMalloc *pMalloc;
				::SHGetMalloc( &pMalloc );
				::SHGetSpecialFolderLocation(NULL,nFolder,&pidl);

				SHFILEINFOA fileinfo;
				::SHGetFileInfoA(reinterpret_cast<char*>(pidl),0,&fileinfo,sizeof(fileinfo),SHGFI_PIDL|SHGFI_DISPLAYNAME);

				pMalloc->Free(pidl);
				pMalloc->Release();

				find_string = SAORI_FUNC::MultiByteToUnicode(fileinfo.szDisplayName);
			}
		}

		WindowIconInfo *pFound = NULL;

		std::vector<WindowIconInfo> vec;

		OSVERSIONINFO osvi;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		::GetVersionEx( &osvi );

		unsigned int count;
		if ( osvi.dwMajorVersion >= 6 ) {
			count = GetDesktopIconInfoListW5(vec,out);
		}
		/*else if ( osvi.dwMajorVersion >= 5 ) {
			if ( osvi.dwMinorVersion >= 1 ) {
				count = GetDesktopIconInfoListW5(vec,out);
			}
			else {
				count = GetDesktopIconInfoListW4(vec,out);
			}
		}*/
		else {
			count = GetDesktopIconInfoListW4(vec,out);
		}

		for ( unsigned int i = 0 ; i < count ; ++i ) {
			string_t item = vec[i].m_name;

			if ( ! pFound ) {
				if ( wcsstr(vec[i].m_name.c_str(),find_string.c_str()) ) {
					pFound = &(vec[i]);
				}
			}

			char_t rect_text[128];
			swprintf(rect_text,L"\1%d,%d",vec[i].m_pt.x,vec[i].m_pt.y);
			item += rect_text;

			out.values.push_back(item);
		}

		out.result_code = SAORIRESULT_OK;

		if ( pFound ) {
			char_t rect_text[128];

			swprintf(rect_text,L"%d\1",pFound->m_index);
			out.result = rect_text;

			out.result += pFound->m_name;

			swprintf(rect_text,L"\1%d,%d",pFound->m_pt.x,pFound->m_pt.y);
			out.result += rect_text;
		}
		else {
			out.result = L"";
		}
	}
	else if ( wcsicmp(in.args[0].c_str(),L"empty_trash") == 0 ) {
		_beginthread(EmptyTrashThread,0,NULL);
		out.result_code = SAORIRESULT_OK;
	}
	else if ( wcsicmp(in.args[0].c_str(),L"query_trash") == 0 ) {
		DWORD totalSize = 0;
		DWORD totalItems = 0;

		QueryTrash(totalSize,totalItems);

		char_t size_text[32];
		char_t item_text[32];
		swprintf(size_text,L"%u",totalSize);
		swprintf(item_text,L"%u",totalItems);

		out.result = item_text;
		out.values.push_back(item_text);
		out.values.push_back(size_text);

		out.result_code = SAORIRESULT_OK;
	}
}

/*====================================================================================
	Windows 2000���O
====================================================================================*/

typedef LPVOID (WINAPI *PF_VIRTUALALLOCEX)(HANDLE,LPVOID,DWORD,DWORD,DWORD);
typedef BOOL   (WINAPI *PF_VIRTUALFREEEX)(HANDLE,LPVOID,DWORD,DWORD);

static unsigned int GetDesktopIconInfoListW4(std::vector<WindowIconInfo> &vec,CSAORIOutput& out)
{
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    ::GetVersionEx( &osvi );

	bool isNT = (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT);

	// Windows NT�n�ł������p�ł��Ȃ��֐����AWindows 95�n�Ɋ��m�����Ȃ��`��
	// �Ăяo�����߂̍H�v
	HMODULE hDLL = GetModuleHandle("kernel32");
	PF_VIRTUALALLOCEX pVirtualAllocEx = (PF_VIRTUALALLOCEX)GetProcAddress(hDLL,"VirtualAllocEx");
	PF_VIRTUALFREEEX  pVirtualFreeEx  = (PF_VIRTUALFREEEX)GetProcAddress(hDLL, "VirtualFreeEx");

	//�f�X�N�g�b�v�E�C���h�E (����0,0)
	HWND hWnd = GetDesktopListView();
	if ( ! hWnd ) { return 0; }

	//PID������āA�����ɗ̈�m��
	DWORD pid;  
	::GetWindowThreadProcessId(hWnd,&pid);

	HANDLE ph;
	void *ptr;
	
	if ( isNT ) {
		ph = ::OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, pid);
		if ( ! ph ) { return 0; }

		ptr = pVirtualAllocEx(ph,NULL,BUFFER_SIZE,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		if ( ! ptr ) {
			::CloseHandle(ph);
			return 0;
		}
	}
	else {
        ph = ::CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, BUFFER_SIZE,NULL);
        ptr = ::MapViewOfFile(ph, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	}

	//�v�f�擾���[�v
	unsigned int count = ListView_GetItemCount(hWnd);

	WindowIconInfo info;

	for ( int i = 0 ; i < count ; ++i ) {
		//�v���Z�X����������LVITEM�������Ď擾�A�c��̗̈�ɕ�����������Ă��炤
		LVITEM li;
		ZeroMemory(&li,sizeof(li));
		li.mask = LVIF_TEXT;  
		li.iItem = i;
		li.iSubItem = 0;
		li.cchTextMax = BUFFER_SIZE-sizeof(LVITEM)-1;
		li.pszText = reinterpret_cast<char*>(ptr)+sizeof(LVITEM);
		
		DWORD numRead;
		char text_buffer[BUFFER_SIZE-sizeof(LVITEM)];

		if ( isNT ) {
			::WriteProcessMemory(ph, ptr, &li, sizeof(LVITEM), &numRead );
		}
		else {
			memcpy(ptr, &li, sizeof(LVITEM) );
		}

		::SendMessage(hWnd, LVM_GETITEM , 0, (LPARAM)ptr);

		if ( isNT ) {
			::ReadProcessMemory(ph, reinterpret_cast<char*>(ptr)+sizeof(LVITEM), text_buffer, BUFFER_SIZE-sizeof(LVITEM)-1, &numRead );
		}
		else {
			memcpy(text_buffer, reinterpret_cast<char*>(ptr)+sizeof(LVITEM), BUFFER_SIZE-sizeof(LVITEM)-1);
		}
		
		info.m_name = SAORI_FUNC::MultiByteToUnicode(text_buffer);

		ListView_GetItemPosition(hWnd,i,ptr);

		if ( isNT ) {
			::ReadProcessMemory(ph, ptr, &info.m_pt, sizeof(POINT), &numRead );
		}
		else {
			memcpy(&info.m_pt, ptr, sizeof(POINT) );
		}

		info.m_index = i;

		vec.push_back(info);
	}

	if ( isNT ) {
		pVirtualFreeEx(ph,ptr,0,MEM_RELEASE);
		::CloseHandle(ph);
	}
	else {
		::UnmapViewOfFile(ptr);
		::CloseHandle(ph);
	}

	return count;
}

/*====================================================================================
	Windows XP����
====================================================================================*/

#ifndef IID_PPV_ARGS
template<typename T> void** IID_PPV_ARGS_Helper(T** pp) 
{
    static_cast<IUnknown*>(*pp);    // make sure everyone derives from IUnknown
    return reinterpret_cast<void**>(pp);
}

#define IID_PPV_ARGS(ppType) __uuidof(**(ppType)), IID_PPV_ARGS_Helper(ppType)
#endif //IID_PPV_ARGS

#define COM_PTR_DEF(type) _com_ptr_t<_com_IIID<type,&__uuidof(type)> >

#ifndef SWC_DESKTOP
#define SWC_DESKTOP 0x8
#endif

static unsigned int GetDesktopIconInfoListW5(std::vector<WindowIconInfo> &vec,CSAORIOutput& out)
{
	HRESULT result;
	
	COM_PTR_DEF(IShellWindows) shellWindows = NULL;
	result = ::CoCreateInstance(CLSID_ShellWindows, NULL, CLSCTX_ALL, IID_PPV_ARGS(&shellWindows) );
	if ( ! SUCCEEDED(result) ) {
		out.opts.insert(map_strpair::value_type(L"X-Error-Reason",L"IShellWindows"));
		return 0;
	}
	
	VARIANT var;
	VariantInit(&var);
	COM_PTR_DEF(IDispatch) dispatch = NULL;
	long hwnd;
	result = shellWindows->FindWindowSW(&var,&var,SWC_DESKTOP,&hwnd,SWFO_NEEDDISPATCH,&dispatch);
	if ( ! SUCCEEDED(result) ) {
		out.opts.insert(map_strpair::value_type(L"X-Error-Reason",L"IDispatch"));
		return 0;
	}
	
	/*COM_PTR_DEF(IWebBrowserApp) webBrowserApp = NULL;
	result = dispatch->QueryInterface(IID_PPV_ARGS(&webBrowserApp));
	if ( ! SUCCEEDED(result) ) {
		out.opts.insert(map_strpair::value_type(L"X-Error-Reason",L"IWebBrowserApp"));
		return 0;
	}*/

	COM_PTR_DEF(IServiceProvider) serviceProvider = NULL;
	//result = webBrowserApp->QueryInterface(IID_PPV_ARGS(&serviceProvider));
	result = dispatch->QueryInterface(IID_PPV_ARGS(&serviceProvider));
	if ( ! SUCCEEDED(result) ) {
		out.opts.insert(map_strpair::value_type(L"X-Error-Reason",L"IServiceProvider"));
		return 0;
	}

	COM_PTR_DEF(IShellBrowser) shellBrowser = NULL;
	result = serviceProvider->QueryService(SID_STopLevelBrowser,IID_PPV_ARGS(&shellBrowser));
	if ( ! SUCCEEDED(result) ) {
		out.opts.insert(map_strpair::value_type(L"X-Error-Reason",L"IShellBrowser"));
		return 0;
	}

	COM_PTR_DEF(IShellView) shellView = NULL;
	result = shellBrowser->QueryActiveShellView(&shellView);
	if ( ! SUCCEEDED(result) ) {
		out.opts.insert(map_strpair::value_type(L"X-Error-Reason",L"IShellView"));
		return 0;
	}

	COM_PTR_DEF(IFolderView) folderView = NULL;
	result = shellView->QueryInterface(IID_PPV_ARGS(&folderView));
	if ( ! SUCCEEDED(result) ) {
		out.opts.insert(map_strpair::value_type(L"X-Error-Reason",L"IFolderView"));
		return 0;
	}

	COM_PTR_DEF(IShellFolder) shellFolder = NULL;
	folderView->GetFolder(IID_PPV_ARGS(&shellFolder));
	if ( ! SUCCEEDED(result) ) {
		out.opts.insert(map_strpair::value_type(L"X-Error-Reason",L"IShellFolder"));
		return 0;
	}

	COM_PTR_DEF(IEnumIDList) enumIDList = NULL;
	result = folderView->Items(SVGIO_FLAG_VIEWORDER,IID_PPV_ARGS(&enumIDList));
	if ( ! SUCCEEDED(result) ) {
		out.opts.insert(map_strpair::value_type(L"X-Error-Reason",L"IEnumIDList"));
		return 0;
	}

	ITEMIDLIST *childpidl = NULL;
	unsigned int count = 0;

	WindowIconInfo info;

	while ( enumIDList->Next(1,&childpidl,NULL) == S_OK ) {
		POINT pos;
		result = folderView->GetItemPosition(childpidl,&pos);

		STRRET strret;
		shellFolder->GetDisplayNameOf(childpidl, SHGDN_NORMAL, &strret);

		wchar_t szDisplayName[256];
		::StrRetToBufW(&strret, childpidl, szDisplayName, sizeof(szDisplayName));

		info.m_index = count;
		info.m_pt = pos;
		info.m_name = szDisplayName;

		vec.push_back(info);

		::ILFree(childpidl);
		++count;
	}

	return count;
}

/*====================================================================================
	���݂΂�����
====================================================================================*/

static void QueryTrash(DWORD &totalSize,DWORD &totalItems)
{
	totalSize = 0;
	totalItems = 0;

	char pathName[1024];
	if ( ! ::GetLogicalDriveStrings(sizeof(pathName)-2,pathName) ) {
		return;
	}

	SHQUERYRBINFO rbInfo;
	rbInfo.cbSize = sizeof(rbInfo);

	char *rootPath = pathName;

	while ( rootPath[0] ) {
		UINT type = ::GetDriveType(rootPath);
		if ( type == DRIVE_FIXED || type == DRIVE_RAMDISK ) {
			if ( ::SHQueryRecycleBin(rootPath,&rbInfo) == S_OK ) {
				totalItems += (DWORD)rbInfo.i64NumItems;
				totalSize += (DWORD)rbInfo.i64Size;
			}
		}
		rootPath += strlen(rootPath) + 1;
	}
}

void __cdecl EmptyTrashThread(void*)
{
	DWORD totalSize = 0;
	DWORD totalItems = 0;

	QueryTrash(totalSize,totalItems);

	if ( totalItems ) {
		::SHEmptyRecycleBin(NULL,NULL,SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
	}

	if ( g_hWnd && ::IsWindow(g_hWnd) ) {
		string_t sstp(L"NOTIFY SSTP/1.1\r\nEvent: OnRecycleBinEmptied\r\nCharset: UTF-8\r\nSender: GetDesktopInfo SAORI\r\nHWnd: 0\r\n");

		char_t tmp[256];

		swprintf(tmp,L"Reference0: %u\r\nReference1: %u\r\n\r\n",totalItems,totalSize);

		sstp += tmp;

		std::string sstp_a = SAORI_FUNC::UnicodeToMultiByte(sstp.c_str(),CP_UTF8);

		DWORD result;
		COPYDATASTRUCT cds;
		cds.dwData = 9801;
		cds.cbData = sstp_a.size();
		cds.lpData = const_cast<char*>(sstp_a.c_str());

		::SendMessageTimeout(g_hWnd,WM_COPYDATA,reinterpret_cast<WPARAM>(g_hWnd),reinterpret_cast<LPARAM>(&cds),SMTO_ABORTIFHUNG|SMTO_BLOCK,2000,&result);
	}
}

