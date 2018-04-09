#include "rapidjsonwrapper.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// rapidjsonWrapper
rapidjsonWrapper::rapidjsonWrapper() :
    rdoc(NULL),
    robjRoot(NULL),
    robj(NULL)
{

}

rapidjsonWrapper::~rapidjsonWrapper()
{

}

void rapidjsonWrapper::setJSONdoc(rapidjson::Document *doc_arg)
{
    rdoc = doc_arg;
}

void rapidjsonWrapper::setJSONobjRoot(rapidjson::Value *robj_arg)
{
    robjRoot = robj_arg;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get from json
bool rapidjsonWrapper::getInt(const char* name, long &val)
{
    if(robjRoot->HasMember(name))
        val =  (*robjRoot)[name].GetInt();
    else
        return false;
    return true;
}

bool rapidjsonWrapper::getDouble(const char *name, double &val)
{
    if(robjRoot->HasMember(name))
        val = (*robjRoot)[name].GetDouble();
    else
        return false;
    return true;
}

bool rapidjsonWrapper::getDoubleArray(const char *name, unsigned int& size, double* ptr)
{
   size = 0;
   if(robjRoot->HasMember(name))
   {
        rapidjson::Value& jsonValue = (*robjRoot)[name];
        ptr = new double[jsonValue.Size()];
        for (rapidjson::Value::ConstValueIterator itr = jsonValue.Begin(); itr != jsonValue.End(); ++itr)
        {
            ptr[size] = itr->GetDouble();
            ++size;
        }
        return true;
   }
   return false;
}

bool rapidjsonWrapper::getIntArray(const char *name, unsigned int& size, int *ptr)
{
   size = 0;
   if(robjRoot->HasMember(name))
   {
        rapidjson::Value& jsonValue = (*robjRoot)[name];
        ptr = new int[jsonValue.Size()];
        for (rapidjson::Value::ConstValueIterator itr = jsonValue.Begin(); itr != jsonValue.End(); ++itr)
        {
            ptr[size] = itr->GetInt();
            ++size;
        }
        return true;
   }
   return false;
}

bool rapidjsonWrapper::hasMember(const char *name)
{
    return robjRoot->HasMember(name);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set in json
void rapidjsonWrapper::createNewObjectValue()
{
    if(robj)
    {
        delete robj;
    }
    robj = new rapidjson::Value(rapidjson::kObjectType);
}

void rapidjsonWrapper::addObjectToDoc(const char* name)
{
    rapidjson::Value key(name, rdoc->GetAllocator());
    robjRoot->AddMember(key, *robj, rdoc->GetAllocator());
}

void rapidjsonWrapper::addInt(const char *name, int value)
{
    if(robjRoot->HasMember(name))
    {
        rapidjson::Value key(name, rdoc->GetAllocator());
        robj->AddMember(key, rapidjson::Value().SetInt(value), rdoc->GetAllocator());
    }
}

void rapidjsonWrapper::addDouble(const char *name, double value)
{
    if(robj->HasMember(name))
    {
        rapidjson::Value key(name, rdoc->GetAllocator());
        robj->AddMember(key, rapidjson::Value().SetDouble(value), rdoc->GetAllocator());
    }
}

void rapidjsonWrapper::addBool(const char *name, bool value)
{
    if(robj->HasMember(name))
    {
        rapidjson::Value key(name, rdoc->GetAllocator());
        robj->AddMember(key, rapidjson::Value().SetBool(value), rdoc->GetAllocator());
    }
}

void rapidjsonWrapper::addDoubleArray(const char* name, double* ptr, unsigned int size)
{
    if(robjRoot->HasMember(name))
    {
        rapidjson::Value key(name, rdoc->GetAllocator());
        rapidjson::Value array(rapidjson::kArrayType);
        for(unsigned int i = 0; i < size; ++i)
        {
            array.PushBack(rapidjson::Value().SetDouble(ptr[i]), rdoc->GetAllocator());
        }
        robj->AddMember(key, array, rdoc->GetAllocator());
    }
}
