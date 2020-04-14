#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>
#include <vector>
#include <iostream>

class Predicate
{
private:
	std::string ID;
	std::vector<std::string> elements;
public:
	Predicate(std::string inID, std::vector<std::string> inElements) {
		ID = inID;
		elements = inElements; 
	}
	Predicate() {

	}
	std::string returnID() {
		return ID;
	}
	std::vector<std::string> returnElements() {
		return elements; 
	}
	void printPredicate() {
		std::cout << ID << "(";
		for (unsigned int i = 0; i < elements.size(); i++) {
			if (i == 0) {
				std::cout << elements.at(i);
			}
			else {
				std::cout << "," << elements.at(i);
			}
		}
		std::cout << ")";
	}

	// TODO add to String method 
	// TODO perhaps add mutator methods. 
};

#endif