#pragma once

#include <cstdint>
#include <vector>
#include "Component.h"

namespace Core
{
	struct Node
	{
		float X = 0.f;
		float Y = 0.f;
		size_t Cost = SIZE_MAX;
		size_t Heuristic = SIZE_MAX;
		size_t FinalCost = SIZE_MAX;
		bool bVisited = false;
		bool bObstacle = false;
		Node* Prev = nullptr;
		std::vector<Node*> Neighbors;
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
		void BuildMap(TTilemap Tilemap);
	};
}