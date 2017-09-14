#include "interpreter.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Builds the geometry for FEM oneD problem
bool interpreter::initFEMLine(ASTNode* buildNode)
{
	// Get the correct type
	ASTNode* eval = NULL;
	int ret = 0;
	ASTmodel_entity_declNode* node = static_cast<ASTmodel_entity_declNode*>(buildNode);

	// Create line object
	FEMOneDLineObject* lo = new FEMOneDLineObject;
	lo->setName(node->sName);
	lo->setType(MODEL);
	lo->setSType("MODEL");
	lo->isFEMOneDLine = true;	  //flag indicating object is a line type
	lo->build_node = buildNode;   //To find arrays etc

	// Scalar or vector
	if (node->bIsArray)
	{
		lo->setDimType(SY_VECTOR);
		lo->vset = false;

		// Add dummy array indices
		LOOP_OVER_NODES(node, astvnArrayIndices)
		{
			lo->Dims.push_back(0);
		}
	}
	else
	{
		lo->setDimType(SY_SCALAR);
	}

	// Process Options (FEMoneD geometry, options are mandatory)
	bool got_basis = false;
	bool got_segment = false;
	if (node->bHasOptions)
	{
		LOOP_OVER_NODES(node, astvnOptions)
		{		
			//------------------------------------------------------PROCESS BASISTYPE---------------------------------------------------------------------
			if (SAME_TYPE(*it_astvnOptions, ASTBasisTypeNode*))
			{
				ASTBasisTypeNode* basis_option = dynamic_cast<ASTBasisTypeNode*>(*it_astvnOptions);
				if (lo->setBasisType(basis_option->sBasisType) != SY_SUCCESS)
				{
					semanticErr_InvalidBasisType(basis_option);
					return false;
				}
				got_basis = true;
				continue;
			}

			//------------------------------------------------------PROCESS SEGMENTS---------------------------------------------------------------------
			if (SAME_TYPE(*it_astvnOptions, ASTSegmentAttributesNode*))
			{				
				ASTSegmentAttributesNode* segment = dynamic_cast<ASTSegmentAttributesNode*>(*it_astvnOptions);

				// Necessary Segment Attributes
				ASTSegmentIdTypeNode* segment_id = NULL;;
				ASTNode* numel = NULL;
				ASTNode* Length = NULL;

				// Semantic flags to prevent redefinition
				bool got_segment_id = false;
				bool got_numel = false;
				bool got_Length = false;

				// Loop through all Segment attributes
				for (auto seg_attr = segment->astvnSegmentAttributes.begin(); seg_attr != segment->astvnSegmentAttributes.end(); ++seg_attr)
				{
					// Segment id
					if (SAME_TYPE(*seg_attr, ASTSegmentIdTypeNode*))
					{
						if (!got_segment_id)
						{
							segment_id = dynamic_cast<ASTSegmentIdTypeNode*>(*seg_attr);
							got_segment_id = true;
						}
						else
						{
							semanticErr_RedefinitionOfSegmentID(*seg_attr);
							return false;
						}
						continue;
					}

					// Number of elements
					if (SAME_TYPE(*seg_attr, ASTNumelNode*))
					{
						if (!got_numel)
						{
							numel = dynamic_cast<ASTNumelNode*>(*seg_attr)->astnExp;
							got_numel = true;
						}
						else
						{
							semanticErr_RedefinitionNumberOfElements(*seg_attr);
							return false;
						}
						continue;
					}

					// Length of element
					if (SAME_TYPE(*seg_attr, ASTLengthNode*))
					{
						if (!got_Length)
						{
							Length = dynamic_cast<ASTLengthNode*>(*seg_attr)->astnExp;
							got_Length = true;
						}
						else
						{
							semanticErr_RedefinitionLength(*seg_attr);
							return false;
						}
						continue;
					}
				}

				// Add segment attributes to the object
				if (!got_segment_id || !got_numel || !got_Length)
				{
					semanticErr_MissingSegmentAttributes(node->tName);
					return false;
				}
		
				// Add data
				if (lo->addSubDomain(segment_id->sID, Length, numel) != SY_SUCCESS)
				{
					semanticErr_SegmentIDExistsAlready(segment_id);
				}
				continue;
			}

			//------------------------------------------------------PROCESS FEM VARIABLES---------------------------------------------------------------------
			if (SAME_TYPE(*it_astvnOptions, ASTPdevarAttributesNode*))
			{
				ASTPdevarAttributesNode* pdevar = dynamic_cast<ASTPdevarAttributesNode*>(*it_astvnOptions);

				// Necessary
				std::string pde_id("");

				// Optional(NOT USED)
				std::vector<ASTNode*> domains;

				// Semantic flags to prevent redefinition
				bool got_pde_id = false;
				bool got_domains = false;
					
				// Loop through all pde attributes
				for (auto pvar_attr = pdevar->astvnSegmentAttributes.begin(); pvar_attr != pdevar->astvnSegmentAttributes.end(); ++pvar_attr)
				{
					// Get Name of variable
					if (SAME_TYPE(*pvar_attr, ASTSegmentIdTypeNode*))
					{
						if (!got_pde_id)
						{
							pde_id = dynamic_cast<ASTSegmentIdTypeNode*>(*pvar_attr)->sID;
							got_pde_id = true;
						}
						else
						{
							semanticErr_RedefinitionPDEid(*pvar_attr);
							return false;
						}
						continue;
					}

					// Get all domains
					if (SAME_TYPE(*pvar_attr, ASTdomainOptionNode*))
					{
						if (!got_domains)
						{
							domains = dynamic_cast<ASTdomainOptionNode*>(*pvar_attr)->astvnDomain;
							got_domains = true;
						}
						else
						{
							semanticErr_RedefinitionDomains(*pvar_attr);
							return false;
						}
					}
				}

				// Add variable
				std::string dName("");
				if (lo->PDEvars.find(pde_id) == lo->PDEvars.end())
				{
					variableAndDomain VinD;
					VinD.vName = pde_id;

					// Add domains
					if (got_domains)
					{
						std::map<std::string, int> domainsDefined; // To identify duplicates
						for (auto vinds = domains.begin(); vinds != domains.end(); vinds++)
						{
							//Check if domain exists
							dName = dynamic_cast<ASTNamedReferenceNode*>(*vinds)->sName;
							if (lo->domains.find(dName) != lo->domains.end())
							{
								auto findDomain = domainsDefined.find(dName);
								if (findDomain == domainsDefined.end())
								{
									domainsDefined[dName] = 1;
									VinD.dNames.push_back(dName);
								}
								else
								{
									semanticErr_RedefinitionDomainName(*vinds);
									return false;
								}
							}
							else
							{
								semanticErr_DomainDoesNotExist(*vinds);
								return false;
							}
						}
					}

					lo->PDEvars[pde_id] = VinD;
				}
			}
			continue;
		}

		// Add line to model
		if (!INSERT_NEW_ENTITY_IN_MODEL(lo->getName(), lo))
		{
			delete lo;
			semanticErr_EntityRedeclaration(node);
			return false;
		}
	}
	else
	{
		semanticErr_UndefinedLine(node);
		return false;
	}

	return true;
}
