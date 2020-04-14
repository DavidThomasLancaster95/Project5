#ifndef PARAMETEROBLIGATORY_H
#define PARAMETEROBLIGATORY_H

#include <string>

class ParameterObligatory
{
private:
	std::string actualData;
public:
	ParameterObligatory(std::string inString) {
		actualData = inString;
	}
	std::string getActualData() {
		return actualData;
	}
	void setActualData(std::string inString) {
		actualData = inString;
	}

};

#endif