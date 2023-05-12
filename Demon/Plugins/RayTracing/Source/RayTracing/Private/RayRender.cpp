// Fill out your copyright notice in the Description page of Project Settings.


#include "RayRender.h"

void SaveArrayToTextTure(TArray<FVector4>* BitmapRef, int TexttureSizeX, int TexttureSizeY)
{
	TArray<FColor> BitMapToBeSave;

	for (int j = 0; j < TexttureSizeX; j++)
	{
		for (int i = 0; i < TexttureSizeY; i++)
		{
			float r = float(i) / float(TexttureSizeX);
			float g = float(j) / float(TexttureSizeY);
			float b = 0.2;

			uint8 cr = 255.99f * r;
			uint8 cg = 255.99f * g;
			uint8 cb = 255.99f * b;
			uint8 ca = 1.0f;
			BitMapToBeSave.Add(FColor(cr, cg, cb, ca));
		}
	}


	if (BitMapToBeSave.Num())
	{
		IFileManager::Get().MakeDirectory(*FPaths::ScreenShotDir(), true);

		const FString ScreenFileName(FPaths::ScreenShotDir() / TEXT("RenderOutputTextture"));

		uint32 ExtendWithMSAA = BitMapToBeSave.Num() / TexttureSizeY;
		FFileHelper::CreateBitmap(*ScreenFileName, ExtendWithMSAA, TexttureSizeY, BitMapToBeSave.GetData());
		UE_LOG(LogConsoleResponse, Display, TEXT("Content was saved to \"%s\""), *FPaths::ScreenShotDir());
	}
	else
	{
		UE_LOG(LogConsoleResponse, Error, TEXT("Failed to save BMP, format or texture type is not supported"));
	}


}


static void RayTracing_RenderThread(FRHICommandListImmediate& RHICommandList, ERHIFeatureLevel::Type FeadtureLevel)
{
	
	check(IsInRenderingThread());
	TArray<FVector4> Bitmap;

	TShaderMapRef<FRayComputeShader> ComputeShader(GetGlobalShaderMap(FeadtureLevel));
	RHICommandList.SetComputeShader(ComputeShader.GetComputeShader());
	//SetComputePipelineState(RHICommandList, ComputeShader.GetComputeShader());


	int sizeX = 256;
	int sizeY = 256;

	//FRHIResourceCreateInfo CreateInfo(TEXT("RayTracing_RenderThread"));
	//FTextureRHIRef Textture = RHICreateTexture(FRHITextureCreateDesc::Create2D(CreateInfo.DebugName)
	//	.SetExtent((int32)sizeX, (int32)sizeY)
	//	.SetFormat((EPixelFormat)PF_A32B32G32R32F)
	//	.SetNumMips((uint8)1)
	//	.SetNumSamples((uint8)1)
	//	.SetFlags(TexCreate_Shared | TexCreate_UAV)
	//	.SetInitialState(ERHIAccess::Unknown)
	//	.SetExtData(CreateInfo.ExtData)
	//	.SetBulkData(CreateInfo.BulkData)
	//	.SetGPUMask(CreateInfo.GPUMask)
	//	.SetClearValue(CreateInfo.ClearValueBinding));

	////FTextureRHIRef Textture = RHICreateTexture2D(sizeX, sizeY, PF_A32B32G32R32F, 1, 1, TexCreate_Shared | TexCreate_UAV, CreateInfo);

	//FUnorderedAccessViewRHIRef TextureUAV = RHICreateUnorderedAccessView(Textture);
	//ComputeShader->SetOutput(RHICommandList, TextureUAV);
	//DispatchComputeShader(RHICommandList, ComputeShader, sizeX / 32, sizeY / 32, 1);
	//ComputeShader->UnbindBuffers(RHICommandList);

	//Bitmap.Init(FVector4(1.0f, 0.0f, 0.0f, 1.0f), sizeX * sizeY);

	//uint32 LolStride = 0;
	//uint8* TexttureDataPtr = (uint8*)RHICommandList.LockTexture2D(Textture, 0, EResourceLockMode::RLM_ReadOnly, LolStride, true);
	//uint8* ArrayData = (uint8*)Bitmap.GetData();

	//FMemory::Memcpy(ArrayData, TexttureDataPtr, sizeX * sizeY);
	//RHICommandList.UnlockTexture2D(Textture, 0, false);

	SaveArrayToTextTure(&Bitmap, sizeX, sizeY);
}


void ARayRender::MainRayRender()
{
	ERHIFeatureLevel::Type FeatureLevel = GetWorld()->Scene->GetFeatureLevel();

	ENQUEUE_RENDER_COMMAND(RayTracingCommand)
		(
			[FeatureLevel](FRHICommandListImmediate& RHiCommandList)
			{
				RayTracing_RenderThread(RHiCommandList, FeatureLevel);
			}
	);

}



ARayRender::ARayRender()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ARayRender::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARayRender::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

