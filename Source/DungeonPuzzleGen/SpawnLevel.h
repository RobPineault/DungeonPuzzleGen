// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "overlapping_wfc.h"
#include "Graph.h"
#include "GameFramework/Actor.h"
#include "SpawnLevel.generated.h"

USTRUCT(BlueprintType)
struct FRoomData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int roomNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> inner;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<int> exits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int distanceFromStart;
};

UCLASS()
class DUNGEONPUZZLEGEN_API ASpawnLevel : public AActor
{
	GENERATED_BODY()

public:	
	int width_in = 0;
	int height_in = 0;
	// Sets default values for this actor's properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC Options")
	FOverlappingWFCOptions options;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC Options")
	int attempts = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC Options")
	int seed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC Options")
	UTexture2D* InputTexture = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Populate")
	void RunWFC(TArray<int>& wangImg, bool& succeeded );

	Graph levelGraph;

	UFUNCTION(BlueprintCallable, Category = "Hi")
	void ProccessOutput(TArray<int> wangTiles, int& spawnIndex, TArray<int>& maxPiece, TArray<FRoomData>& Rooms);

	UFUNCTION(BlueprintCallable, Category = "Hi")
	void GetTreasureRoom(int start,  TArray<int>& endRoom);

	UFUNCTION(BlueprintCallable, Category = "Level")
	void GetRoomData(int roomIndex, TArray<int>& room) {
	};
	UFUNCTION(BlueprintCallable, Category = "Level")
	void GetNodes(TArray<int>& Nodes) {
		TArray<int> res;
		for (Node n : levelGraph.Nodes) {
			res.Add(n.index);
		}
		Nodes = res;
	};
	UFUNCTION(BlueprintCallable, Category = "Level")
	void GetExit(int& exitIndex) {

		int exit = levelGraph.exitIndex;
		exitIndex = exit;
	};
	UFUNCTION(BlueprintCallable, Category = "Level")
	void GetSegment(FSegment& segment) {
		FSegment s;
		if (levelGraph.Segments.Num() > 0) {
			s = levelGraph.Segments[FMath::RandRange(0, levelGraph.Segments.Num() - 1)];
			s.Tiles.Sort();
		}

		segment = s;
	};
	//UFUNCTION(BlueprintCallable, Category = "Populate")
	//void getPieces(int& pieces);

	int directions_x[8] = {0,1,1,1,0,-1,-1,-1 };
	//enum direction_x {
	//	TOP = 0,
	//};
	int directions_y[8] = {-1,-1,0,1,1,1,0,-1 };

	//Graph* levelGraph;

	TArray<FLinearColor> CopyTexture();
	
	TArray<int> ProcessImage(TArray<FLinearColor> image);
	/*
	// Set Mesh		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Populate")
	UStaticMesh* StaticMesh;


	UFUNCTION(BlueprintCallable, Category = "Populate")
	virtual void CreateWorkerInstance();

	// Transform
	UFUNCTION(BlueprintCallable, Category = "Populate")
	virtual void AddInstance(FTransform Transformation);
	*/
	ASpawnLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//UPROPERTY(VisibleAnywhere)
	//UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

};