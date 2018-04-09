#pragma once

#include "rapidjson/document.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// rapidjson macros
#define NEW_OBJECT_RAPIDJSON new rapidjson::Value(rapidjson::kObjectType)
#define MAX_ARRAY_SIZE_IN_JSON 1000

#define RJ_GETINT(name)                 ops->rjw.getInt(#name, ops->name)
#define RJ_GETDOUBLE(name)              ops->rjw.getDouble(#name, ops->name)
#define RJ_GET_DOUBLE_ARRAY(name, size) ops->rjw.getDoubleArray(#name, size, ops->name)
#define RJ_GET_INT_ARRAY(name, size)    ops->rjw.getIntArray(#name, size, ops->name)

#define RJ_ADDINT(name)                 out->rjw.addInt(#name, out->name)
#define RJ_ADDDOUBLE(name)              out->rjw.addDouble(#name, out->name)
#define RJ_ADDDOUBLE_ARRAY(name, size)  out->rjw.addDoubleArray(#name, out->name, size)
#define RJ_ADDINT_ARRAY(name, size)     out->rjw.addIntArray(#name, out->name, size)

#define RJ_OUT_HAS(name) out->rjw.hasMember(#name)
#define RJ_OPS_HAS(name) ops->rjw.hasMember(#name)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// wraps the usage of rapidjson library
class rapidjsonWrapper
{
    rapidjson::Document* rdoc;
    rapidjson::Value* robjRoot;
    rapidjson::Value* robj;

public:
    rapidjsonWrapper();
    ~rapidjsonWrapper();

    void setJSONdoc(rapidjson::Document* doc_arg);
    void setJSONobjRoot(rapidjson::Value* obj_arg);

    //get functions
    bool getInt(const char *name, long int& val);
    bool getDouble(const char* name, double& val);
    bool getDoubleArray(const char* name, unsigned int& size, double *ptr);
    bool getIntArray(const char *name, unsigned int &size, int* ptr);
    bool hasMember(const char* name);

    //add functions
    void createNewObjectValue();
    void addObjectToDoc(const char *name);
    void addInt(const char* name, int value);
    void addDouble(const char* name, double value);
    void addBool(const char* name, bool value);

    void addDoubleArray(const char *name, double *ptr, unsigned int size);
};
