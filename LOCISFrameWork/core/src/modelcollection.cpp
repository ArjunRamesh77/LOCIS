#include "modelcollection.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// modelCollections
modelCollection::modelCollection()
{

}

modelCollection::~modelCollection()
{
    for (auto it = models.begin(); it != models.end(); ++it)
    {
        if (it->second)
            delete it->second;
    }
}

bool modelCollection::insertModel(std::string &symbolName, ASTNode* node)
{
    if (models[symbolName] == NULL)
    {
        //models.insert(std::make_pair(27, 10));
        models[symbolName] = node;
        return true;
    }
    return false; //model with same name already exists
}

ASTNode* modelCollection::getModel(std::string &symbolName)
{
    return models[symbolName];
}

std::unordered_map<std::string, ASTNode*>* modelCollection::getAllmodel()
{
    return &models;
}
