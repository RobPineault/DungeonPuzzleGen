// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph.h"

Graph::Graph()
{   
}
void Graph::Init(TArray<int> arr, int w) {
    wangTiles = arr;
    width = w;
}

void Graph::addEdge(int v, int w) {

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
        for (int i = 1; i <= 64; i*=4)
        {
            if (i == 1 && width > u)
                continue;
            if(i == 4 && u % width == width -1)
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

TArray<TArray<int>> Graph::BFS() {
	TArray<bool> visited;
	visited.Init(false, wangTiles.Num());
    TArray<TArray<int>> pieces;

    for (int i = 0; i < wangTiles.Num(); i++) {
        if (wangTiles[i] != 0 && !visited[i]) {
            BFSUtil(i, visited, pieces);
            //pieces[pieces.Num()- 1].Add(i);
        }   
    }
    return pieces;
}
void Graph::BFSRooms(int u, TArray<bool>& visited)
{
    int roomNumber = Rooms.Num();
    Room newRoom;
    Rooms.Add(newRoom);

    TDoubleLinkedList<int> q;

    visited[u] = true;
    q.AddHead(u);

    while (!q.IsEmpty())
    {
        // Dequeue a vertex from queue and print it
        TDoubleLinkedList<int>::TDoubleLinkedListNode* head = q.GetHead();
        u = head->GetValue();
        if(isConnector(u))
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
/*
void Graph::SetEdges() {
    TArray<bool> visited;
    visited.Init(false, wangTiles.Num());

    for (int i = 0; i < wangTiles.Num(); i++) {
        if (wangTiles[i] != 0 && !isHall(i) && !visited[i]) {
            BFSRooms(i, visited);
        }
    }
}
*/

Graph::~Graph()
{
}
