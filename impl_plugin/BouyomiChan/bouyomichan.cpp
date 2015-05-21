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
#include <process.h>
#include <winsock.h>
#include <new>

#include "bouyomichan.h"

//////////WINDOWS DEFINE///////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
///////////////////////////////////////////////////

/*===============================================================
	�C���X�^���X�쐬�icsaori_base����Ă΂��j
===============================================================*/

CSAORIBase* CreateInstance(void)
{
	return new CBouyomiChan();
}

/*===============================================================
	������(DllMain����)
===============================================================*/

CBouyomiChan::CBouyomiChan(void) : thread_active(true), speak_type(0)
{
}

CBouyomiChan::~CBouyomiChan()
{
}

/*===============================================================
	������(DllMain�Ƃ͕�)
===============================================================*/
static unsigned int __stdcall BYMThreadProc(void* pv)
{
	CBouyomiChan *pb = reinterpret_cast<CBouyomiChan*>(pv);
	pb->ThreadProc();
	return 0;
}

bool CBouyomiChan::load()
{
	WSADATA wsaData;
	::WSAStartup(MAKEWORD(1,0),&wsaData);

	thread_event = ::CreateEvent(NULL,FALSE,FALSE,NULL);
	while ( ! thread_queue.empty() ) {
		thread_queue.pop_front();
	}
	thread_active = true;
	thread_handle = reinterpret_cast<HANDLE>(_beginthreadex(NULL,0,BYMThreadProc,this,0,NULL));

	ini_file_path = SAORI_FUNC::UnicodeToMultiByte(getModulePath().c_str());
	ini_file_path += "profile\\plugin.ini";

	speak_type = ::GetPrivateProfileIntA("BouyomiChan","SpeakType",0,ini_file_path.c_str());
	if ( speak_type > 2 ) { speak_type = 2; }

	return true;
}

bool CBouyomiChan::unload()
{
	thread_active = false;

	::SetEvent(thread_event);
	if ( ::WaitForSingleObject(thread_handle,5000) == WAIT_TIMEOUT ) {
		::TerminateThread(thread_handle,0);
	}
	
	::CloseHandle(thread_handle);
	thread_handle = NULL;

	::CloseHandle(thread_event);
	thread_event = NULL;

	::WSACleanup();

	return true;
}

/*===============================================================
	�ʐM��
===============================================================*/
#include <pshpack1.h>
typedef struct tagBouyomiChanOption {
	short command;
	short speed;
	short tone;
	short volume;
	short voice;
	char charset;
	long len;
} BouyomiChanOption;
#include <poppack.h>

void CBouyomiChan::ThreadProc()
{
	while ( thread_active ) {
		::WaitForSingleObject(thread_event,INFINITE);


		{ //���b�N�J���p�u���[�X
			SAORI_FUNC::CCriticalSectionLock lock(thread_lock);
			CBouyomiChanData sendstr;
			std::string utf8str;
			std::vector<char> buffer;

			while ( (!thread_queue.empty()) && thread_active ) {
				sendstr = *(thread_queue.begin());
				thread_queue.pop_front();

				SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
				if ( sock == INVALID_SOCKET ) { continue; }
				
				sockaddr_in server;
				server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
				server.sin_port             = htons(50001);
				server.sin_family           = AF_INET;

				if ( connect(sock, (struct sockaddr *)&server, sizeof(server)) != 0 ) { continue; }
				utf8str = SAORI_FUNC::UnicodeToMultiByte(sendstr.str.c_str(),CP_UTF8);

				buffer.assign(utf8str.length()+1+15,0);

				strcpy(&(buffer[15]),utf8str.c_str());
				BouyomiChanOption *opt = reinterpret_cast<BouyomiChanOption*>(&(buffer[0]));
				opt->command = 1; //1:���b�Z�[�W�ǂݏグ
				opt->speed = -1;
				opt->tone = -1;
				opt->volume = -1;
				opt->charset = 0;
				opt->len = utf8str.length();

				if ( sendstr.ctx == 0 ) {
					if ( speak_type == 0 ) {
						opt->voice = 1;
					}
					else if ( speak_type == 1 ) {
						opt->voice = 3;
					}
					else /*if ( speak_type == 2 ) */ {
						opt->voice = 7;
					}
				}
				else if ( sendstr.ctx == 1 ) {
					if ( speak_type == 0 ) {
						opt->voice = 2;
					}
					else if ( speak_type == 1 ) {
						opt->voice = 4;
					}
					else /*if ( speak_type == 2 ) */ {
						opt->voice = 8;
					}
				}
				else {
					opt->voice = 5;
				}

				send(sock,&(buffer[0]),utf8str.length()+15,0);
				closesocket(sock);
			}
		}
	}
}

/*===============================================================
	���s��
===============================================================*/
static char_t* SplitCharTalk(char_t *scr,int &ctx)
{
	char_t* ptr = NULL;
	char_t* next = NULL;

	ctx = 0;

	char_t* s = wcsstr(scr,L"\\0");
	if ( s && (!ptr || ptr > s) ) {
		ptr = s;
		next = s + 2;
		ctx = 0;
	}

	s = wcsstr(scr,L"\\1");
	if ( s && (!ptr || ptr > s) ) {
		ptr = s;
		next = s + 2;
		ctx = 1;
	}

	s = wcsstr(scr,L"\\h");
	if ( s && (!ptr || ptr > s) ) {
		ptr = s;
		next = s + 2;
		ctx = 0;
	}

	s = wcsstr(scr,L"\\u");
	if ( s && (!ptr || ptr > s) ) {
		ptr = s;
		next = s + 2;
		ctx = 1;
	}

	s = wcsstr(scr,L"\\p");
	if ( s && (!ptr || ptr > s) ) {
		ptr = s;
		next = s + 2;
		if ( *next == L'[' ) {
			ctx = wcstoul(next+1,NULL,10);
			char_t *end = wcschr(next+1,L']');
			if ( end ) {
				next = end+1;
			}
			else {
				next = NULL;
			}
		}
		else if ( *next >= L'0' && *next <= L'9' ) {
			ctx = (*next - L'0');
			++next;
		}
	}

	if ( next && *next == 0 ) {
		next = NULL;
	}
	if ( ptr ) {
		*ptr = 0;
	}
	return next;
}

static void ClearTag(char_t *t,int &inQuickSection)
{
	if ( inQuickSection ) {
		char_t *last = wcsstr(t,L"\\_q");
		if ( last ) {
			memmove(t,last+3,(wcslen(last+3)+1)*sizeof(char_t)); //�[���I�[���R�s�[
			inQuickSection = false;
		}
		else {
			t[0] = 0;
			return;
		}
	}

	size_t len = wcslen(t);

	for ( size_t i = 0 ; i < len ; ++i ) {
		if ( t[i] == L'\\' ) {
			if ( t[i+1] == L'\\' ) {
				memcpy(t+i,t+i+1,(len-i/*-1+1 ZERO��*/)*sizeof(char_t));
				len -= 1;
			}
			else {
				size_t j = i+1;
				size_t uscount = 0;
				while ( t[j] == L'_' ) { ++j; ++uscount; } //��s�A���_�[�X�R�A

				char_t tagc = t[j];
				if ( iswalnum(tagc) || tagc == '!' || tagc == '?' ) { //�^�O������
					++j;
					if ( tagc == L'q' && uscount == 1 ) { //�N�C�b�N
						char_t *last = wcsstr(t+j,L"\\_q");
						if ( last ) {
							j = (last-t)+3;
						}
						else {
							inQuickSection = true;
							t[i] = 0; //�����ŃJ�b�g
							return;
						}
					}
					else if ( t[j] == L'[' ) { //�p�����[�^��������
						++j;
						bool in_quote = false;
						while ( true ) {
							if ( t[j] == 0 ) { break; }
							if ( in_quote ) {
								if ( t[j] == L'"' ) {
									in_quote = false;
								}
							}
							else {
								if ( t[j] == L'"' ) {
									in_quote = true;
								}
								else if ( t[j] == L']' ) {
									++j;
									break;
								}
							}
							++j;
						}
					}
					else if ( t[j] >= L'0' && t[j] <= L'9' ) {
						if ( tagc == L'w' || tagc == L's' ) {
							++j;
						}
					}
				}
				if ( len == j ) {
					*(t+i) = 0;
					len = 0;
				}
				else {
					memmove(t+i,t+j,(len-j+1/*+1����ZERO��*/)*sizeof(char_t));
					len -= (j-i);
					i -= 1;
				}
			}
		}
	}
}

void CBouyomiChan::ShowMenu(CSAORIOutput& out)
{
	out.result = 
		L"\\0\\_q\\t\\f[bold,true]<�_�ǂ݂���񑗐M�v���O�C��>\\f[bold,default]\\n\\n[half]"
		L"�������鐺���̐ݒ�\\n\\n[half]";

	char_t* result[3] = {L"��",L"��",L"��"};
	result[speak_type] = L"��";

	out.result += string_t(L"\\![*]\\q[") + result[0] + L"�����D��,female]\\n";
	out.result += string_t(L"\\![*]\\q[") + result[1] + L"�j���D��,male]\\n";
	out.result += string_t(L"\\![*]\\q[") + result[2] + L"�@�B���D��,machine]\\n\\n[half]";

	out.result += L"\\![*]\\q[�~����,close]\\_q\\e";

	char tmp[32];
	sprintf(tmp,"%d",speak_type);
	::WritePrivateProfileStringA("BouyomiChan","SpeakType",tmp,ini_file_path.c_str());

	out.result_code = SAORIRESULT_OK;
}

void CBouyomiChan::exec(const CSAORIInput& in,CSAORIOutput& out)
{
	//--------------------------------------------------------
	if ( wcsicmp(in.id.c_str(),L"OnMenuExec") == 0 ) {
		ShowMenu(out);
		return;
	}
	//--------------------------------------------------------
	if ( wcsicmp(in.id.c_str(),L"OnChoiceSelect") == 0 ) {
		if ( in.args.size() ) {
			if ( wcsicmp(in.args[0].c_str(),L"female") == 0 ) {
				speak_type = 0;
			}
			else if ( wcsicmp(in.args[0].c_str(),L"male") == 0 ) {
				speak_type = 1;
			}
			else if ( wcsicmp(in.args[0].c_str(),L"machine") == 0 ) {
				speak_type = 2;
			}
			else if ( wcsicmp(in.args[0].c_str(),L"close") == 0 ) {
				out.result = L"\\0\\b[-1]\\e";
				out.result_code = SAORIRESULT_OK;
				return;
			}
			else {
				out.result = SAORIRESULT_NO_CONTENT;
				return;
			}
		}
		ShowMenu(out);
		return;
	}
	//--------------------------------------------------------
	if ( wcsicmp(in.id.c_str(),L"OnOtherGhostTalk") == 0 ) {
		//�_�ǂ݂����N���`�F�b�N
		OSVERSIONINFO inf;
		inf.dwOSVersionInfoSize = sizeof(inf);
		::GetVersionEx(&inf);

		HANDLE hMutex = NULL;
		if ( inf.dwMajorVersion >= 5 ) {
			hMutex = ::CreateMutex(NULL,FALSE,"Local\\BouyomiChan");
			if ( ! hMutex ) {
				hMutex = ::CreateMutex(NULL,FALSE,"Local\\�_�ǂ݂����");
			}
		}
		if ( ! hMutex ) {
			hMutex = ::CreateMutex(NULL,FALSE,"BouyomiChan");
			if ( ! hMutex ) {
				hMutex = ::CreateMutex(NULL,FALSE,"�_�ǂ݂����");
			}
		}
		DWORD err = ::GetLastError();

		if ( ! hMutex ) {
			return;
		}
		::CloseHandle(hMutex);

		if ( err != ERROR_ALREADY_EXISTS ) { //ERROR_ALREADY_EXISTS�̎��i���m�F�����łɂ���j�̂ݏ���
			return;
		}

		//�X�N���v�g��Ref4
		if ( in.args.size() < 5 ) {
			return;
		}
		const string_t &str = in.args[4];

		char_t *buf = (char_t*)malloc((str.size()+1)*sizeof(char_t));
		wcscpy(buf,str.c_str());

		char_t *split = buf;
		char_t *next = split;
		size_t count = 0;
		int ctx = 0;
		int lastctx = 0;
		int inQuickSection = false;

		do {
			next = SplitCharTalk(split,ctx);
			ClearTag(split,inQuickSection);
			if ( *split == 0 ) {
				if ( next ) {
					split = next;
					lastctx = ctx;
					continue;
				}
				else {
					break;
				}
			}
			{ //�J���p�u���[�X
				SAORI_FUNC::CCriticalSectionLock lock(thread_lock);
				thread_queue.push_back(CBouyomiChanData(string_t(split),lastctx));
				++count;
			}
			split = next;
			lastctx = ctx;
		} while ( split );

		free(buf);

		if ( count ) {
			::SetEvent(thread_event);
		}
		return;
	}
	//--------------------------------------------------------
}

