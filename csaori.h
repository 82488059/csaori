/*
 * csaori.h
 * 
 * written by Ukiya http://ukiya.sakura.ne.jp/
 * based by ���т���l "gethwnd.dll"
 */

#pragma once

#include "csaori_base.h"

extern CSAORIBase* pSaori;

class CSAORI : public CSAORIBase {
public:
	CSAORI(){}
	virtual ~CSAORI(){}

	//�C���^�[�t�F�[�X�ŗL������Q
	virtual const string_t& s_saori_version(void) const;
	virtual const string_t& s_saori_def(void) const;
	virtual const string_t& s_saori_argument(void) const;
	virtual const string_t& s_saori_value(void) const;
	virtual const string_t& s_saori_result(void) const;

	//�ȉ����������ׂ��֐�
	virtual void exec(const CSAORIInput& in,CSAORIOutput& out);
	virtual bool unload();
	virtual bool load();
};
