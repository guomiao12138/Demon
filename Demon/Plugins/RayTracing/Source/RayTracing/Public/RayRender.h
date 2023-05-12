// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RayRender.generated.h"

UCLASS()
class RAYTRACING_API ARayRender : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARayRender();

	UFUNCTION(BlueprintCallable, Category = RayRender)
		void MainRayRender();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};



class FRayComputeShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FRayComputeShader, Global);


public:

	FRayComputeShader() {}

	FRayComputeShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer) : FGlobalShader(Initializer)
	{
		OutputSurface.Bind(Initializer.ParameterMap, TEXT("OutputSurface"));
	}

	static bool shouldCach(EShaderPlatform Platform)
	{
		return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5);
	}

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parmaters)
	{
		return IsFeatureLevelSupported(Parmaters.Platform, ERHIFeatureLevel::SM5);
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& Environment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, Environment);
		Environment.CompilerFlags.Add(CFLAG_StandardOptimization);
	}

	void SetOutput(FRHICommandList& RHICmdList, FUnorderedAccessViewRHIRef OutputSurfaceUAV)
	{
		RHICmdList.SetUAVParameter(RHICmdList.GetBoundComputeShader(), OutputSurface.GetBaseIndex(), OutputSurfaceUAV);
	}

	void UnbindBuffers(FRHICommandList& RHICmdList)
	{
		FRHIComputeShader* ComputeShaderRHI = RHICmdList.GetBoundComputeShader();

		if (OutputSurface.IsBound())
		{
			RHICmdList.SetUAVParameter(ComputeShaderRHI, OutputSurface.GetBaseIndex(), nullptr);
		}
	}


public:
	LAYOUT_FIELD(FShaderResourceParameter, OutputSurface);

};
IMPLEMENT_SHADER_TYPE(, FRayComputeShader, TEXT("/Plugin/RayTracing/Private/RayTracing.usf"), TEXT("MainCS"), SF_Compute);