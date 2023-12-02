#pragma once

#include <cstdint>
#include <vector>
#include "Component.h"

namespace Core
{
	/** Structure that represents a cell in the world */
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
		virtual Component* Clone(Object* Owner);
		virtual void SetupClone(Component* Child);

		/** Get the path to follow to go from the owner's location to the target's location
		*	@param TargetLoc: Location of the target
		*/
		std::vector<Vector<float>> GetPath(const Vector<float>& TargetLoc);

		/** Move the owner toward the target */
		void Move();

		/** Stop moving the owner toward the target */
		void StopMove();
		
	private:
		/** Whether or not 2 nodes are adjascent in the world
		*	@param Node1: First node to compare
		*	@param Node2: Second node to compare
		*/
		bool AreNodesAdjascent(const Node* Node1, const Node* Node2) const;

		/** Put the tiles from multiple layers into one single list of one dimension */
		std::vector<Node*> FlattenTiles(const std::vector<Layer>& Layers);

		/** Build the adjascent list from a tilemap
		*	@param Tilemap: Tilemap to build an adj list from
		*/
		void BuildAdjList(const std::vector<Layer>& Tilemap);

		/** Get all the nodes that has a specific location
		*	@param Loc: Location to check
		*/
		Node* GetNodeByLoc(const Vector<float>& Loc) const;

		/** Attribute a cost and a previous reference to all the neighbours of a specific node
		*	@param n: Node to process the neighbours
		*	@param targetLoc: Location of the target
		*/
		void ProcessNeighbours(Node* n, const Vector<float>& targetLoc);

		/** Quicksort of the nodes by cost
		*	@param list: List of node to be sorted
		*	@param left: Index of the list's start
		*	@param right: Index of the list's end - 1
		*/
		void SortNodeListByCost(std::vector<Node*>& list, int left, int right);

		/** Reset the nodes informations. Useful before calculating a new path */
		void ResetNodesInAdjList();

		std::vector<Node*> mAdjList;
		std::vector<Node*> mVisitedList;
		std::vector<Vector<float>> mPath;
		Vector<float> mCurrPathPoint;
		float mTolerance = 35.f;
		bool ShouldMove = true;
	};
}