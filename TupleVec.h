#ifndef TUPLEVEC_H
#define TUPLEVEC_H

#include <vector>
#include <string>


class TupleVec
{
private:
	std::vector<std::string> TupleVector;
public:
	TupleVec() {

	}
	void pushBack(std::string inString) {
		TupleVector.push_back(inString);
	}

	void clearVector() {
		TupleVector.clear();
	}

	int size() {
		return TupleVector.size();
	}

	std::string at(int i) {
		return TupleVector.at(i);
	}

	std::vector<std::string> getVector() {
		return TupleVector;
	}

	void setAt(int i, std::string inString) {
		TupleVector.at(i) = inString;
	}
	// Obligatory piece of crap that I don't know what it is. 

	/*The standard library set requires that its templated type have a less - than operator. Since
		you will want a set of Tuple, you need to add the less - than operator:*/
	bool operator< (const TupleVec& other) const {
		if (TupleVector < other.TupleVector) {
			return true;
		}
		else {
			return false;
		}
		// compare this Tuple to other Tuple
	}

	/*void removeElementByValue(std::string inString) {
		TupleVector.erase(std::remove(TupleVector.begin(), TupleVector.end(), inString), TupleVector.end());
	}*/

	void removeElementByIndex(int index) {
		TupleVector.erase(TupleVector.begin() + index);
	}

};

#endif