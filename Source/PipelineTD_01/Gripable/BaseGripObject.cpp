// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University


#include "BaseGripObject.h"

// Sets default values
ABaseGripObject::ABaseGripObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	GripComponent = CreateDefaultSubobject<UGrippableComponent>(TEXT("GripComponent"));

	/// set a static mesh (& maybe also look at the overlap properies)
}

// Called when the game starts or when spawned
void ABaseGripObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseGripObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

