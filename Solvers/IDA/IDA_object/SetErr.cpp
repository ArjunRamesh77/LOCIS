#include "IDA_Error.h"

void IDA_HELPERS::error_IDA::SetErr(int errCode, std::string errType)
{

	err.ErrLines.clear();
	err.ErrCode = errCode;
	err.ErrType.assign(errType);
};
