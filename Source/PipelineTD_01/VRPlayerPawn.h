// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University

#pragma once

#include "Engine.h"
#include "Core.h"
#include "GameFramework/Pawn.h"

#include "NavMesh/RecastNavMesh.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h"
#include "Runtime/HeadMountedDisplay/Public/HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "VRFunctionLibrary.h"
#include "GrippableComponent.h"
#include "ProcNavMeshComponent.h"

#include "VRPlayerPawn.generated.h"

UENUM(BlueprintType)
enum class EControllerLR : uint8
{
	Left = 0,
	Right = 1,
};

UENUM(BlueprintType)
enum class ETurnType : uint8
{
	NoRotation = 0,
	Smooth = 1,
	Snap35 = 2,
	Snap45 = 3,
};

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	NoTeleportation = 0,
	Teleportation = 1,
	DirectionTeleport = 2,
	WalkingSimulatedTeleport = 3,
};

UENUM(BlueprintType)
enum class ETeleportStyle : uint8
{
	Straight = 0,
	Arc = 1,
};

UENUM(BlueprintType)
enum class EDeviceType : uint8
{
	Desktop = 0,
	DetectedHMD = 1,
	OcculusTouch = 2,
	ValveIndex = 3, 
	HTCVive = 4,
};

/***
* Basic VR Player Pawn. 
*/
UCLASS()
class PIPELINETD_01_API AVRPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool FindNavMesh();

	// Movement Functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	// Handles horizontal camera rotation.
	void TurnAxis(float AxisValue);
	// Handles vertical camera rotation (outside of VR only).
	void TurnUp(float AxisValue);

	void Turn(float Rotation);
	bool bIsReadyToTurn = true;

	// Teleportation
	UFUNCTION(BlueprintCallable)
		void TeleportRightPressed();
	UFUNCTION(BlueprintCallable)
		void TeleportRightReleased();
	UFUNCTION(BlueprintCallable)
		void TeleportLeftPressed();
	UFUNCTION(BlueprintCallable)
		void TeleportLeftReleased();

	UFUNCTION(BlueprintCallable)
		void ExecuteTeleportation(EControllerLR _ControllerLR);

	bool bRightTeleporterActive = false;
	bool bLeftTeleporterActive = false;
	FVector TeleportLocation;

	/* Grip */
	UFUNCTION(BlueprintCallable)
		void GripRightPressed();
	UFUNCTION(BlueprintCallable)
		void GripRightReleased();

	UFUNCTION(BlueprintCallable)
		void GripLeftPressed();
	UFUNCTION(BlueprintCallable)
		void GripLeftReleased();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
		AActor* rightGrippedObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
		AActor* leftGrippedObject;

	UGrippableComponent* rightOjectGripComponent;
	UGrippableComponent* leftOjectGripComponent;

	UFUNCTION(BlueprintCallable)
		void letGo(EControllerLR _ControllerLR);

	/* Object Grip Actions */
	UFUNCTION(BlueprintCallable)
		void GripActionLeftPressed();
	UFUNCTION(BlueprintCallable)
		void GripActionLeftReleased();

	UFUNCTION(BlueprintCallable)
		void GripActionRightPressed();
	UFUNCTION(BlueprintCallable)
		void GripActionRightReleased();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Device Setup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DeviceSetup)
		EDeviceType DeviceType = EDeviceType::DetectedHMD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DeviceSetup)
		bool bRunInVR = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DeviceSetup)
		bool bUseHandMeshes = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DeviceSetup)
		UStaticMesh* OcculusController;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DeviceSetup)
		UStaticMesh* IndexController;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DeviceSetup)
		UStaticMesh* ViveController;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DeviceSetup)
		UStaticMesh* DefaultHands;


	/*  Scene Components  */

	// VR Tracking Centre
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* VRTrackingCentre;
	// Camera at Head Position
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* Head;
	// Bounding Area for head collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* HeadBound;
	// Text display in front of camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextRenderComponent* DebugOutputText;
	// Teleport UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* TeleportCylinder;
	
	// Motion Controllers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMotionControllerComponent* LeftController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMotionControllerComponent* RightController;
	// Hand Spring Attachment Meshes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* LeftHandConstraintAttach;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* RightHandConstraintAttach;
	// Hand Springs
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPhysicsConstraintComponent* LeftHandConstraint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPhysicsConstraintComponent* RightHandConstraint;
	// Hand Meshes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* LeftHand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* RightHand;
	// Hand Sphere Grab Bounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* LeftBound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* RightBound;


	/* NAVIGATION */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ARecastNavMesh* NavMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enables)
		float MaxTeleportDistance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed)
		float TranslationSpeed = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed)
		float RotationSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enables)
		EMovementType MovementType = EMovementType::Teleportation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enables)
		ETurnType TurnType = ETurnType::Smooth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enables)
		ETeleportStyle TeleportStyle = ETeleportStyle::Arc;


	/* Movement Enables & Options */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enables)
		bool bCanMove = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enables)
		bool bCanTurn = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enables)
		bool bCanTeleport = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enables)
		bool bCanGrab = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enables)
		bool bCanObjectAction = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enables)
		bool bCanTeleportAccrossGaps = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enables)
		float TeleportArcRadius = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enables)
		float TeleportArcStrength = 800.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enables)
		float TeleportArcSimTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UProcNavMeshComponent* ProcNavMesh;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "HMD")
		void DeviceSetup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enables)
		UStaticMesh* NavPointVisualiserMesh;

private:
	void TraceTeleport(EControllerLR _ControllerLR);
	void TraceTeleportArc(UMotionControllerComponent* controller, UStaticMeshComponent* hand);
	void TraceTeleportLine(UMotionControllerComponent* controller, UStaticMeshComponent* hand);

	TArray<AStaticMeshActor*> NavPointsVisualiser;
	UPROPERTY()
		UNiagaraComponent* NavigationArcVisualiser;
};
