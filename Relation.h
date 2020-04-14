#ifndef RELATION_H
#define RELATION_H

#include <string>
#include <set>
#include <vector>
#include "SchemeVec.h"
#include "TupleVec.h"
#include <string>
#include "ConstantMaker.h"
#include <algorithm>

class Relation
{
private:
	std::string nameOfRelation;

	SchemeVec schemeVector;
	
	std::set<TupleVec> setOfTupleVecs;

public:

	Relation joinRelation(Relation inRelation) {

		Relation newRelation;

		SchemeVec newSchemeVector = schemeVector;
		std::set<TupleVec> newSetOfTupleVecs;
		std::vector<std::string> commonSymbols;
		

		// combine scheme vectors, mark common symbols
			// push the first set
		
		
		combineSchemeVectors(inRelation, newSchemeVector, commonSymbols);
		//----------------------------------------------------------------

		std::vector <std::pair <std::string, std::string>> pairVector;

		// original tuple vec
		for (auto it = setOfTupleVecs.begin(); it != setOfTupleVecs.end(); ++it) {
			TupleVec testVec = *it;
			bool matches = false; 
			TupleVec holdTupleVec;
			TupleVec holdTupleVec2;
			pairVector.clear();

			if (testVec.size() > 0) {
				for (int k = 0; k < testVec.size(); k++) {
					if (vectorContains(commonSymbols, schemeVector.at(k))) {
						std::pair <std::string, std::string> commonPair(schemeVector.at(k), testVec.at(k)); // scheme then val
						pairVector.push_back(commonPair);
					}
					holdTupleVec.pushBack(testVec.at(k));
					holdTupleVec2.pushBack(testVec.at(k));
				}

				std::set<TupleVec> holdSetOfTupleVecs = inRelation.getSetOfTupleVecs();
				SchemeVec inRelationSchemeVec = inRelation.getSchemeVector();

				for (auto it2 = holdSetOfTupleVecs.begin(); it2 != holdSetOfTupleVecs.end(); ++it2) {
					TupleVec testVec2 = *it2;
					for (int j = 0; j < testVec2.size(); j++) {
						

						if (pairVectorsMatch(pairVector, testVec2, inRelationSchemeVec)) {
							//holdTupleVec.pushBack(testVec2.at(j));

							// if its not in the pair, add it.

							if (!holdsPairElement1Match(pairVector, inRelationSchemeVec.at(j))) {
								holdTupleVec.pushBack(testVec2.at(j));
							}

							// submit to holder vector?



							matches = true; 
						}
						else {
							matches = false;
						}
						
						 
						
					}
					if (matches == true) {
						newSetOfTupleVecs.insert(holdTupleVec);
						holdTupleVec.clearVector();
						holdTupleVec = holdTupleVec2;
					}
					
				}
				

				
			}

			
		}
	
		//----------------------------------------------------------------
		


		newRelation.setSchemeVector(newSchemeVector);
		newRelation.setSetOfTupleVecs(newSetOfTupleVecs);
		newRelation.setNameOfRelation("boh");
		// combine tuples, where they have same common symbols, add a line. 
		return newRelation;
	}

	bool pairVectorsMatch(std::vector <std::pair <std::string, std::string>> pairVector, TupleVec testVec, SchemeVec inSchemeVec) {
		for (int i = 0; i < inSchemeVec.size(); i++) {
			if (holdsPairElement1Match(pairVector, inSchemeVec.at(i))) {
				if (holdsPairElement2Match(pairVector, inSchemeVec.at(i), testVec.at(i))) {

				}
				else {
					return false;
				}
			}
		}
		return true; 
	}

	bool holdsPairElement2Match(std::vector <std::pair <std::string, std::string>> pairVector, std::string inString, std::string testVecString) {
		for (unsigned int i = 0; i < pairVector.size(); i++) {
			if (pairVector.at(i).first == inString) {
				if (pairVector.at(i).second == testVecString) {
					return true; 
				}
				else {
					return false; 
				}
			}
		}
		return false;
	}

	bool holdsPairElement1Match(std::vector <std::pair <std::string, std::string>> pairVector, std::string inString) {
		for (unsigned int i = 0; i < pairVector.size(); i++) {
			if (pairVector.at(i).first == inString) {
				return true;
			}
		}
		return false; 
	}


	void setSchemeVector(SchemeVec inSchemeVec) {
		schemeVector = inSchemeVec;
		
	}
	void setSetOfTupleVecs(std::set<TupleVec> inSetOfTupleVecs) {
		setOfTupleVecs = inSetOfTupleVecs;
	}

	void setNameOfRelation(std::string inString) {
		nameOfRelation = inString;
	}
	void addToSchemeVector(std::string inString) {
		schemeVector.pushBack(inString);
	}
	void addTupleVectorToSet(TupleVec tupleVec) {
		setOfTupleVecs.insert(tupleVec);
		
	}
	std::string getNameOfRelation() {
		return nameOfRelation;
	}

	SchemeVec getSchemeVector() {
		return schemeVector;
	}
	
	void printRelation() {
		std::cout << "Name:" << nameOfRelation << "\n";
		for (int i = 0; i < schemeVector.size(); i++) {
			std::cout << schemeVector.at(i) << "- ";
			
		}
		std::cout << "\n";

		for (auto it = setOfTupleVecs.begin(); it != setOfTupleVecs.end(); ++it) {
			TupleVec testVec = *it;
			for (int k = 0; k < testVec.size(); k++) {
				std::cout << testVec.at(k) << " ";
			}
			std::cout << "\n";
		}
	}
	Relation SelectString(std::string inString, int index) {
		Relation returnRelation;
		// Set the Name
		returnRelation.setNameOfRelation(nameOfRelation);

		// Add all the parts to the vector
		for (int i = 0; i < schemeVector.size(); i++) {
			returnRelation.addToSchemeVector(schemeVector.at(i));
		}
		// Add the Correct Rows
		for (auto it = setOfTupleVecs.begin(); it != setOfTupleVecs.end(); ++it) {
			TupleVec testVec = *it;
			if (testVec.at(index) == inString) {
				returnRelation.addTupleVectorToSet(testVec);
			}
			
		}
		
		return returnRelation;
	}

	bool integerIsInVectorOfIntegers(std::vector<int> inVec, int index){
		for (unsigned int i = 0; i < inVec.size(); i++) {
			if (inVec.at(i) == index){
				return true;
			}
		}
		return false;
	}
	
	std::set<TupleVec> getSetOfTupleVecs() {
		return setOfTupleVecs;
	}

	Relation SelectConstant(ConstantMaker constantMaker) {
		Relation returnRelation;
		// Set the Name
		returnRelation.setNameOfRelation(nameOfRelation);

		// Add all the parts to the vector
		for (int i = 0; i < schemeVector.size(); i++) {
			 
			// This part below does the renaming.... Could be changed to use the rename algorithm. 
			if (/* i is a constant marker*/integerIsInVectorOfIntegers(constantMaker.getPositionVector(), i)) {
				// add the constantMaker.name to return relation
				returnRelation.addToSchemeVector(constantMaker.getConstantValue());
			}
			else {
				returnRelation.addToSchemeVector(schemeVector.at(i));
			}
			

			//returnRelation.addToSchemeVector(schemeVector.at(i));
		}

		// Add the Correct Rows
		for (auto it = setOfTupleVecs.begin(); it != setOfTupleVecs.end(); ++it) {
			TupleVec testVec = *it;
			
			if (elementsAreEqual(constantMaker, testVec)) {
				returnRelation.addTupleVectorToSet(testVec);
			}

		}

		return returnRelation;
	}

	void Rename(std::string inName, std::string nameToChange){
		for (int i = 0; i < schemeVector.size(); i++) {
			if (schemeVector.at(i) == nameToChange) {
				schemeVector.at(i) == inName;
			}
		}
	}

	void StripAndRename(ConstantMaker constantMaker){
		Project(constantMaker.getPositionVector());
	}

	void PrintRelationTuples() {

		//Id='33333', A='12 Apple St.', P='555-1234'
		
		

		for (auto it = setOfTupleVecs.begin(); it != setOfTupleVecs.end(); ++it) {
			TupleVec testVec = *it;

			if (testVec.size() > 0) {
				std::cout << "  ";
				for (int k = 0; k < testVec.size(); k++) {
					if (k == testVec.size() - 1) {
						std::cout << schemeVector.at(k) << "=";
						std::cout << testVec.at(k);
					}
					else {
						std::cout << schemeVector.at(k) << "=";
						std::cout << testVec.at(k) << ", ";
					}
				}
				if (setOfTupleVecs.size() > 1) {
					//std::cout << "\n";
				}
				std::cout << "\n";
			}
		}
	}

	void Project(std::vector<int> inputVector){
		for (int i = schemeVector.size() - 1; 0 < i; i--) {
			if (!(std::count(inputVector.begin(), inputVector.end(), i))) {
				schemeVector.at(i+1).erase(schemeVector.at(i+1).begin() + i+1);
				std::cout << "Removed\n";
			}
		}


		// TODO finish this
	}


	void ProjectByIndex(int index) {
		
		for (int i = schemeVector.size() - 1; i >= 0; i--) {
			if (i == index) {

				schemeVector.changeVal(i, "FAG");
				if (schemeVector.at(i) == "FAG") {
					schemeVector.removeElementByValue("FAG");
				}
			}
		}

		// now to remove all the tuples

		std::set<TupleVec> replacementSet;


		for (auto it = setOfTupleVecs.begin(); it != setOfTupleVecs.end(); ++it) {
			TupleVec testVec = *it;

			TupleVec replacementTupleVec;

			for (int k = 0; k < testVec.size(); k++) {
				if (k != index) {
					replacementTupleVec.pushBack(testVec.at(k));
				}
			}

			replacementSet.insert(replacementTupleVec);
		}
		setOfTupleVecs = replacementSet;
	}

	void ProjectByString(std::string removeString) {
		int removeIndex = 0;

		for (int i = schemeVector.size() - 1; i >= 0 ; i--) {
			if (schemeVector.at(i) == removeString) {
				
				removeIndex = i;
				schemeVector.changeVal(i, "FAG");
				if (schemeVector.at(i) == "FAG") {
					schemeVector.removeElementByValue("FAG");
				}
			}
		}

		// now to remove all the tuples

		std::set<TupleVec> replacementSet;

		for (auto it = setOfTupleVecs.begin(); it != setOfTupleVecs.end(); ++it) {
			TupleVec testVec = *it;

			TupleVec replacementTupleVec;

			for (int k = 0; k < testVec.size(); k++) {
				if (k != removeIndex) {
					replacementTupleVec.pushBack(testVec.at(k));
				}
			}
			//std::cout << removeIndex << " " << testVec.at(removeIndex) << "\n";

			replacementSet.insert(replacementTupleVec);
		
			//testVec.removeElementByIndex(removeIndex);
			
		}

		setOfTupleVecs = replacementSet;

	}

	bool elementsAreEqual(ConstantMaker constantMaker, TupleVec tupleVec) {
		
		std::string commonValue = tupleVec.at(constantMaker.getPositionVector().at(0));

		for (unsigned int i = 0; i < constantMaker.getPositionVector().size(); i++) {
			if (tupleVec.getVector().at(constantMaker.getPositionVector().at(i)) == commonValue) {

			}
			else {
				return false; 
			}
		}

		return true; 

	}

	
	// Join Helper functions
	
	bool vectorContains(std::vector<std::string> inVector, std::string inSymbol) {
		for (unsigned int i = 0; i < inVector.size(); i++) {
			if (inVector.at(i) == inSymbol) {
				return true; 
			}
		}
		return false;
	}

	void combineSchemeVectors(Relation& inRelation, SchemeVec& newSchemeVector, std::vector<std::string>& commonSymbols) {
		for (int i = 0; i < inRelation.getSchemeVector().size(); i++) {
			if (vectorContains(newSchemeVector.getVector(), inRelation.getSchemeVector().at(i))) {
				commonSymbols.push_back(inRelation.getSchemeVector().at(i));
			}
			else {
				newSchemeVector.pushBack(inRelation.getSchemeVector().at(i));
			}
		}
	}

	void unite(Relation inRelation) {
		
		
		std::set<TupleVec> mySetTupleVec = inRelation.getSetOfTupleVecs();

		for (auto it = mySetTupleVec.begin(); it != mySetTupleVec.end(); ++it) {
			TupleVec testVec = *it;

			setOfTupleVecs.insert(testVec);
			
		}

	}

	void reorder(std::string inString, int position) {
		int theFoundPosition;

		for (int i = 0; i < schemeVector.size(); i++) {
			if (schemeVector.at(i) == inString) {

				theFoundPosition = i;
				//movetopandtuplestoposition
				// save value
				std::string schemeVectorValue = schemeVector.at(i);
				std::string originalValue = schemeVector.at(position);

				schemeVector.setAt(i, originalValue);
				schemeVector.setAt(position, schemeVectorValue);

				std::set<TupleVec> newTupVecSet;
				// fix all the tupleVectors
				for (auto it = setOfTupleVecs.begin(); it != setOfTupleVecs.end(); ++it) {
					TupleVec testVec = *it;

					std::string tupfoundval = testVec.at(theFoundPosition);
					std::string tuporiginval = testVec.at(position);

					if (testVec.size() > 0) {

						testVec.setAt(i, tuporiginval);
						testVec.setAt(position, tupfoundval);
					}

					newTupVecSet.insert(testVec);
				}

				setOfTupleVecs = newTupVecSet;


			}
		}

		


	}

	

};




#endif