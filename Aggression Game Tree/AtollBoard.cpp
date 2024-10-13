#include <iostream>
#include "AtollBoard.h"

using namespace std;


bool AtollBoard::isVulnerable(int space) {
	int armySelector = playerToMove == 1 ? 1 : -1; // constant that filters out player 2's armies if player 1 is to move
	int clockwiseIndex = space + 1 == board.size() ? 0 : space + 1;
	int counterClockwiseIndex = space - 1 < 0 ? board.size() - 1 : space - 1;
	int attackValueOnSpace = 0;
	attackValueOnSpace += board[clockwiseIndex] * armySelector > 0 ? board[clockwiseIndex] * armySelector : 0;
	if (size != 2) attackValueOnSpace += board[counterClockwiseIndex] * armySelector > 0 ? board[counterClockwiseIndex] * armySelector : 0;
	return attackValueOnSpace > abs(board[space]);
}


void AtollBoard::finalizeDeployment() {
	// Mark the firstToAttack if not already done
	if (!firstToAttack && p1UnusedArmies == 0) { firstToAttack = 1; }
	if (!firstToAttack && p2UnusedArmies == 0) { firstToAttack = 2; }

	// Check to see if the deployment phase is over
	bool boardIsFull = true;
	for (int armyCount : board) {
		if (!armyCount) {
			boardIsFull = false;
		}
	}
	bool playersOutOfArmies = p1UnusedArmies == 0 && p2UnusedArmies == 0;
	if (boardIsFull || playersOutOfArmies) {
		inDeploymentPhase = false;
		firstToAttack = firstToAttack ? firstToAttack : 1;
		playerToMove = firstToAttack;
	}
	skipCount = 0;
}




AtollBoard::AtollBoard(int _size) : size(_size), previousDeploymentMove(0, 0) {
	board = vector<int>(size);
	p1UnusedArmies = size;
	p2UnusedArmies = size;
}





int AtollBoard::getPlayerToMove() {
	return playerToMove;
}



vector<int> AtollBoard::getBoard() {
	return board;
}



bool AtollBoard::tryToSkipTurn() {
	if (skipCount == 2) return false;
	playerToMove ^= 3;
	skipCount += 1;
	return true;
}


vector<DeploymentMove> AtollBoard::getAvailableDeploymentMoves() {
	vector<DeploymentMove> deploymentMoves;
	int armySelector = playerToMove == 1 ? 1 : -1;
	int unusedArmies = playerToMove == 1 ? p1UnusedArmies : p2UnusedArmies;
	for (int i = 0; i < board.size(); i++) {
		if (board[i] == 0) {
			for (int armyCount = 1; armyCount <= unusedArmies; armyCount++) {
				deploymentMoves.push_back(DeploymentMove(i, armyCount * armySelector));
			}
		}
	}
	return deploymentMoves;
}


vector<int> AtollBoard::getAvailableAttackMoves() {
	int armySelector = playerToMove == 1 ? -1 : 1; // constant that filters out player 1's armies if player 1 is to move
	vector<int> vulnerableSpaces = vector<int>();
	// Do the filtering of the list
	for (int i = 0; i < board.size(); i++) {
		if (board[i] * armySelector > 0 && isVulnerable(i)) {
			vulnerableSpaces.push_back(i);
		}
	}
	return vulnerableSpaces;
}


void AtollBoard::deploy(DeploymentMove move) {
	if (playerToMove == 1) {
		board[move.index] = move.armyCount;
		p1UnusedArmies -= move.armyCount;
	}
	else {
		board[move.index] = move.armyCount;
		p2UnusedArmies += move.armyCount;
	}
	playerToMove ^= 3;
	previousDeploymentMove = move;
	finalizeDeployment();
}


void AtollBoard::attack(int index) {
	board[index] = 0;
	playerToMove ^= 3;
	skipCount = 0;
}


int AtollBoard::getWinningPlayer() {
	// Count the regions and armies each player controls
	int p1ControlledRegions = 0, p2ControlledRegions = 0;
	int p1ControlledArmies = 0, p2ControlledArmies = 0;
	for (int armyCount : board) {
		if (armyCount > 0) {
			p1ControlledRegions += 1;
			p1ControlledArmies += armyCount;
		}
		else if (armyCount < 0) {
			p2ControlledRegions += 1;
			p2ControlledArmies -= armyCount;
		}
	}

	// Determine the winner
	if (p1ControlledRegions != p2ControlledRegions) {
		if (p1ControlledRegions > p2ControlledRegions) return 1;
		else return 2;
	}
	else if (p1ControlledArmies != p2ControlledArmies) {
		if (p1ControlledArmies > p2ControlledArmies) return 1;
		else return 2;
	}
	else return firstToAttack;
}


string AtollBoard::toString() {
	string s = "[";
	for (int i = 0; i < board.size(); i++) {
		s = s + to_string(board[i]);
		if (i != board.size() - 1) {
			s = s + ", ";
		}
	}
	s = s + "]";
	return s;
}

ostream& operator<<(ostream& out, const AtollBoard& atollBoard) {
	out << "[";
	for (int i = 0; i < atollBoard.board.size(); i++) {
		out << atollBoard.board[i];
		if (i != atollBoard.board.size() - 1) out << ", ";
	}
	out << "]";
	return out;
}
