// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph.h"

int directions4_x[4] = { 0, 1, 0, -1 };
int directions4_y[4] = { -1, 0, 1, 0 };
//int relDirection(int a, int b, int w) {
//	int delta = b - a;
//	for (int i = 0; i < 4; i++) {
//		if (delta == directions4_x[i] * w + directions4_y[i])
//			return i;
//	}
//}
int bitIndex[4] = { 1,4,16,64 };
int oppDir[4] = { 2,3,0,1 };
int relDirection(int a) {
	for (int i = 0; i < 4; i++) {
		if (a == bitIndex[i])
			return i;
	}
	return 0;
}

Graph::Graph()
{
}
void Graph::Init(TArray<int> arr, int w) {
	wangTiles = arr;
	width = w;
	//ExtractLargestPiece();
	//CreateRooms();
	//SetPaths();
}


int Graph::getBitIndex(int tileBit, int tileIndex) {
	switch (tileBit) {
	case 1: return tileIndex - width;
	case 4: return tileIndex + 1;
	case 16: return tileIndex + width;
	case 64: return tileIndex - 1;
	}
	return 0;
}
// A utility function to do BFS of graph
// from a given vertex u.
void Graph::BFSUtil(int u, TArray<bool>& visited, TArray<TArray<int>>& pieces)
{
	int pieceNumber = pieces.Num();
	TArray<int> newPiece;
	//newPiece.Add(u);
	pieces.Add(newPiece);

	TDoubleLinkedList<int> q;

	visited[u] = true;
	q.AddHead(u);
	//pieces[pieceNumber].Emplace(u);

	while (!q.IsEmpty())
	{
		// Dequeue a vertex from queue and print it
		TDoubleLinkedList<int>::TDoubleLinkedListNode* head = q.GetHead();
		u = head->GetValue();
		pieces[pieceNumber].Add(u);
		q.RemoveNode(head);

		// Get all adjacent vertices of the dequeued
		// vertex s. If an adjacent has not been visited,
		// then mark it visited and enqueue it
		for (int i = 1; i <= 64; i *= 4)
		{
			if (i == 1 && width > u)
				continue;
			if (i == 4 && u % width == width - 1)
				continue;
			if (i == 16 && u + width >= wangTiles.Num())
				continue;
			if (i == 64 && u % width == 0)
				continue;
			int neighborIndex = getBitIndex(i, u);
			if (wangTiles[u] & i && !visited[neighborIndex])
			{
				visited[neighborIndex] = true;
				q.AddTail(neighborIndex);
				//pieces[pieceNumber].Add(neighborIndex);
			}
		}
	}
}

void Graph::ExtractLargestPiece() {
	TArray<bool> visited;
	visited.Init(false, wangTiles.Num());
	TArray<TArray<int>> pieces;

	for (int i = 0; i < wangTiles.Num(); i++) {
		if (wangTiles[i] != 0 && !visited[i]) {
			BFSUtil(i, visited, pieces);
			//pieces[pieces.Num()- 1].Add(i);
		}
	}
	int maxPieceCount = 0;
	int maxPieceIndex = 0;
	for (int i = 0; i < pieces.Num(); i++) {
		if (maxPieceCount < pieces[i].Num()) {
			maxPieceCount = pieces[i].Num();
			maxPieceIndex = i;
		}
	}
	largestPiece = pieces[maxPieceIndex];
	for (int i = 0; i < wangTiles.Num(); i++) {
		if (wangTiles[i] != 0 && !largestPiece.Contains(i)) {
			wangTiles[i] = 0;
		}
	}
}
void Graph::GenerateNodes() {
	for (int i = 0; i < wangTiles.Num(); i++) {
		if (wangTiles[i] != 0) {
			if (isConnector(i)) {
				Node node;
				node.index = i;
				node.isRoom = true;
				Nodes.Add(node);
			}
			else if (isJunction(i)) {
				Node node;
				node.index = i;
				Nodes.Add(node);
			}
			else if (isHallEnd(i)) {
				Node node;
				node.index = i;
				node.isHallEnd = true;
				Nodes.Add(node);
			}
		}
	}
	for (int j = 0; j < Nodes.Num(); j++) {
		int u = Nodes[j].index;
		Node neighborNode;
		for (int i = 1; i <= 64; i *= 4)
		{
			if (i == 1 && width > u)
				continue;
			if (i == 4 && u % width == width - 1)
				continue;
			if (i == 16 && u + width >= wangTiles.Num())
				continue;
			if (i == 64 && u % width == 0)
				continue;
			if (wangTiles[u] & i)
			{
				int neighborIndex = getBitIndex(i, u);
				int nodeIndex = FindNodeByIndex(neighborIndex);
				if (nodeIndex != -1) {
					Nodes[j].addConnection( neighborNode, relDirection(i));
					Nodes[nodeIndex].addConnection(Nodes[j], oppDir[relDirection(i)]);
				}
			}
		}
	}
}
void Graph::GenerateRoomsAndSegments() {
	TArray<bool> visited;
	visited.Init(false, wangTiles.Num());
	for (int i = 0; i < wangTiles.Num(); i++) {
		if (wangTiles[i] != 0 && !visited[i]) {
			if (isRoom(i)) {
				MakeRoom(i, visited);
			} else if (isHall(i)) {
				MakeSegment(i, visited);
			}
		}
	}
}

void Graph::MakeRoom(int u, TArray<bool>& visited)
{
	int roomNumber = Rooms.Num();
	Room newRoom;
	newRoom.roomNumber = roomNumber;
	Rooms.Add(newRoom);
	
	TDoubleLinkedList<int> q;
	visited[u] = true;
	q.AddHead(u);

	while (!q.IsEmpty())
	{
		TDoubleLinkedList<int>::TDoubleLinkedListNode* head = q.GetHead();
		u = head->GetValue();

		if (isConnector(u)) {
			Node& node = Nodes[FindNodeByIndex(u)];
			node.setRoom(roomNumber);
			Rooms[roomNumber].exits.Add(node);
			q.RemoveNode(head);
			continue;
		}
		else
			Rooms[roomNumber].innerTiles.Add(u);
		q.RemoveNode(head);
		for (int i = 1; i <= 64; i *= 4)
		{
			if (i == 1 && width > u)
				continue;
			if (i == 4 && u % width == width - 1)
				continue;
			if (i == 16 && u + width >= wangTiles.Num())
				continue;
			if (i == 64 && u % width == 0)
				continue;
			int neighborIndex = getBitIndex(i, u);
			if (wangTiles[u] & i && !visited[neighborIndex] && (isRoom(neighborIndex) || isConnector(neighborIndex)))
			{
				visited[neighborIndex] = true;
				q.AddTail(neighborIndex);
			}
		}
	}
}
//void Graph::GenerateSegments() {
//	TArray<bool> visited;
//	visited.Init(false, wangTiles.Num());
//	for (int i = 0; i < Nodes.Num(); i++) {
//		//MakeSegment(Nodes[i], visited);
//	}
//}

void Graph::MakeSegment(int u, TArray<bool>& visited)
{
	int segmentNumber = Segments.Num();
	FSegment newSegment;
	newSegment.segmentNumber = segmentNumber;
	Segments.Add(newSegment);
	
	bool setA = false;
	bool setB = false;
	Node nodeA;
	int dirA = 0;
	Node nodeB;
	int dirB = 0;
	TDoubleLinkedList<int> q;
	visited[u] = true;
	q.AddHead(u);
	int dirLastAdded = 0;
	while (!q.IsEmpty())
	{
		TDoubleLinkedList<int>::TDoubleLinkedListNode* head = q.GetHead();
		u = head->GetValue();
		if (isConnector(u)) {
			if (!setA) {
				nodeA = Nodes[FindNodeByIndex(u)];
				dirA = oppDir[relDirection(dirLastAdded)];
				setA = true;
			}
			else {
				nodeB = Nodes[FindNodeByIndex(u)];
				dirB = oppDir[relDirection(dirLastAdded)];
				setB = true;
			}
			q.RemoveNode(head);
			continue; // don't search nodes
		}
		else
			Segments[segmentNumber].Tiles.Add(u);
		q.RemoveNode(head);

		for (int i = 1; i <= 64; i *= 4)
		{
			if (i == 1 && width > u)
				continue;
			if (i == 4 && u % width == width - 1)
				continue;
			if (i == 16 && u + width >= wangTiles.Num())
				continue;
			if (i == 64 && u % width == 0)
				continue;
			int neighborIndex = getBitIndex(i, u);
			if (wangTiles[u] & i && !visited[neighborIndex] && (isHall(neighborIndex) || isConnector(neighborIndex)))
			{
				visited[neighborIndex] = true;
				q.AddTail(neighborIndex);
				dirLastAdded = i;
			}
		}
	}
	if (setB) { // not a dead end
		nodeA.addConnection(Segments[segmentNumber], nodeB, dirA);
		nodeB.addConnection(Segments[segmentNumber], nodeA, dirB);
	}
}
//void Graph::BFSRooms(int u, TArray<bool>& visited)
//{
//	int roomNumber = Rooms.Num();
//	Room newRoom;
//	newRoom.roomId = roomNumber;
//	Rooms.Add(newRoom);
//
//	TDoubleLinkedList<int> q;
//
//	visited[u] = true;
//	q.AddHead(u);
//
//	while (!q.IsEmpty())
//	{
//		// Dequeue a vertex from queue and print it
//		TDoubleLinkedList<int>::TDoubleLinkedListNode* head = q.GetHead();
//		u = head->GetValue();
//		if (isConnector(u)) 
//			Rooms[roomNumber].exits.Add(u);
//		else
//			Rooms[roomNumber].innerTiles.Add(u);
//
//		q.RemoveNode(head);
//
//		// Get all adjacent vertices of the dequeued
//		// vertex s. If an adjacent has not been visited,
//		// then mark it visited and enqueue it
//
//		for (int i = 1; i <= 64; i *= 4)
//		{
//			if (i == 1 && width > u)
//				continue;
//			if (i == 4 && u % width == width - 1)
//				continue;
//			if (i == 16 && u + width >= wangTiles.Num())
//				continue;
//			if (i == 64 && u % width == 0)
//				continue;
//			int neighborIndex = getBitIndex(i, u);
//			if (wangTiles[u] & i && !isHall(neighborIndex) && !visited[neighborIndex])
//			{
//				visited[neighborIndex] = true;
//				q.AddTail(neighborIndex);
//				//pieces[pieceNumber].Add(neighborIndex);
//			}
//		}
//	}
//}
//void Graph::CreateRooms() {
//	TArray<bool> visited;
//	visited.Init(false, wangTiles.Num());
//
//	for (int i = 0; i < wangTiles.Num(); i++) {
//		if (wangTiles[i] != 0 && !isHall(i) && !visited[i]) {
//			BFSRooms(i, visited);
//		}
//	}
//	PathAdj.Init(Rooms.Num());
//}
////TArray<int> reverseArr(TArray<int> arr) {
//	//arr.
////}
//void Graph::MapPath(int roomIndex, int exitIndex) {
//
//	//int pathNum = room.paths.Num();
//	Path newPath;
//	//room.paths.Add(newPath);
//	TArray<int>seen;
//	TArray<int>stack;
//	Rooms[roomIndex].visited[exitIndex] = true;
//
//	int u = Rooms[roomIndex].exits[exitIndex];
//	stack.Add(u);
//	seen.Add(u);
//	bool backtracking = false;
//	while (!stack.IsEmpty())
//	{
//		// Dequeue a vertex from queue and print it
//		//TDoubleLinkedList<int>::TDoubleLinkedListNode* head = q.GetTail();
//		u = stack.Last();
//
//		bool found = false;
//
//		// search unseen neighbors
//		for (int i = 1; i <= 64; i *= 4)
//		{
//			if (i == 1 && width > u)
//				continue;
//			if (i == 4 && u % width == width - 1)
//				continue;
//			if (i == 16 && u + width >= wangTiles.Num())
//				continue;
//			if (i == 64 && u % width == 0)
//				continue;
//			int neighborIndex = getBitIndex(i, u);
//			if (wangTiles[u] & i)
//			{
//				if (!seen.Contains(neighborIndex) && !isRoom(neighborIndex)) {
//					// path to a room
//					if (isConnector(neighborIndex)) {
//						int dest = FindRoomByExit(neighborIndex);
//						int curExitIndex =  Rooms[dest].exits.Find(neighborIndex);
//
//						// add to adj
//						//seen.Add(neighborIndex);
//						bool isDuplicate = false;
//						for (Path path : PathAdj.get(roomIndex, dest)) {
//							if (path.path.Num() == stack.Num()) {
//								bool equal = true;
//								for (int n = 0; n < stack.Num(); n++) {
//									if (!stack.Contains(path.path[n]))
//										equal = false;
//								}
//								if (equal) {
//									isDuplicate = true;
//									break;
//								}
//							}
//						}
//						if (Rooms[roomIndex].exits.IsValidIndex(curExitIndex) && !isDuplicate) {
//							//Rooms[dest].visited[curExitIndex];
//							Path p;
//							p.path = stack;
//							p.deadEnd = false;
//							PathAdj.addPath(roomIndex, dest, p);
//						}
//					}
//					// add found to stack
//					else {
//						if (!found) {
//							found = true;
//							backtracking = false;
//						}
//						stack.Add(neighborIndex);
//						seen.Add(neighborIndex);
//						break;
//					}
//				}
//			}
//		} // end neighbor search
//		if (!found) {
//			if (!backtracking) {
//				Path p;
//				p.path = stack;
//				p.deadEnd = true;
//				Rooms[roomIndex].deadEnds.Add(p);
//				backtracking = true;
//			}
//			// pop if none are found
//			stack.Pop();
//		}
//	}
//}
//
//void Graph::SetPaths() {
//	for (int i = 0; i < Rooms.Num(); i++) {
//		Rooms[i].visited.Init(false, Rooms[i].exits.Num());
//	}
//	for (int i = 0; i < Rooms.Num(); i++) {
//		for (int exitIndex = 0; exitIndex < Rooms[i].exits.Num(); exitIndex++) {
//			if (!Rooms[i].visited[exitIndex]) {
//				MapPath(i, exitIndex);
//			}
//		}
//	}
//}

void Graph::FarthestRoom() {
	Node& startNode = RandomHallEnd();
	Node& endNode = RandomHallEnd();

	for (int i = 0; i < 10; i++) {
		if (startNode.index == endNode.index)
			endNode = RandomHallEnd();
		else
			break;
	}


	spawnIndex = startNode.index;
	exitIndex = endNode.index;
	//int startRoom = FindRoomWithEnd(startIndex);

	TArray<bool> sptSet;
	sptSet.Init(false, Nodes.Num());

	//Node startNode = Nodes[FindNodeByIndex(spawnIndex)];
	startNode.distFromSpawn = 0;

	// Find shortest path for all vertices
	for (int count = 0; count < Nodes.Num() - 1; count++) {
		int u = minDistance(sptSet);
		sptSet[u] = true;

		for (int v = 0; v < Nodes.Num(); v++){
			int distance = Nodes[u].distanceTo(Nodes[v]);
			// Update dist[v] only if is not in sptSet, there is an edge from
			// u to v, and total weight of path from src to  v through u is
			// smaller than current value of dist[v]
			if (!sptSet[v] && distance != -1 && Nodes[u].distFromSpawn != INT_MAX
				&& Nodes[u].distFromSpawn + distance < Nodes[v].distFromSpawn) {
				Nodes[v].distFromSpawn = Nodes[u].distFromSpawn + distance;
			}
		}
	}
	int farthest = 0;
	int maxDistance = 0;
	for (int i = 0; i < Nodes.Num(); i++) {
		
		if (Nodes[i].isRoom && Nodes[i].getRoom() > 0) {
			//Node* n = &Nodes[i];
			if (maxDistance < Nodes[i].distFromSpawn) {
				maxDistance = Nodes[i].distFromSpawn;
				farthest = Nodes[i].getRoom();
			}
		}
	}
	if(Rooms.IsValidIndex(farthest)){
		Rooms[farthest].isEndRoom = true;
		//bool found = false;
		//for (int i = 0; i < Rooms[farthest].exits.Num(); i++) {
		//	if (Rooms[farthest].exits[i].isHallEnd) {
		//		Rooms[farthest].exits[i].isExit = true;
		//		exitIndex = Rooms[farthest].exits[i].index;
		//		found = true;
		//	}
		//}
		//if (!found) {
		//	int dist = 0;
		//	do {
		//		Node& exit = RandomHallEnd();
		//		dist = exit.distFromSpawn;
		//		if (dist > 0) {
		//			exit.isExit = true;
		//			exitIndex = exit.index;
		//		}

		//	} while (dist == 0);
		//}
	}
	//Rooms[farthest.roomNumber];
}

//Room Graph::FarthestRoom() {
//	int startIndex = RandomOneTile();
//	spawnIndex = startIndex;
//	int startRoom = FindRoomWithEnd(startIndex);
//	if (Rooms.Num() == 0){
//		Room myRoom;
//		return myRoom;
//	} 
//	Rooms[startRoom].isStartRoom = true;
//	Rooms[startRoom].distanceFromStart = 0;
//	TArray<int> dist; // The output array.  dist[i] will hold the shortest
//// distance from src to i
//	dist.Init(INT_MAX, Rooms.Num());
//
//	TArray<bool> sptSet; // sptSet[i] will be true if vertex i is included in shortest
//	// path tree or shortest distance from src to i is finalized
//	sptSet.Init(false, Rooms.Num());
//
//	// Distance of source vertex from itself is always 0
//
//	dist[startRoom] = 0;
//
//	// Find shortest path for all vertices
//	for (int count = 0; count < Rooms.Num() - 1; count++) {
//		// Pick the minimum distance vertex from the set of vertices not
//		// yet processed. u is always equal to src in the first iteration.
//		int u = minDistance(dist, sptSet);
//
//		// Mark the picked vertex as processed
//		sptSet[u] = true;
//
//		// Update dist value of the adjacent vertices of the picked vertex.
//		for (int v = 0; v < Rooms.Num(); v++) {
//			int distance = PathAdj.getMin(u, v).path.Num();
//			// Update dist[v] only if is not in sptSet, there is an edge from
//			// u to v, and total weight of path from src to  v through u is
//			// smaller than current value of dist[v]
//			if (!sptSet[v] && distance != 0 && dist[u] != INT_MAX
//				&& dist[u] + distance < dist[v]) {
//				dist[v] = dist[u] + distance;
//				Rooms[v].distanceFromStart = dist[v];
//			}
//		}
//	}
//	int farthestRoom = startRoom;
//	int maxDistance = 0;
//	for (int i = 0; i < Rooms.Num(); i++) {
//		if (maxDistance < dist[i]) {
//			maxDistance = dist[i];
//			farthestRoom = i;
//		}
//	}
//	Rooms[farthestRoom].isEndRoom = true;
//	return Rooms[farthestRoom];
//}


Graph::~Graph()
{
}
