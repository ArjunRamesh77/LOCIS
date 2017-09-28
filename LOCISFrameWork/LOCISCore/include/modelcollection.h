#ifndef MODELCOLLECTION_H
#define MODELCOLLECTION_H

#include "AST.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Stores all the models
class modelCollection
{

public:
    std::unordered_map<std::string, ASTNode*> models;

public:
    modelCollection();
    ~modelCollection();

    // Insert a new model
    bool insertModel(std::string &symbolName, ASTNode* node);

    // Get model
    ASTNode* getModel(std::string &symbolName);

    // Get all models
    std::unordered_map<std::string, ASTNode*>* getAllmodel();
};

#endif // MODELCOLLECTION_H
