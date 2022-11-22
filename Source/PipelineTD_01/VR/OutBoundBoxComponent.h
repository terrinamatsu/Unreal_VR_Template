// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

#include "../VRPlayerPawn.h"

#include "OutBoundBoxComponent.generated.h"

/**
 * Component to set out of bouds areas in VR, e.g. to darken screen when out of bounds.
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class PIPELINETD_01_API UOutBoundBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
	
public : 
	UOutBoundBoxComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected : 
	
	UFUNCTION()
		void OnBoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnBoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fade)
		FLinearColor FadeColour = FLinearColor::Black; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fade)
		bool bFadeAudio = false;

private : 
	bool IsOverlapping;
	USphereComponent* PlayerHead;
	AVRPlayerPawn* Player;
};
