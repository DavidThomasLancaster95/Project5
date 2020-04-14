#ifndef SCHEMEVEC_H
#define SCHEMEVEC_H

#include <vector>
#include <string>

class SchemeVec
{
private:
	std::vector<std::string> SchemeVector;
public:
	SchemeVec() {

	}

	std::vector<std::string> getVector() {
		return SchemeVector;
	}

	void pushBack(std::string inString) {
		SchemeVector.push_back(inString);
	}
	int size() {
		return SchemeVector.size();
	}
	std::string at(int i) {
		return SchemeVector.at(i);
	}

	void setAt(int i, std::string inString) {
		SchemeVector.at(i) = inString;
	}
	
	void changeVal(int index, std::string inString) {
		SchemeVector.at(index) = inString;
	}

	void removeElementByValue(std::string inString) {
		SchemeVector.erase(std::remove(SchemeVector.begin(), SchemeVector.end(), inString), SchemeVector.end());
	}

};


#endif