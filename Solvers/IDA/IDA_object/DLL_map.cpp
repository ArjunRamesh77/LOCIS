#include "IDAClass.h"
#include "IDA_dll.h"

extern int objectcount;
extern std::vector<std::pair <int, solver_IDA*>> objMap;

solver_IDA* map(int num)
{
	solver_IDA* tmpptr;

	if ((num > objectcount) || (num < 0)) return NULL;

	tmpptr = (objMap.at(num - 1)).second;

	return tmpptr;
}