#include "Components/PathFindingComponent.h"
#include "Components/TilemapComponent.h"
#include "Object.h"
#include "Services/Collision.h"

Core::PathFindingComponent::PathFindingComponent(Object* Owner) :
	Component(Owner)
{
}

void Core::PathFindingComponent::Start()
{
	Component::Start();

	Object* TmObj = World().GetTilemapObject();
	if (TmObj)
	{
		TilemapComponent* TmComp = TmObj->GetComponent<TilemapComponent>();
		if (TmComp)
		{
			std::vector<Layer> Layers = TmComp->GetLayers();
			BuildAdjList(Layers);
		}
	}
}

void Core::PathFindingComponent::Destroy()
{
	Component::Destroy();

	for (Node* n : mAdjList)
	{
		delete n;
	}
}

void Core::PathFindingComponent::BuildAdjList(const std::vector<Layer>& Layers)
{
	mAdjList = FlattenTiles(Layers);

	for (int i = 0; i < mAdjList.size(); i++)
	{
		for (int j = 0; j < mAdjList.size(); j++)
		{
			if (i != j && AreNodesAdjascent(mAdjList[i], mAdjList[j]))
			{
				mAdjList[i]->Neighbours.push_back(mAdjList[j]);
			}
		}
	}
	int bob;
}

bool Core::PathFindingComponent::AreNodesAdjascent(const Node* Node1, const Node* Node2) const
{
	if (!Node1 || !Node2) return false;

	Tile tile1 = Node1->BaseTile;
	Tile tile2 = Node2->BaseTile;
	float Threshold = 1.f;

	if (abs(tile1.X + tile1.W - tile2.X) < Threshold) return true;
	if (abs(tile1.X - tile2.X + tile2.W) < Threshold) return true;
	if (abs(tile1.Y + tile1.H - tile2.Y) < Threshold) return true;
	if (abs(tile1.Y - tile2.Y + tile2.H) < Threshold) return true;



	return false;
}

std::vector<Core::Node*> Core::PathFindingComponent::FlattenTiles(const std::vector<Layer>& Layers)
{
	std::vector<Node*> OutList;

	for (int i = 0; i < Layers.size(); i++)
	{
		for (const Tile& tile : Layers[i]._Tiles)
		{
			Node* n = new Node();
			n->X = tile.X + tile.W / 2;
			n->Y = tile.X + tile.H / 2;
			n->BaseTile = tile;
			n->bObstacle = tile.ParentLayerName == "Wall" ? true : false;
			OutList.push_back(n);
		}
	}
	return OutList;
}

std::vector<Core::Vector<float>> Core::PathFindingComponent::GetPath(const Vector<float>& TargetLoc)
{
	if (!mOwner) return std::vector<Core::Vector<float>>();

	Node* StartNode = GetNodeByLoc(mOwner->GetLocation());
	Node* EndNode = GetNodeByLoc(TargetLoc);
	if (!StartNode || !EndNode) return std::vector<Core::Vector<float>>();

	Node* CurrNode = StartNode;
	while (CurrNode != EndNode)
	{
		if (!CurrNode->bVisited)
		{
			CurrNode->bVisited = true;
			mVisitedList.push_back(CurrNode);
		}
		ProcessNeighbours(CurrNode, TargetLoc);
		SortNodeListByCost(mVisitedList, 0, mVisitedList.size() - 1);
		CurrNode = mVisitedList[0];
	}

	// Build path from end
	std::vector<Vector<float>> OutPath;
	Node* PathNode = EndNode;
	while (PathNode != StartNode)
	{
		OutPath.push_back(Vector<float>(PathNode->X, PathNode->Y));
		PathNode = PathNode->Prev;
		mPath.push_back(PathNode);
	}
}

Core::Node* Core::PathFindingComponent::GetNodeByLoc(const Vector<float>& Loc) const
{
	for (Node* n : mAdjList)
	{
		Tile t = n->BaseTile;
		if (Collision::PointWithRect(Loc, Rect<float>(t.X, t.Y, t.W, t.H)))
		{
			return n;
		}
	}
	return nullptr;
}

void Core::PathFindingComponent::ProcessNeighbours(Node* n, const Vector<float>& targetLoc)
{
	for (int i = 0; i < n->Neighbours.size(); i++)
	{
		Node* CurrNeighbour = n->Neighbours[i];
		const Vector<float> CurrNodeLoc(n->X, n->Y);
		size_t Heuristic = Vector<float>::Dist(CurrNodeLoc, targetLoc);
		size_t EstimatedTotalCost = 1 + Heuristic;

		if (!CurrNeighbour->bObstacle && (CurrNeighbour->TotalCost == SIZE_MAX || CurrNeighbour->TotalCost > EstimatedTotalCost))
		{
			CurrNeighbour->TotalCost = EstimatedTotalCost;
			CurrNeighbour->Prev = n;
		}
		if (!CurrNeighbour->bVisited)
		{
			CurrNeighbour->bVisited = true;
			mVisitedList.push_back(CurrNeighbour);
		}
	}
}

void Core::PathFindingComponent::SortNodeListByCost(std::vector<Node*> list, int left, int right)
{
	if (left >= right)
	{
		return;
	}
	int l = left;
	int r = right;
	size_t pivot = list[(left + right) / 2]->TotalCost;

	while (left <= right)
	{
		while (list[left]->TotalCost < pivot)
		{
			left++;
		}
		while (list[right]->TotalCost > pivot)
		{
			right--;
		}
		if (left <= right)
		{
			Node* temp = list[left];
			list[left] = list[right];
			list[right] = temp;
			left++;
			right--;
		}
		int index = left;
		SortNodeListByCost(list, l, index - 1);
		SortNodeListByCost(list, index, r);
	}
}
