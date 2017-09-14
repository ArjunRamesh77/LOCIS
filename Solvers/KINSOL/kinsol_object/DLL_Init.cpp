#include "Kinsol_Class.h"
#include "kinsol_dll.h"
#include<mutex>
std::mutex mtx1;

// Kinsol Object Manager
extern int objectcount;
extern std::vector<std::pair <int, solver_kinsol*>> objMap;

int INIT(int newobj)
{

	std::pair <int, solver_kinsol*> ID;
	int objout;

	mtx1.lock();
	solver_kinsol* obj;
	obj = NULL;
	obj = new solver_kinsol;
	if (obj == NULL) return -666;

	if (newobj == 0)
	{
		objectcount = objectcount + 1;

		ID.first = objectcount;
		ID.second = obj;

		objMap.push_back(ID);
		objout = objectcount;
	}
	else
	{
		ID.first = newobj;
		ID.second = obj;

		objMap.at(newobj - 1) = ID;
		objout = newobj;

	}
	mtx1.unlock();
	return objout;

}
