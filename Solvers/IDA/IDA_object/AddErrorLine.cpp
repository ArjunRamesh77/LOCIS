#include "IDA_Error.h"

void IDA_HELPERS::error_IDA::AddErrorLine(std::string line)
{

	err.ErrLines.push_back(line);
};

// Atharv added a comment
