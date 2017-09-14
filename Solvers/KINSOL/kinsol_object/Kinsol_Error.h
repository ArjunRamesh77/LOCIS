#ifndef KINSOL_ERROR_H
#define KINSOL_ERROR_H

#include <string>
#include <vector>

namespace KINSOL_HELPERS
{

	struct error
	{

		int ErrCode;
		std::string ErrType;
		std::vector<std::string> ErrLines;
	};

	class error_kinsol
	{

		error err;
		std::vector<error> errors;

	public:

		void SetErr(int errCode, std::string errType);

		void AddErrorLine(std::string line);

		void AddErrortovector();

		error GetLastError();

		~error_kinsol();

	};


}
#endif
