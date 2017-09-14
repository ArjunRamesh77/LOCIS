#ifndef IDA_ERROR_H
#define IDA_ERROR_H

#include <string>
#include <vector>

namespace IDA_HELPERS
{
	struct error
	{

		int ErrCode;
		std::string ErrType;
		std::vector<std::string> ErrLines;
	};

	class error_IDA
	{

		error err;
		std::vector<error> errors;

	public:

		void SetErr(int errCode, std::string errType);

		void AddErrorLine(std::string line);

		void AddErrortovector();

		error GetLastError();

		~error_IDA();

	};
}
#endif
