// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "overlapping_wfc.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "fast_WFC.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONPUZZLEGEN_API Ufast_WFC : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
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

	int directions_x[8] = { 0,1,1,1,0,-1,-1,-1 };
	int directions_y[8] = { -1,-1,0,1,1,1,0,-1 };

	UFUNCTION(BlueprintCallable, Category = "Populate")
	void RunWFC(TArray<FLinearColor>& colorImg, TArray<int>& wangImg);




};
