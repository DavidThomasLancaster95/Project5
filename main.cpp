#include <iostream>
#include <string>
#include <fstream>
#include <vector> // used in both main and tokenizer
#include <ctype.h> // used in the tokenizer
#include "CheapToken.h"
#include "ScannerMachine.h"
#include "DatalogParser.h"
#include "DataBase.h"




int main(int argc, char* argv[1]) {

	
	ScannerMachine scannerMachine;
	scannerMachine.tokenize(argv[1]);  /*argv[1] or data.txt*/


	DatalogParser myDatalogParser;
	myDatalogParser.setTokenVector(scannerMachine.getTokensVector());
	myDatalogParser.stripCommentsTokenVector();

	if (myDatalogParser.propperGrammar()) {
		DatalogProgram datalogProgram;
		datalogProgram = myDatalogParser.getDatalogProgramDataStructure();

		//datalogProgram.printRawQueriesPredicateVector();

		//std::cout << "\n\n\n";

		//datalogProgram.printAll();

		DataBase database(datalogProgram);

	}








	return 0;
}