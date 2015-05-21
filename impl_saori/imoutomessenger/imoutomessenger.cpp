#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include "csaori.h"


class ILWindowsDef {
public:
	ILWindowsDef(HWND h,const char *t) : hwnd(h), title(t) { }

	HWND hwnd;
	std::string title;
};

typedef std::vector<ILWindowsDef> ILWindowsList;

class CSaoriIMM : public CSAORI {
private:
	friend static BOOL CALLBACK EnumILWindowsProc(HWND hwnd,LPARAM lParam);

	ILWindowsList m_list;
	int m_mainwnd;
	DWORD m_send_result;

	void UpdateWindowList(void);
	bool EnumProc(HWND hwnd);

	bool SendTo(CSAORIOutput& out,const char_t *string,const char_t *send_to);

public:
	CSaoriIMM() : m_mainwnd(-1) {
	}
	virtual ~CSaoriIMM() {
	}

	virtual void exec(const CSAORIInput& in,CSAORIOutput& out);
	virtual bool unload();
	virtual bool load();
};

CSAORIBase* CreateInstance(void)
{
	return new CSaoriIMM();
}

/*---------------------------------------------------------
	������
---------------------------------------------------------*/
bool CSaoriIMM::load()
{
	return true;
}

/*---------------------------------------------------------
	���
---------------------------------------------------------*/
bool CSaoriIMM::unload()
{
	return true;
}

/*---------------------------------------------------------
	�E�C���h�E��
---------------------------------------------------------*/
static BOOL CALLBACK EnumILWindowsProc(HWND hwnd,LPARAM lParam)
{
	CSaoriIMM *p = reinterpret_cast<CSaoriIMM*>(lParam);
	return p->EnumProc(hwnd);
}

void CSaoriIMM::UpdateWindowList(void)
{
	m_list.clear();
	m_mainwnd = -1;

	::EnumWindows(EnumILWindowsProc,reinterpret_cast<LPARAM>(this));

	if ( m_mainwnd < 0 ) {
		if ( m_list.size() ) {
			m_mainwnd = 0;
		}
	}
}

bool CSaoriIMM::EnumProc(HWND hwnd)
{
	char buf[512];
	::GetWindowText(hwnd,buf,sizeof(buf)-1);

	if ( strncmp(buf,"IMOUTOMAIN_",11) == 0 ) {
		m_list.push_back(ILWindowsDef(hwnd,buf+11));

		if ( strstr(buf,"�������ƃ����`���[") ) {
			m_mainwnd = m_list.size()-1;
		}
	}

	return TRUE;
}

/*---------------------------------------------------------
	���s
---------------------------------------------------------*/
void CSaoriIMM::exec(const CSAORIInput& in,CSAORIOutput& out)
{
	out.result_code = SAORIRESULT_BAD_REQUEST;
	if ( in.args.size() < 1 ) { return; }

	DWORD flag = SND_ASYNC;
	std::string filepath;

	if ( wcsnicmp(in.args[0].c_str(),L"list",4) == 0 ) {
		UpdateWindowList();

		if ( m_list.size() == 0 ) {
			out.result_code = SAORIRESULT_NO_CONTENT;
			return;
		}

		out.result_code = SAORIRESULT_OK;

		string_t txt;

		size_t n = m_list.size();
		string_t title;

		for ( size_t i = 0 ; i < n ; ++i ) {
			title = SAORI_FUNC::MultiByteToUnicode(m_list[i].title.c_str());
			out.values.push_back(title);
			txt += title;
			txt += L"\1";
		}

		if ( txt.size() ) {
			txt.erase(txt.size()-1,1);
		}
		out.result = txt;
	}
	else if ( wcsnicmp(in.args[0].c_str(),L"send",4) == 0 ) {
		if ( in.args.size() < 2 ) {
			return;
		}

		const char_t *send_to = NULL;
		if ( in.args.size() >= 3 ) {
			send_to = in.args[2].c_str();
		}
		SendTo(out,in.args[1].c_str(),send_to);
	}
	else if ( wcsnicmp(in.args[0].c_str(),L"focus",5) == 0 ) {
		if ( in.args.size() < 2 ) {
			return;
		}

		const char_t *send_to = NULL;
		if ( in.args.size() >= 3 ) {
			send_to = in.args[1].c_str();
		}
		SendTo(out,L"$TTPC_FOCUS",send_to);
	}
}

bool CSaoriIMM::SendTo(CSAORIOutput& out,const char_t *string,const char_t *send_to)
{
	UpdateWindowList();

	if ( m_list.size() == 0 ) {
		out.result_code = SAORIRESULT_NO_CONTENT;
		return false;
	}

	HWND hWndToSend = NULL;
	string_t window_title;

	if ( send_to && *send_to ) {
		size_t n = m_list.size();
		std::string find_str = SAORI_FUNC::UnicodeToMultiByte(send_to);

		for ( size_t i = 0 ; i < n ; ++i ) {
			if ( strstr(m_list[i].title.c_str(),find_str.c_str()) ) {
				hWndToSend = m_list[i].hwnd;
				window_title = SAORI_FUNC::MultiByteToUnicode(m_list[i].title.c_str());
				break;
			}
		}
	}
	if ( hWndToSend == NULL ) {
		hWndToSend = m_list[m_mainwnd].hwnd;
		window_title = SAORI_FUNC::MultiByteToUnicode(m_list[m_mainwnd].title.c_str());
	}

	if ( hWndToSend ) {
		out.result_code = SAORIRESULT_OK;
		out.result = window_title;

		std::string text = SAORI_FUNC::UnicodeToMultiByte(string,CP_UTF8);

		COPYDATASTRUCT cp;
		cp.dwData = 0;
		cp.cbData = text.size();
		cp.lpData = const_cast<char*>(text.c_str());

		::SendMessageTimeout(hWndToSend,WM_COPYDATA,reinterpret_cast<WPARAM>(hWndToSend),
			reinterpret_cast<LPARAM>(&cp),SMTO_ABORTIFHUNG | SMTO_NORMAL,1000,&m_send_result);
		return true;
	}
	else {
		out.result_code = SAORIRESULT_NO_CONTENT;
		return false;
	}
}
