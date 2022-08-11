// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Node.generated.h"


//struct FConnection;

USTRUCT(BlueprintType)
struct FSegment {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int segmentNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> Tiles;
};

struct Neighbor;
//USTRUCT(BlueprintType)
//struct FConnection {
//	GENERATED_BODY()
//
//		UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		int direction;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		FSegment Path;
//	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	Node* DestNode;
//};

/**
 * 
 */
class DUNGEONPUZZLEGEN_API Node
{

	// 1 to 4 FConnections
public:
	
	int index;
	int distFromSpawn = INT_MAX;
	bool isRoom = false;
	bool isHallEnd = false;
	bool isExit = false;

	TArray<Node> Neighbors;
	TArray<FSegment> Paths;
	TArray<int> Directions;

	Node();
	virtual ~Node() {};
	void addConnection(FSegment s, Node node, int dir) {
		Neighbors.Add(node);
		Paths.Add(s);
		Directions.Add(dir);
		//FConnection c;
		//c.DestNode = node;
		//c.Path = s;
		//c.direction = dir;
		//Connections.Add(c);
	}
	void addConnection(Node node, int dir) {
		FSegment empty;
		Neighbors.Add(node);
		Paths.Add(empty);
		Directions.Add(dir);

		//FConnection c;
		//c.DestNode = node;
		//c.Path = empty;
		//c.direction = dir;
		//Connections.Add(c);
	}
	//bool connectedTo(Node n) {
	//	for (Node con : Neighbors) {
	//		if (&n == &con)
	//			return true;
	//	}
	//	return false;
	//}
	int distanceTo(Node n) {
		for (int i = 0; i < Neighbors.Num();i++) {
			if (n.index == Neighbors[i].index)
				return Paths[i].Tiles.Num();
		}
		return -1;
	}
	virtual void setRoom(int num) {
		roomNumber = num;
	}
	int virtual getRoom() {
		return roomNumber;
	}
protected:
	int roomNumber = -1;
	//TArray<Neighbor> segmentNeighbors() {
	//	TArray<Neighbor> Neighbors;
	//	for (FConnection connection : Connections) {
	//		Neighbor N;
	//		N.Node = connection.DestNode;
	//		N.distance = connection.Path.Tiles.Num();
	//		Neighbors.Add(N);
	//	}
	//	return Neighbors;
	//}

	//TArray<Neighbor> getNeighbors() {
	//	return segmentNeighbors();
	//}

};

class Room {
public:
	int roomNumber;
	bool isStartRoom;
	bool isEndRoom;
	int distanceFromStart;
	TArray<int> innerTiles;
	TArray<Node> exits;
	//TArray<Path> deadEnds;
	//TArray<bool> visited;
	Room() : isStartRoom(false), isEndRoom(false) {}

	int size() {
		return innerTiles.Num() + exits.Num();
	}

};

class RoomNode : public Node
{
public:
	Room room;

	int roomSize;

	void setRoom(int num) {
		roomNumber = num;
		//room = r;
	}
	virtual ~RoomNode() {
	};
	int getRoom() {
		return roomNumber;
	}
	//TArray<Neighbor> getNeighbors() {
	//	TArray<Neighbor> Neighbors = segmentNeighbors();
	//	for (Node* Node : Room->exits) {
	//		if (Node != this) {
	//			Neighbor N;

	//			N.Node = Node;
	//			N.distance = 0;
	//			Neighbors.Add(N);
	//		}
	//	}
	//	return Neighbors;
	//}
};



struct Neighbor {
	int distance;
	Node Node;
};


/*
All tile iteration

Create nodes

search rooms
	node pointer array



for room tiles
	gather room tiles in array
	create nodes from exits
for hall tiles 
	create segment
for junction tile
	create node


*/