#ifndef DATABASE_H
#define DATABASE_H

#include "DatalogProgram.h"
#include <vector>
#include <stack>
#include "Relation.h"
#include <string>
#include "SchemeVec.h"
#include "ConstantMaker.h"
#include "Node.h"
#include "DependencyGraph.h"
#include <algorithm>

class DataBase
{
private:

	std::vector<Relation> RelationsVector;
	
	DatalogProgram rawData;

	std::vector<Relation> QueryResultsVector;

	DependencyGraph globalDependencyGraph;

	std::vector<int> globalSccVector;

public:

	DataBase(DatalogProgram InRawData) {
		//std::cout << "STARTED!\n";
		rawData = InRawData;
		buildRelationsVector();

		advancedProcessRules(); // lab 5, getting the sccs in order. Designed to replace processRules
		//processRules(); // Generate extra facts from rules

		processQueries();
		////printAllQueryResults();
		projectRemove();
		printAndProcess();

		

		//std::cout << "FINISHED!\n";
	}

	void advancedProcessRules() {
		 
		DependencyGraph dependencyGraph = createDependencyGraphFromRules();
		printDependencyGraph(dependencyGraph);
		DependencyGraph reverseDependencyGraph = reverseADependencyGraph(dependencyGraph);
		std::vector<int> postOrderBox = getPostOrderBoxFromDG(reverseDependencyGraph);
		std::vector<std::vector<int>> sccVector = getSCCs(dependencyGraph, postOrderBox);
		
		globalDependencyGraph = dependencyGraph;

		customProcessRules(sccVector);

		std::cout << "\n";
		//int x = 0; 

	}

	void customProcessRules(std::vector<std::vector<int>> sccVector) {
		std::cout << "Rule Evaluation\n";
		for (unsigned int i = 0; i < sccVector.size(); i++) {
			printSccHeader(sccVector.at(i)); // SCC: R1,R2

			constrainedProcessRules(sccVector.at(i));
			printScc(sccVector.at(i));
		}
	}

	void constrainedProcessRules(std::vector<int> sccVector) {
		// Get vector<Rule> from SCCs
		std::vector<Rule> ruleVector;
		for (unsigned int i = 0; i < sccVector.size(); i++) {
			ruleVector.push_back(rawData.getRuleVector().at(sccVector.at(i)));
			
		}
		constrainedProcessRules2(ruleVector, sccVector);
	}

	void constrainedProcessRules2(std::vector<Rule> rulesVector, std::vector<int> sccVector) {
		//std::cout << "Rule Evaluation\n";

		bool tuplesMade = true;
		bool hasBeenMarked = false;
		bool noDependencies = false;

		DependencyGraph holderDependencyGraph = globalDependencyGraph;



		// find out if there are no dependencies for that specific rule
		if (holderDependencyGraph.accessMainVector().at(sccVector.at(0)).size() == 0) {
			noDependencies = true;
		}
		if (allDependenciesAreProcessed(sccVector)) {
			noDependencies = true;
		}


		int countNumber = 0;

		while (tuplesMade || hasBeenMarked) {
			hasBeenMarked = false;
			for (unsigned int i = 0; i < rulesVector.size(); i++) {                            // <-----
				Predicate headpredicate = rulesVector.at(i).getHeadPredicate();					  // <-----
				std::vector<Predicate> predicateList = rulesVector.at(i).getPredicateList();
				// Add into ruleRelationsVector
				std::vector<Relation> ruleRelationsVector;
				Relation joinedRelationTotal;

				// go through each predicate
				for (unsigned int k = 0; k < predicateList.size(); k++) {
					Predicate thePredicate = predicateList.at(k);
					ruleRelationsVector.push_back(getRelationFromPredicate(thePredicate));
				}

				// join them together
				joinedRelationTotal = ruleRelationsVector.at(0);
				if (ruleRelationsVector.size() == 1) {

				}
				else {
					for (unsigned int k = 1; k < ruleRelationsVector.size(); k++) {
						joinedRelationTotal = joinedRelationTotal.joinRelation(ruleRelationsVector.at(k));
					}
				}

				// project only needed parts
				std::vector<std::string> removeStrings;

				for (int k = 0; k < joinedRelationTotal.getSchemeVector().size(); k++) {
					if (!vectorContains(headpredicate.returnElements(), joinedRelationTotal.getSchemeVector().at(k))) {
						removeStrings.push_back(joinedRelationTotal.getSchemeVector().at(k));
						// remove time

					}
				}
				for (unsigned int k = 0; k < removeStrings.size(); k++) {
					joinedRelationTotal.ProjectByString(removeStrings.at(k));
				}

				// Reorder to match head>>>>>>>>>>>>>

				for (unsigned int k = 0; k < headpredicate.returnElements().size(); k++) {
					if (joinedRelationTotal.getSchemeVector().at(k) != headpredicate.returnElements().at(k)) {
						// reorder joinRelaitonTotal.
						joinedRelationTotal.reorder(headpredicate.returnElements().at(k), k);
					}
				}

				// print all the crap you need - header and relations
					// header

				printRuleFromRule(rulesVector.at(i)); // <<< printing header here                 // <-----

				// relations

				// ----->joinedRelationTotal.PrintRelationTuples();

				// checking if tuples are being made

				//------>std::cout << joinedRelationTotal.getSetOfTupleVecs().size() << std::endl; 

				int before = -1;
				for (unsigned int k = 0; k < RelationsVector.size(); k++) {			                                // <-----
					if (RelationsVector.at(k).getNameOfRelation() == headpredicate.returnID()) {                      // <-----
						//----->std::cout << "BEFORE: " << RelationsVector.at(k).getSetOfTupleVecs().size() << std::endl;
						before = RelationsVector.at(k).getSetOfTupleVecs().size();                                // <-----
					}
				}

				// do a union with relational database. 

				for (unsigned int k = 0; k < RelationsVector.size(); k++) {
					if (RelationsVector.at(k).getNameOfRelation() == headpredicate.returnID()) {                 // <-----

						// Actually, the printing is just done in this function here. Forget what it says around....
						printAdditions(RelationsVector.at(k), joinedRelationTotal); // <<< printing relations here    // <-----
						RelationsVector.at(k).unite(joinedRelationTotal);                                         // <-----

					}
				}

				// check size difference
				int after = -1;
				for (unsigned int k = 0; k < RelationsVector.size(); k++) {                                        // <-----
					if (RelationsVector.at(k).getNameOfRelation() == headpredicate.returnID()) {                    // <-----            
						
						after = RelationsVector.at(k).getSetOfTupleVecs().size();                                    // <-----
					}
				}

				if (before == after) {
					tuplesMade = false;
				}
				else {
					tuplesMade = true;
					hasBeenMarked = true;
				} 

			}
			countNumber++;
			if (noDependencies == true) { // added to stop no dependencies from running a second time. 
				//std::cout << "\npreemtive break\n";
				break;
			}
		}

		std::cout << countNumber << " passes: ";

		for (unsigned int i = 0; i < sccVector.size(); i++) {
			globalSccVector.push_back(sccVector.at(i));
		}

	}

	void processRules() {

		std::cout << "Rule Evaluation\n";

		bool tuplesMade = true; 
		bool hasBeenMarked = false; 

		int countNumber = 0; 

		while (tuplesMade || hasBeenMarked) {
			hasBeenMarked = false; 
			for (unsigned int i = 0; i < rawData.getRuleVector().size(); i++) {                            // <-----
				Predicate headpredicate = rawData.getRuleVector().at(i).getHeadPredicate();					  // <-----
				std::vector<Predicate> predicateList = rawData.getRuleVector().at(i).getPredicateList();
				// Add into ruleRelationsVector
				std::vector<Relation> ruleRelationsVector;
				Relation joinedRelationTotal;

				// go through each predicate
				for (unsigned int k = 0; k < predicateList.size(); k++) {
					Predicate thePredicate = predicateList.at(k);
					ruleRelationsVector.push_back(getRelationFromPredicate(thePredicate));
				}

				// join them together
				joinedRelationTotal = ruleRelationsVector.at(0);
				if (ruleRelationsVector.size() == 1) {

				}
				else {
					for (unsigned int k = 1; k < ruleRelationsVector.size(); k++) {
						joinedRelationTotal = joinedRelationTotal.joinRelation(ruleRelationsVector.at(k));
					}
				}

				// project only needed parts
				std::vector<std::string> removeStrings;

				for (int k = 0; k < joinedRelationTotal.getSchemeVector().size(); k++) {
					if (!vectorContains(headpredicate.returnElements(), joinedRelationTotal.getSchemeVector().at(k))) {
						removeStrings.push_back(joinedRelationTotal.getSchemeVector().at(k));
						// remove time

					}
				}
				for (unsigned int k = 0; k < removeStrings.size(); k++) {
					joinedRelationTotal.ProjectByString(removeStrings.at(k));
				}

				// Reorder to match head>>>>>>>>>>>>>

				for (unsigned int k = 0; k < headpredicate.returnElements().size(); k++) {
					if (joinedRelationTotal.getSchemeVector().at(k) != headpredicate.returnElements().at(k)) {
						// reorder joinRelaitonTotal.
						joinedRelationTotal.reorder(headpredicate.returnElements().at(k), k);
					}
				}

				// print all the crap you need - header and relations
					// header

				printRuleFromRule(rawData.getRuleVector().at(i)); // <<< printing header here                 // <-----

				// relations

				// ----->joinedRelationTotal.PrintRelationTuples();

				// checking if tuples are being made

				//------>std::cout << joinedRelationTotal.getSetOfTupleVecs().size() << std::endl; 

				int before;
				for (unsigned int k = 0; k < RelationsVector.size(); k++) {			                                // <-----
					if (RelationsVector.at(k).getNameOfRelation() == headpredicate.returnID()) {                      // <-----
						//----->std::cout << "BEFORE: " << RelationsVector.at(k).getSetOfTupleVecs().size() << std::endl;
						before = RelationsVector.at(k).getSetOfTupleVecs().size();                                // <-----
					}
				}

				// do a union with relational database. ^^^*|*^^^

				for (unsigned int k = 0; k < RelationsVector.size(); k++) {
					if (RelationsVector.at(k).getNameOfRelation() == headpredicate.returnID()) {                 // <-----
						
						// Actually, the printing is just done in this function here. Forget what it says around....
						printAdditions(RelationsVector.at(k), joinedRelationTotal); // <<< printing relations here    // <-----
						RelationsVector.at(k).unite(joinedRelationTotal);                                         // <-----
					
					}
				}
				
				// check size difference
				int after;
				for (unsigned int k = 0; k < RelationsVector.size(); k++) {                                        // <-----
					if (RelationsVector.at(k).getNameOfRelation() == headpredicate.returnID()) {                    // <-----            
						//------>std::cout << "After: " << RelationsVector.at(k).getSetOfTupleVecs().size() << std::endl;
						after = RelationsVector.at(k).getSetOfTupleVecs().size();                                    // <-----
					}
				}

				if (before == after) {
					tuplesMade = false;
				}
				else {
					tuplesMade = true;
					hasBeenMarked = true; 
				}
				//std::cout << "1 Pass Completed" << std::endl; 


			}
			

			countNumber++;

		}

		std::cout << "\nSchemes populated after " << countNumber << " passes through the Rules.\n\n";

		

			// Follow procedure for rule. 

				// 1. evaluate predicates - get them as Relations

				// 2. join them together*

				// 3. Project to only include columns in the head predicate*

				// 4. Reorder to match the head predicate.*

				// 5. Print that relation*

				

	}

	void buildRelationsVector() {
		// For every element of SchemesPredicate vector...
		for (unsigned int i = 0; i < rawData.getSchemesPredicateVector().size(); i++) {

			// CREATE A NEW RELATION
			Relation relation;

			// ADD A NEW NAME
			relation.setNameOfRelation(rawData.getSchemesPredicateVector().at(i).returnID());

			// ADD every Element of the Scheme Vector
			for (unsigned int k = 0; k < rawData.getSchemesPredicateVector().at(i).returnElements().size(); k++) {
				
				relation.addToSchemeVector(rawData.getSchemesPredicateVector().at(i).returnElements().at(k));
				
			}

			// make a tuple vector
			for (unsigned int k = 0; k < rawData.getFactsPredicateVector().size(); k++) {
				TupleVec myTupleVec;
				
				if (rawData.getFactsPredicateVector().at(k).returnID() == relation.getNameOfRelation()) {
					for (unsigned int j = 0; j < rawData.getFactsPredicateVector().at(k).returnElements().size(); j++) {
						myTupleVec.pushBack(rawData.getFactsPredicateVector().at(k).returnElements().at(j));
						
					}
					relation.addTupleVectorToSet(myTupleVec);

				}
			}
			RelationsVector.push_back(relation);
		}
	}

	void processQueries() {
		std::cout << "Query Evaluation\n";

		for (unsigned int i = 0; i < rawData.getQueriesPredicateVector().size(); i++) {
			processSingleQuery(rawData.getQueriesPredicateVector().at(i));
		}
	}

	void processSingleQuery(Predicate inPredicate) {
		
		for (unsigned int i = 0; i < RelationsVector.size(); i++) {


			if (RelationsVector.at(i).getNameOfRelation() == inPredicate.returnID()) { // match the name
				Relation returnRelation = RelationsVector.at(i);
				// for each parameter...
				
 				for (unsigned int k = 0; k < inPredicate.returnElements().size(); k++) {
					if (/*Its a string */isAString(inPredicate.returnElements().at(k))) {
						returnRelation = returnRelation.SelectString(inPredicate.returnElements().at(k), k);
					}
					else {
						// it its a consonant
						
						ConstantMaker constantMaker(inPredicate.returnElements().at(k));
							// Find and add the values
						//constantMaker.pushPositionVector(k);
						for (unsigned int j = k; j < inPredicate.returnElements().size(); j++) {
							if (inPredicate.returnElements().at(j) == inPredicate.returnElements().at(k)) {
								constantMaker.pushPositionVector(j);
							}
						}

						for (unsigned int x = 0; x < constantMaker.getPositionVector().size(); x++) {
							//std::cout << constantMaker.getPositionVector().at(x) << " ";
						}

						//std::cout << "-n";
						// select for the constant marker. 
						returnRelation = returnRelation.SelectConstant(constantMaker);
					}
				}
				//int x = 1; // what is the return relation value????
				QueryResultsVector.push_back(returnRelation);
			}
		}
	}

	Relation getRelationFromPredicate(Predicate inPredicate) {
		// Loop through the relations vector
		for (unsigned int i = 0; i < RelationsVector.size(); i++) {
			//once the name matches the inPredicate returnId
			if (RelationsVector.at(i).getNameOfRelation() == inPredicate.returnID()) {
				// Create a new Relation
				Relation returnRelation = RelationsVector.at(i);
				// Go through the elements of the inPredicate
				for (unsigned int k = 0; k < inPredicate.returnElements().size(); k++) {
					// This code gets the relation.... I think. 
					if (/*Its a string */isAString(inPredicate.returnElements().at(k))) {
						returnRelation = returnRelation.SelectString(inPredicate.returnElements().at(k), k);
					}
					else {
						// it its a consonant

						ConstantMaker constantMaker(inPredicate.returnElements().at(k));
						// Find and add the values
					//constantMaker.pushPositionVector(k);
						for (unsigned int j = k; j < inPredicate.returnElements().size(); j++) {
							if (inPredicate.returnElements().at(j) == inPredicate.returnElements().at(k)) {
								constantMaker.pushPositionVector(j);
							}
						}

						for (unsigned int x = 0; x < constantMaker.getPositionVector().size(); x++) {
							//std::cout << constantMaker.getPositionVector().at(x) << " ";
						}

						//std::cout << "-n";
						// select for the constant marker. 
						returnRelation = returnRelation.SelectConstant(constantMaker);
					}
				}
				return returnRelation;
			}
		}

		Relation bsRelation;
		return bsRelation;

	}

	void projectRemove() {
		// Rename the appropriate schemes
		
		//printAllQueryResults();
		// remove the rows.
		for (/*unsigned */int i = QueryResultsVector.size() - 1; i >= 0 ; i--) { // try iterating backwards on this one too original code: unsigned int i = 0; i < QueryResultsVector.size(); i++
			for (/*unsigned */int k = QueryResultsVector.at(i).getSchemeVector().size() - 1; k >= 0 ; k--) { // Try iterating backwards original code: unsigned int k = 0; k < QueryResultsVector.at(i).getSchemeVector().size(); k++
				
				std::string asdf = rawData.getQueriesPredicateVector().at(i).returnElements().at(k);
				if (isAString(asdf)) {
					//std::cout << QueryResultsVector.at(i).getSchemeVector().at(k) << "\n";

					QueryResultsVector.at(i).ProjectByString(QueryResultsVector.at(i).getSchemeVector().at(k));
					
				}
				else {
					//removedDuplicateConstants();
				}
			}

			//std::cout << "\n";
		}

		//printAllQueryResults();

		removedDuplicateConstants();

	}
	
	void removedDuplicateConstants() {
		for (int i = QueryResultsVector.size() - 1; i >= 0; i--) {
			for (int k = QueryResultsVector.at(i).getSchemeVector().size() - 1; k >= 0; k--) {
				std::string value = QueryResultsVector.at(i).getSchemeVector().at(k);  //rawData.getQueriesPredicateVector().at(i).returnElements().at(k);
				if (!isAString(value)) {
					std::string currentValue = value;
					for (int j = k - 1; j >= 0; j--) {
						std::string value2 = QueryResultsVector.at(i).getSchemeVector().at(j);
						if (value2 == currentValue) {

							//QueryResultsVector.at(i).ProjectByString(currentValue);

							QueryResultsVector.at(i).ProjectByIndex(k);
						}
					}
				}
			}
		}
	}

	void printAndProcess() {
		for (unsigned int i = 0; i < QueryResultsVector.size(); i++) {
			std::cout << rawData.getQueriesPredicateVector().at(i).returnID();
			std::cout << "(";
			for (unsigned int k = 0; k < rawData.getQueriesPredicateVector().at(i).returnElements().size(); k++) {
				if (k == rawData.getQueriesPredicateVector().at(i).returnElements().size() - 1) {
					std::cout << rawData.getQueriesPredicateVector().at(i).returnElements().at(k);
				}
				else {
					std::cout << rawData.getQueriesPredicateVector().at(i).returnElements().at(k) << ",";
				}
			}
			std::cout << ")? ";

			// Yes/no (x) 

			if (QueryResultsVector.at(i).getSetOfTupleVecs().size() > 0) {
				std::cout << "Yes(" << QueryResultsVector.at(i).getSetOfTupleVecs().size() << ")";
			}
			else {
				std::cout << "No";
			}

			std::cout << "\n";

			


			//for (unsigned int k = 0; k < rawData.getQueriesPredicateVector().at(i).returnElements().size(); k++) {
			//	
			//	std::cout << rawData.getQueriesPredicateVector().at(i).returnElements().at(k) << "\n";
			//	if (!isAString(rawData.getQueriesPredicateVector().at(i).returnElements().at(k))) {
			//		// Project that element by string. 
			//		//QueryResultsVector.at(i).
			//		std::cout << "___" << rawData.getQueriesPredicateVector().at(i).returnElements().at(k) << "___\n";
			//		//std::cout << "---" << QueryResultsVector.at(i).getSetOfTupleVecs(). << "---\n";
			//		QueryResultsVector.at(i).ProjectByString(rawData.getQueriesPredicateVector().at(i).returnElements().at(k));
			//	}
			//}

			//QueryResultsVector.at(i).printRelation();
			QueryResultsVector.at(i).PrintRelationTuples();
		}

	}
	
	// ------------------CustomProcessRules Helper Functions-------------------------

	bool allDependenciesAreProcessed(std::vector<int> sccVector) {
		std::vector<int> basket;

		// put all the eggs in the basket
		for (unsigned int i = 0; i < sccVector.size(); i++) {
			for (unsigned int k = 0; k < globalDependencyGraph.accessMainVector().size(); k++) {
				unsigned int holder = sccVector.at(i);
				if (holder/*sccVector.at(i)*/ == k) {
					for (unsigned int j = 0; j < globalDependencyGraph.accessMainVector().at(k).size(); j++) {
						int egg = globalDependencyGraph.accessMainVector().at(k).at(j).getRValue();
						basket.push_back(egg);
					}
				}
			}
		}

		// make sure globalsccvector has all of the basket items.
		for (unsigned int i = 0; i < basket.size(); i++) {
			int searchInt = basket.at(i);
			bool found = false;
			for (unsigned int k = 0; k < globalSccVector.size(); k++) {
				if (globalSccVector.at(k) == searchInt) {
					found = true;
				}
			}
			if (found == false) {
				return false;
			}
		}

		return true;

	}

	void printSccHeader(std::vector<int> sccVectorPart) {
		std::cout << "SCC: ";
		for (unsigned int i = 0; i < sccVectorPart.size(); i++) {
			std::cout << "R";
			std::cout << sccVectorPart.at(i);
			if (i == sccVectorPart.size() - 1) {
				
			}
			else {
				std::cout << ",";
			}
		}
		std::cout << "\n";
	}

	void printScc(std::vector<int> sccVectorPart) {
		for (unsigned int i = 0; i < sccVectorPart.size(); i++) {
			std::cout << "R";
			std::cout << sccVectorPart.at(i);
			if (i == sccVectorPart.size() - 1) {

			}
			else {
				std::cout << ",";
			}
		}
		std::cout << "\n";
	}

	// -----------------AdvancedProcessRules Helper Functions----------------

	void printDependencyGraph(DependencyGraph dependencyGraph) {
		std::cout << "Dependency Graph\n";
		for (unsigned int i = 0; i < dependencyGraph.accessMainVector().size(); i++) {
			std::cout << "R" << i << ":";
			for (unsigned int k = 0; k < dependencyGraph.accessMainVector().at(i).size(); k++) {
				std::cout << "R";
				std::cout << dependencyGraph.accessMainVector().at(i).at(k).getRValue();
				if (k == dependencyGraph.accessMainVector().at(i).size() - 1) {
					
				}
				else {
					std::cout << ",";
				}
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
	
	std::vector<std::vector<int>> getSCCs(DependencyGraph dependencyGraph, std::vector<int> postOrderBox) {
		std::stack<int> varStack;
		std::vector<std::vector<int>> returnItem;
		std::vector<bool> visitedVector(dependencyGraph.accessMainVector().size());

		// while the visitedVector is not all true
		while (!boolVecAllTrue(visitedVector)) {

			int highestUnvisited = getHighestFromVisitedVector(visitedVector, postOrderBox);

			//returnItem.push_back(treeExplorer(dependencyGraph, visitedVector, highestUnvisited));
			returnItem.push_back(treeExplorer(dependencyGraph, visitedVector, varStack, highestUnvisited, returnItem));

		}

		// go to the highest value on the chart
			// int highestvalue = get highest froom chart.
		// make a structure to keep track of visited branches <... ... ...>
		// visit all nodes you can and mark them on the structure
		// visitedNodes -> returnItem
		// go to the next lowest unvisited...

		// reorder the second vector
		reorderSecondVector(returnItem);

		return returnItem;
	}

	std::vector<int> treeExplorer(DependencyGraph& dependencyGraph, std::vector<bool>& visitedVector, std::stack<int>& varStack, int inNumber, std::vector<std::vector<int>>& returnItem) {
		std::vector<int> littleReturnVector;

		restart:

		// 1 Add to stack if...
		if (varStack.empty()) {
 			varStack.push(inNumber);
		}
		else if (varStack.top() != inNumber) {
			varStack.push(inNumber);
		}
		int currentR = inNumber;

		// 2 Mark
		visitedVector.at(currentR) = true;

		// 3 Get best available
		currentR = goToBestAvailable(currentR, visitedVector, dependencyGraph);

		// if != -1
		if (currentR != -1) {
			inNumber = currentR;
			goto restart;
		}
		else {
			littleReturnVector.push_back(varStack.top());

			if (varStack.empty()) {
				return littleReturnVector;
			}

			varStack.pop();

			if (varStack.empty()) {
				return littleReturnVector;
			}

			inNumber = varStack.top();
			goto restart;
			//littleReturnVector.push_back(treeExplorer(dependencyGraph, visitedVector, varStack, varStack.top(), returnItem));
		}
		return littleReturnVector;
	}

	std::vector<int> getPostOrderBoxFromDG(DependencyGraph inDependencyGraph) {
		std::stack<int> varStack;
		std::vector<int> postOrderBox;
		std::vector<bool> visitedVector(inDependencyGraph.accessMainVector().size());

		while (!boolVecAllTrue(visitedVector)) {

			int lowestUnvisited = getLowestFromVisitedVector(visitedVector);

			stackBreaker(inDependencyGraph, visitedVector, varStack, lowestUnvisited, postOrderBox);

		}

		return postOrderBox;
	}

	int stackBreaker(DependencyGraph& inDependencyGraph, std::vector<bool>& visitedVector, std::stack<int>& varStack, int inNumber, std::vector<int>& postOrderBox) {

		// get the lowest point on the visited vector, get the int value
		//int lowestUnvisited = getLowestFromVisitedVector(visitedVector);
		
		// add number to stack
		if (varStack.empty()) {
			varStack.push(inNumber);
		}
		else if ( varStack.top() != inNumber) {
			varStack.push(inNumber);
		}

		int currentR = inNumber;
		
		// mark visitedvector
		visitedVector.at(currentR) = true;

		// look through that dependency graph, find the lowest number that isn't true on the visited stack
		currentR = goToBestAvailable(currentR, visitedVector, inDependencyGraph);

		if (currentR != -1) {
			// Run Stack Breaker Again...
			stackBreaker(inDependencyGraph, visitedVector, varStack, currentR, postOrderBox);
		} 
		else {

			postOrderBox.push_back(varStack.top());

			if (varStack.empty()) {
				return 0;
			}

			varStack.pop();

			if (varStack.empty()) {
				return 1;
			}

			stackBreaker(inDependencyGraph, visitedVector, varStack, varStack.top(), postOrderBox);
		}

		return 0;
	}

	int goToBestAvailable(int currentR, std::vector<bool> visitedVector, DependencyGraph inDependencyGraph) {
		std::vector<Node> nodeVector = inDependencyGraph.accessMainVector().at(currentR);
		int currentLowest = 666;
		bool valueFound = false;

		for (unsigned int i = 0; i < nodeVector.size(); i++) {
			int valueInt = nodeVector.at(i).getRValue();
			if (visitedVector.at(valueInt) != true && valueInt < currentLowest) {
				currentLowest = valueInt;
				valueFound = true;
			}
		}

		if (valueFound == true) {
			return currentLowest;
		}
		else {
			return -1;
		}
		
	}

	int getLowestFromVisitedVector(std::vector<bool> inVisitedVector) {
		for (unsigned int i = 0; i < inVisitedVector.size(); i++) {
			if (inVisitedVector.at(i) == false) {
				return i;
			}
		}
		return -1;
	}

	int getHighestFromVisitedVector(std::vector<bool> inVisitedVector, std::vector<int> postOrderBox) {

		int highest = -1;
		//int highestmarker = -1;
		
		// VERSION 1

		//for (unsigned int i = 0; i < inVisitedVector.size(); i++) {
		//	if (inVisitedVector.at(i) == false && postOrderBox.at(i) > highest) { // making a change here...
		//		highest = postOrderBox.at(i);
		//		highestmarker = i;
		//	}
		//}

		//return highestmarker;

		// VERSION 2

		for (unsigned int i = 0; i < postOrderBox.size(); i++) {
			bool myBool = inVisitedVector.at(postOrderBox.at(i));
			
			if (myBool == false) {
				int x = i;
				if (x > highest) {
					highest = i; 
				}


			}

			//if ((inVisitedVector.at(i) == false) && (i > highest)) { // making a change here...
			//	highest = i;
			//	
			//}
		}

		return postOrderBox.at(highest);

		/*for (int i = inVisitedVector.size() - 1; i >= 0; i++) {
			if (inVisitedVector.at(i) == false) {
				return i;
			}
		}
		return -1;*/
	}

	bool boolVecAllTrue(std::vector<bool> inVec) {
		for (unsigned int i = 0; i < inVec.size(); i++) {
			if (inVec.at(i) == false) {
				return false;
			}
		}
		return true;
	}

	DependencyGraph createDependencyGraphFromRules() {
		DependencyGraph returnDependencyGraph;
		//int x = 0;

		// Go through every element in the rule vector
		for (unsigned int i = 0; i < rawData.getRuleVector().size(); i++) {

			// set up a vector of nodes for that element
			std::vector<Node> nodeVector;

			// go through every element in the predicate list
			for (unsigned int k = 0; k < rawData.getRuleVector().at(i).getPredicateList().size(); k++) {

				// go back and look through every element in the rule vector
				for (unsigned int j = 0; j < rawData.getRuleVector().size(); j++) {

					// if the headpredicate of the rulevector element == the current predicate ID we are looking at
					// Make a node and add it to the nodeVector
					//std::cout << rawData.getRuleVector().at(j).getHeadPredicate().returnID() << " : " << rawData.getRuleVector().at(i).getPredicateList().at(k).returnID() << "\n";
					if (rawData.getRuleVector().at(j).getHeadPredicate().returnID() == rawData.getRuleVector().at(i).getPredicateList().at(k).returnID()) {
						
						//std::cout << "		Success: " << j << std::endl;
						// stop it from adding doubles!!!
						if (nodeVectorContains(j, nodeVector)) {

						}
						else {
							Node newNode(j);
							nodeVector.push_back(newNode);
						}
						
					}
				}
			}
			// reorder node vectors
			reorderNodeVector(nodeVector);
			returnDependencyGraph.pushBackVectorOfNode(nodeVector);
		}
		return returnDependencyGraph;
	}

	DependencyGraph reverseADependencyGraph(DependencyGraph inDependencyGraph) {
		DependencyGraph returnDependencyGraph;
		
		std::vector<std::vector<Node>> vectorOfVectorOfNodes;

		// Gets the number right
		for (unsigned int i = 0; i < inDependencyGraph.accessMainVector().size(); i++) {
			std::vector<Node> nodeVector;
			vectorOfVectorOfNodes.push_back(nodeVector);
		}

		for (unsigned int i = 0; i < inDependencyGraph.accessMainVector().size(); i++) {

			for (unsigned int k = 0; k < inDependencyGraph.accessMainVector().at(i).size(); k++) {
				Node newNode(i);

				int rToEdit = inDependencyGraph.accessMainVector().at(i).at(k).getRValue();
				vectorOfVectorOfNodes.at(rToEdit).push_back(newNode);
			}
		}
		returnDependencyGraph.setMainVector(vectorOfVectorOfNodes);
		return returnDependencyGraph;
	}

	// -----------------Helper Functions---------------------

	void reorderNodeVector(std::vector<Node>& nodeVector) {
		std::vector<int> intVector;
		std::vector<Node> newNodeVector;
		for (unsigned int i = 0; i < nodeVector.size(); i++) {
			intVector.push_back(nodeVector.at(i).getRValue());
		}
		std::sort(intVector.begin(), intVector.end());

		for (unsigned int i = 0; i < nodeVector.size(); i++) {
			Node node(intVector.at(i));
			newNodeVector.push_back(node);
		}

		nodeVector = newNodeVector;

	}

	void reorderSecondVector(std::vector<std::vector<int>>& item) {
		for (unsigned int i = 0; i < item.size(); i++) {
			std::sort(item.at(i).begin(), item.at(i).end());
		}
	}

	bool nodeVectorContains(int inNumber, std::vector<Node> inVector) {
		for (unsigned int i = 0; i < inVector.size(); i++) {
			if (inVector.at(i).getRValue() == inNumber) {
				return true; 
			}
		}
		return false;
	}

	bool isAString(std::string inString) {
		char firstChar = inString.at(0);
		if (firstChar == '\'') {
			return true;
		}
		else {
			return false;
		}
	}

	void printAllQueryResults() {
		std::cout << "\n\n----PRINTING QUERY TESTS----\n\n";
		

		for (unsigned int i = 0; i < QueryResultsVector.size(); i++) {
			QueryResultsVector.at(i).printRelation();


			QueryResultsVector.at(i).PrintRelationTuples();
			std::cout << "\n\n";
		}
	}

	bool vectorContains(std::vector<std::string> inVector, std::string inSymbol) {
		for (unsigned int i = 0; i < inVector.size(); i++) {
			if (inVector.at(i) == inSymbol) {
				return true;
			}
		}
		return false;
	}

	void printRuleFromRule(Rule inRule) {
		//std::cout << "   ";
		std::cout << inRule.getHeadPredicate().returnID();
		std::cout << "(";
		for (unsigned int i = 0; i < inRule.getHeadPredicate().returnElements().size(); i++) {
			if (i == 0) {
				std::cout << inRule.getHeadPredicate().returnElements().at(i);
			}
			else {
				std::cout << "," << inRule.getHeadPredicate().returnElements().at(i);
			}
		}
		std::cout << ") :- ";
		for (unsigned int i = 0; i < inRule.getPredicateList().size(); i++) {
			std::cout << inRule.getPredicateList().at(i).returnID();
			std::cout << "(";
			for (unsigned int k = 0; k < inRule.getPredicateList().at(i).returnElements().size(); k++) {
				if (k == 0) {
					std::cout << inRule.getPredicateList().at(i).returnElements().at(k);
				}
				else {
					std::cout << "," << inRule.getPredicateList().at(i).returnElements().at(k);
				}
			}
			
			if (i == inRule.getPredicateList().size() - 1) {
				std::cout << ").";
			}
			else {
				std::cout << "),";
			}
		}
		std::cout << "\n";
	}

	void printAdditions(Relation mainRelation, Relation joinedRelationTotal) {
		

		Relation uniqueAdditions;

		Relation originalRelation = mainRelation;

		mainRelation.unite(joinedRelationTotal);
		
		std::set<TupleVec> tupVecSet = mainRelation.getSetOfTupleVecs();
		std::set<TupleVec> tupVecSet2 = originalRelation.getSetOfTupleVecs();

		for (auto it = tupVecSet.begin(); it != tupVecSet.end(); ++it) {
			TupleVec testVec = *it;

			bool hasit = false;
			for (auto it2 = tupVecSet2.begin(); it2 != tupVecSet2.end(); ++it2) {
				TupleVec testVec2 = *it2; 

				if (testVec2.getVector() == testVec.getVector()) {
					hasit = true;
				}
			}

			if (!hasit) {
				uniqueAdditions.addTupleVectorToSet(testVec);
			}
		}

		uniqueAdditions.setSchemeVector(originalRelation.getSchemeVector());
		uniqueAdditions.setNameOfRelation(originalRelation.getNameOfRelation());
		uniqueAdditions.PrintRelationTuples();

	}

	

};

#endif