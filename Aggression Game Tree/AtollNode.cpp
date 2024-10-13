#include "AtollNode.h"
#include <iostream>



Node::Node(AtollBoard* _board, Node* _parent) : board(_board), parent(_parent) {}

int Node::getPlayerWhoWins() {
	if (cachedPlayerWhoWins) return cachedPlayerWhoWins;


	// If this node contains a leaf board (The game is over and a player has obviously won)
	if (children.empty()) {
		cachedPlayerWhoWins = board->getWinningPlayer();
		return cachedPlayerWhoWins;
	}

	bool playerCanForceAWin = false;
	int childIndex = 0;
	for (Node* child : children) {

		if (board->getPlayerToMove() == child->getPlayerWhoWins()) playerCanForceAWin = true;

		childIndex++;
	}
	cachedPlayerWhoWins = playerCanForceAWin ? board->getPlayerToMove() : board->getPlayerToMove() ^ 3;
	return cachedPlayerWhoWins;
}


void Node::addChild(Node* _child) {
	children.push_back(_child);
	// Extend the children winner cache to match the length of the children list
	cachedChildrenWinners.push_back(0);
}


void Node::passInfoUp(std::vector<Node*>& nodesToDelete) {
	
	// Determine if there is conclusive information to pass up
	if (children.empty()) {
		cachedPlayerWhoWins = board->getWinningPlayer();
	}
	else {
		bool playerCanForceAWin = false;
		int childIndex = 0;
		for (Node* child : children) {
			if (cachedChildrenWinners[childIndex] == 0) { return; } // We need more children nodes to be processed before we can pass info up about this node

			if (board->getPlayerToMove() == cachedChildrenWinners[childIndex]) playerCanForceAWin = true;

			childIndex++;
		}
		cachedPlayerWhoWins = playerCanForceAWin ? board->getPlayerToMove() : board->getPlayerToMove() ^ 3;
	}

	// If the root node is calling this then return
	if (parent == NULL) {
		std::cout << "Root node successfully called pass info up" << std::endl;
		return;
	}



	std::vector<Node*>& siblings = parent->children;
	// Find this node's entry in the siblings and remove the reference to it. Store the cached player who wins with the parent
	for (int i = 0; i < siblings.size(); i++) {
		if (this == siblings[i]) {
			parent->cachedChildrenWinners[i] = cachedPlayerWhoWins;
			siblings[i] = NULL;
			nodesToDelete.push_back(this);
			parent->passInfoUp(nodesToDelete);
			return;
		}
	}
	std::cout << "Couldn't find a reference to a child in the parent inside AtollNode.cpp. This message should never be shown." << std::endl;
}

std::string Node::toString(bool showWinner) {
	std::string s = "P" + std::to_string(board->getPlayerToMove()) + " to move, in " + (board->inDeploymentPhase ? "Deployment" : "Attack    ");
	if (showWinner) {
		s += ". P" + std::to_string(getPlayerWhoWins()) + " Wins";
	}
	return s;
}

