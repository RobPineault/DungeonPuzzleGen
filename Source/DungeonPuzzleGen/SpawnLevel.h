// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "overlapping_wfc.h"
#include "GameFramework/Actor.h"
#include "SpawnLevel.generated.h"


UCLASS()
class DUNGEONPUZZLEGEN_API ASpawnLevel : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASpawnLevel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC Options")
	int out_width = 32;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC Options")
	int out_height = 32;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC Options")
	int N = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC Options")
	bool periodic_output = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC Options")
	bool periodic_input = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC Options")
	bool ground = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC Options")
	int symmetry = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC Options")
	int attempts = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC Options")
	int seed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC Options")
	UTexture2D* InputTexture = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Populate")
	virtual TArray<FLinearColor> RunWFC();

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//UPROPERTY(VisibleAnywhere)
	//UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//private
	//AActor* InstancedActor;
};
