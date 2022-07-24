// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct WangTiles {
    int h[15] = { 1,4,16,64,5,20,80,65, 17,	68,21,	84,	81,	69, 85 };
	int c[16] = { 23, 92, 113, 197,29,116,209,71, 87,93,117,213, 95,125,245,215 };
	int r[15] = { 7,28,112,193,31,124,241,199,119,221,127,253,247,223, 255 };

    bool isHall(int wang) {
        TArray<int> halls(&h[0], 15);
        return halls.Contains(wang);
    }
    bool isConnector(int wang) {
        TArray<int> connectors(&c[0], 16);
        return connectors.Contains(wang);
    }
    bool isRoom(int wang) {
        TArray<int> rooms(&r[0], 15);
        return rooms.Contains(wang);
    }
};


struct Edge {
    int roomId;
    TArray<int> path;
};

class Room {
public:
    int roomId;
    int size;
    TArray<int> innerTiles;
    TArray<int> exits;
    TArray<Edge> adj;
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

    TArray<Room> Rooms;
    Graph();  // Constructor

    void Init(TArray<int>, int w);

    // function to add an edge to graph
    void addEdge(int v, int w);

    // prints BFS traversal from a given source s
    TArray<TArray<int>> BFS();

    void CreateRooms();

    bool isHall(int i) {
        return wangData.isHall(wangTiles[i]);
    }
    bool isConnector(int i) {
        return wangData.isConnector(wangTiles[i]);
    }
    bool isRoom(int i) {
        return wangData.isRoom(wangTiles[i]);
    }

    ~Graph();
private:
    void BFSUtil(int u, TArray<bool>& visited, TArray<TArray<int>>& pieces);
    void BFSRooms(int u, TArray<bool>& visited);

    int getBitIndex(int tileBit, int tileIndex);
};
