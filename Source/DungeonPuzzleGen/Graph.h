// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
struct AdjList {
    int a;
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
public:
    TArray<int> adj;

    Graph();  // Constructor

    void Init(TArray<int>, int w);

    // function to add an edge to graph
    void addEdge(int v, int w);

    // prints BFS traversal from a given source s
    TArray<TArray<int>> BFS();

    ~Graph();
private:
    void BFSUtil(int u, TArray<bool>& visited, TArray<TArray<int>>& pieces);

    int getBitIndex(int tileBit, int tileIndex);
};
