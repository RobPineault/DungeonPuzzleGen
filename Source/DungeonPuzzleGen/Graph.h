// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Node.h"
#include "Graph.generated.h"

USTRUCT(BlueprintType)
struct FPathData {
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int startRoom;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int destRoom;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int> path;
};



USTRUCT(BlueprintType)
struct FWangTilesGroups {
    GENERATED_BODY()
    int h[10] = { 1,4,16,64,5,20,80,65,17,68 };
	int c[16] = { 23,92,113,197,29,116,209,71,87,93,117,213,95,125,245,215 };
	int r[15] = { 7,28,112,193,31,124,241,199,119,221,127,253,247,223,255 };
    int j[5] = { 21,84,81,69,85 };
};



class Path {
public:
    bool deadEnd;
    int adjIndex;
    TArray<int> path;
    FPathData Package() {
        FPathData data;
        //data.deadEnd = deadEnd;
        data.path = path;
        return data;
    }
};


class Adjacency {
public:
    TArray<TArray<Path>> adj;
    int roomCount;
    void Init(int numRooms) {
        TArray<Path> pGroup;
        adj.Init(pGroup, numRooms * numRooms);
        roomCount = numRooms;
    }
    void addPath(int i, int j, Path p) noexcept {
        adj[j + i * roomCount].Add(p);
        adj[i + j * roomCount].Add(p);
    }
    TArray<Path>& get(int i, int j) noexcept {
        return adj[j + i * roomCount];
    }
    Path getMin(int i, int j) {
        TArray<Path> paths = adj[j + i * roomCount];
        if (paths.Num() == 0) {
            Path p;
            return p;
        }
        int minDist = INT_MAX;
        int minIndex = 0;
        for (int n = 0; n < paths.Num(); n++) {
            if (paths[n].path.Num() < minDist) {
                minDist = paths[n].path.Num();
                minIndex = n;
            }
        }
        return paths[minIndex];
    }
    TArray<FPathData> Package() {
        TArray<FPathData> pathData;
        for (int n = 0; n < adj.Num(); n++) {
            int i = n / roomCount;
            int j = n % roomCount;
            if (j - i >= 0) {
                FPathData data = getMin(i, j).Package();
                data.startRoom = i;
                data.destRoom = j;
                pathData.Add(data);
            }
        }
        // UE_LOG(LogTemp, Warning, TEXT("Sending: %d"), pathData.Num());
        return pathData;
    }
};

//class Room {
//public:
//    int roomId; 
//    bool isStartRoom;
//    bool isEndRoom;
//    int distanceFromStart;
//    TArray<int> innerTiles;
//    TArray<int> exits;
//    TArray<Path> deadEnds;
//    TArray<bool> visited;
//    Room() : isStartRoom(false), isEndRoom(false) {}
//
//    int size() {
//        return innerTiles.Num() + exits.Num();
//    }
//
//    TArray<FPathData> PackagePaths() {
//        TArray<FPathData> pathData;
//        for (Path p : deadEnds) {
//            FPathData data = p.Package();
//            data.startRoom = roomId;
//            pathData.Add(data);
//        }
//       // UE_LOG(LogTemp, Warning, TEXT("Sending: %d"), pathData.Num());
//        return pathData;
//    }
//};

/**
 * 
 */
class DUNGEONPUZZLEGEN_API Graph
{
    int width;
    FWangTilesGroups wangData;

public:
    TArray<int> wangTiles;
    TArray<int> largestPiece;

    TArray<Room> Rooms;
    Adjacency PathAdj;

    TArray<Node> Nodes;
    TArray<FSegment> Segments;

    Graph();

    int spawnIndex;
    int exitIndex;

    void Init(TArray<int>, int w);

    void ExtractLargestPiece();

    void GenerateNodes();
    void GenerateRoomsAndSegments();
    //void GenerateSegments();
    void MakeRoom(int u, TArray<bool>& visited);
    void MakeSegment(int u, TArray<bool>& visited);
   // void CreateRooms();

    //void SetPaths();

    void FarthestRoom();

    bool isHall(int i) {
        int* hallPointer = wangData.h;
        TArray<int> halls(hallPointer, 10);
        return halls.Contains(wangTiles[i]);
    }
    bool isJunction(int i) {
        int* junctionPointer = wangData.j;
        TArray<int> junctions(junctionPointer, 5);
        return junctions.Contains(wangTiles[i]);
    }
    bool isConnector(int i) {
        int* connectorPointer = wangData.c;
        TArray<int> connectors(connectorPointer, 16);
        return connectors.Contains(wangTiles[i]);
    }
    bool isRoom(int i) {
        int* roomPointer = wangData.r;
        TArray<int> rooms(roomPointer, 15);
        return rooms.Contains(wangTiles[i]);
    }
    bool isHallEnd(int i) {
        int endPointer[4] = {1,4,16,64};
        TArray<int> ends(endPointer, 4);
        return ends.Contains(wangTiles[i]);
    }
    //bool isNeighbor(int room) {
    //    for (Path path : Paths) {
    //        if (!path.deadEnd && path.destRoom == room) {
    //            return true;
    //        }
    //    }
    //    return false;
    //}
    //int distanceToNeighbor(int start, int dest) {
    //    PathAdj.get(start, dest).path.Num();
    //    for (Path path : Paths) {
    //        if (!path.deadEnd && path.destRoom == room) {
    //            return path.path.Num();
    //        }
    //    }
    //    return -1;
    //}
    int FindNodeByIndex(int index) {
        for (int i = 0; i < Nodes.Num(); i++) {
            if (Nodes[i].index == index)
                return i;
        }
        return -1;
    }
    //int FindRoomByExit(int index) {
    //    for (int i = 0; i < Rooms.Num(); i++) {
    //        if (Rooms[i].exits.Contains(index))
    //            return i;            
    //    }
    //    return -1;
    //}
    //int FindRoomWithEnd(int deadEndIndex) {
    //    for (int i = 0; i < Rooms.Num(); i++) {
    //        for (Path path : Rooms[i].deadEnds) {
    //            if (path.path.Contains(deadEndIndex))
    //                return i;
    //        }
    //    }
    //    return 0;
    //}
    Node& RandomHallEnd() {
        TArray<int> HallEndNodes;
        for (int i = 0; i < Nodes.Num(); i++) {
            if (Nodes[i].isHallEnd) {
                HallEndNodes.Add(i);
            }
        }
        if (HallEndNodes.Num() > 0) {
            int randNode = FMath::RandRange(0, HallEndNodes.Num() - 1);
            return Nodes[HallEndNodes[randNode]];
        }
        else 
            return Nodes[FMath::RandRange(0, Nodes.Num() - 1)];
    }

    ~Graph();

private:
    void MapPath(int room, int exitIndex);
    void BFSUtil(int u, TArray<bool>& visited, TArray<TArray<int>>& pieces);
    void BFSRooms(int u, TArray<bool>& visited);

    int getBitIndex(int tileBit, int tileIndex);
    int minDistance(TArray<bool> sptSet)
    {
        int min = INT_MAX, min_index = 0;

        for (int v = 0; v < Nodes.Num(); v++)
            if (sptSet[v] == false && Nodes[v].distFromSpawn <= min)
                min = Nodes[v].distFromSpawn, min_index = v;

        return min_index;
    }
};
