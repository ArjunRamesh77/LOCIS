#include "kinsol_dll.h"
#include "Kinsol_Class.h"

// Kinsol Object Manager
int objectcount = 0;
std::vector<std::pair <int, solver_kinsol*>> objMap;