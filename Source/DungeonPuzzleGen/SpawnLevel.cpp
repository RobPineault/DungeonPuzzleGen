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
	//PrimaryActorTick.bCanEverTick = true;

}

int positionBitValue(int i, int j) {
	if (i == -1 && j == 0)
		return 1;
	if (i == -1 && j == 1)
		return 2;
	if (i == 0 && j == 1)
		return 4;
	if (i == 1 && j == 1)
		return 8;
	if (i == 1 && j == 0)
		return 16;
	if (i == 1 && j == -1)
		return 32;
	if (i == 0 && j == -1)
		return 64;
	if (i == -1 && j == -1)
		return 128;
	return 0;
}

void ASpawnLevel::RunWFC(TArray<FLinearColor>& colorImg, TArray<int>& wangImg)
{
	//TArray<FLinearColor>colorImg;
	if (InputTexture == nullptr)
		return;

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
		//FOverlappingWFCOptions options = { out_height, out_width, periodic_input, periodic_output,  symmetry, ground, N };
		OverlappingWFC<FLinearColor> mywfc(Array2D<FLinearColor>(height, width, imgInLinear), options, seed);
		Array2D<FLinearColor> success = mywfc.run();
		int out_height = options.out_height;
		int out_width = options.out_width;

		if (success.complete) {			
			//res = success.data;
			colorImg = success.data;
			TArray<int>wangTemp;
			wangTemp.SetNum(success.data.Num());
			UE_LOG(LogTemp, Warning, TEXT("success. try #: %d"), test);		
			FLinearColor White = FLinearColor().White;
			for (int i = 0; i < out_height; i++) {
				for (int j = 0; j < out_width; j++) {
					int index = i * out_width + j;
					int wangValue = 0;
					if (White == success.data[index]) {
						int topi = (i - 1) * out_width + j;
						int righti = i * out_width + (j + 1);
						int bottomi = (i + 1) * out_width + j;
						int lefti = i * out_width + (j - 1);
						bool top = false;
						bool right = false;
						bool bottom = false;
						bool left = false;

						if (i - 1 >= 0 && White == success.data[topi]) {
							wangValue += positionBitValue(-1, 0);
							top = true;
						}
						if (j + 1 < out_width && White == success.data[righti]) {
							wangValue += positionBitValue(0, 1);
							right = true;
						}
						if (i + 1 < out_height && White == success.data[bottomi]) {
							wangValue += positionBitValue(1, 0);
							bottom = true;
						}
						if (j - 1 >= 0 && White == success.data[lefti]) {
							wangValue += positionBitValue(0, -1);
							left = true;
						}

						if (top && right && White == success.data[(i - 1) * out_width + (j + 1)])
							wangValue += positionBitValue(-1, 1);
						if (right && bottom && White == success.data[(i + 1) * out_width + (j + 1)])
							wangValue += positionBitValue(1, 1);
						if (bottom && left && White == success.data[(i + 1) * out_width + (j - 1)])
							wangValue += positionBitValue(1, -1);
						if (left && top && White == success.data[(i - 1) * out_width + (j - 1)])
							wangValue += positionBitValue(-1, -1);
					}
					/*
					for (int u = -1; u <= 1; u++)
					{
						for (int v = -1; v <= 1; v++)
						{
							if (i - u >= 0 && j - v >= 0 && i - u < out_height && j - v < out_width && !(u == 0 && v == 0))
							{
								int mindex = (i - u) * out_width + (j - v);
								if (FLinearColor().White == success.data[mindex])
									wangValue += positionBitValue(u, v);
							}
						}
					}
					*/
					wangTemp[index] = wangValue;
				}
			}
			wangImg = wangTemp;
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

}

// Called when the game starts or when spawned
void ASpawnLevel::BeginPlay()
{
	Super::BeginPlay();

}
