#ifndef DATALOGPROGRAM_H
#define DATALOGPROGRAM_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "Predicate.h"
#include "Rule.h"

class DatalogProgram
{
private:

	std::vector<Predicate> SchemesPredicateVector;
	std::vector<Predicate> FactsPredicateVector;
	std::vector<Rule> RuleVector;
	std::vector<Predicate> QueriesPredicateVector;
	std::vector<std::string> domainsVector;
	
public:
	DatalogProgram(std::vector<Predicate> inSchemesVector, std::vector<Predicate> inFactsVector, std::vector<Rule> inRulesVector, std::vector<Predicate> inQueriesVector) {
		SchemesPredicateVector = inSchemesVector;
		FactsPredicateVector = inFactsVector;
		RuleVector = inRulesVector;
		QueriesPredicateVector = inQueriesVector;

		//get raw domains into domain vector
		for (unsigned int i = 0; i < FactsPredicateVector.size(); i++) {
			for (unsigned int k = 0; k < FactsPredicateVector.at(i).returnElements().size(); k++) {
				domainsVector.push_back(FactsPredicateVector.at(i).returnElements().at(k));
			}
		}
		// sorrt the domain vector
		
		cleanoutDuplicates();

		sortAlphabetical();

	}

	void sortAlphabetical() {
		std::sort(domainsVector.begin(), domainsVector.end());
	}

	void cleanoutDuplicates() {
		auto end = domainsVector.end();
		for (auto it = domainsVector.begin(); it != end; ++it) {
			end = std::remove(it + 1, end, *it);
		}
		domainsVector.erase(end, domainsVector.end());
	}

	DatalogProgram() {

	}


	std::vector<Predicate> getSchemesPredicateVector() {
		return SchemesPredicateVector;
	}
	std::vector<Predicate> getFactsPredicateVector() {
		return FactsPredicateVector;
	}
	std::vector<Rule> getRuleVector() {
		return RuleVector;
	}
	std::vector<Predicate> getQueriesPredicateVector() {
		return QueriesPredicateVector;
	}

	void printSchemesPredicateVector() {
		std::cout << "Schemes(" << SchemesPredicateVector.size() << "):\n";
		for (unsigned int i = 0; i < SchemesPredicateVector.size(); i++) {
			std::cout << "  ";
			SchemesPredicateVector.at(i).printPredicate();
			std::cout << "\n";
		}
	}
	void printFactsPredicateVector() {
		std::cout << "Facts(" << FactsPredicateVector.size() << "):\n";
		for (unsigned int i = 0; i < FactsPredicateVector.size(); i++) {
			std::cout << "  ";
			FactsPredicateVector.at(i).printPredicate();
			std::cout << ".\n";
		}
	}
	void printQueriesPredicateVector() {
		std::cout << "Queries(" << QueriesPredicateVector.size() << "):\n";
		for (unsigned int i = 0; i < QueriesPredicateVector.size(); i++) {
			std::cout << "  ";
			QueriesPredicateVector.at(i).printPredicate();
			std::cout << "?\n";
		}
	}
// ADDITION
	void printRawQueriesPredicateVector() {
		for (unsigned int i = 0; i < QueriesPredicateVector.size(); i++) {
			std::vector<std::string> predicateElements;
			predicateElements = QueriesPredicateVector.at(i).returnElements();
			
			for (unsigned int k = 0; k < predicateElements.size(); k++) {
				std::cout << predicateElements.at(k) << "\n";
			}
			
		}
	}
// ^^^^^^^^^^^^^
	void printRuleVector() {
		std::cout << "Rules(" << RuleVector.size() << "):\n";
		for (unsigned int i = 0; i < RuleVector.size(); i++) {
			std::cout << "  ";
			std::vector<Predicate> predicateVector;
			predicateVector = RuleVector.at(i).getPredicateList();
			RuleVector.at(i).getHeadPredicate().printPredicate();
			std::cout << " :- ";
			predicateVector.at(0).printPredicate();
			for (unsigned int k = 1; k < predicateVector.size(); k++) {
				std::cout << ",";
				predicateVector.at(k).printPredicate();
			}
			std::cout << ".\n";
		}
	}

	void printDomains() {
		std::cout << "Domain(" << domainsVector.size() << "):\n";
		for (unsigned int i = 0; i < domainsVector.size(); i++) {
			std::cout << "  ";
			std::cout << domainsVector.at(i);
			std::cout << "\n";
		}
	}

	void printAll() {
		printSchemesPredicateVector();
		printFactsPredicateVector();
		printRuleVector();
		printQueriesPredicateVector();
		printDomains();
	}



};

#endif