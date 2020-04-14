#ifndef CHEAPTOKEN_H
#define CHEAPTOKEN_H

#include <iostream>
#include <string>


class CheapToken {
private:
	std::string actualData;
	std::string tokenType;
	int lineNumber;
public:
	CheapToken(std::string actualDatax, std::string tokenTypex, int lineNumberx) {
		actualData = actualDatax;
		tokenType = tokenTypex;
		lineNumber = lineNumberx;
	}
	void toString() {
		std::cout << "(" << CheapToken::tokenType << ",\"" << CheapToken::actualData << "\",";
		std::cout << CheapToken::lineNumber << ")\n";
	}
	std::string getString() {
		std::string returnString = "(" + CheapToken::tokenType + ",\"" + CheapToken::actualData + "\",";
		std::string s = std::to_string(CheapToken::lineNumber);
		returnString += s + ")";
		return returnString;
	}
	std::string getTokenType() {
		return CheapToken::tokenType;
	}
	std::string getTokenData() {
		return CheapToken::actualData;
	}
};

#endif
