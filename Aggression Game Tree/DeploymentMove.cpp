#include "DeploymentMove.h"




DeploymentMove::DeploymentMove(int _index = 0, int _armyCount = 0) {
		index = _index;
		armyCount = _armyCount;
}

std::string DeploymentMove::toString() const {
	return "(Index: " + std::to_string(index) + ", ArmyCount: " + std::to_string(armyCount) + ")";
}

std::ostream& operator<<(std::ostream& out, const DeploymentMove deploymentMove) {
	std::cout << deploymentMove.toString();
	return out;
}

std::ostream& operator<<(std::ostream& out, const std::vector<DeploymentMove> deploymentMoves) {
	out << "[";
	for (size_t i = 0; i < deploymentMoves.size(); i++) {
		std::cout << deploymentMoves[i].toString();
		if (i != deploymentMoves.size() - 1) std::cout << ", ";
	}
	out << "]";
	return out;
}
