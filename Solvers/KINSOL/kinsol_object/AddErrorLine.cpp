#include "Kinsol_Error.h"

void KINSOL_HELPERS::error_kinsol::AddErrorLine(std::string line)
{

	err.ErrLines.push_back(line);
};

// Atharv added a comment
