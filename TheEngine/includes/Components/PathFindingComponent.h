#pragma once

#include <cstdint>
#include <vector>
#include "Component.h"

namespace Core
{
	struct Node
	{
		Tile BaseTile;
		float X = 0.f;
		float Y = 0.f;
		size_t TotalCost = SIZE_MAX;
		bool bVisited = false;
		bool bObstacle = false;
		Node* Prev = nullptr;
		std::vector<Node*> Neighbours;
	};

	struct Graph
	{
		std::vector<Node*> Map;
	};

	class PathFindingComponent : public Component
	{
	public:
		PathFindingComponent(Object* Owner);
		virtual void Start() override;
		virtual void Destroy() override;
		std::vector<Vector<float>> GetPath(const Vector<float>& TargetLoc);
		
	private:
		bool AreNodesAdjascent(const Node* Node1, const Node* Node2) const;
		std::vector<Node*> FlattenTiles(const std::vector<Layer>& Layers);
		void BuildAdjList(const std::vector<Layer>& Tilemap);
		Node* GetNodeByLoc(const Vector<float>& Loc) const;
		void ProcessNeighbours(Node* n, const Vector<float>& targetLoc);
		void SortNodeListByCost(std::vector<Node*>& list, int left, int right);
		void ResetNodesInAdjList();

		std::vector<Node*> mAdjList;
		std::vector<Node*> mVisitedList;
		std::vector<Node*> mPath;
	};
}