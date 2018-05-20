#include "Unit.h"

Unit::Unit(GameController* game, int x, int y): GameObject(game, x, y) {

}

void Unit::Update() {
	if (!path.empty()) {
		if (updatesSinceMove >= moveSpeed) {
			PathCoordinates nextHop = path.top();
			if (world->grid[nextHop.x][nextHop.y]) {
				// if there's something in the way find a new path
				GeneratePath();
				nextHop = path.top();
			}
			world->grid[x][y] = 0;
			x = nextHop.x;
			y = nextHop.y;
			world->grid[x][y] = this;
			path.pop();

			D3DXMatrixTranslation(&location, (float)x + 0.5f, 0.5f, (float)y + 0.5f);
			D3DXMatrixTranspose(&location, &location);
			vbc->UpdateInstance({ scale, rotation, location }, *instanceIndex);

			updatesSinceMove = 0;
		}
		else {
			updatesSinceMove++;
		}
	}
}

void Unit::ReceiveClick(int x, int y) {
	xTarget = x;
	yTarget = y;
	GeneratePath();
}

struct AStarNode {
	int gCost, hCost, fCost, x, y;
	AStarNode* parent;
};

void Unit::GeneratePath() {
	// if the target location is filled find the nearest by open tile and go there instead
	if (world->grid[xTarget][yTarget]) {
		FindOpenNearTarget();
	}

	while (!path.empty()) {
		path.pop();
	}
	std::vector<AStarNode*> open;
	std::vector<AStarNode*> closed;
	AStarNode* grid[GRID_X][GRID_Y];
	ZeroMemory(grid, GRID_X * GRID_Y * sizeof(AStarNode*));
	AStarNode start;
	start.x = x;
	start.y = y;
	start.hCost = (int)(sqrt(pow(xTarget - start.x, 2) + pow(yTarget - start.y, 2)) * 10);
	start.gCost = 0;
	start.fCost = start.hCost + start.gCost;
	start.parent = 0;
	open.push_back(&start);
	grid[x][y] = &start;

	while (true) {
		// find minimum node
		AStarNode* current = open[0];
		for (AStarNode* minCheckNode : open) {
			if (minCheckNode->fCost < current->fCost) {
				current = minCheckNode;
			}
		}

		open.erase(std::find(open.begin(), open.end(), current));
		closed.push_back(current);

		// the target node has been found
		if (current->x == xTarget && current->y == yTarget) {
			pathfindingFinish:
			// iterate back through the nodes and add coordinates to the path
			while (current->parent) {
				path.push({ current->x, current->y });
				current = current->parent;
			}

			// delete nodes and return
			for (auto it = open.begin(); it != open.end(); it++) {
				delete *it;
			}
			for (auto it = closed.begin() + 1; it != closed.end(); it++) {
				// +1 since the first node is assigned on heap
				delete *it;
			}
			return;
		}

		for (int xNeighbour = current->x - 1; xNeighbour <= current->x + 1; xNeighbour++) {
			for (int yNeighbour = current->y - 1; yNeighbour <= current->y + 1; yNeighbour++) {
				if (xNeighbour < 0 || yNeighbour < 0 || xNeighbour >= GRID_X || yNeighbour >= GRID_Y) {
					continue;
				}
				AStarNode* neighbour = grid[xNeighbour][yNeighbour];

				// if the neighbour has already been marked as a node
				if (neighbour) {
					// if node is in closed list do nothing
					if (std::find(closed.begin(), closed.end(), neighbour) != closed.end()) {
						continue;
					}
					// otherwise it must be in the open list, so check if new f value is lower
					else {
						// if both the x and y values have a difference of 1 (resulting in 2)
						// the node is diagonal and cost is 14
						int newGCost = current->gCost + (abs(current->x - neighbour->x) +
							abs(current->y - neighbour->y) > 1 ?
							14 : 10);
						if (newGCost < neighbour->gCost) {
							neighbour->gCost = newGCost;
							neighbour->parent = current;
						}
					}
				}
				else {
					// if the game grid has something in it don't add a node there
					if (world->grid[xNeighbour][yNeighbour]) {
						// if we found the target but it's obstructed take the current node as the closest we can get
						if (xNeighbour == xTarget && yNeighbour == yTarget) {
							goto pathfindingFinish;
						}
						continue;
					}
					else {
						AStarNode* newNode = new AStarNode;
						newNode->x = xNeighbour;
						newNode->y = yNeighbour;
						newNode->gCost = current->gCost + (abs(current->x - newNode->x) +
							abs(current->y - newNode->y) > 1 ?
							14 : 10);
						newNode->hCost = (int)(sqrt(pow(xTarget - newNode->x, 2) + pow(yTarget - newNode->y, 2)) * 10);
						newNode->fCost = newNode->gCost + newNode->hCost;
						newNode->parent = current;
						open.push_back(newNode);
					}
				}
			}
		}
	}
}

struct OpenNode {
	int targetDist, sourceDist, x, y;
};

bool FindOpenComparator(OpenNode* a, OpenNode* b) {
	if (a->targetDist == b->targetDist) {
		return a->sourceDist < b->sourceDist;
	}
	return a->targetDist < b->targetDist;
}

void Unit::FindOpenNearTarget() {
	bool found = false;
	int searchArea = 1;
	while (!found) {
		std::vector<OpenNode*> openNodes;
		for (int x = xTarget - searchArea; x <= xTarget + searchArea; x++) {
			for (int y = yTarget - searchArea; y <= yTarget + searchArea; y++) {
				if (world->grid[x][y] == 0) {
					OpenNode* newNode = new OpenNode;
					newNode->x = x;
					newNode->y = y;
					newNode->targetDist = (int)(sqrt(pow(xTarget - newNode->x, 2) + pow(yTarget - newNode->y, 2)) * 10);
					newNode->sourceDist = (int)(sqrt(pow(this->x - newNode->x, 2) + pow(this->y - newNode->y, 2)) * 10);
					openNodes.push_back(newNode);
				}
			}
		}

		// if we have a result
		if (openNodes.size() > 0) {
			// if only one node is found in current range, it must be closest to target
			if (openNodes.size() > 1) {
				// if multiple nodes are found compare them to find the best one
				std::sort(openNodes.begin(), openNodes.end(), FindOpenComparator);
			}

			xTarget = openNodes[0]->x;
			yTarget = openNodes[0]->y;

			for (auto it = openNodes.begin(); it != openNodes.end(); it++) {
				delete *it;
			}

			found = true;
		}

		searchArea++;
	}
}
