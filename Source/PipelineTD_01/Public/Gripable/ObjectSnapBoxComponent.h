// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

#include "../../GrippableComponent.h"

#include "ObjectSnapBoxComponent.generated.h"

UENUM(BlueprintType)
enum class ESnapMode : uint8
{
	OnDrop = 0,
	OnEnter = 1,
};

/**
 * Box Component that has the ability to snap Grippable Objects to it. 
 */
UCLASS()
class PIPELINETD_01_API UObjectSnapBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
	

public:
	UObjectSnapBoxComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESnapMode SnapMode = ESnapMode::OnDrop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* SnappedObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UGrippableComponent* SnappedGripComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bSnapOrientation = true;

	UFUNCTION() // Generic Snap Implementation
		void SnapGripObject(UGrippableComponent* ToSnap);
	UFUNCTION() // Boundary Overlap Event
		void OnBoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
