// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University


#include "VRPlayerPawn.h"

// Sets default values
AVRPlayerPawn::AVRPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Find default meshes for device controllers & hands
	static ConstructorHelpers::FObjectFinder<UStaticMesh> OcculusControllerMesh(TEXT("StaticMesh'/Engine/VREditor/Devices/Oculus/OculusControllerMesh.OculusControllerMesh'"));
	OcculusController = OcculusControllerMesh.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> IndexControllerMesh(TEXT("StaticMesh'/Engine/VREditor/Devices/STEM/STEMControllerMesh.STEMControllerMesh'"));
	IndexController = IndexControllerMesh.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ViveControllerMesh(TEXT("StaticMesh'/Engine/VREditor/Devices/Vive/VivePreControllerMesh.VivePreControllerMesh'"));
	ViveController = ViveControllerMesh.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HandMesh(TEXT("StaticMesh'/Engine/VREditor/Devices/Oculus/OculusControllerMesh.OculusControllerMesh'"));
	DefaultHands = HandMesh.Object;
	
	// Add Components To Scene
	VRTrackingCentre = CreateDefaultSubobject<USceneComponent>(TEXT("VRTackingCentre"));
	RootComponent = VRTrackingCentre;
	
	/* HEAD : CAMERA COMPONENT */
	Head = CreateDefaultSubobject<UCameraComponent>(TEXT("Head"));
	Head->SetupAttachment(VRTrackingCentre);
	
	/* Head BOUND : SPHERE */
	HeadBound = CreateDefaultSubobject<USphereComponent>(TEXT("HeadBound"));
	HeadBound->SetupAttachment(Head);
	HeadBound->SetHiddenInGame(true);    // hidden in game
	HeadBound->SetGenerateOverlapEvents(true);    // Generate OverLap Events

	/* DEBUG OUTPUT TEXT : TEXT RENDER */
	DebugOutputText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("outputText"));
	// Set Attachment
	DebugOutputText->SetupAttachment(Head);
	// Set Text Material
	static ConstructorHelpers::FObjectFinder<UMaterial> unlitText(TEXT("Material'/Engine/EngineMaterials/DefaultTextMaterialTranslucent.DefaultTextMaterialTranslucent'"));
	DebugOutputText->SetMaterial(0, unlitText.Object);
	DebugOutputText->SetTextRenderColor(FColor::Red);
	// Set Text Alignment
	DebugOutputText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	DebugOutputText->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
	// Set Rotation & Location
	DebugOutputText->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	DebugOutputText->SetRelativeLocation(FVector(150.0f, 0.0f, 0.0f));

	/* LEFT CONTROLLER : MOTION CONTROLLER */
	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
	LeftController->SetupAttachment(VRTrackingCentre);
	// Set Motion Controller Hand
	LeftController->MotionSource = FXRMotionControllerBase::LeftHandSourceId;
	
	/* Left Hand Constraint Attachment : STATIC MESH */
	LeftHandConstraintAttach = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftHandConstraintAttach"));
	LeftHandConstraintAttach->SetupAttachment(LeftController);

	/* LEFT HAND : STATIC MESH */
	LeftHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftHandMesh"));
	// Set Attachment
	LeftHand->SetupAttachment(LeftController);
	// Set Mesh
	LeftHand->SetStaticMesh(OcculusController);
	// Set Material
	//static ConstructorHelpers::FObjectFinder<UMaterial> greymat(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	//LeftHand->SetMaterial(0, greymat.Object);
	// Set Rotation & Scale
	LeftHand->SetRelativeRotation(FRotator(0, 180, 180));
	LeftHand->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	// Set Physics & Collision
	LeftHand->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeftHand->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	/* Left Constraint : PHYSICS CONSTRAINT */
	LeftHandConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LeftHandConstraint"));
	LeftHandConstraint->SetupAttachment(LeftHandConstraintAttach);
	LeftHandConstraint->SetConstrainedComponents(LeftHandConstraintAttach, "", LeftHand, "");
	LeftHandConstraint->SetDisableCollision(true);
	LeftHandConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Limited, 10.0f);
	LeftHandConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Limited, 10.0f);
	LeftHandConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 10.0f);

	/* LEFT BOUND : SPHERE */
	LeftBound = CreateDefaultSubobject<USphereComponent>(TEXT("LeftBound"));
	LeftBound->SetupAttachment(LeftController);
	LeftBound->SetRelativeLocation(FVector(0, 0, 0));
	LeftBound->SetRelativeScale3D(FVector(1.1f, 1.1f, 1.1f));
	LeftBound->SetHiddenInGame(true);    // hidden in game
	LeftBound->SetGenerateOverlapEvents(true);    // Generate OverLap Events

	/* RIGHT CONTROLLER : MOTION CONTROLLER */
	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
	RightController->SetupAttachment(VRTrackingCentre);
	// Set Motion Controller Hand
	RightController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	
	/* Right Hand Constraint Attachment : STATIC MESH */
	RightHandConstraintAttach = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightHandConstraintAttach"));
	RightHandConstraintAttach->SetupAttachment(RightController);

	/* RIGHT HAND : STATIC MESH */
	RightHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightHandMesh"));
	RightHand->SetupAttachment(RightController);
	RightHand->SetStaticMesh(OcculusController);
	//RightHand->SetMaterial(0, greymat.Object);
	RightHand->SetRelativeRotation(FRotator(0, 180, 180));
	RightHand->SetRelativeScale3D(FVector(1.0f, -1.0f, 1.0f));
	// Set Physics & Collision
	RightHand->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightHand->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	/* RIGHT Constraint : PHYSICS CONSTRAINT */
	RightHandConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RightHandConstraint"));
	RightHandConstraint->SetupAttachment(RightHandConstraintAttach);
	RightHandConstraint->SetConstrainedComponents(RightHandConstraintAttach, "", RightHand, "");
	RightHandConstraint->SetDisableCollision(true);
	RightHandConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Limited, 10.0f);
	RightHandConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Limited, 10.0f);
	RightHandConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 10.0f);

	/* RIGHT BOUND : SPHERE */
	RightBound = CreateDefaultSubobject<USphereComponent>(TEXT("RightBound"));
	RightBound->SetupAttachment(RightController);
	RightBound->SetRelativeLocation(FVector(0, 0, 0));
	RightBound->SetRelativeScale3D(FVector(1.1f, 1.1f, 1.1f));
	RightBound->SetHiddenInGame(true);    // hidden in game
	RightBound->SetGenerateOverlapEvents(true);    // Generate OverLap Events

	/* TELEPORT CYLINDER : STATIC MESH */
	TeleportCylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportCylinder"));
	TeleportCylinder->SetupAttachment(RootComponent);
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> cylinder(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> cylinder(TEXT("StaticMesh'/Engine/VREditor/Teleport/TeleportDirectionMesh.TeleportDirectionMesh'"));
	TeleportCylinder->SetStaticMesh(cylinder.Object);
	TeleportCylinder->SetVisibility(false);
	TeleportCylinder->SetHiddenInGame(true);
	TeleportCylinder->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));
	// Set Physics & Collision
	TeleportCylinder->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TeleportCylinder->SetGenerateOverlapEvents(false);
	TeleportCylinder->CanCharacterStepUpOn = ECB_No;
	TeleportCylinder->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	/* PROC NAVMESH : PROCEDURAL MESH NAVMESH*/
	ProcNavMesh = CreateDefaultSubobject<UProcNavMeshComponent>(TEXT("ProceduralMeshNavMesh"));
	ProcNavMesh->SetVisibility(false);
	ProcNavMesh->SetHiddenInGame(true);


	// Set Player to Receive Input
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Find & Set Up NavMesh
	NavMesh = dynamic_cast<ARecastNavMesh*>(UGameplayStatics::GetActorOfClass(GetWorld(), ARecastNavMesh::StaticClass()));
	if (!FindNavMesh())
	{
		//Destroy();
	}

	// Setup NavigationArcVisualiser
	NavigationArcVisualiser = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NavigationArcVisualiser"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ArcNiagaraSystem(TEXT("NiagaraSystem'/Game/VRTemplate/VFX/NS_TeleportTrace.NS_TeleportTrace'"));
	if (ArcNiagaraSystem.Succeeded())
	{
		NavigationArcVisualiser->SetAsset(ArcNiagaraSystem.Object);
	}
}

bool AVRPlayerPawn::FindNavMesh()
{
	// Function to find a useable navmesh for the VR_Pawn. returns true if it can find one (or one is already attached), false if not.
	if (!NavMesh)
	{
		NavMesh = dynamic_cast<ARecastNavMesh*>(UGameplayStatics::GetActorOfClass(GetWorld(), ARecastNavMesh::StaticClass()));

		if (NavMesh)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}

// Called when the game starts or when spawned
void AVRPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	FindNavMesh();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

	//UHeadMountedDisplayFunctionLibrary::EnableHMD(false);
	//UHeadMountedDisplayFunctionLibrary::GetHMDWornState();
	//UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
	//UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected();

	DeviceSetup();
}

void AVRPlayerPawn::DeviceSetup()
{
	if (bRunInVR)
	{
		UHeadMountedDisplayFunctionLibrary::EnableHMD(true);
	}
	else
	{
		UHeadMountedDisplayFunctionLibrary::EnableHMD(false);
	}

	EDeviceType DeviceSelection = DeviceType;
	if (DeviceType == EDeviceType::DetectedHMD)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected() && UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			FName CurrentDeviceName = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName();
#			
			if (CurrentDeviceName == "OculusHMD")
			{
				DeviceSelection = EDeviceType::OcculusTouch;
			}
			else if (CurrentDeviceName == "SteamVR")
			{
				// Currently SteamVR is the name used for both Steam Index and HTC Vive HMDs, so both will default to steam settings. 
				DeviceSelection = EDeviceType::ValveIndex;
			}
		}
		else
		{
			// No HMD
			DeviceSelection = EDeviceType::Desktop;
		}
	}

	switch (DeviceSelection)
	{
	case EDeviceType::Desktop:
		LeftHand->bHiddenInGame = true;
		RightHand->bHiddenInGame = true;
		break;
	case EDeviceType::OcculusTouch:
		LeftHand->bHiddenInGame = false;
		RightHand->bHiddenInGame = false;
		LeftHand->SetStaticMesh(OcculusController);
		RightHand->SetStaticMesh(OcculusController);
		break;
	case EDeviceType::ValveIndex:
		LeftHand->bHiddenInGame = false;
		RightHand->bHiddenInGame = false;
		LeftHand->SetStaticMesh(IndexController);
		RightHand->SetStaticMesh(IndexController);
		break;
	case EDeviceType::HTCVive:
		LeftHand->bHiddenInGame = false;
		RightHand->bHiddenInGame = false;
		LeftHand->SetStaticMesh(ViveController);
		RightHand->SetStaticMesh(ViveController);
		break;
	default:
		LeftHand->bHiddenInGame = false;
		RightHand->bHiddenInGame = false;
		LeftHand->SetStaticMesh(OcculusController);
		RightHand->SetStaticMesh(OcculusController);
		break;
	}
}

// Called every frame
void AVRPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Teleportation */
	TraceTeleport(EControllerLR::Left);
	TraceTeleport(EControllerLR::Right);

	///* Projecting Player to NavMesh */
	//FNavLocation outnav;
	//
	//if (NavMesh->ProjectPoint(Head->GetComponentLocation(), outnav, FVector(1000, 1000, 1000)))
	//{
	//	FVector HeadLocal = FVector(Head->GetRelativeLocation().X, Head->GetRelativeLocation().Y, 0);
	//	FVector NewLocation = outnav.Location - HeadLocal;

	//	this->SetActorLocation(NewLocation);
	//}

	//DebugOutputText->SetText(FString::Printf(TEXT("Headloc: %f, %f, %f"), Head->GetComponentLocation().X, Head->GetComponentLocation().Y, Head->GetComponentLocation().Z));
}

void AVRPlayerPawn::TraceTeleport(EControllerLR _ControllerLR)
{
	bool teleporterActive = (_ControllerLR == EControllerLR::Left) ? bLeftTeleporterActive : bRightTeleporterActive;
	UMotionControllerComponent* controller = (_ControllerLR == EControllerLR::Left) ? LeftController : RightController; 
	UStaticMeshComponent* hand = (_ControllerLR == EControllerLR::Left) ? LeftHand : RightHand;

	FName StickAxisY = (_ControllerLR == EControllerLR::Left) ? TEXT("LeftThumbstickY") : TEXT("RightThumbstickY");
	FName StickAxisX = (_ControllerLR == EControllerLR::Left) ? TEXT("LeftThumbstickX") : TEXT("RightThumbstickX");


	if (teleporterActive)
	{
		switch (TeleportStyle)
		{
			case ETeleportStyle::Arc:
				TraceTeleportArc(controller, hand);
				break;
			case ETeleportStyle::Straight:
				TraceTeleportLine(controller, hand);
				break;
			default:
				break;
		}

		// Rotate the teleport target by stick direction
		FVector RotationVector = FVector(GetInputAxisValue(StickAxisY), GetInputAxisValue(StickAxisX), 0.0f);
		FRotator Rotation = RotationVector.Rotation();
		
		DebugOutputText->SetText(FString::Printf(TEXT("%f %f %f"), RotationVector.X, RotationVector.Y, RotationVector.Z));

		//TeleportCylinder->SetRelativeRotation(Rotation);
		TeleportCylinder->SetWorldRotation(FRotator(0.0f, Head->GetComponentRotation().Yaw, 0.0f) + Rotation);
	}
}

void AVRPlayerPawn::TraceTeleportLine(UMotionControllerComponent* controller, UStaticMeshComponent* hand)
{
	// Make Trace Hit Result
	FHitResult hit;

	// Set Up Trace Ignore Array
	TArray<AActor*> ignored;
	if (rightGrippedObject) ignored.Add(rightGrippedObject);
	if (leftGrippedObject) ignored.Add(leftGrippedObject);
	ignored.Add(this); // Not really necessary

	// Trace
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		controller->GetComponentLocation(),
		controller->GetComponentLocation() + (hand->GetForwardVector() * MaxTeleportDistance),
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_WorldStatic),
		false,
		ignored,
		EDrawDebugTrace::None,
		hit,
		true))
	{
		//// Show the text of the hit actor (DEBUG)
		//if (hit.GetActor())
		//{
		//	this->DebugOutputText->SetText(hit.GetActor()->GetName());
		//}

		// Testing if the hit point is navageable 
		FNavLocation outnav;
		bool projectedSuccess = NavMesh->ProjectPoint(hit.ImpactPoint, outnav, FVector(1000, 1000, 1000));

		// If can't teleport across gaps, search NavMesh for valid path to outnav point
		FPathFindingResult path;
		if (!bCanTeleportAccrossGaps)
		{
			FPathFindingQuery query(this, *NavMesh, this->GetActorLocation(), outnav.Location);
			query.SetAllowPartialPaths(false);
			path = NavMesh->FindPath(this->GetNavAgentPropertiesRef(), query);

			
		}

		// Move the Teleport Cylinder location to the new point, if valid
		if (projectedSuccess && bCanTeleportAccrossGaps || !bCanTeleportAccrossGaps && path.IsSuccessful() && !path.IsPartial())
		{
			TeleportLocation = (outnav.Location);
			TeleportCylinder->SetWorldLocation(TeleportLocation);
			TeleportCylinder->SetVisibility(true);
		}
		else
		{
			// Found point is not valid
			TeleportCylinder->SetVisibility(false);
		}
	}
	else
	{
		// Trace failed
		TeleportCylinder->SetVisibility(false);
	}
}

void AVRPlayerPawn::TraceTeleportArc(UMotionControllerComponent* controller, UStaticMeshComponent* hand)
{
	// Set Up Trace Ignore Array
	TArray<AActor*> ignored;
	if (rightGrippedObject) ignored.Add(rightGrippedObject);
	if (leftGrippedObject) ignored.Add(leftGrippedObject);
	ignored.Add(this); // Not really necessary

	FPredictProjectilePathParams PathParams = FPredictProjectilePathParams(TeleportArcRadius, controller->GetComponentLocation(), hand->GetForwardVector() * TeleportArcStrength, TeleportArcSimTime, ECollisionChannel::ECC_WorldStatic, this);
	FPredictProjectilePathResult PathResult;
	if (UGameplayStatics::PredictProjectilePath(GetWorld(), PathParams, PathResult))
	{
		// Testing if the hit point is navageable 
		FNavLocation outnav;
		bool projectedSuccess = NavMesh->ProjectPoint(PathResult.HitResult.ImpactPoint, outnav, FVector(1000, 1000, 1000));

		// If can't teleport across gaps, search NavMesh for valid path to outnav point
		FPathFindingResult path;
		if (!bCanTeleportAccrossGaps)
		{
			FPathFindingQuery query(this, *NavMesh, this->GetActorLocation(), outnav.Location);
			query.SetAllowPartialPaths(false);
			path = NavMesh->FindPath(this->GetNavAgentPropertiesRef(), query);

			// Displaying Navigation Path
			TArray<FNavPathPoint> NavigationPath = path.Path.Get()->GetPathPoints();

			for (auto& Point : NavPointsVisualiser)
			{
				Point->Destroy();
			}
			NavPointsVisualiser.Empty();

			for (auto& Point : NavigationPath)
			{
				NavPointsVisualiser.Add(GetWorld()->SpawnActor<AStaticMeshActor>());
				NavPointsVisualiser.Last()->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
				NavPointsVisualiser.Last()->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				NavPointsVisualiser.Last()->GetStaticMeshComponent()->SetStaticMesh(NavPointVisualiserMesh);
				NavPointsVisualiser.Last()->SetActorLocation(Point.Location);
			}			
		}

		// Move the Teleport Cylinder location to the new point, if valid
		if (projectedSuccess && bCanTeleportAccrossGaps || !bCanTeleportAccrossGaps && path.IsSuccessful() && !path.IsPartial())
		{
			TeleportLocation = (outnav.Location);
			TeleportCylinder->SetWorldLocation(TeleportLocation);
			TeleportCylinder->SetVisibility(true);

			// Setup the Niagara Arc Visualiser
			NavigationArcVisualiser->SetVisibility(true);

			TArray<FVector> ArcPoints;
			for (auto& arcPathPoint : PathResult.PathData)
			{
				ArcPoints.Add(arcPathPoint.Location);
			}
			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(NavigationArcVisualiser, "User.PointArray", ArcPoints);
		}
		else
		{
			// Found point is not valid
			TeleportCylinder->SetVisibility(false);
			NavigationArcVisualiser->SetVisibility(false);
		}

	}
	else
	{
		// Trace failed
		TeleportCylinder->SetVisibility(false);
		NavigationArcVisualiser->SetVisibility(false);
	}
}

// Called to bind functionality to input
void AVRPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* Bind Actions */
	// Teleportation
	PlayerInputComponent->BindAction("TeleportRight", EInputEvent::IE_Pressed, this, &AVRPlayerPawn::TeleportRightPressed);
	PlayerInputComponent->BindAction("TeleportRight", EInputEvent::IE_Released, this, &AVRPlayerPawn::TeleportRightReleased);

	PlayerInputComponent->BindAction("TeleportLeft", EInputEvent::IE_Pressed, this, &AVRPlayerPawn::TeleportLeftPressed);
	PlayerInputComponent->BindAction("TeleportLeft", EInputEvent::IE_Released, this, &AVRPlayerPawn::TeleportLeftReleased);

	// Gripping
	PlayerInputComponent->BindAction("GripRight", EInputEvent::IE_Pressed, this, &AVRPlayerPawn::GripRightPressed);
	PlayerInputComponent->BindAction("GripRight", EInputEvent::IE_Released, this, &AVRPlayerPawn::GripRightReleased);

	PlayerInputComponent->BindAction("GripLeft", EInputEvent::IE_Pressed, this, &AVRPlayerPawn::GripLeftPressed);
	PlayerInputComponent->BindAction("GripLeft", EInputEvent::IE_Released, this, &AVRPlayerPawn::GripLeftReleased);

	// Grip Actions
	PlayerInputComponent->BindAction("ActionRight", EInputEvent::IE_Pressed, this, &AVRPlayerPawn::GripActionRightPressed);
	PlayerInputComponent->BindAction("ActionRight", EInputEvent::IE_Released, this, &AVRPlayerPawn::GripActionRightReleased);

	PlayerInputComponent->BindAction("ActionLeft", EInputEvent::IE_Pressed, this, &AVRPlayerPawn::GripActionLeftPressed);
	PlayerInputComponent->BindAction("ActionLeft", EInputEvent::IE_Released, this, &AVRPlayerPawn::GripActionLeftReleased);

	/* Bind Axis */
	// Movement
	PlayerInputComponent->BindAxis("LeftThumbstickX", this, &AVRPlayerPawn::MoveRight);
	PlayerInputComponent->BindAxis("LeftThumbstickY", this, &AVRPlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis("RightThumbstickX", this, &AVRPlayerPawn::TurnAxis);
	PlayerInputComponent->BindAxis("RightThumbstickY", this, &AVRPlayerPawn::TurnUp);
}

void AVRPlayerPawn::MoveForward(float AxisValue)
{
	if (AxisValue != 0.0f && bCanMove && !(bRightTeleporterActive || bLeftTeleporterActive))
	{
		// add movement in that direction if can move and not teleporting

		// Check for NavMesh
		if (!FindNavMesh())
		{
			Destroy();
		}

		// Navmesh Projection Method
		FNavLocation outnav;
		FVector NormalisedHeadVector = FVector(Head->GetForwardVector().X, Head->GetForwardVector().Y, 0.0f);
		NormalisedHeadVector.Normalize();

		if (NavMesh->ProjectPoint(GetActorLocation() + (NormalisedHeadVector * AxisValue * TranslationSpeed), outnav, FVector(1000, 1000, 1000)))
		{
			this->SetActorLocation(outnav.Location);
		}

		/*
		FVector Movement = (NormalisedHeadVector * AxisValue * TranslationSpeed);
		FVector MoveLocation = Head->GetComponentLocation() + Movement;


		if (NavMesh->ProjectPoint(MoveLocation, outnav, FVector(1000, 1000, 1000)))
		{
			FVector HeadLocal = FVector(Head->GetRelativeLocation().X, Head->GetRelativeLocation().Y, 0);
			FVector NewLocation = outnav.Location - HeadLocal;

			DebugOutputText->SetText(FString::Printf(TEXT("Foreward: %f, %f, %f:: %f, %f"), Head->GetForwardVector().X, Head->GetForwardVector().Y, Head->GetForwardVector().Z, AxisValue, TranslationSpeed));
			this->SetActorLocation(NewLocation);
		}*/
	}
}

void AVRPlayerPawn::MoveRight(float AxisValue)
{
	if (AxisValue != 0.0f && bCanMove && !(bRightTeleporterActive || bLeftTeleporterActive))
	{
		// add movement in that direction

		// Check for NavMesh
		if (!FindNavMesh())
		{
			Destroy();
		}

		// Navmesh Projection Method
		FNavLocation outnav;

		if (NavMesh->ProjectPoint(GetActorLocation() + (Head->GetRightVector() * AxisValue * TranslationSpeed), outnav, FVector(1000, 1000, 1000)))
		{
			this->SetActorLocation(outnav.Location);
		}

		//FVector Movement = (Head->GetRightVector() * AxisValue * TranslationSpeed);
		//FVector MoveLocation = Head->GetComponentLocation() + Movement; 

		////DebugOutputText->SetText(FString::Printf(TEXT("Right: %f, %f, %f:: %f, %f"), Head->GetRightVector().X, Head->GetRightVector().Y, Head->GetRightVector().Z, AxisValue, TranslationSpeed));

		//if (NavMesh->ProjectPoint(MoveLocation, outnav, FVector(1000, 1000, 1000)))
		//{
		//	this->SetActorLocation(outnav.Location - FVector(Head->GetRelativeLocation().X, Head->GetRelativeLocation().Y, 0));
		//}
	}
}

void AVRPlayerPawn::TurnAxis(float AxisValue)
{
	if (bCanTurn && !(bRightTeleporterActive || bLeftTeleporterActive))
	{
		switch (TurnType)
		{
		case ETurnType::NoRotation:
			break;
		case ETurnType::Smooth:
			Turn(AxisValue * RotationSpeed);
			break;
		case ETurnType::Snap35:
			// Turn 35 degrees, times the sign of the axisvalue, if axis is more than deadzone
			if (abs(AxisValue) > 0.2f)
			{
				if (bIsReadyToTurn)
				{
					Turn(35 * ((AxisValue < 0) ? -1 : 1));
					bIsReadyToTurn = false;
				}
			}
			else
			{
				bIsReadyToTurn = true;
			}
			break;
		case ETurnType::Snap45:
			// Turn 45 degrees, times the sign of the axisvalue, if axis is more than deadzone
			if (abs(AxisValue) > 0.2f)
			{
				if (bIsReadyToTurn)
				{
					Turn(45 * ((AxisValue < 0) ? -1 : 1));
					bIsReadyToTurn = false;
				}
			}
			else
			{
				bIsReadyToTurn = true;
			}
			break;
		default:
			break;
		}
	}
}

void AVRPlayerPawn::Turn(float Rotation)
{
	/// The following section is a direct translation of blueprints in the current Unreal Engine 4.27 VR template, from the VRPawn blueprint, SnapTurn function.
	/// Epic Games (2021). 

	FRotator BaseRotation = FRotator(0, Rotation, 0);

	FVector LocalCamLocation = Head->GetComponentLocation();
	FTransform LocalCamTransform = Head->GetRelativeTransform();

	FTransform NewTransform = FTransform(UKismetMathLibrary::ComposeRotators(GetActorRotation(), BaseRotation), GetActorLocation(), FVector(1, 1, 1));

	this->AddActorWorldRotation(BaseRotation);
	this->SetActorLocation(LocalCamLocation - (LocalCamTransform * NewTransform).GetLocation() + GetActorLocation());

	/// end of Citation
}

void AVRPlayerPawn::TurnUp(float AxisValue)
{
	if (bRunInVR)
	{
		// Currently disabled, only should update when not in VR, but I don't know how to check for this yet.
		if ((Head->GetRelativeRotation().Pitch + AxisValue * RotationSpeed) < -90)
		{
			Head->SetRelativeRotation(FRotator(-90, Head->GetRelativeRotation().Yaw, Head->GetRelativeRotation().Roll));
		}
		else if ((Head->GetRelativeRotation().Pitch + AxisValue * RotationSpeed) > 90)
		{
			Head->SetRelativeRotation(FRotator(90, Head->GetRelativeRotation().Yaw, Head->GetRelativeRotation().Roll));
		}
		else
		{
			Head->AddLocalRotation(FRotator(AxisValue * RotationSpeed, 0, 0));
		}
	}
	
}

void AVRPlayerPawn::TeleportRightPressed()
{
	if (bCanTeleport)
	{
		bRightTeleporterActive = true;
		bLeftTeleporterActive = false;

		TeleportCylinder->SetVisibility(true);

		if (ProcNavMesh)
		{
			ProcNavMesh->SetHiddenInGame(false);
		}
	}
}

void AVRPlayerPawn::TeleportRightReleased()
{
	// If Teleporter is active
	// Execute Teleportation

	if (bCanTeleport)
	{
		ExecuteTeleportation(EControllerLR::Right);

		if (ProcNavMesh && !bLeftTeleporterActive)
		{
			ProcNavMesh->SetHiddenInGame(true);
		}
	}
}

void AVRPlayerPawn::TeleportLeftPressed()
{
	if (bCanTeleport)
	{
		bLeftTeleporterActive = true;
		bRightTeleporterActive = false;

		TeleportCylinder->SetVisibility(true);

		if (ProcNavMesh)
		{
			ProcNavMesh->SetHiddenInGame(false);
		}
	}
}

void AVRPlayerPawn::TeleportLeftReleased()
{
	// If Teleporter is active
	// Execute Teleportation

	if (bCanTeleport)
	{
		ExecuteTeleportation(EControllerLR::Left);

		if (ProcNavMesh && !bRightTeleporterActive)
		{
			ProcNavMesh->SetHiddenInGame(true);
		}
	}
}

void AVRPlayerPawn::ExecuteTeleportation(EControllerLR _ControllerLR)
{
	// Check if the TeleportCylinder is not visible, and therefore not on a valid location
	if (!TeleportCylinder->GetVisibleFlag())
	{
		// Disable Teleporters
		bLeftTeleporterActive = false;
		bRightTeleporterActive = false;

		return;
	}

	// If A Teleporter is Active
	if ((_ControllerLR == EControllerLR::Left) ? bLeftTeleporterActive : bRightTeleporterActive)
	{
		// Disable Teleporters
		bLeftTeleporterActive = false;
		bRightTeleporterActive = false;

		// Rotation
		
		SetActorRotation(TeleportCylinder->GetComponentRotation());
		

		// Moving Teleport Location Relative to Headset Location (from tracking centre)
		FRotator Orientation;
		FVector Position;
		//UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(Orientation, Position);
		Position = Head->GetRelativeLocation();
		Orientation = Head->GetRelativeRotation();
		TeleportLocation -= FVector(Position.X, Position.Y, 0.0f);

		GLog->Log(FString::Printf(TEXT("Teleporting: %d, %d"), Position.X, Position.Y));

		// Fade Out
		APlayerCameraManager* cam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (cam)
		{
			cam->StartCameraFade(1.0f, 0.0f, 0.1f, FLinearColor::Black, false, false);
		}

		// Teleport
		this->SetActorLocation(TeleportLocation);
		TeleportCylinder->SetVisibility(false);
	}
}

void AVRPlayerPawn::GripRightPressed()
{
	if (bCanGrab)
	{
		// Get Overlapping Actors
		TArray<AActor*> outOverlap;
		RightBound->GetOverlappingActors(outOverlap);

		if (outOverlap.Num() > 0)
		{
			// Get Closest of the Collectible Class
			UGrippableComponent* trycast = dynamic_cast<UGrippableComponent*>(UVRFunctionLibrary::GetClosestComponent(outOverlap, UGrippableComponent::StaticClass(), RightController->GetComponentLocation()));

			if (trycast)
			{
				//this->outputText->SetText(trycast->GetName());

				trycast->Grip(RightHandConstraintAttach);
				//trycast->AttachToComponent(RightController, FAttachmentTransformRules::KeepWorldTransform);

				rightGrippedObject = trycast->GetOwner();
				rightOjectGripComponent = trycast;

				if (trycast == leftOjectGripComponent || trycast->GetOwner() == leftGrippedObject)
				{
					leftGrippedObject = nullptr;
					leftOjectGripComponent = nullptr;
				}
			}
		}
	}
}

void AVRPlayerPawn::GripRightReleased()
{
	letGo(EControllerLR::Right);
}

void AVRPlayerPawn::GripLeftPressed()
{
	if (bCanGrab)
	{
		// Get Overlapping Actors
		TArray<AActor*> outOverlap;
		LeftBound->GetOverlappingActors(outOverlap);

		if (outOverlap.Num() > 0)
		{
			// Get Closest of the Collectible Class
			UGrippableComponent* trycast = dynamic_cast<UGrippableComponent*>(UVRFunctionLibrary::GetClosestComponent(outOverlap, UGrippableComponent::StaticClass(), LeftController->GetComponentLocation()));

			if (trycast)
			{
				//this->outputText->SetText(trycast->GetName());

				trycast->Grip(LeftHandConstraintAttach);
				//trycast->AttachToComponent(RightController, FAttachmentTransformRules::KeepWorldTransform);

				leftGrippedObject = trycast->GetOwner();
				leftOjectGripComponent = trycast;

				if (trycast == rightOjectGripComponent || trycast->GetOwner() == rightGrippedObject)
				{
					rightGrippedObject = nullptr;
					rightOjectGripComponent = nullptr;
				}
			}
		}
	}
}

void AVRPlayerPawn::GripLeftReleased()
{
	letGo(EControllerLR::Left);
}

void AVRPlayerPawn::letGo(EControllerLR _ControllerLR)
{
	// Set Controller
	AActor* GrippedObject = (_ControllerLR == EControllerLR::Left) ? leftGrippedObject : rightGrippedObject;
	UGrippableComponent* GripComponent = (_ControllerLR == EControllerLR::Left) ? leftOjectGripComponent : rightOjectGripComponent;
	UMotionControllerComponent* ControllerComp = (_ControllerLR == EControllerLR::Left) ? LeftController : RightController;

	//If Object is valid
	if (GrippedObject && GripComponent)
	{
		// If Object's parent is the controller
		if (GrippedObject->GetRootComponent()->GetAttachParent() == ControllerComp)
		{
			GripComponent->Drop();
		}
	}

	//Empty Gripped Object pointers
	if ((_ControllerLR == EControllerLR::Left))
	{
		leftGrippedObject = nullptr;
		leftOjectGripComponent = nullptr;
	}
	else
	{
		rightGrippedObject = nullptr;
		rightOjectGripComponent = nullptr;
	}
}

void AVRPlayerPawn::GripActionLeftPressed()
{
	if (bCanObjectAction && leftOjectGripComponent)
	{
		leftOjectGripComponent->GripActionPress();
	}
}

void AVRPlayerPawn::GripActionLeftReleased()
{
	if (bCanObjectAction && leftOjectGripComponent)
	{
		leftOjectGripComponent->GripActionRelease();
	}
}


void AVRPlayerPawn::GripActionRightPressed()
{
	if (bCanObjectAction && rightOjectGripComponent)
	{
		rightOjectGripComponent->GripActionPress();
	}
}

void AVRPlayerPawn::GripActionRightReleased()
{
	if (bCanObjectAction && rightOjectGripComponent)
	{
		rightOjectGripComponent->GripActionRelease();
	}
}