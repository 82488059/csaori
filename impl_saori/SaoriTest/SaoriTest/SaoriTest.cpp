#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#endif

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
	���s
---------------------------------------------------------*/
void CSAORI::exec(const CSAORIInput& in,CSAORIOutput& out){
	out.result_code = SAORIRESULT_OK;

	out.result = getModulePath();
	out.result += in.args[0];
	out.result += in.args[1];
}

