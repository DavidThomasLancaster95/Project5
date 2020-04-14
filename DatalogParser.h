#ifndef DATALOGPARSER_H
#define DATALOGPARSER_H

#include <iostream>
#include <vector>
#include <string.h>
#include "CheapToken.h"
#include "Predicate.h"
#include "Rule.h"
#include "DatalogProgram.h"

//TODO clean out all the couts 


class DatalogParser {
private:
	std::vector<CheapToken> tokenVector;
	std::vector<Predicate> FactsPredicateVector;
	std::vector<Predicate> SchemesPredicateVector;
	std::vector<Predicate> QueriesPredicateVector;
	std::vector<Rule> RuleVector;
	std::string faggot; 
	int index = 0; 
	int failureIndex = 0;
	//std::vector<int> failureVector; probably can get rid of this. 
	std::vector<std::string> submitVector;
public:

	// Export DatalogProgram

	DatalogProgram getDatalogProgramDataStructure() {
		DatalogProgram datalogProgram(SchemesPredicateVector, FactsPredicateVector, RuleVector, QueriesPredicateVector);
		return datalogProgram; 
	}

	//

	void setTokenVector(std::vector<CheapToken> inVector) {
		tokenVector = inVector;
	}

	void adjustFailureIndex(int inputFailureIndex) {
		if (inputFailureIndex > failureIndex) {
			failureIndex = inputFailureIndex;
		}
	}

	void stripCommentsTokenVector() {
		std::vector<CheapToken> holder;
		for (unsigned int i = 0; i < tokenVector.size(); i++) {
			if (tokenVector.at(i).getTokenType() != "COMMENT") {
				holder.push_back(tokenVector.at(i));
			}
		}
		tokenVector = holder;

		
	}

	/*
	void printTokenVector() {
		for (unsigned int i = 0; i < DatalogParser::tokenVector.size(); i++) {
			std::cout << DatalogParser::tokenVector.at(i).getString() << "\n";
		}
	}
*/
	void printSubmitVector() {
		for (unsigned int i = 0; i < DatalogParser::submitVector.size(); i++) {
			std::cout << DatalogParser::submitVector.at(i) << "\n";
		}
		std::cout << "\n\n";
	}
	
	//TESTER
	void printFactsPredicateVector() {
		for (unsigned int i = 0; i < FactsPredicateVector.size(); i++) {
			FactsPredicateVector.at(i).printPredicate();
		}
	}

	void printSchemesPredicateVector() {
		for (unsigned int i = 0; i < SchemesPredicateVector.size(); i++) {
			SchemesPredicateVector.at(i).printPredicate();
		}
	}

	void printQueriesPredicateVector() {
		for (unsigned int i = 0; i < QueriesPredicateVector.size(); i++) {
			QueriesPredicateVector.at(i).printPredicate();
		}
	}

	void PrintRulesVector() {
		for (unsigned int i = 0; i < RuleVector.size(); i++) {
			Predicate predicate;
			predicate = RuleVector.at(i).getHeadPredicate();
			predicate.printPredicate();

			std::vector<Predicate> predList = RuleVector.at(i).getPredicateList();
			for (unsigned int k = 0; k < predList.size(); k++) {
				Predicate predicate2;
				predicate2 = predList.at(k);
				predicate2.printPredicate();
			}
			std::cout << "\n-";
		}
	}
	//^^^^^^

	void processSubmitVectorRule() {
		// The body parts
		std::vector<Predicate> bodyPredicateVector;
		//
		std::string headPredicateID = submitVector.at(0);
		std::vector<std::string> headPredicateElements;
		unsigned int processIndex = 1; 
		while (submitVector.at(processIndex) != "END HEAD") {
			headPredicateElements.push_back(submitVector.at(processIndex));
			processIndex++;
		}
		//make head predicate
		processIndex++;
		Predicate headPredicate(headPredicateID, headPredicateElements);
		headPredicateElements.clear();
		// get the sub predicates
		while (processIndex < submitVector.size() - 1) {
			headPredicateID = submitVector.at(processIndex);
			processIndex++;
			while (submitVector.at(processIndex) != "END PRED TEST") {
				
				if (submitVector.at(processIndex) == "(") {
					std::string builderStringx = "";

					int counter = 0;
					do  {
						builderStringx += submitVector.at(processIndex);
						if (submitVector.at(processIndex) == "(") {
							counter++;
						}
						if (submitVector.at(processIndex) == ")") {
							counter--;
						}
						processIndex++;
					} while (counter != 0);
					//TODO THIS CRAP
					headPredicateElements.push_back(builderStringx);
					//processIndex++;
				}
				else {
					headPredicateElements.push_back(submitVector.at(processIndex));
					processIndex++;
				}
				
			}
			processIndex++;
			Predicate bodyPredicate(headPredicateID, headPredicateElements);
			bodyPredicateVector.push_back(bodyPredicate);
			headPredicateElements.clear();
		}
		
		Rule rule(headPredicate, bodyPredicateVector);
		RuleVector.push_back(rule);


	}

	void pushToCustomVector(std::string type) {
		std::vector<std::string> holderVector;
		for (unsigned int i = 0; i < submitVector.size(); i++) {
			if (type == "QUERIES" && (i != 0)) {
				
				std::string parenBuilderString = "";
				

				if (submitVector.at(i) != "(") {
					//std::cout << submitVector.at(i) << "\n";
					holderVector.push_back(submitVector.at(i));
				} 
				else {
					i++;
					int parenCounter = 1; 
					parenBuilderString += "(";

					while(parenCounter != 0){
						if(submitVector.at(i) == "(") {
							parenCounter++;
						}
						else if (submitVector.at(i) == ")"){
							parenCounter--;
						}
						parenBuilderString += submitVector.at(i);
						i++;
					}
					i--;
					//std::cout << parenBuilderString << "\n";
					holderVector.push_back(parenBuilderString);
				}


			}
			else if (i != 0) {
				holderVector.push_back(submitVector.at(i));
			}
		}
		Predicate predicate(submitVector.at(0), holderVector);
		if (type == "FACTS") {
			FactsPredicateVector.push_back(predicate);
		}
		else if (type == "SCHEMES") {
			SchemesPredicateVector.push_back(predicate);
		}
		else if (type == "QUERIES") {
			QueriesPredicateVector.push_back(predicate);
		}

	}

	

	void clearSubmitVector() {
		submitVector.clear();
	}


	

	bool propperGrammar() {
		index = 0;
		if (SchemesSectionTest() && FactsSectionTest() && RulesSectionTest() && QueriesSectionTest() == true) {
			//std::cout << "Success!\n";
			return true;
		}
		std::cout << "Failure!\n";
		tokenVector.at(failureIndex).toString();
		

		return false; 
	}

	// Helper Function

	bool NextIsFacts() {
		if (tokenVector.at(index).getTokenType() == "FACTS") {
			return true; 
		}
		return false;
	}

	bool NextIsCustom(std::string inString) {
		if (tokenVector.at(index).getTokenType() == inString) {
			return true;
		}
		return false;
	}

	bool EOFTest() {
		if (tokenVector.at(index).getTokenType() == "EOF") {
			index++;
			return true;
		}
		//adjustFailureIndex(index);
		//std::cout << "EOF false \n";
		return false;
	}

	bool STRINGTest() {
		if (tokenVector.at(index).getTokenType() == "STRING") {
			submitVector.push_back(tokenVector.at(index).getTokenData());
			index++;
			return true;
		}
		
		adjustFailureIndex(index);
		//std::cout << "STRING false " << index << "\n";
		return false;
	}

	bool RULESTest() {
		if (tokenVector.at(index).getTokenType() == "RULES") {
			index++;
			return true;
		}
		adjustFailureIndex(index);
		//std::cout << "RULES false \n";
		return false;
	}

	bool PERIODTest() {
		if (tokenVector.at(index).getTokenType() == "PERIOD") {
			index++;
			return true;
		}
		adjustFailureIndex(index);
		//std::cout << "PERIOD false " << index << "\n";
		return false;
	}

	bool Q_MARKTest() {
		if (tokenVector.at(index).getTokenType() == "Q_MARK") {
			//submitVector.push_back(tokenVector.at(index).getString());
			index++;
			return true;
		}
		adjustFailureIndex(index);
		//std::cout << "Q_MARK false \n";
		return false;
	}

	bool SCHEMESTest() {
		if (tokenVector.at(index).getTokenType() == "SCHEMES") {
			index++;
			return true;
		}
		adjustFailureIndex(index);
		//std::cout << "SCHEMES false \n";
		return false;
	}

	bool QUERIESTest() {
		if (tokenVector.at(index).getTokenType() == "QUERIES") {
			index++;
			return true;
		}
		adjustFailureIndex(index);
		//std::cout << "QUERIES false \n";
		return false;
	}

	bool FACTSTest() {
		if (tokenVector.at(index).getTokenType() == "FACTS") {
			index++;
			return true;
		}
		adjustFailureIndex(index);
		//std::cout << "FACTS false " << index << "\n";
		return false;
	}

	bool COLONTest() {
		if (tokenVector.at(index).getTokenType() == "COLON") {
			index++;
			return true;
		}
		adjustFailureIndex(index);
		//std::cout << "COLON false " << index << "\n";
		return false;
	}

	bool COLONDASHTest() {
		if (tokenVector.at(index).getTokenType() == "COLON_DASH") {
			index++;
			return true;
		}
		adjustFailureIndex(index);
		//std::cout << "COLON_DASH false " << index << "\n";
		return false;
	}

	bool IDTest() {
		if (tokenVector.at(index).getTokenType() == "ID") {
			submitVector.push_back(tokenVector.at(index).getTokenData());
			index++;
			return true;
		}
		//std::cout << DatalogParser::tokenVector.at(index).getTokenType() << "\n";
		adjustFailureIndex(index);
		//std::cout << index << " ID false " << index << "\n";
		return false;
	}

	bool RIGHTPARENTest() {
		if (tokenVector.at(index).getTokenType() == "RIGHT_PAREN") {
			index++;
			return true;
		}
		adjustFailureIndex(index);
		//std::cout << "RIGHT_PAREN false " << index << "\n";
		return false;
	}

	bool LEFTPARENTest() {
		if (tokenVector.at(index).getTokenType() == "LEFT_PAREN") {
			index++;
			return true;
		}
		adjustFailureIndex(index);
		//std::cout << "LEFT_PAREN false " << index << "\n";
		return false;
	}

	bool RIGHTPARENExpTest() {
		if (tokenVector.at(index).getTokenType() == "RIGHT_PAREN") {
			submitVector.push_back(tokenVector.at(index).getTokenData());
			index++;
			return true;
		}
		adjustFailureIndex(index);
		//std::cout << "RIGHT_PAREN false " << index << "\n";
		return false;
	}

	bool LEFTPARENExpTest() {
		if (tokenVector.at(index).getTokenType() == "LEFT_PAREN") {
			submitVector.push_back(tokenVector.at(index).getTokenData());
			index++;
			return true;
		}
		adjustFailureIndex(index);
		//std::cout << "LEFT_PAREN false " << index << "\n";
		return false;
	}

	bool COMMATest() {
		if (tokenVector.at(index).getTokenType() == "COMMA") {
			index++;
			return true;
		}
		adjustFailureIndex(index);
		//std::cout << "COMMA false " << index << "\n";
		return false;
	}

	bool idListTest() {
		int backtrack = index; 
		if (COMMATest() && IDTest() && idListTest()) {
			return true;
		}
		index = backtrack;
		if (RIGHTPARENTest()) {
			return true; 
		}
		index = backtrack;
		if (EOFTest()) {
			return true;
		}
		index = backtrack;
		//std::cout << "idList false \n";
		return false; 
	}

	bool SchemeTest() {
		clearSubmitVector();
		if (IDTest() && LEFTPARENTest() && IDTest() && idListTest()) {
			pushToCustomVector("SCHEMES");
			clearSubmitVector();
			return true; 
		}
			return false; 
	}

	bool SchemeListTest() {
		int backtrack = index;
		if (SchemeTest() && SchemeListTest()) {
			return true;
		}
		index = backtrack;
		if (EOFTest()) {
			return true;
		}
		index = backtrack;
		if (EOFTest()) {
			return true;
		}
		if (NextIsFacts()) {
			return true; 
		}
		index = backtrack;
		//std::cout << "idList false \n";
		return false;
	}

	bool SchemesSectionTest() {
		return SCHEMESTest() && COLONTest() && SchemeTest()  && SchemeListTest() ;
	}

	// Facts Workout

	bool StringListTest() {
		int backtrack = index;
		if (COMMATest() && STRINGTest() && StringListTest()) {
			return true;
		}
		index = backtrack;
		if (RIGHTPARENTest() && PERIODTest()) {
			return true;
		}
		//std::cout << "StringListTest false \n";
		
		return false;
	}

	bool FactTest() {
		clearSubmitVector();  
		if (IDTest() && LEFTPARENTest() && STRINGTest() && StringListTest()) {
			//printSubmitVector();
			pushToCustomVector("FACTS");
			clearSubmitVector();
			return true;
		}
		return false; 
		
	}

	bool FactListTest() {
		int backtrack = index;
		if (FactTest() && FactListTest()) {
			return true;
		}
		index = backtrack;
		if (NextIsCustom("RULES")) { 
			return true;
		}
		index = backtrack;
		//std::cout << "FactListTest false \n";
		
		return false;
	}

	bool FactsSectionTest() {
		return FACTSTest() && COLONTest()  && FactListTest() ;
	}

	// Rules Workout

	bool idListTestForRules() {
		int backtrack = index;
		if (COMMATest() && IDTest() && idListTestForRules()) {
			return true;
		}
		index = backtrack;
		if (NextIsCustom("RIGHT_PAREN")) {  
			return true;
		}
		index = backtrack;
		return false;
	}

	bool HeadPredicateTest() {
		if (IDTest() && LEFTPARENTest() && IDTest() && idListTestForRules() && RIGHTPARENTest()) {
			submitVector.push_back("END HEAD");
			return true; 
		}
		return false; 

		
	}

	bool PredicateListTest() {
		int backtrack = index;
		if (COMMATest() && PredicateTest() && PredicateListTest()) {
			return true; 
		}
		index = backtrack;
		if (NextIsCustom("PERIOD")) {
			return true;
		}
		index = backtrack;
		return false; 
	}

	bool OperatorTest() {
		int backtrack = index; 
		if (NextIsCustom("ADD")) {
			submitVector.push_back(tokenVector.at(index).getTokenData());
			index++;
			return true;
		}
		index = backtrack;
		if (NextIsCustom("MULTIPLY")) {
			submitVector.push_back(tokenVector.at(index).getTokenData());
			index++;
			return true;
		}
// Addition
adjustFailureIndex(index);
// ^^^^^^^^
		index = backtrack;
		return false; 
	}

	bool ExpressionTest() {
		return LEFTPARENExpTest() && ParameterTest() && OperatorTest() && ParameterTest() && RIGHTPARENExpTest();
	}

	bool ParameterTest() {
		int backtrack = index;
		if (STRINGTest()) {
			return true;
		}
		index = backtrack;
		if (IDTest()) {
			return true;
		}
		index = backtrack;
		if (ExpressionTest()) {
			return true;
		}
		index = backtrack;
		return false; 
	}

	bool ParameterListTest() {
		int backtrack = index;
		if (COMMATest() && ParameterTest() && ParameterListTest()) {
			return true; 
		}
		index = backtrack;
		if (NextIsCustom("RIGHT_PAREN")) {
			return true;
		}
		index = backtrack;
		return false; 
	}

	bool PredicateTest() {
		if (IDTest() && LEFTPARENTest() && ParameterTest() && ParameterListTest() && RIGHTPARENTest()) {
			submitVector.push_back("END PRED TEST");
			return true;
		}
		return false; 
		
	}

	bool PredicateTestForQuery() {
		if (IDTest() && LEFTPARENTest() && ParameterTest() && ParameterListTest() && RIGHTPARENTest()) {
			return true;
		}
		return false;
	}

//ADDITION
	bool PredicateTestForQueryWithExpTest(){
		if (IDTest() && LEFTPARENExpTest() && ParameterTest() && ParameterListTest() && RIGHTPARENExpTest()) {
			return true; 
		}
		return false;
	}
//^^^^^^^^^^^^^
	bool RuleTest() {
		clearSubmitVector();
		if (HeadPredicateTest() && COLONDASHTest() && PredicateTest() && PredicateListTest() && PERIODTest()) {
			//printSubmitVector();
			processSubmitVectorRule();
			clearSubmitVector();
			return true;
		}
		return false; 
	}

	bool RuleListTest() {
		int backtrack = index;
		if (RuleTest() && RuleListTest()) {
			return true;
		}
		index = backtrack;
		if (/*EOFTest()*/NextIsCustom("QUERIES")) {  
			return true;
		}
		index = backtrack;
		return false;
	}

	bool RulesSectionTest() {
		return RULESTest() && COLONTest() && RuleListTest();
	}
	
	// Queries Workout

	bool QueryTest() {
		clearSubmitVector();
// ADDITION was Originally PredicateTestForQuery() changed to PredicateTestForQueryWithExpTest() /changed back/
// tried changing predicatetestforquery to predicatetest() /changed back/
		if (PredicateTestForQuery() && Q_MARKTest()) {
// ^^^^^^^^^^^^^^^^^^
			//printSubmitVector();
			pushToCustomVector("QUERIES");
			clearSubmitVector();
			return true; 
		}
		return false;
	}

	bool QueryListTest() {
		int backtrack = index;
		if (QueryTest() && QueryListTest()) {
			return true;
		}
		index = backtrack;
		if (EOFTest()) {  // TODO TO ADJUST THIS FOR THE NEXT ACTUAL QUERIES AND NOT EOF!!!
			return true;
		}
		index = backtrack;
		return false;
	}

	bool QueriesSectionTest() {
		return QUERIESTest() && COLONTest() && QueryTest() && QueryListTest();
	}

	// ^^^^^^^^^^^^^^^^

};

#endif