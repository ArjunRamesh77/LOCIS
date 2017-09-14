#include "IDAClass.h"
#include "IDA_dll.h"
#include<mutex>
std::mutex mtx1;

extern int objectcount;
extern std::vector<std::pair <int, solver_IDA*>> objMap;

int INIT(int newobj) // newobj is 0 if we want to increase objectcount and objnum otherwise
{
	std::pair <int, solver_IDA*> ID;
	int objout;

	mtx1.lock();
	solver_IDA* obj;
	obj = NULL;
	obj = new solver_IDA;
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
