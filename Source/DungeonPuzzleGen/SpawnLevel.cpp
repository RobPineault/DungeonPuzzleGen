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

TArray<FLinearColor> ASpawnLevel::CopyTexture(){
	TArray<FLinearColor> imgInLinear;
	if (InputTexture == nullptr)
		return imgInLinear;

	TextureCompressionSettings OldCompressionSettings = InputTexture->CompressionSettings;
	TextureMipGenSettings OldMipGenSettings = InputTexture->MipGenSettings;
	bool OldSRGB = InputTexture->SRGB;

	InputTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	InputTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	InputTexture->SRGB = false;
	InputTexture->UpdateResource();
	FTexturePlatformData* TextureData = InputTexture->GetPlatformData();
	const FColor* FormatedImageData = static_cast<const FColor*>(TextureData->Mips[0].BulkData.LockReadOnly());

	width_in = TextureData->SizeX;
	height_in = TextureData->SizeY;
	TArray<FColor>imgIn = TArray<FColor>(FormatedImageData, width_in * height_in);

	imgInLinear.SetNum(imgIn.Num());

	for (int i = 0; i < imgInLinear.Num(); i++) {
		imgInLinear[i] = FLinearColor(imgIn[i]);
	}
	
	TextureData->Mips[0].BulkData.Unlock();
	InputTexture->CompressionSettings = OldCompressionSettings;
	InputTexture->MipGenSettings = OldMipGenSettings;
	InputTexture->SRGB = OldSRGB;
	InputTexture->UpdateResource();
	return imgInLinear;
}

TArray<int> ASpawnLevel::ProcessImage(TArray<FLinearColor> image) {
	int out_height = options.out_height;
	int out_width = options.out_width;
	TArray<int>wangTemp;
	wangTemp.SetNum(image.Num());
	//levelGraph->Init();
	FLinearColor White = FLinearColor().White;
	for (int i = 0; i < out_height; i++) {
		for (int j = 0; j < out_width; j++) {
			int index = i * out_width + j;
			int wangValue = 0;
			if (White == image[index]) {
				int topi = (i - 1) * out_width + j;
				int righti = i * out_width + (j + 1);
				int bottomi = (i + 1) * out_width + j;
				int lefti = i * out_width + (j - 1);
				bool top = false;
				bool right = false;
				bool bottom = false;
				bool left = false;

				if (i - 1 >= 0 && White == image[topi]) {
					wangValue += positionBitValue(-1, 0);
					top = true;
				}
				if (j + 1 < out_width && White == image[righti]) {
					wangValue += positionBitValue(0, 1);
					right = true;
				}
				if (i + 1 < out_height && White == image[bottomi]) {
					wangValue += positionBitValue(1, 0);
					bottom = true;
				}
				if (j - 1 >= 0 && White == image[lefti]) {
					wangValue += positionBitValue(0, -1);
					left = true;
				}

				if (top && right && White == image[(i - 1) * out_width + (j + 1)])
					wangValue += positionBitValue(-1, 1);
				if (right && bottom && White == image[(i + 1) * out_width + (j + 1)])
					wangValue += positionBitValue(1, 1);
				if (bottom && left && White == image[(i + 1) * out_width + (j - 1)])
					wangValue += positionBitValue(1, -1);
				if (left && top && White == image[(i - 1) * out_width + (j - 1)])
					wangValue += positionBitValue(-1, -1);
			}
			wangTemp[index] = wangValue;
		}
	}
	//levelGraph->Init(wangTemp, out_width);
	return wangTemp;
}

void ASpawnLevel::RunWFC(TArray<int>& wangImg, bool& succeeded)
{
	// Run WFC
	TArray<FLinearColor> ImageIn = CopyTexture();
	bool successful;
	for (int test = 0; test < attempts; test++) {
		OverlappingWFC<FLinearColor> mywfc(Array2D<FLinearColor>(height_in, width_in, ImageIn), options, seed);
		Array2D<FLinearColor> success = mywfc.run();
	
		if (success.complete) {		
			wangImg = ProcessImage(success.data);
			successful = true;
			break;
		}
		else if(test + 1 == attempts) {
			successful = false;
		}
	}
	succeeded = successful;
}
void ASpawnLevel::NumPieces(TArray<int> wangTiles, int width, int& numPieces, int& maxPieceSize, TArray<int>& maxPiece) {
	Graph graph;
	graph.Init(wangTiles, width);
	TArray<TArray<int>> pieces = graph.BFS();
	int maxPieceCount = 0;
	int maxPieceIndex = 0;
	for (int i = 0; i < pieces.Num(); i++) {
		if (maxPieceCount < pieces[i].Num()) {
			maxPieceCount = pieces[i].Num();
			maxPieceIndex = i;
		}
	}
	numPieces = pieces.Num();
	maxPieceSize = maxPieceCount;
	maxPiece = pieces[maxPieceIndex];
}

// Called when the game starts or when spawned
void ASpawnLevel::BeginPlay()
{
	Super::BeginPlay();

}
