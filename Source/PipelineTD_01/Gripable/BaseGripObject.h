// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../GrippableComponent.h"

#include "BaseGripObject.generated.h"

UCLASS()
class PIPELINETD_01_API ABaseGripObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGripObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grip)
		UGrippableComponent* GripComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grip)
		UStaticMeshComponent* Mesh;
};
