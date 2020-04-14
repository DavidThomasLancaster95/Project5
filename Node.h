#ifndef NODE_H
#define NODE_H

class Node
{
private:
	int rValue;
	bool visited;
public:
	Node(int inRValue) {
		rValue = inRValue;
		visited = false; 
	}

	bool getVisited() {
		return visited;
	}

	int getRValue() {
		return rValue;
	}

	void setRValue(int inValue) {
		rValue = inValue;
	}

	void setVisited(bool inBool) {
		visited = inBool;
	}

};

#endif