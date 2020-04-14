#ifndef RULE_H
#define RULE_H

#include <vector>
#include <iostream>
#include "Predicate.h"


class Rule
{
private:
	Predicate headPredicate;
	std::vector<Predicate> predicateList;
public:
	Rule(Predicate inPredicate, std::vector<Predicate> inPredicateVector) {
		headPredicate = inPredicate;
		predicateList = inPredicateVector;
	}
	Predicate getHeadPredicate() {
		return headPredicate;
	}
	std::vector<Predicate> getPredicateList() {
		return predicateList;
	}
};

#endif