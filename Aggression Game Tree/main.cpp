#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include "AtollBoard.h"
#include "AtollNode.h"
#include "strategy.h"
#include <cassert>

using namespace std;

void print(vector<DeploymentMove> v) {
	for (DeploymentMove move : v) {
		std::cout << "index: " << move.index << " armyCount: " << move.armyCount << endl;
	}
}

string to_string(vector<int> v) {
	string s = "[";
	for (int i = 0; i < v.size(); i++) {
		s = s + to_string(v[i]);
		if (i != v.size() - 1) {
			s = s + ", ";
		}
	}
	s = s + "]";
	return s;
}

void print(vector<vector<int>> v) {
	for (int i = 0; i < v.size(); i++) {
		cout << "Pos " << i << ": " << to_string(v[i]) << endl;
	}
}

void displayWinningMoves(Node* targetNode, Node* root = NULL) {
	int expectedWinner = root ? root->getPlayerWhoWins() : 1;
	std::cout << "Here are winning outcomes for P" << expectedWinner << " when P" << targetNode->board->getPlayerToMove() << " moves on " << targetNode->board->toString() << endl;

	vector<int> baseBoard = targetNode->board->getBoard();
	int boardSize = baseBoard.size();
	vector<vector<int>> winningMoves(boardSize);
	for (int i = 0; i < targetNode->children.size(); i++) {
		if (targetNode->children[i]->getPlayerWhoWins() == expectedWinner) {
			int inconsistentIndex;
			for (int j = 0; j < boardSize; j++) {
				if (baseBoard[j] != targetNode->children[i]->board->getBoard()[j]) {
					winningMoves[j].push_back(targetNode->children[i]->board->getBoard()[j]);
					break;
				}
			}
		}
	}
	print(winningMoves);
}

void displayChildrenInformation(Node* targetNode, Node* root = NULL) {
	int expectedWinner = root ? root->getPlayerWhoWins() : 1;
	if (targetNode->board->getPlayerToMove() != expectedWinner) {std::cout << "Warning: This is not really relevent since P" << targetNode->board->getPlayerToMove() << " is not the player who wins so any move they make will be beaten." << endl;}
	std::cout << "Here are options for P" << targetNode->board->getPlayerToMove() << " to move on " << targetNode->board->toString() << ":" << endl;
	

	for (int i = 0; i < targetNode->children.size(); i++){
		Node* child = targetNode->children[i];
		std::cout << i << ".\t" << child->board->toString() << (child->getPlayerWhoWins() == expectedWinner ? "  " : "  \t");
		std::cout << child->toString(true) << endl;
	}
}

bool startsWith(string s, string prefix) {
	if (prefix.size() > s.size()) return false;
	for (int i = 0; i < prefix.size(); i++) {
		if (prefix[i] != s[i]) return false;
	}
	return true;
}

bool isNumeric(string s) {
	for (char letter : s) {
		if (!isdigit(letter)) return false;
	}
	return true;
}


void makeDeploymentMoves(vector<DeploymentMove>& moves, Node* parentNode, stack<Node*>& incompleteNodes) {
	AtollBoard* parentBoard = parentNode->board;
	for (DeploymentMove move : moves) {
		AtollBoard* newBoard = new AtollBoard(*parentBoard);
		newBoard->deploy(move);
		Node* newNode = new Node(newBoard, parentNode);
		parentNode->addChild(newNode);
		incompleteNodes.push(newNode);
	}
}



int main() {
	Strategy strategy;

	int atollSize = 8;
	bool persistGameTree = true;

	AtollBoard rootBoard(atollSize);
	Node* root = new Node(&rootBoard);
	stack<Node*> incompleteNodes;

	string selectedStrategy = "default"; // Options are: "default", "1 more"

	// For regular game simulations
	if (selectedStrategy == "default") {
		// Make the first moves for P1 manually
		vector<DeploymentMove> reasonableFirstMoves;
		int maximumReasonableFirstMove = (2 * atollSize) / 3 + 1;  // This formula proved by Kristen Barnard, Beth Kelly, Alexandra Yuhas, Aric Parker, Gyanu Karki, Samriddha KC
		for (int i = 1; i <= maximumReasonableFirstMove; i++) {
			DeploymentMove move(0, i);
			reasonableFirstMoves.push_back(move);
		}
		makeDeploymentMoves(reasonableFirstMoves, root, incompleteNodes);
	}

	// For the 1 more strategy
	if (selectedStrategy == "1 more") {
		// P1's initial mess up
		vector<DeploymentMove> losingFirstMoves;
		int minimumLosingMove = ceil(((float)atollSize) / 2.0);
		for (int i = minimumLosingMove; i <= atollSize; i++) {
			DeploymentMove move(0, i);
			losingFirstMoves.push_back(move);
		}
		makeDeploymentMoves(losingFirstMoves, root, incompleteNodes);
		// P2's mirror
		vector<DeploymentMove> mirrorMoves;
		DeploymentMove move(atollSize / 2, -1);
		mirrorMoves.push_back(move);
		for (int i = 0; i < losingFirstMoves.size(); i++) {
			Node* parent = incompleteNodes.top();
			incompleteNodes.pop();
			makeDeploymentMoves(mirrorMoves, parent, incompleteNodes);
		}
	}

	// ---------------------GENERATE GAME TREE--------------------------------------------------
	while (!incompleteNodes.empty()) {
		Node* parent = incompleteNodes.top();
		incompleteNodes.pop();
		AtollBoard* parentBoard = parent->board;
		if (parentBoard->inDeploymentPhase) {
			vector<DeploymentMove> deploymentMoves = parentBoard->getAvailableDeploymentMoves();
			// if there are no moves for this player to make, try to skip their turn
			if (deploymentMoves.empty()) {
				if (parentBoard->tryToSkipTurn()) {
					incompleteNodes.push(parent);
				}
				continue;
			}
			// Assert that the strategy never returns zero avaialble moves if the player can make any
			vector<DeploymentMove> compliantDeploymentMoves;
			if (selectedStrategy == "default") compliantDeploymentMoves = strategy.allMoves(deploymentMoves, parentBoard);
			if (selectedStrategy == "1 more")  compliantDeploymentMoves = strategy.oneMoreStrategy(deploymentMoves, parentBoard);
			if (compliantDeploymentMoves.empty()) cout << "Major problem. The currently selected deployment strategy returned no valid deployment moves when there were " << deploymentMoves.size() << " before the strategy on board: " << *parentBoard << " with previous move " << parentBoard->previousDeploymentMove << " and P" << parentBoard->getPlayerToMove() << " to move" << endl;
			// Since there are moves to be made, copy the board and make all of them
			makeDeploymentMoves(compliantDeploymentMoves, parent, incompleteNodes);
		}
		else {
			vector<int> attackMoves = parentBoard->getAvailableAttackMoves();
			// if there are no moves for this player to make try to skip their turn
			if (attackMoves.empty()) {
				if (parentBoard->tryToSkipTurn()) {
					incompleteNodes.push(parent);
				}
				else if (!persistGameTree) {
					// Pass the results up through the nodes and delete this node.
					std::vector<Node*> nodesToDelete;
					parent->passInfoUp(nodesToDelete); // Should call some things in the parent element to pass back the result.
					for (Node* node : nodesToDelete) {
						if (node != root) { delete node; }
					}
					nodesToDelete.clear();
				}
				continue;
			}
			// Since there are moves to be made, copy the board and make all of them
			for (int move : attackMoves) {
				AtollBoard* newBoard = new AtollBoard(*parentBoard);
				newBoard->attack(move);
				Node* newNode = new Node(newBoard, parent);
				parent->addChild(newNode);
				incompleteNodes.push(newNode);
			}
		}
	}







    // --------------------GAME TREE FINISHED GENERATING----------------------------------------------------
	std::cout << "Player " << root->getPlayerWhoWins() << " wins on an atoll of size " << atollSize << endl;
	std::cout << endl;


	Node* targetNode = root;
	string response = "";
	stack<Node*> parents;

	if (!persistGameTree) { return 0; }

	while (response != "quit") {
		for (int i = 0; i < 110; i++) { cout << "*";} cout << endl;
		std::cout << "Type the index of a child, 'display', 'winning', 'back' or 'quit': ";
		cin >> response;
		if (isNumeric(response)) {
			int targetIndex = stoi(response);
			if (targetIndex >= targetNode->children.size()) {
				std::cout << "Failed, this node only has " << targetNode->children.size() << " children." << endl;
				continue;
			}
			std::cout << "Swapping nodes..." << endl;
			parents.push(targetNode);
			targetNode = targetNode->children[targetIndex];
			displayChildrenInformation(targetNode, root);
			std::cout << "Here are all of the winning moves for " << root->getPlayerWhoWins() << endl;
			displayWinningMoves(targetNode, root);
		}

		else if (startsWith("display", response)) {
			std::cout << "Showing information for the current board..." << endl;
			displayChildrenInformation(targetNode, root);
		}

		else if (startsWith("winning", response)) {
			std::cout << "Here are all of the winning moves for " << root->getPlayerWhoWins() << endl;
			displayWinningMoves(targetNode, root);
		}

		else if (startsWith("back", response)) {
			if (parents.empty()) {
				std::cout << "You're already looking at the root node." << endl;
				continue;
			}
			targetNode = parents.top();
			parents.pop();
		}
		
	}
	



}
