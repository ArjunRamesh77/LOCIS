#ifndef LOCISCOMMON_H
#define LOCISCOMMON_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common for the whole project
#define REAL_CLASS double
#define INTEGER_CLASS unsigned int

#define DELETE_VECTOR_ENTRIES(name) for(auto it = name.begin(); it != name.end(); ++it) if(*it) delete *it; name.clear();


#endif // LOCISCOMMON_H
