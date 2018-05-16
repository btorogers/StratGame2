#include "Unit.h"

bool Unit::modelLoaded = false;
int Unit::indexOfModel = 0;

Unit::Unit(GameController* game, int x, int y): GameObject(game->GetVertexBufferController(), x, y), game(game) {
	D3DXMatrixIdentity(&rotation);
	if (!modelLoaded) {
		Sphere s(0.0f, 0.5f, 0.0f, 0.5f, D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f), vbc);
		indexOfModel = s.AddSelfForRendering(true);
		modelLoaded = true;
	}
	D3DXMatrixIdentity(&scale);
	D3DXMatrixIdentity(&rotation);
	D3DXMatrixTranslation(&location, x + 0.5f, 0.5f, y + 0.5f);

	D3DXMatrixTranspose(&scale, &scale);
	D3DXMatrixTranspose(&rotation, &rotation);
	D3DXMatrixTranspose(&location, &location);

	instanceIndex = vbc->AddInstance({ scale, rotation, location });
}

Unit::~Unit() {
	vbc->DeleteInstance(instanceIndex);
}

void Unit::Render() {
	if (instanceIndex)
		vbc->RenderInstanced(indexOfModel, 960, *instanceIndex, 1);
}

void Unit::Update() {
	if (!path.empty()) {
		if (updatesSinceMove >= moveSpeed) {
			PathCoordinates nextHop = path.top();
			if (game->grid[nextHop.x][nextHop.y]) {
				// if there's something in the way find a new path
				GeneratePath();
				nextHop = path.top();
			}
			game->grid[x][y] = 0;
			x = nextHop.x;
			y = nextHop.y;
			game->grid[x][y] = (GameObject*)this;
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

void Unit::GeneratePath() {
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
				// +1 since the start is assigned on heap
				delete *it;
			}
			return;
		}

		for (int xNeighbour = current->x - 1; xNeighbour <= current->x + 1; xNeighbour++) {
			for (int yNeighbour = current->y - 1; yNeighbour <= current->y + 1; yNeighbour++) {
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
					if (game->grid[xNeighbour][yNeighbour]) {
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
