#include "FEM1D.h"
//#include "Operators.h"
#include <iostream>
#include <fstream>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implement all the semantic actions

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Process Node that holds all the models

class Equation;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FEM::SetExpressionTree()
{

	if (boundaryLoc == 0)
	{
		ASTequationNode* localtree = static_cast<ASTequationNode*> (tree);
		ASTNode* retL = localtree->astnLHS->visit(this);
		ASTNode* retR = localtree->astnRHS->visit(this);

		forIntegration.at(global_state.integNodeNo) = "";
		forIntegration.at(global_state.integNodeNo) = retL->equation_data + " - " + retR->equation_data;
	}
	else if (boundaryLoc == 1)
	{
		ASTequationNode* lefttree = static_cast<ASTequationNode*> (leftBoundary);
		ASTNode* retLeft = lefttree->visit(this);

		allEquations.at(0) += retLeft->equation_data + " + ";
	}
	else if (boundaryLoc == 2)
	{
		ASTequationNode* righttree = static_cast<ASTequationNode*> (rightBoundary);
		ASTNode* retRight = righttree->visit(this);

		allEquations.at(numEquations - 1) += retRight->equation_data + " + ";
	}
	else
	{

	}



//	std::cout << ret->equation_data << "\n";
	int a;
	a = 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FEM::setData(FEMUserData *userData, equation* equationptr, ASTNode* expressionTree, ASTNode* leftBoundarylocal,
	ASTNode* rightBoundarylocal, std::vector<EquationUnit>* localEqVecPtr, interpreter* Ip)
{

	solve_options.allElementData = userData->allElementData;
	solve_options.numElements = userData->numElements;

	tree = expressionTree;
	equationPtr = equationptr; // store pointer from equations class

	startNode = userData->startNode;

	equationVecPtr = localEqVecPtr;

	boundaryTypes = userData->boundaryTypes;
	totalNumNodes = userData->totalNumNodes;

	rightBoundary = rightBoundarylocal;
	leftBoundary = leftBoundarylocal;

	interpreterPtr = Ip;
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FEM::preProcess()
{
	// change this later. Now assume both DBC
    // numEquations = solve_options.numElements - 1;
	// numEquations = 2 * solve_options->numElements - 1;
	numEquations = totalNumNodes;


	if (boundaryTypes == ND || boundaryTypes == DN)
	{
		numEquations = numEquations - 1;
//		numEquations += 1;
	}
	else if (boundaryTypes == DD) // this was NN for the other logic
	{
		numEquations = numEquations - 2;
		//numEquations += 2;
	}

	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FEM::build()
{
	genericElement1D *elem;
	basis1D *mybasis;
	integration1D *myintegration;
	int globalNo;
	double globalLocation;

	// get number of equations
	preProcess();

	globalNo = 0;
	globalLocation = 0;
	for (auto it = solve_options.allElementData.begin(); it != solve_options.allElementData.end(); ++it)
	{
		elem = NULL;
		elem = new genericElement1D;
		mybasis = NULL;
		myintegration = NULL;

		// ***************INTEGRATION***********************************
		if (it->integrationRule == INTEGRATION_TRAPEZOID)
		{
			myintegration = new trapezoid;
		}
		else if (it->integrationRule == INTEGRATION_SIMPSON13)
		{
					myintegration = new simpson13;
		}
		else if (it->integrationRule == INTEGRATION_SIMPSON38)
		{
			myintegration = new simpson38;
		};

		// ***************BASIS****************************************
		if (it->basisFunctionType == BASIS_LINEAR)
		{
			mybasis = new linearBasis1D;
		}
		else if (it->basisFunctionType == BASIS_QUADRATIC)
		{
			mybasis = new quadraticBasis1D;
		}
		else if (it->basisFunctionType == BASIS_CUBIC)
		{
			mybasis = new cubicBasis1D;
		}
		else
		{
		};

		// Define right and left somehow.
		// elemType is same for now

		elem->setElem(globalNo, globalLocation, it->elemType, it->len, numEquations,
			problem_data.weakForm, NULL, NULL, mybasis, myintegration);
		globalLocation += it->len;
		if (it->basisFunctionType == BASIS_LINEAR)
		{
			globalNo++;
		}
		else if (it->basisFunctionType == BASIS_QUADRATIC)
		{
			globalNo = globalNo + 2;
		}
		else if (it->basisFunctionType == BASIS_CUBIC)
		{
			globalNo = globalNo + 3;
		}
		else
		{
		};

		line.push_back(elem);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FEM::solve()
{
	// We have all the data needed.
	// this function is supposed to iterate over everything and give vector of equations

	// replace u with sum (phi_i * x_i + ...) where i is each node in an element
	// replace v with phi_j where j is the equation number
	// doing this will make a string that is just the function of x
	// finally integrate the string
	// our approach in this will be exactly opposite!
	int i, j, k, diffNodes;
	bool enter_first_time;
	int enter_count;
	std::string allNodesSum;
	EquationUnit equationElem;

	// allocate vectors
	allEquations.resize(numEquations);

	for (j = 0; j < numEquations; j++)
	{
		enter_first_time = true;
		//taking care of number of equations for different boundary conditions
		// ex: if there are 3 elements -> 4 nodes
		// DD equations will go from 1, 2, 3
		// ND will go from  0, 1, 2, 3
		// DN will go from 1, 2, 3, 4
		// NN will go from 0, 1, 2, 3, 4
		// everything else stays the same
		if (boundaryTypes == DD || boundaryTypes == DN)
		{
			global_state.eqNo = j + 1;
		}
		else if (boundaryTypes == ND || boundaryTypes == NN)
		{
			global_state.eqNo = j;
		}
		else
		{

		}
		allEquations.at(j) = "(";
		for (i = 0; i < solve_options.numElements; i++) // for each element
		{

			global_state.element_info = line.at(i)->getElementInfo();
			// allocate vectors
			forIntegration.resize(global_state.element_info.basis->getnumNodes());

			global_state.elemNo = i;
			// global node number is the global number of first node of that element
			// following condition says that if given equation node is less than the first node
			// of an element, or greater than the last node of an element, contribution from that
			// element will be zero.
			diffNodes = global_state.eqNo - global_state.element_info.globalNodeNo;
			if (diffNodes < 0 || diffNodes >= global_state.element_info.basis->getnumNodes())
			{
			}
			else
			{

				if (boundaryTypes == DD)
				{

				}
				else
				{
					// for neumann, enter expression tree twice

					if (boundaryTypes == NN || boundaryTypes == ND)
					{
						// CHECK: this logic is not correct. check again
						if (j == 0) // first equation
						{
							boundaryLoc = 1; // add left boundary
							SetExpressionTree();

						}
					}
					if (boundaryTypes == NN || boundaryTypes == DN)
					{
						// CHECK: this logic is not correct. check again
						if (j == numEquations - 1) // last equation
						{
							boundaryLoc = 2; // add right boundary
							SetExpressionTree();
						}
					}
				}

				// get all parts required for numerical integration
				for (k = 0; k < global_state.element_info.basis->getnumNodes(); k++) 
				{
					// at this point all conversions should be ready 
					// everything should be a function of x
					// get diffnodes;
					enter_count = 0;

					global_state.integNodeNo = k;

						// one for usual equation and one for bounadry
						boundaryLoc = 0;
						SetExpressionTree(); // populate forNodes

				}

				

				if (enter_first_time == true)
				{
					allEquations.at(j) += global_state.element_info.integration->
						compositeIntegral(forIntegration, global_state.element_info.length);
					enter_first_time = false;
				}
				else
				{
					allEquations.at(j) += " + " + global_state.element_info.integration->
						compositeIntegral(forIntegration, global_state.element_info.length);
				}
			}


		}
		allEquations.at(j) += ")";
	}


	std::ofstream myfile;
	myfile.open("equationOut.txt");
	for (i = 0; i < numEquations; i++)
	{
		myfile << allEquations.at(i) << "\n";
		equationElem.sEquation = allEquations.at(i);
		// TODO: instead of populating allequations populate equationvecptr
		equationVecPtr->push_back(equationElem);
	}
	myfile.close();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Expression Evaluation
ASTNode* FEM::dispatch(ASTMathBinaryOpNode* node)
{
	ASTNode* left_eval = node->astnLeft->visit(this);
	ASTNode* right_eval = node->astnRight->visit(this);

	switch (node->iBinaryOp)
	{
	case(OP_PLUS):
		node->equation_data = left_eval->equation_data + "+" + right_eval->equation_data;
		break;

	case(OP_MULT):
		node->equation_data = left_eval->equation_data + "*" + right_eval->equation_data;
		break;

	case (OP_MINUS):
		node->equation_data = left_eval->equation_data + "-" + right_eval->equation_data;

	case OP_DIV:
		node->equation_data = left_eval->equation_data + "/" + right_eval->equation_data;
		
	case OP_RAISE:
		node->equation_data = left_eval->equation_data + "**" + right_eval->equation_data;
	}

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Expression Evaluation

ASTNode* FEM::dispatch(ASTMathUnaryNode* node)
{
	ASTNode* right_eval = NULL;

	right_eval = node->astnRight->visit(this);

	switch (node->iUnaryOp)
	{
	case OP_PLUS:
		node->equation_data = "( +" + right_eval->equation_data + ")";
		break;

	case OP_MINUS:
		node->equation_data = "( -" + right_eval->equation_data + ")";
		break;
	}

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Just Evaluate and return the node
ASTNode* FEM::dispatch(ASTQualifiedNamedReferenceNode* node)
{

	int diffNodes, i, numNodes, indexToReplace;
	bool enter_first_time_u, enter_first_time_ud;

	enter_first_time_u = true;
	enter_first_time_ud = true;
	numNodes = global_state.element_info.basis->getnumNodes();
	diffNodes = global_state.eqNo - global_state.element_info.globalNodeNo;
	ASTNamedReferenceNode* name = static_cast<ASTNamedReferenceNode*>(node->astvnQualifiedName.at(0));
	

	// check if fem variable
	// if it is fem get variable name C[0-10] C
	// get offset and get 
	if (node->me == NULL)
	{
		// not an FEM variable. use equation_data
		// this will be automatically taken care of in binary operations
		
	}
	else
	{
		if (boundaryLoc == 0)
		{
			if (node->me->isFEMOneDdVariable) // time dependent
			{
				// TODO:
				if (node->iNumDerivs == 0) // u
				{
					node->equation_data = "(";
					for (i = 0; i < numNodes; i++)
					{
						if (enter_first_time_u)
						{
							indexToReplace = global_state.element_info.globalNodeNo + i + startNode;
							node->equation_data += equationPtr->getSubstitutedEntityNameVector(node->me, indexToReplace) + " *"
									+ global_state.element_info.basis->get_phi(i + 1,
										global_state.integNodeNo + 1, global_state.element_info.length);


							//node->equation_data += "DV[" + std::to_string(
							//	global_state.element_info.globalNodeNo + i + startNode) + "] *"
							//	+ global_state.element_info.basis->get_phi(i + 1,
							//		global_state.integNodeNo + 1, global_state.element_info.length);
							enter_first_time_u = false;
						}
						else
						{
							indexToReplace = global_state.element_info.globalNodeNo + i + startNode;
							node->equation_data += " + " + equationPtr->getSubstitutedEntityNameVector(node->me, indexToReplace) + " *"
									+ global_state.element_info.basis->get_phi(i + 1,
										global_state.integNodeNo + 1, global_state.element_info.length);

							//node->equation_data += " + DV[" + std::to_string(
							//	global_state.element_info.globalNodeNo + i + startNode) + "] *"
							//	+ global_state.element_info.basis->get_phi(i + 1,
							//		global_state.integNodeNo + 1, global_state.element_info.length);
						}
					}
					node->equation_data += ")";

				}
				// are we supporting du'/dt??
				else if (node->iNumDerivs == 1) // du 
				{
					

					//node->equation_data = "(";
					//for (i = 0; i < numNodes; i++)
					//{
					//	if (enter_first_time_ud == true)
					//	{
					//		node->equation_data += "V[" + std::to_string(
					//			global_state.element_info.globalNodeNo + i + startNode) + "] *"
					//			+ global_state.element_info.basis->get_dphi(i + 1,
					//				global_state.integNodeNo + 1, global_state.element_info.length);
					//		enter_first_time_ud = false;
					//	}
					//	else
					//	{
					//		node->equation_data += " + V[" + std::to_string(
					//			global_state.element_info.globalNodeNo + i + startNode) + "] *"
					//			+ global_state.element_info.basis->get_dphi(i + 1,
					//				global_state.integNodeNo + 1, global_state.element_info.length);

					//	}
					//}
					//node->equation_data += ")";
				}
				else
				{
					// do nothing
				}

			}
			// not time dependent
			else if (node->me->isFEMOneDVariable) 
			{

				if (node->iNumDerivs == 0) // u
				{
					node->equation_data = "(";
					for (i = 0; i < numNodes; i++)
					{
						if (enter_first_time_u)
						{
							indexToReplace = global_state.element_info.globalNodeNo + i + startNode;
							node->equation_data += equationPtr->getSubstitutedEntityNameVector(node->me, indexToReplace)
								+ " *"
								+ global_state.element_info.basis->get_phi(i + 1,
									global_state.integNodeNo + 1, global_state.element_info.length);
							
							//node->equation_data += "V[" + std::to_string(
							//	global_state.element_info.globalNodeNo + i + startNode) + "] *"
							//	+ global_state.element_info.basis->get_phi(i + 1,
							//		global_state.integNodeNo + 1, global_state.element_info.length);
							enter_first_time_u = false;
						}
						else
						{
							indexToReplace = global_state.element_info.globalNodeNo + i + startNode;
							node->equation_data += " + " + equationPtr->getSubstitutedEntityNameVector(node->me, indexToReplace) + " *"
								+ global_state.element_info.basis->get_phi(i + 1,
									global_state.integNodeNo + 1, global_state.element_info.length);


							//node->equation_data += " + V[" + std::to_string(
							//	global_state.element_info.globalNodeNo + i + startNode) + "] *"
							//	+ global_state.element_info.basis->get_phi(i + 1,
							//		global_state.integNodeNo + 1, global_state.element_info.length);
						}
					}
					node->equation_data += ")";

				}
				else if (node->iNumDerivs == 1) // du
				{
					node->equation_data = "(";
					for (i = 0; i < numNodes; i++)
					{
						if (enter_first_time_ud == true)
						{
							indexToReplace = global_state.element_info.globalNodeNo + i + startNode;
							node->equation_data += equationPtr->getSubstitutedEntityNameVector(node->me, indexToReplace) + " *"
								+ global_state.element_info.basis->get_dphi(i + 1,
									global_state.integNodeNo + 1, global_state.element_info.length);

							//node->equation_data += "V[" + std::to_string(
							//	global_state.element_info.globalNodeNo + i + startNode) + "] *"
							//	+ global_state.element_info.basis->get_dphi(i + 1,
							//		global_state.integNodeNo + 1, global_state.element_info.length);
							enter_first_time_ud = false;
						}
						else
						{
							indexToReplace = global_state.element_info.globalNodeNo + i + startNode;
							node->equation_data += " + " + equationPtr->getSubstitutedEntityNameVector(node->me, indexToReplace) + " *"
								+ global_state.element_info.basis->get_dphi(i + 1,
									global_state.integNodeNo + 1, global_state.element_info.length);

							//node->equation_data += " + V[" + std::to_string(
							//	global_state.element_info.globalNodeNo + i + startNode) + "] *"
							//	+ global_state.element_info.basis->get_dphi(i + 1,
							//		global_state.integNodeNo + 1, global_state.element_info.length);

						}
					}
					node->equation_data += ")";
				}
				else
				{
					// do nothing
				}

			}
			else if (node->me->isFEMtrialFunction) //v
			{
				if (node->iNumDerivs == 0) // v
				{

					node->equation_data = global_state.element_info.basis->get_phi(diffNodes + 1,
						global_state.integNodeNo + 1, global_state.element_info.length);
				}
				else if (node->iNumDerivs == 1) // dv
				{
					node->equation_data = global_state.element_info.basis->get_dphi(diffNodes + 1,
						global_state.integNodeNo + 1, global_state.element_info.length);
				}
				else
				{
					// do nothing
				}
			}
			else if (node->me->isFEMxval)
			{
				node->equation_data = std::to_string(global_state.element_info.globalNodeLocation +
					global_state.integNodeNo * global_state.element_info.length / (global_state.element_info.basis->getnumNodes() - 1));
			}
			else if (node->me->isTval)
			{
				// do nothing. it will be automatically taken care of
			}

		}
		else
		{
			// we are processing boundary tree
			if (node->me->isFEMOneDdVariable) // time dependent
			{
				// TODO: 
				// Assume no time dependence in boundary for now


			}
			else if (node->me->isFEMOneDVariable) // not time dependent
			{

				if (node->iNumDerivs == 0) // u
				{
				// assume no u in the boundary condition for now

				}
				else if (node->iNumDerivs == 1) // du
				{
					node->equation_data = "(";

					if (boundaryLoc == 1)
					{
						indexToReplace = startNode + totalNumNodes;//node->me->nVvalue[startNode + totalNumNodes + 1];
						node->equation_data += equationPtr->getSubstitutedEntityNameVector(node->me, indexToReplace);

//						node->equation_data += "V[" + std::to_string(node->me->nVvalue[startNode + totalNumNodes]) + "]";
					}
					else if (boundaryLoc == 2)
					{

						indexToReplace = startNode + totalNumNodes + 1; // node->me->nVvalue[startNode + totalNumNodes + 1];
						node->equation_data += equationPtr->getSubstitutedEntityNameVector(node->me, indexToReplace);

//						node->equation_data += "V[" +  std::to_string(node->me->nVvalue[startNode + totalNumNodes + 1]) + "]";
					}
					else
					{

					}

					node->equation_data += ")";
				}
				else
				{
					// do nothing
				}

			}
			else if (node->me->isFEMtrialFunction) //v
			{
				if (node->iNumDerivs == 0) // v
				{
					// it entered here implies that v exists. Therefore, 1
					node->equation_data = "1";
				}
				else if (node->iNumDerivs == 1) // dv
				{
					// assume boundary conditions won't have v'
				}
				else
				{
					// do nothing
				}
			}

		}



	}



	return node;
}