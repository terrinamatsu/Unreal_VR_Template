// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University


#include "Gripable/ObjectSnapBoxComponent.h"



UObjectSnapBoxComponent::UObjectSnapBoxComponent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryComponentTick.bCanEverTick = true;
	SetRelativeScale3D(FVector(10.0f, 10.0f, 10.0f));

	this->OnComponentBeginOverlap.AddDynamic(this, &UObjectSnapBoxComponent::OnBoundBeginOverlap);
}

void UObjectSnapBoxComponent::OnBoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Try to get the GrippableComponent of the OtherActor
	UGrippableComponent* trycast = dynamic_cast<UGrippableComponent*>(OtherActor->GetComponentByClass(UGrippableComponent::StaticClass()));

	// If it has a GrippableComponent, and there is no currently SnappedObject
	if (trycast && !SnappedObject)
	{
		SnapGripObject(trycast);
	}
}

void UObjectSnapBoxComponent::SnapGripObject(UGrippableComponent* ToSnap)
{
	SnappedObject = ToSnap->GetOwner();
	SnappedGripComponent = ToSnap;

	SnappedGripComponent->Drop();
	SnappedObject->SetActorRelativeLocation(this->GetComponentLocation());
	SnappedObject->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
}