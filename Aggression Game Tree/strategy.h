#pragma once
#include <vector>
#include <iostream>
#include <cstdlib>
#include "DeploymentMove.h"
#include "AtollBoard.h"

using namespace std;

class Strategy {

public:	

	static vector<DeploymentMove> allMoves(vector<DeploymentMove> moves, AtollBoard* board) {
		return moves;
	}
	// TODO: Figure out what's causing player 1 to not have any moves at all after their first move.
	static vector<DeploymentMove> oneMoreStrategy(const vector<DeploymentMove>& moves, AtollBoard* board) {
		// This strategy is a one-hand-tied strategy for player 2's moves only
		if (board->getPlayerToMove() == 1) return moves;

		
		vector<DeploymentMove> strategicallyApplicableMoves;
		DeploymentMove previousMove = board->previousDeploymentMove;
		int player2Midpoint = board->size / 2;

		// If player 2 gets to make more deployment moves than player 1 (a.k.a go twice in a row), accept moves that play 1s
		if (previousMove.armyCount < 0) {
			for (DeploymentMove move : moves) {
				if (abs(move.armyCount) == 1) {
					strategicallyApplicableMoves.push_back(move);
				}
			}
			return strategicallyApplicableMoves;
		}

		// Accept any posisble moves as long as they move next to P1's previous move and have an armyCount one higher than P1's prevous move
		for (DeploymentMove move : moves) {
			int clockwiseIndex = move.index + 1 == board->size ? 0 : move.index + 1;
			int counterClockwiseIndex = move.index - 1 < 0 ? board->size - 1 : move.index - 1;
			// Check that the placement of the move aligns with the strategy
			if (move.index < player2Midpoint) {
				// If the previous move is on the second half of the board, evenly indexed moves must be responded to by the counterclockwise odd and visa versa
				if (previousMove.index % 2 == 0) {
					if (clockwiseIndex != previousMove.index) continue;
				}
				else {
					if (counterClockwiseIndex != previousMove.index) continue;
				}
			} else {
				// If the previous move is on the second half of the board, evenly indexed moves must be responded to by the clockwise odd and visa versa
				if (previousMove.index % 2 == 0) {
					if (counterClockwiseIndex != previousMove.index) continue;
				}
				else {
					if (clockwiseIndex != previousMove.index) continue;
				}
			}
			// Check that the armyCount of the move aligns with the strategy
			if (!(abs(move.armyCount) == previousMove.armyCount + 1)) continue;
			strategicallyApplicableMoves.push_back(move);
		}
		return strategicallyApplicableMoves;
	}

};


