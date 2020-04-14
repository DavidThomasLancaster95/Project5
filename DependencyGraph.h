#ifndef DEPENDENCYGRAPH_H
#define DEPENDENCYGRAPH_H

#include <vector>
#include "Node.h"

class DependencyGraph
{
private:
	std::vector<std::vector<Node>> mainVector;
public:

	void pushBackVectorOfNode(std::vector<Node> inVectorOfNode) {
		mainVector.push_back(inVectorOfNode);
	}

	void setMainVector(std::vector<std::vector<Node>> inMainVector) {
		mainVector = inMainVector;
	}

	std::vector<std::vector<Node>> accessMainVector() {
		return mainVector;
	}
	
	
};

#endif
