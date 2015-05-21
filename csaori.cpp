//setlocale//
#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma setlocale("japanese")
#endif
#endif
//setlocale end//

/*
 * csaori.cpp
 */

#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#pragma warning( disable : 4996 )
#endif

#include "csaori.h"

//////////WINDOWS DEFINE///////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
///////////////////////////////////////////////////

const string_t& CSAORI::s_saori_version(void) const
{
	static string_t s = L"SAORI/1.0";
	return s;
}
const string_t& CSAORI::s_saori_def(void) const
{
	static string_t s = L" SAORI/";
	return s;
}
const string_t& CSAORI::s_saori_argument(void) const
{
	static string_t s = L"Argument";
	return s;
}
const string_t& CSAORI::s_saori_value(void) const
{
	static string_t s = L"Value";
	return s;
}
const string_t& CSAORI::s_saori_result(void) const
{
	static string_t s = L"Result";
	return s;
}

//CUSTOM_CREATE_INSTANCE���`����ƁACreateInstance�����O��
//�p�ӂ��Ȃ���΂Ȃ�Ȃ�����ɁA�h���N���X�ɂ��f�[�^�̐����Ȃǂ�
//���R�ɍs�����Ƃ��ł���悤�ɂȂ�܂��B

#ifndef CUSTOM_CREATE_INSTANCE

//�݊��p�FCSAORI�t�@�N�g���[�֐�

CSAORIBase* CreateInstance(void)
{
	return new CSAORI();
}

#else //CUSTOM_CREATE_INSTANCE

//�h���N���X�ɂăI�[�o�[���[�h���邽�߂̃_�~�[�֐�

void CSAORI::exec(const CSAORIInput& in,CSAORIOutput& out) {
	return;
}
bool CSAORI::unload() {
	return true;
}
bool CSAORI::load() {
	return true;
}

#endif //CUSTOM_CREATE_INSTANCE

