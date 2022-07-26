// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct WangTiles {
    int h[15] = { 1,4,16,64,5,20,80,65,17,68,21,84,81,69,85 };
	int c[16] = { 23,92,113,197,29,116,209,71,87,93,117,213,95,125,245,215 };
	int r[15] = { 7,28,112,193,31,124,241,199,119,221,127,253,247,223,255 };
};


class Path {
public:
    bool deadEnd;
    int destRoom;
    TArray<int> path;
    //Path(TArray<int> p, int size) {
        //TArray<int> newPath(p, size);
        //path = p;
    //}
    //Path(int* p, int size, int dest) {
    //    TArray<int> newPath(p, size);
    //    path = newPath;
    //    destRoom = dest;
    //}

};

class Room {
public:
    int roomId;
    int size; 
    bool isStartRoom;
    int distanceFromStart;
    TArray<int> innerTiles;
    TArray<int> exits;
    TArray<Path> paths;
    TArray<bool> visited;
    bool isNeighbor(int room) {
        for (Path path : paths) {
            if (!path.deadEnd && path.destRoom == room) {
                return true;
            }
        }
        return false;
    }
    int distanceToNeighbor(int room) {
        for (Path path : paths) {
            if (!path.deadEnd && path.destRoom == room) {
                return path.path.Num();
            }
        }
        return -1;
    }
};

/**
 * 
 */
class DUNGEONPUZZLEGEN_API Graph
{

    //int V;    // No. of vertices
    int maxV;
    // Pointer to an array containing adjacency
    // lists
    int width;
    WangTiles wangData;
public:
    TArray<int> wangTiles;
    TArray<int> largestPiece;

    TArray<Room> Rooms;
    Graph();  // Constructor

    void Init(TArray<int>, int w);


    // prints BFS traversal from a given source s
    void ExtractLargestPiece();

    void CreateRooms();

    void SetPaths();

    Room FarthestRoom(int startIndex);

    bool isHall(int i) {
        int* hallPointer = wangData.h;
        TArray<int> halls(hallPointer, 15);
        return halls.Contains(wangTiles[i]);
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
    int FindRoomByExit(int index) {
        for (Room room : Rooms) {
            if (room.exits.Contains(index))
                return room.roomId;            
        }
        return -1;
    }
    int FindRoomWithEnd(int deadEndIndex) {
        for (Room room : Rooms) {
            for (Path path : room.paths) {
                if (path.deadEnd && path.path.Contains(deadEndIndex))
                    return room.roomId;
            }
        }
        return 0;
    }
    ~Graph();
private:
    void MapPath(Room room, int exitIndex);
    void BFSUtil(int u, TArray<bool>& visited, TArray<TArray<int>>& pieces);
    void BFSRooms(int u, TArray<bool>& visited);

    int getBitIndex(int tileBit, int tileIndex);
    int minDistance(TArray<int> dist, TArray<bool> sptSet)
    {

        // Initialize min value
        int min = INT_MAX, min_index = 0;

        for (int v = 0; v < Rooms.Num(); v++)
            if (sptSet[v] == false && dist[v] <= min)
                min = dist[v], min_index = v;

        return min_index;
    }
};
