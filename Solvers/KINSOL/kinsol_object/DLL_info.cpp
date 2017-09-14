#include "Kinsol_Class.h"
#include "kinsol_dll.h"

void IHFUN(const char *module, const char *function, char *msg, void *user_data)
{

//	CallbackInfo infolocal;
	KINInfoFuncData *data;

	infomsgs info;

	data = (KINInfoFuncData*)user_data;
//	infolocal = NULL;

//	infolocal = data->UserInfoFuncPtr;


	info.module = module;
	info.function = function;
	info.msg = msg;

	data->UserInfoFuncPtr(&info, 0);
};
