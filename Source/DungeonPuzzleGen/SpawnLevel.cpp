// Fill out your copyright notice in the Description page of Project Settings.



#include "SpawnLevel.h"
/*
void ASpawnLevel::CreateTexture() 
{
	UTextureRenderTarget2D* TextureRenderTarget;
	// Creates InputTexture to store TextureRenderTarget content
	UInputTexture* Texture = UInputTexture::CreateTransient(TextureRenderTarget->SizeX, TextureRenderTarget->SizeY, PF_B8G8R8A8);
#if WITH_EDITORONLY_DATA
	Texture->MipGenSettings = TMGS_NoMipmaps;
#endif
	Texture->SRGB = TextureRenderTarget->SRGB;

	// Read the pixels from the RenderTarget and store them in a FLinearColor array
	TArray<FLinearColor> SurfData;
	FRenderTarget* RenderTarget = TextureRenderTarget->GameThread_GetRenderTargetResource();
	RenderTarget->ReadPixels(SurfData);

	// Lock and copies the data between the textures
	void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	const int32 TextureDataSize = SurfData.Num() * 4;
	FMemory::Memcpy(TextureData, SurfData.GetData(), TextureDataSize);
	Texture->PlatformData->Mips[0].BulkData.Unlock();
	// Apply Texture changes to GPU memory
	Texture->UpdateResource();
	// Sets default values
}
*/

ASpawnLevel::ASpawnLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

TArray<FLinearColor> ASpawnLevel::RunWFC()
{
	TArray<FLinearColor> res;
	if (InputTexture == nullptr)
		return res;

	TextureCompressionSettings OldCompressionSettings = InputTexture->CompressionSettings;
	TextureMipGenSettings OldMipGenSettings = InputTexture->MipGenSettings;
	bool OldSRGB = InputTexture->SRGB;

	InputTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	InputTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	InputTexture->SRGB = false;
	InputTexture->UpdateResource();

	FTexturePlatformData* TextureData = InputTexture->GetPlatformData();
	const FColor* FormatedImageData = static_cast<const FColor*>(TextureData->Mips[0].BulkData.LockReadOnly());

	int32 width, height;
	width = TextureData->SizeX;
	height = TextureData->SizeY;
	TArray<FColor> imgIn = TArray<FColor>(FormatedImageData, width * height);
	TArray<FLinearColor> imgInLinear;
	imgInLinear.SetNum(imgIn.Num());

	for (int i = 0; i < imgInLinear.Num(); i++) {
		imgInLinear[i] = FLinearColor(imgIn[i]);
	}

	// Run WFC
	for (int test = 0; test < attempts; test++) {
		FOverlappingWFCOptions options = { out_height, out_width, periodic_input, periodic_output,  symmetry, ground, N };
		OverlappingWFC<FLinearColor> mywfc(Array2D<FLinearColor>(height, width, imgInLinear), options, seed);
		Array2D<FLinearColor> success = mywfc.run();
		
		if (success.complete) {			
			res = success.data;

			UE_LOG(LogTemp, Warning, TEXT("success. try #: %d"), test);		

			for (int i = 0; i < res.Num(); i++) {
				// process out image data
			}
			break;
		}
		else {
			if(test == attempts -1)
				UE_LOG(LogTemp, Warning, TEXT("failed"));
		}
	}
	// Restore input texture
	TextureData->Mips[0].BulkData.Unlock();
	InputTexture->CompressionSettings = OldCompressionSettings;
	InputTexture->MipGenSettings = OldMipGenSettings;
	InputTexture->SRGB = OldSRGB;
	InputTexture->UpdateResource();
	return res;
}

// Called when the game starts or when spawned
void ASpawnLevel::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASpawnLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
