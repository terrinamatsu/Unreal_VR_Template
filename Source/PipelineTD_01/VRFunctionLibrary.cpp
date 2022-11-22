// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University


#include "VRFunctionLibrary.h"

AActor* UVRFunctionLibrary::GetClosestObjectByComponent(TArray<FHitResult> hits, TSubclassOf<UActorComponent> ComponentSubClass, FVector Centre)
{
	float closestDistance = 10000000.0f;
	AActor* closestObject = nullptr;


	for (auto& hit : hits)
	{
		UActorComponent* trycast = hit.GetActor()->GetComponentByClass(ComponentSubClass);
		if (trycast)
		{
			if ((Centre - hit.GetActor()->GetActorLocation()).Size() < closestDistance)
			{
				closestDistance = (Centre - hit.GetActor()->GetActorLocation()).Size();
				closestObject = hit.GetActor();
			}
		}
	}

	return closestObject;
}

UActorComponent* UVRFunctionLibrary::GetClosestComponent(TArray<AActor*> Actors, TSubclassOf<UActorComponent> ComponentSubClass, FVector Centre)
{
	float closestDistance = 10000000.0f;
	UActorComponent* closestObject = nullptr;

	for (auto& Actor : Actors)
	{
		UActorComponent* trycast = Actor->GetComponentByClass(ComponentSubClass);
		if (trycast)
		{
			if ((Centre - Actor->GetActorLocation()).Size() < closestDistance)
			{
				closestDistance = (Centre - Actor->GetActorLocation()).Size();
				closestObject = trycast;
			}
		}
	}

	return closestObject;
}