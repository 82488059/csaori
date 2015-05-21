#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
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
void CSAORI::exec(const CSAORIInput& in,CSAORIOutput& out){
	out.result_code = SAORIRESULT_BAD_REQUEST;
	if ( in.args.size() < 1 ) { return; }

	std::string filepath = checkAndModifyPath(SAORI_FUNC::UnicodeToMultiByte(in.args[0]));

	const char* pFile = filepath.c_str();

	out.result_code = SAORIRESULT_OK;

	//�܂�size
	DWORD tmp;
	DWORD dwVersionSize = ::GetFileVersionInfoSize(pFile,&tmp);
	if ( ! dwVersionSize ) {
		out.result = L"ERROR_OPEN";
	}

	CAutoFree pBuf = malloc(dwVersionSize + 16); //�O�̂���16�o�C�g�قǂ��܂����Ă݂�

	//���擾���C��
	if ( ! ::GetFileVersionInfo(pFile,NULL,dwVersionSize + 16,pBuf) ) {
		out.result = L"ERROR_NORESOURCE";
		return;
	}

	//���̌�̕�����擾�ɔ����Č��ꎯ�ʎq�̎擾
#include <pshpack1.h>
	struct LANGANDCODEPAGE {
	  WORD wLanguage;
	  WORD wCodePage;
	} *pTranslate;
#include <poppack.h>

	UINT size;
	::VerQueryValue(pBuf, "\\VarFileInfo\\Translation", (void**)&pTranslate, &size);

	char langText[10] = "000004b0";
	if ( size ) {
		sprintf(langText,"%04x%04x",pTranslate->wLanguage,pTranslate->wCodePage);
	}

	//��{�I�ȌŒ�T�C�Y���
	UINT verInfoLen;
	VS_FIXEDFILEINFO *verInfo = NULL;
	::VerQueryValue(pBuf,"\\",(void**)&verInfo,&verInfoLen);

	if ( ! verInfoLen || ! verInfo ) {
		out.result = L"ERROR_NOINFO";
		return;
	}

	wchar_t version_str[70];
	swprintf(version_str,L"%u.%u.%u.%u",
		verInfo->dwFileVersionMS >> 16,verInfo->dwFileVersionMS & 0xFFFFU,
		verInfo->dwFileVersionLS >> 16,verInfo->dwFileVersionLS & 0xFFFFU);

	out.result = version_str;

	//������n���
	const char *pLangList[] = {langText,"040904e4"};
	const char *pInfoList[] = {"ProductName","FileDescription","Comments","LegalCopyright","LegalTrademarks"};

	for ( unsigned int lang = 0 ; lang < sizeof(pLangList) / sizeof(pLangList[0]) ; ++lang ) {
		std::string str_file_info_base = std::string("\\StringFileInfo\\") + pLangList[lang] + "\\";
		std::string str_file_info;

		out.values.clear();

		size_t count = 0;
		
		for ( unsigned int i = 0 ; i < sizeof(pInfoList) / sizeof(pInfoList[0]) ; ++i ) {
			str_file_info = str_file_info_base + pInfoList[i];

			UINT verInfoLen;
			char *pText = NULL;
			::VerQueryValue(pBuf,const_cast<char*>(str_file_info.c_str()),(void**)&pText,&verInfoLen);

			if ( verInfoLen && pText ) {
				out.values.push_back(SAORI_FUNC::MultiByteToUnicode(pText));
				++count;
			}
			else {
				out.values.push_back(L"");
			}
		}

		if ( count ) {
			break;
		}
	}
}

