----------------------------------------------------------------------
���uCSAORI�v�FSAORI�T���v���N���X
��By CSAORI Project Team
http://code.google.com/p/csaori/
----------------------------------------------------------------------

����������������������������������������������������������������������
������͉���������̂�
����������������������������������������������������������������������

�@SAORI�̃T���v���N���X�ł��B

����������������������������������������������������������������������
�������
����������������������������������������������������������������������

�EVisualStudio.NET2005�@�Ȃ�

����������������������������������������������������������������������
���g�p���@
����������������������������������������������������������������������

���P�D��{�I�\����

CSAORI�́A�ȉ��̍\������Ȃ��Ă��܂��B�i���͔h���j

CSaori_base �N���X�@�@�uSAORI�I�vDLL�C���^�t�F�[�X���K�肷��N���X
��CSaori �N���X�@�@�@ SAORI DLL���K�肷��N���X 
��CPlugin �N���X�@�@�@Plugin DLL���K�肷��N���X

CSAORIInput �N���X�@�@�x�[�X�E�F�A����SAORI�ւ̌Ăяo���p�����[�^��
�@�@�@�@�@�@�@�@�@�@�@�\������N���X
CSAORIOutput�N���X�@�@SAORI����x�[�X�E�F�A�ւ̕ԋp�l��\������N���X

SAORI_FUNC���O��ԁ@�@SAORI�쐬�ɗL�p�Ǝv���郆�[�e�B���e�B�֐���
�@�@�@�@�@�@�@�@�@�@�@�W�߂��N���X

���Q�D�ł���{�I�ȗ��p�@��

�ł��ȒP��CSAORI���g����SAORI DLL�̃\�[�X�R�[�h�́A���L�̂悤�ɂȂ�܂��B
�i������SAORITEST�v���W�F�N�g���Q�l�ɂ��Ă��������j
�P���ȏ����ł���΁ACSAORI�N���X����N���X�̔h�����s���K�v�͂���܂���B

----------------------------------------------------------------------
SaoriTest.cpp
----------------------------------------------------------------------
#include "csaori.h"
bool CSAORI::load(){
	return true;
}
bool CSAORI::unload(){
	return true;
}
void CSAORI::exec(const CSAORIInput& in,CSAORIOutput& out){
	out.result_code = SAORIRESULT_OK;

	out.result = getModulePath();
	out.result += in.args[0];
	out.result += in.args[1];
}
----------------------------------------------------------------------

�i�P�j���܂��Ȃ�

CSAORI�𗘗p����ɂ́A�J������csaori.cpp�Acsaori_base.cpp�A�y�ї��t
�@�C����.h���Q�Ɛݒ肵����ŁA�����ō��\�[�X�t�@�C���ihoge.cpp�Ƃ���
���j�̐擪�ŁAcsaori.h��include���Ă��������B
�܂��A�����N�̐ݒ�ŁAuser32.lib���Q�Ɛݒ肵�Ă��������B

�i�Q�j���p�҂��������Ȃ���΂Ȃ�Ȃ��֐�

���p�҂��������Ȃ���΂Ȃ�Ȃ��֐��́Aload�Aunload�Aexec�ł��B

�@ load

SAORI DLL�����[�h���ꂽ�ۂɌĂяo����܂��B
�������Ȃǂ̏������s���ĉ������B�����ɐ���������Atrue��Ԃ��ĉ������B

�A unload

SAORI DLL���A�����[�h���ꂽ�ۂɌĂяo����܂��B
�I���Ȃǂ̏������s���ĉ������B�����ɐ���������Atrue��Ԃ��ĉ������B

�B exec

SAORI DLL���iSHIORI�Ȃǂ̎��s�n����j���ۂɃ��N�G�X�g���ꂽ�ۂ�
�Ăяo����܂��B
�����Ƃ��� CSAORIInput�ւ̎Q�ƁA�ԋp�l�Ƃ���CSAORIOutput�ւ̎Q�Ƃ�
�����Ă��܂��B
CSAORIInput�����ď������s������ACSAORIOutput��result_code�A�y��
result��K�؂ɐݒ肵�ĉ������B

���R�DCSAORI���s���Ă���邱�Ɓ�

CSAORI�́A�ȉ��̏������s���܂��B

�ESAORI DLL�̃C���^�t�F�[�X�̎���
�ESHIORI�Ȃǂ̎��s�n����̌Ăяo���������CSAORIInput�ɓZ�߂�
�Eexec�������CSAORIOutput��SHIORI�ɕԂ�

�Ȃ��ACSAORI�����ł́A�����R�[�h��UNICODE�iwchar_t�j��p���Ă��܂��B
���s�n����̌Ăяo���ɉ����āAUNICODE�Ƃ̑��ݕϊ����s���܂��̂ŁA
���p�ґ��́Aload,unload,exec�̎����ɂ������ĕ����R�[�h���ӎ�����K�v��
���܂肠��܂���B�S��UNICODE�ł���肳���Ǝv���ĉ������B

���S�DCSAORI�@�ȈՃ��t�@�����X��

�{���́ACSAORI�̃\�[�X������̂ɂ������āA�����ǂ��ɂ��邩�𗝉������
�����ׂ̈ɁA���p�҂��m���Ă����ׂ����Ǝv���鎖���ɂ��ċL�ڂ�������
�ł��B���S�ȃ��t�@�����X�͌��ݑ��݂��܂���̂ŁA�\�[�X�R�[�h���m�F����
���������B

----------------------------------------------------------------------
��string_t,char_t�iCSaori_Base.h�j
----------------------------------------------------------------------

CSAORI�ł͕����y�ѕ������\���̂ɁAchar_t �y�� string_t �Ƃ������O��
���p���Ă��܂��B

----------------------------------------------------------------------
��SAORIRESULT�iCSaori_Base.h�j
----------------------------------------------------------------------

SAORI�̃��U���g�R�[�h���`���Ă��܂��Bexec�̏����ł́A�K��CSAORIOutput
��result_code�ɁA���̗񋓑̂̂ǂꂩ��ݒ肵�Ă��������B

----------------------------------------------------------------------
���O���[�o�� pSaori �I�u�W�F�N�g�iCSaori_base.cpp�y��CSaori.h�j
----------------------------------------------------------------------

CSAORIBase* pSaori;
	CSAORI�̗B��̃I�u�W�F�N�g���i�[����܂��BDLL�����[�h����Ă����
	���̃I�u�W�F�N�g�����݂��A���W���[���p�X���̏���ێ����܂��Bcsa
	ori.h �� extern ����Ă��邽�߁A���p�ґ�����́A�Ⴆ�� pSaori->get
	ModulePath() ���Ƃ��邱�ƂŁACSAORI�I�u�W�F�N�g�̋@�\�ɃA�N�Z�X��
	���܂��B

----------------------------------------------------------------------
��SAORI_FUNC���O��Ԃ̊֐�
----------------------------------------------------------------------

UnicodeToMultiByte
	string_t ��Shift-JIS���̃}���`�o�C�g������ɕϊ����܂��B

MultiByteToUnicode
	��L�̋t�ł��B

CodePagetoString
	Windows�R�[�h�y�[�W���A�����R�[�h��\��������ɕϊ����܂��B

StringtoCodePage
	��L�̋t�ł��B

CP�ԍ�	CODEPAGE		������i�啶���������͖�������܂��j
932		CP_SJIS			Shift_JIS �܂��� x-sjis
50220	CP_ISO2022JP	ISO-2022-JP
20932	CP_EUCJP		EUC-JP �܂��� x-euc-jp
65001	CP_UTF8			UTF-8


getResultString
	SAORI���U���g�R�[�h�ԍ����A�����\��������ɕϊ����܂��B
	�i��j	200	��	OK
			204	��	No Content

getLine
	��������s�P�ʂɐ؂�o���܂��B

intToString
	���l�𕶎���ɕϊ����܂��B

AsyncMessageBox
	�񓯊��Ƀ��b�Z�[�WBOX��\�����܂��B

CCriticalSection
	�N���e�B�J���Z�N�V�����������N���X�ł��B

----------------------------------------------------------------------
��CSAORIInput�N���X�iCsaori_base.h�@�y�� .cpp�j
----------------------------------------------------------------------

vector_str args;
	���N�G�X�g�̈�����string_t�iUNICODE������j�̔z��œ����Ă��܂��B
	args[0] ���Ƃ��ăA�N�Z�X�ł��܂��B


------ �ȉ��̃����o�́A���p�҂͂��܂�ӎ�����K�v�͂���܂���B ------

unsigned int codepage;
	���N�G�X�g�̌��̕����R�[�h�̃R�[�h�y�[�W�ԍ��������Ă��܂��B

string_t cmd;
	GET Version SAORI/1.0��EXECUTE SAORI/1.0�Ƃ������A���N�G�X�g�̍ŏ�
	�̍s�������Ă��܂��B

string_t id;
	���N�G�X�g�� ID: ������΁A���ꂪ�����Ă��܂��B

map_strpair opts;
	���̑��̃��N�G�X�g�w�b�_���Akey-value�y�A�œ����Ă��܂��B

bool parseString(const string_t &src);
	���N�G�X�g���������͂��Aargs��opts���̃����o�[��ݒ肷�鏈���ł��B

----------------------------------------------------------------------
��CSAORIOutput�N���X�iCsaori_base.h�@�y�� .cpp�j
----------------------------------------------------------------------

SAORIRESULT result_code;
	SAORI/1.0 200 OK ���̃��U���g�R�[�h��Ԃ����߂ɁASAORIRESULT�񋓑�
	�̂ǂꂩ��ݒ肵�Ă��������B

string_t result;
	Result: �s�ŕԂ����e��ݒ肵�Ă��������B

vector_str values;
	Values: �s�ŕԂ����e��ݒ肵�Ă��������B

------ �ȉ��̃����o�́A���p�҂͂��܂�ӎ�����K�v�͂���܂���B ------

map_strpair opts;
	Result��Values�ȊO�̃��U���g�w�b�_��ݒ肵�����ꍇ�ɗp���܂��B

unsigned int codepage;
	�����I�Ƀ��N�G�X�g�̕����R�[�h�ɂ��킹�Đݒ肳��܂��B

void setResultEmpty();	
	����CSAORIOutput�I�u�W�F�N�g���A�uSAORI�����ŉ����Ԃ��Ȃ��v��\��
	���U���g��Ԃɂ��܂��B
	result�Avalues�̓N���A����Aresult_code�́i�����ݒ肪�Ȃ���΁j
	204 No Content �ɂȂ�܂��B

string_t toString();
	����CSAORIOutput�I�u�W�F�N�g���A���s�n�ɕԂ�������ɂ��܂��B

----------------------------------------------------------------------
��CSAORIBase�N���X�iCsaori_base.h�@�y�� .cpp�j
----------------------------------------------------------------------

virtual bool load()
	load�֐��ł��B���p�ґ��Ŏ������Ă��������B

virtual bool unload()
	unload�֐��ł��B���p�ґ��Ŏ������Ă��������B

virtual void exec(const CSAORIInput& in,CSAORIOutput& out)
	���ۂ̃��N�G�X�g����������exec�֐��ł��B���p�ґ��Ŏ������Ă��������B

const string_t& getModulePath(void) 
	���s�n����n�����ADLL�����݂���f�B���N�g���p�X���擾���܂��B

HANDLE getModuleHandle(void) 
	DLL���A�^�b�`���ꂽ�ۂ�DLLMain�œn�����HANDLE���擾���܂��B

std::string checkAndModifyPath(const std::string &path);
string_t checkAndModifyPathW(const string_t &path);
	�p�X�\�L�̐��K�����s���܂��BW�̂��Ă���̂�UNICODE�o�[�W�����ł��B

	�E���ϐ��͓W�J����܂��B
	�EDLL�̂���ʒu����̑��΃p�X�\�L�́AgetModulePath�̕ԋp�l�ƌ�������A
	�@�t���p�X�ɂȂ�܂��B

------ �ȉ��̃����o�́A���p�҂͂��܂�ӎ�����K�v�͂���܂���B ------

virtual bool exec_insecure(const CSAORIInput& in,CSAORIOutput& out)
	SecLevel Remote �Ń��N�G�X�g�������ꍇ��exec�֐��ł��B
	�f�t�H���g�ŉ������Ȃ��悤�ɂ��Ă��܂��̂ŁA���̂܂܉������Ȃ����Ƃ�
	�������܂��B

virtual void exec_before(const CSAORIInput& in,CSAORIOutput& out)
	exec�֐������s�����O�̏���������΂����ɋL�ڂ��܂��B

virtual void exec_after(const CSAORIInput& in,CSAORIOutput& out)
	exec�֐������s���ꂽ��̏���������΂����ɋL�ڂ��܂��B


����������������������������������������������������������������������
���z�z������
����������������������������������������������������������������������

�ȉ��̔z�z�����ŋK�肷�郉�C�Z���X�́ACSAORI�T���v���N���X�{�̂ɓK�p
����郉�C�Z���X�ł���ACSAORI��p���č쐬���ꂽ�h��SAORI�́ACSAORI
�Ƃ͈قȂ郉�C�Z���X�ɂȂ��Ă���ꍇ������܂��B�eSAORI�̃��C�Z���X��
�m�F���Ă��������B

�ȉ��̕��͂ɂ����āACSAORI�Ƃ͈ȉ��̃t�@�C�����������̂Ƃ��܂��B

csaori.cpp
csaori.h
csaori_base.cpp
csaori_base.h
cplugin.cpp
cplugin.h
readme.txt�i���̃t�@�C���j


CSAORI�́A�ȉ���2.�Ŗ��L�������C�Z���X�ɏ]���ė��p�ł��܂��B
�������A�u�f���S�[�X�g�v�Ƃ��̎��Ӄv���O�����ɑg�ݍ���ŗ��p����ꍇ�́A
1.�̃��C�Z���X��K�p������̂Ƃ��܂��B

�v����ɁA�S�[�X�g�₻��Ɋ֌W���郆�[�e�B���e�B�ɑg�ݍ���Ŏg���ꍇ�́A
�����C�ɂ��Ȃ��Ă����ł��B

----------------------------------------------------------------
1.�f���S�[�X�g�p���C�Z���X

�g�r�����肹�����R�ɗ��p���邱�Ƃ��o���܂��B

�z�z�p�b�P�[�W�Ɋ܂܂�Ă��邷�ׂẴt�@�C�������R�Ɏ�舵�����Ƃ��o���܂��B
�ꕔ�̃t�@�C���A�������͂��̃t�@�C���̓��e�̈ꕔ�𔲂��o���āA
���̍\���Ɋ܂߂Ă����܂��܂���B 
���̃v���O�������g�p�������ʁA���Ȃ��ɉ��炩�̑��Q���������Ă��A
���̐ӔC����҂͕����܂���B 

----------------------------------------------------------------
2.�C��BSD���C�Z���X

Copyright (C) 2008-, CSaori Project All rights reserved.
http://code.google.com/p/csaori/

�\�[�X�R�[�h�`�����o�C�i���`�����A�ύX���邩���Ȃ������킸�A
�ȉ��̏����𖞂����ꍇ�Ɍ���A�ĔЕz����юg�p��������܂��B

�E�\�[�X�R�[�h���ĔЕz����ꍇ�A��L�̒��쌠�\���A�{�����ꗗ�A
�@����щ��L�Ɛӏ������܂߂邱�ƁB
�E�o�C�i���`���ōĔЕz����ꍇ�A�Еz���ɕt���̃h�L�������g���̎����ɁA
�@��L�̒��쌠�\���A�{�����ꗗ�A����щ��L�Ɛӏ������܂߂邱�ƁB
�E���ʂɂ����ʂ̋��Ȃ��ɁA�{�\�t�g�E�F�A����h���������i�̐�`�܂���
�@�̔����i�ɁA�uCSaori Project�v�̖��O�܂��͍v���҂̖��O���g�p���Ă͂Ȃ�Ȃ��B

�{�\�t�g�E�F�A�́A���쌠�҂���эv���҂ɂ���āu����̂܂܁v�񋟂���Ă���A
�����َ����킸�A���ƓI�Ȏg�p�\���A����ѓ���̖ړI�ɑ΂���K�����Ɋւ���
�Öق̕ۏ؂��܂߁A�܂�����Ɍ��肳��Ȃ��A�����Ȃ�ۏ؂�����܂���B���쌠�҂�
�v���҂��A���R�̂�������킸�A ���Q�����̌�����������킸�A���ӔC�̍���
���_��ł��邩���i�ӔC�ł��邩�i�ߎ����̑��́j�s�@�s�ׂł��邩���킸�A
���ɂ��̂悤�ȑ��Q����������\����m�炳��Ă����Ƃ��Ă��A�{�\�t�g�E�F�A��
�g�p�ɂ���Ĕ��������i��֕i�܂��͑�p�T�[�r�X�̒��B�A�g�p�̑r���A�f�[�^�̑r���A
���v�̑r���A�Ɩ��̒��f���܂߁A�܂�����Ɍ��肳��Ȃ��j���ڑ��Q�A�Ԑڑ��Q�A
�����I�ȑ��Q�A���ʑ��Q�A�����I���Q�A�܂��͌��ʑ��Q�ɂ��āA��ؐӔC��
����Ȃ����̂Ƃ��܂��B

���X�V����

http://code.google.com/p/csaori/wiki/ChangeLog
