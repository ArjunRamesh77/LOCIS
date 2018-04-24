#include "blockdecomposition.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// takes the equation sets and generates the new set of residuals
int blockDecomposition::generateResidualsFromConnectedSystem()
{
    //Loop over all variable sets(excexpt the last set, its the dummy node)
    unsigned int varIndex = 0;
    unsigned int funcIndex = 0;
    std::vector<virtualOper>* mainResidualInstr = mainInputResidual->getAllInst();
    unsigned int blockCount = 1;
    for(std::list<std::list<incidenceGraphNode*>*>::const_iterator it1 = scc->begin(); it1 != scc->end(); ++it1)
    {        
        if(blockCount == scc->size())
            break;

         //get the variables from each set
         int resiType = FUNCTION_TYPE_ALG;
         std::map<unsigned int, unsigned int> allVars;
         for(std::list<incidenceGraphNode*>::const_iterator it2 = (*it1)->begin(); it2 != (*it1)->end(); ++it2)
         {
             varIndex = (*it2)->getIndex();
             funcIndex  = igraph.getVariableAt(varIndex)->getMatching()->getIndex();
             allVars[varIndex] = funcIndex;
         }

         //compute variable1 indices
         varIndex = 0;
         std::vector<unsigned int>* vecV1map = new std::vector<unsigned int>;
         std::vector<unsigned int>* vecV2map = NULL;
         if(!isInitializer)
           vecV2map = new std::vector<unsigned int>;

         //Map block equation variables to the original set
         //first do algebraic variables, the correspoding differential variable will also
         //share the same index, regardless whether its present or not
         //this will make sure eg. x[0] is aligned to x'[0]
         if(!isInitializer)
         {
             for(unsigned int i = 0; i < numVar; ++i)
             {
                 mapVar1[i].second = 0;
                 mapVar2[i].second = 0;
                 if(allVars.find(mapVar1[i].first) != allVars.end())
                 {
                     vecV1map->push_back(i);
                     vecV2map->push_back(i);
                     mapVar1[i].second = varIndex + 1;
                     mapVar2[i].second = varIndex + 1;
                     varIndex++;
                 }
             }

             //second, add all extra differential variables if any
             for(unsigned int i = 0; i < numVar; ++i)
             {
                 if(mapVar1[i].second == 0)
                 {
                     if(allVars.find(mapVar2[i].first) != allVars.end())
                     {
                         vecV2map->push_back(i);
                         mapVar2[i].second = varIndex + 1;
                         varIndex++;
                     }
                 }
             }
         }
         else
         {
             for(unsigned int i = 0; i < numVar; ++i)
             {
                 mapVar1[i].second = 0;
                 if(allVars.find(mapVar1[i].first) != allVars.end())
                 {
                     vecV1map->push_back(i);
                     mapVar1[i].second = varIndex + 1;
                     varIndex++;
                 }
             }
         }

         //add the maps
         blockVarsMap.push_back(std::make_pair(vecV1map, vecV2map));

         //Now generate each new residual by looping over the matched functions for the corresponding variables
         //if a variable is found that is not part of a matching then, just convert them to const and set the signal VR_CONST_VARX
         unsigned int fIndex = 0;
         virtualOper* instrPointer = NULL;
         genericResidual* blockResi = new genericResidual;
         blockResi->createNewInstructionStack(NULL);
         bool endInstLoop = false;
         unsigned int blockNumEqu = 0;
         for(std::map<unsigned int, unsigned int>::const_iterator it = allVars.begin(); it != allVars.end(); ++it)
         {
            //get the function index
            fIndex = residualIndexMap.at(it->second);
            instrPointer = &mainResidualInstr->at(fIndex);

            //Loop over all the instructions in the residual
            while(!endInstLoop)
            {
                if(instrPointer->signal == VR_SIGNAL_LAST)
                {
                    endInstLoop = true;
                    blockResi->setNumEquations(++blockNumEqu);
                }

                switch(instrPointer->operType)
                {
                case VR_VAR1_INDEX:
                    //next check if part of the set
                    varIndex = mapVar1[instrPointer->index].second;
                    if(varIndex == 0)
                    {
                        blockResi->addGeneralInstr(VR_CONST_VAR1, VR_DUMMY_DOUBLE, instrPointer->index);
                    }
                    else
                    {
                        blockResi->addGeneralInstr(instrPointer->operType, VR_DUMMY_DOUBLE, varIndex - 1);
                    }
                    break;

                case VR_VAR2_INDEX:
                    resiType = FUNCTION_TYPE_DAE;
                    varIndex = mapVar2[instrPointer->index].second;
                    if(varIndex == 0)
                    {
                        blockResi->addGeneralInstr(VR_CONST_VAR2, VR_DUMMY_DOUBLE, instrPointer->index);
                    }
                    else
                    {
                        blockResi->addGeneralInstr(instrPointer->operType, VR_DUMMY_DOUBLE, varIndex - 1);
                    }
                    break;

                default:
                    blockResi->addCopyInstruction(instrPointer);
                }
                ++instrPointer;
            }
            endInstLoop = false;
         }

         //set residual type
         blockResi->setFunctionType(resiType);

         //Now add to list of residuals
         outputResiduals.push_back(blockResi);

         blockCount++;
    }
    return 0;
}



