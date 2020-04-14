#ifndef CONSTANT_MAKER
#define CONSTANT_MAKER

#include<string>
#include<vector>

class ConstantMaker
{
private:
	std::string ConstantValue;
	std::vector<int> positionVector;

public:
	ConstantMaker(std::string inConstantValue) {
		ConstantValue = inConstantValue;
	}
	void pushPositionVector(int inString) {
		positionVector.push_back(inString);
	}
	std::string getConstantValue() {
		return ConstantValue;
	}
	std::vector<int> getPositionVector() {
		return positionVector; 
	}

};


#endif
