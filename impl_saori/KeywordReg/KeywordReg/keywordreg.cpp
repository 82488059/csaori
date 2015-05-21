#include "csaori.h"

#define BUFLEN 1024*4

class Keyword{
public:
	string_t resultstr;
	std::vector<string_t> keywords;
	bool isHit;
};

std::vector<Keyword> kwds;

string_t _searchkeyword(string_t text);
bool _loadkeywordfile(string_t path);
string_t _toHiragana(string_t input,string_t& out,std::vector<string_t> &all,bool isGetAll=false,bool isReverse=true);
//char* _toHiraganaCore(string input,string_t& out,vector<string_t> &all,bool isGetAll=false,bool isReverse=true);
string_t _getkeyword(string_t text,             string_t& out,string_t& hiragana);
string_t _getkeyword(string_t text,string_t opt,string_t& out,string_t& hiragana);
string_t _imeconvert(string_t in,string_t &res,std::vector<string_t> &val,bool isReverse);

void CSAORI::exec(const CSAORIInput& in,CSAORIOutput& out){
	if(in.args.size()==0){
		out.result=L"";
		out.result_code=SAORIRESULT_BAD_REQUEST;
		return;
	}
	
	//---------------GETKEYWORD
	if(in.args[0].find(L"GETKEYWORD")!=string_t::npos){
		if(in.args.size()<2){
			out.result=L"";
			out.result_code=SAORIRESULT_BAD_REQUEST;
			return;
		}
		string_t result;
		string_t keywords;
		string_t hiragana;
		if(in.args.size()==2){
			result=_getkeyword(in.args[1],keywords,hiragana);
		}else{
			result=_getkeyword(in.args[1],in.args[2],keywords,hiragana);
		}
		if(result.empty()){
			out.result=keywords;
			out.values.push_back(hiragana);
			out.result_code=SAORIRESULT_OK;
			return;
		}else{
			out.result=result;
			out.result_code=SAORIRESULT_INTERNAL_SERVER_ERROR;
			return;
		}
	}

	//---------------GETYOMI
	if(in.args[0].find(L"GETYOMI")!=string_t::npos){
		if(in.args.size()<2){
			out.result=L"";
			out.result_code=SAORIRESULT_BAD_REQUEST;
			return;
		}
		string_t result;

		result=_imeconvert(in.args[1],out.result,out.values,true);
		//out.values��_imeconvert�ŃZ�b�g�����

		if(result.empty()){
			out.result=SAORI_FUNC::intToString((int)out.values.size());
			out.result_code=SAORIRESULT_OK;
			return;
		}else{
			out.result_code=SAORIRESULT_INTERNAL_SERVER_ERROR;
			out.result=result;
			return;
		}
	}

	//---------------GETKANJI
	if(in.args[0].find(L"GETKANJI")!=string_t::npos){
		if(in.args.size()<2){
			out.result=L"";
			out.result_code=SAORIRESULT_BAD_REQUEST;
			return;
		}
		string_t result;

		result=_imeconvert(in.args[1],out.result,out.values,false);
		//out.values��_imeconvert�ŃZ�b�g�����

		if(result.empty()){
			out.result=SAORI_FUNC::intToString((int)out.values.size());
			out.result_code=SAORIRESULT_OK;
			return;
		}else{
			out.result_code=SAORIRESULT_INTERNAL_SERVER_ERROR;
			out.result=result;
			return;
		}
	}

	out.result=L"";
	out.result_code=SAORIRESULT_BAD_REQUEST;
	return;
}

bool CSAORI::unload(){
	return true;
}

bool CSAORI::load(){
	return _loadkeywordfile(checkAndModifyPathW(L"keyword.txt"));
}

//---------------------�n���h�����O

string_t _getkeyword(string_t text,string_t& out,string_t& hiragana){return _getkeyword(text,L"",out,hiragana);}
string_t _getkeyword(string_t text,string_t  opt,string_t& resultout,string_t& hiraganaout){
	string_t hiragana;
	string_t result;
	std::vector<string_t> all;
	bool isConvert=true;

	if(opt.find(L"NOCONVERT")!=string_t::npos){
		isConvert=false;
	}

	if(isConvert){
		result=_toHiragana(text,hiragana,all);
		if(!result.empty()){
			return result;
		}
	}else{
		hiragana=text;
	}

	result=_searchkeyword(hiragana);
	if(!result.empty()){
		return result;
	}

	string_t out=L"";
	for(unsigned int i=0;i<kwds.size();i++){
		if(kwds[i].isHit){
			out+=kwds[i].resultstr+L"�@";
		}
	}
	if(!out.empty()){
		out.erase(out.length()-1,1);//�I�[�󔒍폜
	}


	//����
	resultout=out;

	//�Ђ炪��
	hiraganaout=hiragana;

	return L"";
}


string_t _imeconvert(string_t in,string_t &res,std::vector<string_t> &val,bool isReverse){
	string_t result;
	string_t hiragana;
	result=_toHiragana(in,hiragana,val,true,isReverse);
	if(!result.empty()){
		return result;
	}
	char_t buf[128];
	swprintf_s(buf,sizeof(buf)/sizeof(buf[0]),L"%d",val.size());
	res.assign(buf,wcslen(buf));
	return L"";
}


string_t _searchkeyword(string_t text){
	if(text.length()==0){
		return L"���������񂪂���܂���";
	}
	if(kwds.size()==0){
		return L"�L�[���[�h���P���o�^����Ă��܂���";
	}
	for(unsigned int i=0;i<kwds.size();i++){
		kwds[i].isHit=false;
	}
	for(unsigned int i=0;i<kwds.size();i++){
		if(kwds[i].keywords.size()==0){
			continue;
		}
		for(unsigned int j=0;j<kwds[i].keywords.size();j++){
			string_t key=kwds[i].keywords[j];
			if(text.find(key)!=string_t::npos){
				kwds[i].isHit=true;
				break;
			}
		}
	}
	return L"";
}


bool _loadkeywordfile(string_t path){
	FILE* fp;
	char buf[BUFLEN];
	string_t ucstr;
	char_t ucbuf[BUFLEN*3];
	if(	_wfopen_s(&fp,path.c_str(),L"rt") != 0){
		return false;
	}

	while(fgets(buf,sizeof(buf),fp)!=NULL){
		if(buf[strlen(buf)-1]=='\n'){
			buf[strlen(buf)-1]='\0';
		}
		if(strlen(buf)==0){
			continue;
		}
		ucstr=SAORI_FUNC::MultiByteToUnicode(std::string(buf),SAORI_FUNC::StringtoCodePage("Shift_JIS"));
		wcscpy_s(ucbuf,BUFLEN,ucstr.c_str());
		if(ucbuf[0]==L'��'||ucbuf[0]==L'#'){
			continue;
		}
		char_t* context;
		char_t* token=wcstok_s(ucbuf,L"��=",&context);
		if(wcslen(token)==0){
			continue;
		}

		Keyword kwd;
		kwd.resultstr=string_t(token);

		while(1){
			token=wcstok_s(NULL,L"�A��C,",&context);
			if(token==NULL){
				break;
			}
			kwd.keywords.push_back(token);
		}
		if(kwd.keywords.size()!=0){
			kwds.push_back(kwd);
		}
	}
	fclose(fp);
	return true;
}

string_t _toHiragana(string_t input,string_t& out,std::vector<string_t> &all,bool isGetAll,bool isReverse){
	string_t str;
	HIMC hIMC=NULL;
	HKL hKL=NULL;
	DWORD dwSize=0;
	LPCANDIDATELIST lpCand=NULL;
	string_t errstr;
	char_t ebuf[1024];

	OSVERSIONINFOA os;
	bool isWin9x=false;


	hIMC = ImmCreateContext();
	hKL = GetKeyboardLayout(0);
	UINT result;

	if(hIMC==NULL){
		errstr=L"ImmCreateContext Failed";
		goto END;
	}
	if(hKL==NULL){
		errstr=L"GetKeyboardLayout Failed";
		goto END;
	}

	os.dwOSVersionInfoSize=sizeof(os);
	GetVersionExA(&os);

	//windows98�n�́AImmGetConversionListW���g���āAShiftJIS���́�Unicode�o��
	//����ȊO�́A   ImmGetConversionListW���g���āAUnicode���́�Unicode�o��
	//�Ȃ񂾂��̃A�z�Ȏd�l�I
	void* tmp_input;
	if(os.dwPlatformId==VER_PLATFORM_WIN32_NT){
		//NT�n
		tmp_input=(void*)input.c_str();
	}else{
		//98�n
		tmp_input=(void*) &(SAORI_FUNC::UnicodeToMultiByte(input,SAORI_FUNC::StringtoCodePage("Shift_JIS")));
		isWin9x=true;
	}

	UINT uFlag1;
	UINT uFlag2;
	if(isReverse){
		uFlag1=GCL_REVERSE_LENGTH;
		uFlag2=GCL_REVERSECONVERSION;
	}else{
		uFlag1=GCL_CONVERSION;
		uFlag2=GCL_CONVERSION;
	}

	dwSize = ImmGetConversionListW(hKL, hIMC, (LPCWSTR)tmp_input, NULL, 0,   uFlag1);
	if(dwSize==0 || dwSize>=0x7FFF8){
		swprintf_s(ebuf,sizeof(ebuf)/sizeof(ebuf[0]),L"ImmGetConversionListW1 Failed %p %p %s %d %d",hKL,hIMC,tmp_input,uFlag1,dwSize);
		errstr.assign(ebuf);
		goto END;
	}
	lpCand = (LPCANDIDATELIST)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,   dwSize);
	if(lpCand==NULL){
		swprintf_s(ebuf,sizeof(ebuf)/sizeof(ebuf[0]),L"HeapAlloc Failed %p %d %s %d",GetProcessHeap(),dwSize);
		errstr.assign(ebuf);
		goto END;
	}
	result=ImmGetConversionListW(hKL, hIMC, (LPCWSTR)tmp_input , lpCand, dwSize,   uFlag2);
	if(result==0){
		swprintf_s(ebuf,sizeof(ebuf)/sizeof(ebuf[0]),L"ImmGetConversionListW2 Failed %p %p %s %d %d",hKL,hIMC,(LPCWSTR)tmp_input,dwSize,uFlag2);
		errstr.assign(ebuf);
		goto END;
	}
	if(lpCand->dwCount==0){
		errstr=L"result candiate is 0";
		goto END;
	}

	//�����܂ŗ���ΐ���
	char_t* pResult=(wchar_t*)((LPBYTE)lpCand + lpCand->dwOffset[0]);
	str.assign(pResult);

	if(isGetAll){
		for(unsigned int i=0;i<lpCand->dwCount;i++){
			string_t allstr;
			char_t* pAll=(wchar_t*)((LPBYTE)lpCand + lpCand->dwOffset[i]);
			allstr.assign(pAll);
			all.push_back(allstr);
		}
	}

END:
	if(hIMC!=NULL){
		ImmReleaseContext(NULL, hIMC);
	}
	if(lpCand!=NULL){
		HeapFree(GetProcessHeap(), 0, lpCand);
	}
	//�J�^�J�i��������Ђ炪�Ȃɂ���
	if(errstr.empty()){
		int len=LCMapStringW(GetUserDefaultLCID(),LCMAP_FULLWIDTH|LCMAP_HIRAGANA,str.c_str(),-1,NULL,0);
		char_t* pBuf=(char_t*)malloc(sizeof(char_t)*len+1);
		LCMapStringW(GetUserDefaultLCID(),LCMAP_FULLWIDTH|LCMAP_HIRAGANA,str.c_str(),-1,pBuf,len);
		out.assign(pBuf);
	}
	return errstr;
}

//IME���p�ŕ������ɕϊ����鐬�������グ�鎎�݂̎c�[
/*
char* _toHiragana(string input,string_t& out,vector<string_t> &all,bool isGetAll,bool isReverse){
	char* result=_toHiraganaCore(input,out,all,isGetAll,isReverse);
	if(result!=NULL){
		string_t temp=_string2string_t(input);
		string_t temp2;
		//�_���������犿���Ƃ����łȂ��̂Ƃŋ�؂��Ă݂�
		bool isKanji=false;
		bool isInsertSpace=false;
		for(unsigned int i=0;i<temp.length();i++){
			isInsertSpace=false;
			if(temp[i]>0x4e00 && temp[i]<0x9fa5){
				//����
				if(!isKanji){isInsertSpace=true;}
				isKanji=true;
			}else{
				//�񊿎�
				if(isKanji){isInsertSpace=true;}
				isKanji=false;
			}
			if(isInsertSpace){
				temp2+=_T('�@');
			}
			temp2+=temp[i];
		}
		string temp3=_string_t2string(temp2);
		result=_toHiraganaCore(temp3,out,all,isGetAll,isReverse);
		if(result!=NULL){
			//��������Ă�����
			return result;
		}
	}
	return NULL;
}
*/
