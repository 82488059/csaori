// AqTalkSaori.cpp : DLL �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "csaori.h"
#include <Mmsystem.h>

typedef enum {M_NOP,M_PLAYSYNC,M_CREATEWAV,M_PLAYASYNC,M_STOP,M_CHANGEVOICE} METHOD;

HMODULE hAq;

bool _LoadAqLib(string_t dllname);
bool _UnloadAqLib();
SAORIRESULT DoWav(METHOD method,string_t& errbuf,const std::vector<string_t> &arg);
void _unloadwavbuf();


#define AQDLL	L"AquesTalk.dll"

typedef unsigned char* (__stdcall *AquesTalk_Synthe_type)(const char *koe, int iSpeed, int *size);
typedef void (__stdcall *AquesTalk_FreeWave_type)(unsigned char *wav);

AquesTalk_Synthe_type AquesTalk_Synthe;
AquesTalk_FreeWave_type AquesTalk_FreeWave;

#define ERRBUFSIZE 1024

unsigned char* wavbuf;

//-------------------------------
//CSAORI�֐�
//-------------------------------

void CSAORI::exec(const CSAORIInput& in,CSAORIOutput& out){
	if(in.args.size()==0){
		out.result=L"ERROR001�F����������܂���";
		out.result_code=SAORIRESULT_BAD_REQUEST;
		return;
	}

	METHOD method=M_NOP;

	if(in.args[0].find(L"PLAYSYNC")!=string_t::npos){
		method=M_PLAYSYNC;
	}else if(in.args[0].find(L"CREATEWAV")!=string_t::npos){
		method=M_CREATEWAV;
	}else if(in.args[0].find(L"PLAYASYNC")!=string_t::npos){
		method=M_PLAYASYNC;
	}else if(in.args[0].find(L"STOP")!=string_t::npos){
		method=M_STOP;
	}else if(in.args[0].find(L"CHANGEVOICE")!=string_t::npos){
		method=M_CHANGEVOICE;
	}

	SAORIRESULT result;
	string_t err;
	switch(method){
		case M_PLAYSYNC:
		case M_CREATEWAV:
		case M_PLAYASYNC:
			result=DoWav(method,err,in.args);
			out.result=err;
			out.result_code=result;
			return;
			break;
		case M_STOP:
			_unloadwavbuf();
			out.result=L"";
			out.result_code=SAORIRESULT_OK;
			return;
			break;
		case M_CHANGEVOICE:
			if(in.args.size()<2){
				out.result=L"ERROR301�FDLL�t�@�C�����w�肳��Ă��܂���";
				out.result_code=SAORIRESULT_BAD_REQUEST;
				return;
			}
			_UnloadAqLib();
			if(!_LoadAqLib(in.args[1])){
				out.result=L"ERROR302�FDLL�t�@�C���̃��[�h�Ɏ��s���܂����i"+in.args[1]+L"�j";
				out.result_code=SAORIRESULT_INTERNAL_SERVER_ERROR;
				return;
			}
			out.result=L"";
			out.result_code=SAORIRESULT_OK;
			return;
			break;
		default:
			out.result=L"ERROR003�F�s���Ȗ��߂ł�";
			out.result_code=SAORIRESULT_BAD_REQUEST;
			return;
			break;
	}

	out.result=L"";
	out.result_code=SAORIRESULT_INTERNAL_SERVER_ERROR;
	return;//�����ɂ͂��Ȃ�
}


bool CSAORI::load(){
	wavbuf=NULL;
	hAq=NULL;
	bool result=_LoadAqLib(AQDLL);
	return result;
}

bool CSAORI::unload(){
	_unloadwavbuf();
	bool result=_UnloadAqLib();
	return result;
}

//-------------------------------
//�����֐�
//-------------------------------

//�����n�֐�
SAORIRESULT DoWav(METHOD method,string_t& err,const std::vector<string_t> &arg){
	int size;

	if(hAq==NULL){
		err=L"ERROR999�FAquesDLL���[�h�Ɏ��s���Ă��܂�";
		return SAORIRESULT_INTERNAL_SERVER_ERROR;
	}

	if(arg.size()<2){
		err=L"ERROR002�F�Œ�Q�ȏ�̈������w�肵�Ă�������";
		return SAORIRESULT_BAD_REQUEST;
	}

	std::string koe=SAORI_FUNC::UnicodeToMultiByte(arg[1],SAORI_FUNC::StringtoCodePage("shift_jis"));
	int speed=100;
	if(arg.size()>2){
		speed=_wtoi(arg[2].c_str());
		if(speed<50 || speed>300){
			speed=100;
		}
	}

	unsigned char* wav=AquesTalk_Synthe(koe.c_str(),speed,&size);
	if(wav==0 || size<=0){
		err=L"ERROR100�FSynthe�̃G���[(" + SAORI_FUNC::intToString((int)wav)+ L")";
		return SAORIRESULT_INTERNAL_SERVER_ERROR;
	}
	_unloadwavbuf();
	wavbuf=(unsigned char*)malloc(size);
	if(wavbuf==NULL){
		err=L"ERROR101�Fmalloc�̃G���[";
		return SAORIRESULT_INTERNAL_SERVER_ERROR;
	}
	memcpy(wavbuf,wav,size);
	AquesTalk_FreeWave(wav);

	BOOL res;
	switch(method){
		case M_PLAYASYNC:
			res=::PlaySound((LPCWSTR)wavbuf,NULL,SND_ASYNC|SND_MEMORY);
			break;
		case M_PLAYSYNC:
			res=::PlaySound((LPCWSTR)wavbuf,NULL,SND_SYNC|SND_MEMORY|SND_NOWAIT);
			break;
		case M_CREATEWAV:
			if(arg.size()<4){
				err=L"ERROR201�F�t�@�C�������w�肳��Ă��܂���";
				return SAORIRESULT_BAD_REQUEST;
			}
			string_t filepath = pSaori->checkAndModifyPathW(arg[3]);

			
			FILE* outfp;
			errno_t result=_wfopen_s(&outfp,filepath.c_str(),L"wb");
			if(result!=0){
				err=L"ERROR202�F�t�@�C�����J���̂Ɏ��s���܂����i"+SAORI_FUNC::intToString((int)result)+L"�j";
				return SAORIRESULT_INTERNAL_SERVER_ERROR;
			}
			fwrite(wavbuf,1,size,outfp);
			fclose(outfp);
			break;
	}

	err=L"";
	return SAORIRESULT_OK;
}



void _unloadwavbuf(){
	if(wavbuf!=NULL){
		::PlaySound(NULL,NULL,SND_MEMORY|SND_ASYNC);
		wavbuf=NULL;
	}
}

bool _LoadAqLib(string_t dllname){
	hAq=NULL;
	string_t aqdll_path=pSaori->getModulePath()+dllname;

	hAq=::LoadLibrary(aqdll_path.c_str());
	if(hAq==NULL){
		_UnloadAqLib();
		return false;
	}
	AquesTalk_Synthe=(AquesTalk_Synthe_type)::GetProcAddress(hAq,"AquesTalk_Synthe");
	if(AquesTalk_Synthe==NULL){
		_UnloadAqLib();
		return false;
	}
	AquesTalk_FreeWave=(AquesTalk_FreeWave_type)::GetProcAddress(hAq,"AquesTalk_FreeWave");
	if(AquesTalk_FreeWave==NULL){
		_UnloadAqLib();
		return false;
	}
	return true;
}

bool _UnloadAqLib(){
	if(hAq!=NULL){
		::FreeLibrary(hAq);
	}
	return true;
}
