#include "Kinsol_Error.h"

void KINSOL_HELPERS::error_kinsol::SetErr(int errCode, std::string errType)
{
	if (!err.ErrLines.empty())
	{
		err.ErrLines.clear();
	}
	err.ErrCode = errCode;
	err.ErrType.assign(errType);
};
