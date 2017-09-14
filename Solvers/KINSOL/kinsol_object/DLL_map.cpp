#include "Kinsol_Class.h"
#include "kinsol_dll.h"

// Kinsol Object Manager
extern int objectcount;
extern std::vector<std::pair <int, solver_kinsol*>> objMap;

solver_kinsol* map(int num)
{
	solver_kinsol* tmpptr;
	if ((num > objectcount) || (num < 0)) return NULL;

	tmpptr = (objMap.at(num - 1)).second;

	return tmpptr;
}

