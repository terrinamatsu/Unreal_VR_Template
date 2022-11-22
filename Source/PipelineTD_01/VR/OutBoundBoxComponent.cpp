// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University


#include "OutBoundBoxComponent.h"

UOutBoundBoxComponent::UOutBoundBoxComponent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

	SetGenerateOverlapEvents(true);
	OnComponentBeginOverlap.AddDynamic(this, &UOutBoundBoxComponent::OnBoundBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UOutBoundBoxComponent::OnBoundEndOverlap);
}

// Called every frame
void UOutBoundBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsOverlapping)
	{
		// Set Camera Fade 
		APlayerCameraManager* cam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (cam)
		{
			TArray<AActor*> ignore;
			ignore.Add(Player);
			FHitResult hit;

			UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
				PlayerHead->GetComponentLocation(),
				PlayerHead->GetComponentLocation(),
				PlayerHead->GetScaledSphereRadius(),
				UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_WorldDynamic),
				false,
				ignore,
				EDrawDebugTrace::None,
				hit,
				false);

			if (hit.GetComponent() && hit.GetComponent() == this)
			{
				float fadeDepth = ((hit.PenetrationDepth) / (PlayerHead->GetScaledSphereRadius()));
				cam->SetManualCameraFade(fadeDepth, FadeColour, bFadeAudio);
				//cam->StartCameraFade(1.0f, 0.0f, 0.1f, FLinearColor::Black, false, false);
			}

		}
	}
}

void UOutBoundBoxComponent::OnBoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVRPlayerPawn* PlayerTest = dynamic_cast<AVRPlayerPawn*>(OtherActor);
	USphereComponent* TestPlayerHead = dynamic_cast<USphereComponent*>(OtherComp);

	if (PlayerTest && TestPlayerHead && TestPlayerHead == PlayerTest->HeadBound)
	{
		PlayerHead = TestPlayerHead;
		IsOverlapping = true;
		Player = PlayerTest;
		//GLog->Log(FString("weee"));
	}
}

void UOutBoundBoxComponent::OnBoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AVRPlayerPawn* PlayerTest = dynamic_cast<AVRPlayerPawn*>(OtherActor);
	USphereComponent* TestPlayerHead = dynamic_cast<USphereComponent*>(OtherComp);

	if (PlayerTest && TestPlayerHead && TestPlayerHead == PlayerTest->HeadBound)
	{
		IsOverlapping = false;

		// Fade in
		APlayerCameraManager* cam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (cam)
		{
			cam->SetManualCameraFade(0.0f, FLinearColor::Black, false);
		}
		//GLog->Log(FString("eeer"));
	}
}