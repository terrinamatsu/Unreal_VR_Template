// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University


#include "GrippableComponent.h"

// Sets default values for this component's properties
UGrippableComponent::UGrippableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrippableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UGrippableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsGripped)
	{
		if (OnGripHold.IsBound())
		{
			OnGripHold.Broadcast(DeltaTime);
		}
	}

	if (bIsAction)
	{
		if (OnActionHold.IsBound())
		{
			OnActionHold.Broadcast(DeltaTime);
		}
	}
}

void UGrippableComponent::Grip(USceneComponent* GrippingComponent)
{
	FAttachmentTransformRules TransformRules = FAttachmentTransformRules::KeepWorldTransform;
	TransformRules.bWeldSimulatedBodies = true;

	switch (GripType)
	{
	case EGripType::Snap:
		GetOwner()->SetActorLocation(GrippingComponent->GetComponentLocation());
		GetOwner()->GetRootComponent()->AttachToComponent(GrippingComponent, TransformRules);
		//GetOwner()->AttachToComponent(GrippingComponent, TransformRules);
		break;
	case EGripType::InPlace:
		GetOwner()->GetRootComponent()->AttachToComponent(GrippingComponent, TransformRules);
		//GetOwner()->AttachToComponent(GrippingComponent, TransformRules);
		break;
	case EGripType::NoAttachment:
		
		break;

	default:
		break;
	}

	GLog->Log(FString("Gripped"));
	if (OnGripPress.IsBound())
	{
		GLog->Log(FString("Grip Broadcasted"));
		OnGripPress.Broadcast();
	}

	bIsGripped = true;

	// Release the grip action, in case object is already gripped and doing an action. 
	if(bIsAction)
	{
		GripActionRelease();
	}
}

void UGrippableComponent::Drop()
{
	FDetachmentTransformRules DetatchRules = FDetachmentTransformRules::KeepWorldTransform;
	//DetatchRules.bCallModify = true;

	UStaticMeshComponent* mesh = dynamic_cast<UStaticMeshComponent*>(GetOwner()->GetRootComponent());

	// Detatch
	switch (GripType)
	{
	case EGripType::InPlace:
		if (mesh)
		{
			GLog->Log("DETACH MESH");
			mesh->DetachFromComponent(DetatchRules);
			mesh->DetachFromParent(true);
		}
		GetOwner()->GetRootComponent()->DetachFromComponent(DetatchRules);
		//GetOwner()->DetachFromActor(DetatchRules);
		break;
	case EGripType::Snap:
		if (mesh)
		{
			GLog->Log("DETACH MESH");
			mesh->DetachFromComponent(DetatchRules);
			mesh->DetachFromParent(true);
		}
		GetOwner()->GetRootComponent()->DetachFromComponent(DetatchRules);
		//GetOwner()->DetachFromActor(DetatchRules);
		break;

	case EGripType::NoAttachment:

		break;

	default:
		break;
	}

	GLog->Log(FString("Grip Released"));
	// Call Grip Release Event
	if (OnGripRelease.IsBound())
	{
		GLog->Log(FString("Grip Release Broadcasted"));
		OnGripRelease.Broadcast();
	}
	bIsGripped = false;

	// Stop Object Actions & Call Release Event if Action is ON
	if (bIsAction)
	{
		bIsAction = false;
		GripActionRelease();
	}
}

void UGrippableComponent::GripActionPress()
{
	if (OnActionPress.IsBound())
	{
		OnActionPress.Broadcast();
	}

	bIsAction = true;
}

void UGrippableComponent::GripActionRelease()
{
	if (OnActionRelease.IsBound())
	{
		OnActionRelease.Broadcast();
	}

	bIsAction = false;
}