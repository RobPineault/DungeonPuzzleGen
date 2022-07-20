// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph.h"

Graph::Graph()
{   
}
void Graph::Init(TArray<int> arr, int w) {
    adj = arr;
    width = w;
}

void Graph::addEdge(int v, int w) {
    //if (adj.IsEmpty()) {
    //    TArray<int> newlist;
    //    newlist.Add(w);
    //    adj.Add(v, newlist);
   //     return;
    //}

    //TArray<int> list = adj[v];
    //if (list == nullptr) {
        //int* newlist;
        //newlist.Add(w);
        //adj.Add(v, newlist);
    //}else
    //{
        //list->Add(w);
    //}
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
    newPiece.Add(u);
    // Create a queue for BFS
    TDoubleLinkedList<int> q;

    // Mark the current node as visited and enqueue it
    visited[u] = true;
    q.AddHead(u);
    pieces.Add(newPiece);
    // 'i' will be used to get all adjacent vertices 4
    // of a vertex list<int>::iterator i;

    while (!q.IsEmpty())
    {
        // Dequeue a vertex from queue and print it
        TDoubleLinkedList<int>::TDoubleLinkedListNode* head = q.GetHead();
        u = head->GetValue();
        q.RemoveNode(head);

       // cout << u << " ";
        //q.pop_front();
        //TArray<int> list = adj[u];
        // Get all adjacent vertices of the dequeued
        // vertex s. If an adjacent has not been visited,
        // then mark it visited and enqueue it
        for (int i = 1; i <= 64; i*=4)
        {
            if (i == 1 && width > u)
                continue;
            if(i == 4 && u % width == width -1)
                continue;
            if (i == 16 && u + width >= adj.Num())
                continue;
            if (i == 64 && u % width == 0)
                continue;
            
            if (adj[u] & i && !visited[getBitIndex(i,u)])
            {
                visited[getBitIndex(i, u)] = true;
                q.AddTail(getBitIndex(i, u));
                pieces[pieceNumber].Add(u);
            }
        }
    }
}

TArray<TArray<int>> Graph::BFS() {
	TArray<bool> visited;
	visited.Init(false, adj.Num());
    TArray<TArray<int>> pieces;

    for (int i = 0; i < adj.Num(); i++) {
        if (adj[i] != 0 && !visited[i]) {
            BFSUtil(i, visited, pieces);
        }
            
    }
    return pieces;
}

Graph::~Graph()
{
}
