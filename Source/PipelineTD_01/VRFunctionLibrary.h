// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "VRFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PIPELINETD_01_API UVRFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static AActor* GetClosestObjectByComponent(TArray<FHitResult> hits, TSubclassOf<UActorComponent> ComponentSubClass, FVector Centre);

	static UActorComponent* GetClosestComponent(TArray<AActor*> Actors, TSubclassOf<UActorComponent> ComponentSubClass, FVector Centre);
};
