// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph.h"

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

void Graph::BFSRooms(int u, TArray<bool>& visited)
{
	int roomNumber = Rooms.Num();
	Room newRoom;
	newRoom.roomId = roomNumber;
	Rooms.Add(newRoom);

	TDoubleLinkedList<int> q;

	visited[u] = true;
	q.AddHead(u);

	while (!q.IsEmpty())
	{
		// Dequeue a vertex from queue and print it
		TDoubleLinkedList<int>::TDoubleLinkedListNode* head = q.GetHead();
		u = head->GetValue();
		if (isConnector(u)) 
			Rooms[roomNumber].exits.Add(u);
		else
			Rooms[roomNumber].innerTiles.Add(u);

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
			if (wangTiles[u] & i && !isHall(neighborIndex) && !visited[neighborIndex])
			{
				visited[neighborIndex] = true;
				q.AddTail(neighborIndex);
				//pieces[pieceNumber].Add(neighborIndex);
			}
		}
	}
}
void Graph::CreateRooms() {
	TArray<bool> visited;
	visited.Init(false, wangTiles.Num());

	for (int i = 0; i < wangTiles.Num(); i++) {
		if (wangTiles[i] != 0 && !isHall(i) && !visited[i]) {
			BFSRooms(i, visited);
		}
	}
}
//TArray<int> reverseArr(TArray<int> arr) {
	//arr.
//}
void Graph::MapPath(Room room, int exitIndex) {

	//int pathNum = room.paths.Num();
	Path newPath;
	room.paths.Add(newPath);
	TArray<int>seen;
	TArray<int>stack;
	room.visited[exitIndex] = true;

	int u = room.exits[exitIndex];
	stack.Add(u);
	seen.Add(u);
	bool backtracking = false;
	while (!stack.IsEmpty())
	{
		// Dequeue a vertex from queue and print it
		//TDoubleLinkedList<int>::TDoubleLinkedListNode* head = q.GetTail();
		u = stack.Last();

		bool found = false;

		// search unseen neighbors
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
			if (wangTiles[u] & i)
			{

				if (isConnector(neighborIndex) && neighborIndex != room.exits[exitIndex]) {
					int dest = FindRoomByExit(neighborIndex);
					int curExitIndex = room.exits.IsValidIndex(room.exits.Find(neighborIndex)) ? room.exits.Find(neighborIndex) : 0;
					if (dest == room.roomId && room.visited[curExitIndex]) {
						continue;
					}

					Path p;
					TArray<int> path(stack.GetData(), stack.Num());
					p.path = path;

					if (dest == room.roomId) {
						room.visited[curExitIndex] = true;

					}
					//else {
					//	Rooms[dest].paths.Add(p.path.reverse());
					//	Rooms[dest].visited[Rooms[dest].exits.IndexOfByKey(neighborIndex)] = true;
					//}
					p.destRoom = dest;
					p.deadEnd = false;
					room.paths.Add(p);
				}
				// add found to stack
				else if (!seen.Contains(neighborIndex)) {
					if (!found) {
						found = true;
						backtracking = false;
					}						
					stack.Add(neighborIndex);
					seen.Add(neighborIndex);
				}
			}
		}
		if (!found) {
			if (!backtracking) {
				Path p;
				TArray<int> path(stack.GetData(), stack.Num());
				p.path = path;
				p.deadEnd = true;
				room.paths.Add(p);
				backtracking = true;
			}
			// pop if none are found
			stack.Pop();
		}
	}
}

void Graph::SetPaths() {
	for (int i = 0; i < Rooms.Num(); i++) {
		Room room = Rooms[i];
		room.visited.Init(false, room.exits.Num());

		for (int exitIndex = 0; exitIndex < room.exits.Num(); exitIndex++) {
			if (!room.visited[exitIndex]) {
				MapPath(room, exitIndex);
			}
		}
	}
}



Room Graph::FarthestRoom(int startIndex) {
	int startRoom = FindRoomWithEnd(startIndex);
	if (Rooms.Num() == 0){
		Room myRoom;
		return myRoom;
	} 
	Rooms[startRoom].isStartRoom = true;
	Rooms[startRoom].distanceFromStart = 0;

	TArray<int> dist; // The output array.  dist[i] will hold the shortest
// distance from src to i
	dist.Init(INT_MAX, Rooms.Num());

	TArray<bool> sptSet; // sptSet[i] will be true if vertex i is included in shortest
	// path tree or shortest distance from src to i is finalized
	sptSet.Init(false, Rooms.Num());

	// Distance of source vertex from itself is always 0

	dist[startRoom] = 0;

	// Find shortest path for all vertices
	for (int count = 0; count < Rooms.Num() - 1; count++) {
		// Pick the minimum distance vertex from the set of vertices not
		// yet processed. u is always equal to src in the first iteration.
		int u = minDistance(dist, sptSet);

		// Mark the picked vertex as processed
		sptSet[u] = true;

		// Update dist value of the adjacent vertices of the picked vertex.
		for (int v = 0; v < Rooms.Num(); v++) {
			int distance = Rooms[u].distanceToNeighbor(v);
			// Update dist[v] only if is not in sptSet, there is an edge from
			// u to v, and total weight of path from src to  v through u is
			// smaller than current value of dist[v]
			if (!sptSet[v] && distance != -1 && dist[u] != INT_MAX
				&& dist[u] + distance < dist[v]) {
				dist[v] = dist[u] + distance;
				Rooms[v].distanceFromStart = dist[v];
			}
		}
	}
	int farthestRoom = startRoom;
	int maxDistance = 0;
	for (int i = 0; i < Rooms.Num(); i++) {
		if (maxDistance < dist[i]) {
			maxDistance = dist[i];
			farthestRoom = i;
		}
	}
	return Rooms[farthestRoom];
}


Graph::~Graph()
{
}
