#pragma once
#include <vector>
#include <string>
#include "DeploymentMove.h"


class AtollBoard {

private:
	std::vector<int> board;
	
	int p1UnusedArmies;
	int p2UnusedArmies;
	int playerToMove = 1;
	int firstToAttack = 0; 
	int skipCount = 0;


	bool isVulnerable(int space);


	void finalizeDeployment();


public:
	int size;
	DeploymentMove previousDeploymentMove;
	bool inDeploymentPhase = true;

	AtollBoard(int _size);


	int getPlayerToMove();


	std::vector<int> getBoard();


	bool tryToSkipTurn();


	std::vector<DeploymentMove> getAvailableDeploymentMoves();

	
	std::vector<int> getAvailableAttackMoves();


	void deploy(DeploymentMove move);


	void attack(int index);


	int getWinningPlayer();


	


	std::string toString();

	friend std::ostream& operator<<(std::ostream& out, const AtollBoard& atollBoard);
};

