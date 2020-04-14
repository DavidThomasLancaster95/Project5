#ifndef SCANNERMACHINE_H
#define SCANNERMACHINE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector> 
#include <ctype.h> 
#include "CheapToken.h"

class ScannerMachine {
private:
	std::vector<CheapToken> tokenVector;
public:
	ScannerMachine() {

	}
	void printTokens() {
		for (unsigned int i = 0; i < ScannerMachine::tokenVector.size(); i++) {
			ScannerMachine::tokenVector.at(i).toString();
		}
		// NOTE CHANGING COUT TO EXPORTS. 
		std::cout << "Total Tokens = ";
		std::cout << ScannerMachine::tokenVector.size();
		//std::cout << '\n';
	}
	std::vector<CheapToken> getTokensVector() {
		return ScannerMachine::tokenVector;
	}
	void addTokenBackdoor(CheapToken cheapToken) {
		ScannerMachine::tokenVector.push_back(cheapToken);
	}
	void tokenize(std::string filename) {
		std::ifstream infile;
		infile.open(filename);
//FIXING EMPTY FILE!!
		// if (infile.peek() == std::ifstream::traits_type::eof()){

		// 	std::cout << "the file is empty \n";
		// 	return null; 
		// }
//^^^FIXING EMPTY FILE^^^^

		int lineCounter = 1;
		//int characterCounter = 1; 
		std::string x = "";
		while (1) {
			char c = infile.get();
//FIXING THE EMPTY FILE!!
            
            if (infile.eof()) {
                CheapToken tokenToken("", "EOF", lineCounter);
				ScannerMachine::addTokenBackdoor(tokenToken);
				break;
            }
//^^^FIXING THE EMPTY FILE^^^^^^^^^^^
            
			// WHERE THE MAGIC HAPPENS

			if (tokenize_singleOpenAssigner(c, lineCounter) == false) {
				while (1) {
					std::string longData(1, c);
					// Check for Colon Dash
					if ((infile.peek() == '-') & (c == ':')) {
						CheapToken tokenToken(":-", "COLON_DASH", lineCounter);
						ScannerMachine::addTokenBackdoor(tokenToken);
						infile.get();
						break;
					}
					// Check for Single Colon
					else if ((infile.peek() != '-') & (c == ':')) {
						CheapToken tokenToken(":", "COLON", lineCounter);
						ScannerMachine::addTokenBackdoor(tokenToken);
						break;
					}
					// Check if id
					else if (isalpha(c)) {
						while (isalnum(infile.peek())) {
							std::string hol(1, infile.get());
							longData += hol;
						}
						//Does this String Match a key word?
						tokenize_idAssigner(longData, lineCounter);
					}
					// check if single normal comment
					else if ((c == '#') & (infile.peek() != '|')) {
						while (infile.peek() != '\n') {
							std::string hol(1, infile.get());
							longData += hol;
						}
						CheapToken tokenToken(longData, "COMMENT", lineCounter);
						ScannerMachine::addTokenBackdoor(tokenToken);
					}
					// check if multiline comment
					else if ((c == '#') & (infile.peek() == '|')) {
						int nCounter = 0;
						std::string hol(1, infile.get());
						longData += hol;
						// If you have #(|)
						while (1) {

							if (infile.peek() == EOF) {
								CheapToken tokenToken(longData, "UNDEFINED", lineCounter - nCounter);
								ScannerMachine::addTokenBackdoor(tokenToken);
								break;
							}
							if (infile.peek() != '|') {

								std::string hol2(1, infile.get());
								longData += hol2;
								if (hol2 == "\n") {
									nCounter++;
									lineCounter++;
								}

								continue;
							}
							if (infile.peek() == '|') {
								longData += "|";
								infile.get();
								if (infile.peek() != '#') {
									continue;
								}
								else {
									longData += "#";
									infile.get();
									CheapToken tokenToken(longData, "COMMENT", lineCounter - nCounter);
									ScannerMachine::addTokenBackdoor(tokenToken);
									break;
								}
							}
						}

					}
					else if (c == '\'') {
						int nCounter = 0;
						while (1) {
							if (infile.peek() == '\n') {
								longData += "\n";
								nCounter++;
								lineCounter++;
								infile.get();
								continue;
							}
							else if (infile.peek() == EOF) {
								CheapToken tokenToken(longData, "UNDEFINED", lineCounter - nCounter);
								ScannerMachine::addTokenBackdoor(tokenToken);
								break;
							}
							else if (infile.peek() == '\'') {
								longData += "'";
								infile.get();
								if (infile.peek() == '\'') {
									longData += '\'';
									infile.get();
									continue;
								}
								else {
									//std::string hol5(1, infile.get());
									//longData += hol5; 
									CheapToken tokenToken(longData, "STRING", lineCounter - nCounter);
									ScannerMachine::addTokenBackdoor(tokenToken);
									break;
								}
							}
							else {
								std::string hol5(1, infile.get());
								longData += hol5;
							}
						}
					}

					else { break; }
					break;
				}
			}
			if (c == '\n') {
				lineCounter++;
			}

			if (/*infile.peek() == '\0' | */infile.peek() == EOF) {

				CheapToken tokenToken("", "EOF", lineCounter);
				ScannerMachine::addTokenBackdoor(tokenToken);
				break;
			}
		}

	}
	bool tokenize_singleOpenAssigner(char c, int lineCounter) {

		// THE SINGLE CHARACTERS
		if (c == ',') {
			CheapToken tokenToken(",", "COMMA", lineCounter);
			ScannerMachine::addTokenBackdoor(tokenToken);
			return true;
		}
		else if (c == '.') {
			CheapToken tokenToken(".", "PERIOD", lineCounter);
			ScannerMachine::addTokenBackdoor(tokenToken);
			return true;
		}
		else if (c == '?') {
			CheapToken tokenToken("?", "Q_MARK", lineCounter);
			ScannerMachine::addTokenBackdoor(tokenToken);
			return true;
		}
		else if (c == '(') {
			CheapToken tokenToken("(", "LEFT_PAREN", lineCounter);
			ScannerMachine::addTokenBackdoor(tokenToken);
			return true;
		}
		else if (c == ')') {
			CheapToken tokenToken(")", "RIGHT_PAREN", lineCounter);
			ScannerMachine::addTokenBackdoor(tokenToken);
			return true;
		}
		else if (c == '*') {
			CheapToken tokenToken("*", "MULTIPLY", lineCounter);
			ScannerMachine::addTokenBackdoor(tokenToken);
			return true;
		}
		else if (c == '+') {
			CheapToken tokenToken("+", "ADD", lineCounter);
			ScannerMachine::addTokenBackdoor(tokenToken);
			return true;
		}
		// THE SINGLE UNDEFINED CHARACTERS
		// note to future, use else ifs above so you don't write everything
		else if ((isalpha(c)) | (c == ':') | (c == '\'') | (c == '#')) {
			std::string s(1, c);
			CheapToken tokenToken(s, "A STARTER TOKEN", lineCounter);
			//ScannerMachine::addTokenBackdoor(tokenToken);
			return false;
		}
		else if ((c == '\n') | (c == '\t') | (c == ' ')) {
			return true;
		}
		else if ((isdigit(c) == true) | (!isalnum(c))) {
			std::string s(1, c);
			CheapToken tokenToken(s, "UNDEFINED", lineCounter);
			ScannerMachine::addTokenBackdoor(tokenToken);
			return true;
		}
		else {

			return true;
		}
	}

	void tokenize_idAssigner(std::string longData, int lineCounter) {
		if (longData == "Schemes") {
			CheapToken tokenToken(longData, "SCHEMES", lineCounter);
			ScannerMachine::addTokenBackdoor(tokenToken);
		}
		else if (longData == "Facts") {
			CheapToken tokenToken(longData, "FACTS", lineCounter);
			ScannerMachine::addTokenBackdoor(tokenToken);
		}
		else if (longData == "Rules") {
			CheapToken tokenToken(longData, "RULES", lineCounter);
			ScannerMachine::addTokenBackdoor(tokenToken);
		}
		else if (longData == "Queries") {
			CheapToken tokenToken(longData, "QUERIES", lineCounter);
			ScannerMachine::addTokenBackdoor(tokenToken);
		}
		else {
			CheapToken tokenToken(longData, "ID", lineCounter);
			ScannerMachine::addTokenBackdoor(tokenToken);
		}
	}
};

#endif
