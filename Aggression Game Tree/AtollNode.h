#pragma once
#include <vector>
#include <string>
#include "AtollBoard.h"


class Node {
private:
	int cachedPlayerWhoWins = 0;
	std::vector<int> cachedChildrenWinners;
public:
	AtollBoard* board;
	std::vector<Node*> children;
	Node* parent;

	Node(AtollBoard* _board, Node* _parent = NULL);
	int getPlayerWhoWins();
	void addChild(Node* _child);
	void passInfoUp(std::vector<Node*>& nodesToDelete);
	std::string toString(bool showWinner = false);

};


