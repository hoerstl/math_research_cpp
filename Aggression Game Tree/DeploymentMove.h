#pragma once
#include <string>
#include <vector>
#include <iostream>

struct DeploymentMove {

	int index;
	int armyCount;

	DeploymentMove(int _index, int _armyCount);
	std::string toString() const;
};

std::ostream& operator<<(std::ostream& out, const DeploymentMove deploymentMove);
std::ostream& operator<<(std::ostream& out, const std::vector<DeploymentMove> deploymentMoves);

