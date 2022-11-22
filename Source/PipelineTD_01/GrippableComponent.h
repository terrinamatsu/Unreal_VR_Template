// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrippableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGripPressDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGripHoldDelegate, float, DeltaTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGripReleaseDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionPressDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionHoldDelegate, float, DeltaTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionReleaseDelegate);

UENUM(BlueprintType)
enum class EGripType : uint8
{
	InPlace,
	Snap,
	NoAttachment, // For objects that dont attach to the players hand, but use the grip event functionality (e.g. buttons)
};

/** Component to make an Actor be able to be gripped. */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PIPELINETD_01_API UGrippableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrippableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Whether or not the object is currently being gripped. */ 
	bool bIsGripped = false;
	/** Whether or not the object is currently having its action called. (Object Action button pressed) */
	bool bIsAction = false;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Grip Actions Delegates */
	UPROPERTY(BlueprintAssignable)
		FOnGripPressDelegate OnGripPress;
	UPROPERTY(BlueprintAssignable)
		FOnGripHoldDelegate OnGripHold;
	UPROPERTY(BlueprintAssignable)
		FOnGripReleaseDelegate OnGripRelease;

	/** Object Action Button Actions Delegates */
	UPROPERTY(BlueprintAssignable)
		FOnActionPressDelegate OnActionPress;
	UPROPERTY(BlueprintAssignable)
		FOnActionHoldDelegate OnActionHold;
	UPROPERTY(BlueprintAssignable)
		FOnActionReleaseDelegate OnActionRelease;

	/** 
	 * Grips the owning actor to an object. 
	 * @param GrippingComponent - The SceneComponent that this actor will be gripped to.
	*/
	UFUNCTION()
		virtual void Grip(USceneComponent* GrippingComponent);
	/** Drops the owning actor from its current gripping object, if currently gripped. */
	UFUNCTION()
		virtual void Drop();

	/** Starts the calls for the Grip Action delegates. */
	UFUNCTION()
		void GripActionPress();
	/** Ends the calls for the Grip Action delegates. */
	UFUNCTION()
		void GripActionRelease();

	/** The type of grip this object has. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Properties)
		EGripType GripType = EGripType::Snap;
	
};