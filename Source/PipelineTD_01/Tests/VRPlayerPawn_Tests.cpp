// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University

#include "VRPlayerPawn_Tests.h"

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"

#include "../Gripable/BaseGripObject.h"

#include "../VRPlayerPawn.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVRPlayerCtorTest, "VRPlayerPawn.Ctor", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FVRPlayerCtorTest::RunTest(FString const& Parameters)
{
	// UE_LOG(Error) to fail the test
	// UE_LOG(Warning) to pass the test with a warning

	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	AVRPlayerPawn* Player = World->SpawnActor<AVRPlayerPawn>();

	TestTrue("VRPlayer should be able to Move by default.", Player->bCanMove);
	TestTrue("VRPlayer should be able to Turn by default.", Player->bCanTurn);
	TestTrue("VRPlayer should be able to Teleport by default.", Player->bCanTeleport);
	TestTrue("VRPlayer should be able to Grab by default.", Player->bCanGrab);
	TestTrue("VRPlayer should be able to perform Object Actions by default.", Player->bCanObjectAction);
	//TestTrue("VRPlayer should be able to Teleport Accross Gaps by default.", Player->bCanTeleportAccrossGaps);

	TestEqual("VRPlayer should default to smooth turning", Player->TurnType, ETurnType::Smooth);
	TestEqual("VRPlayer should default to teleportation movement type", Player->MovementType, EMovementType::Teleportation);

	TestEqual("VRPlayer should default to rotation speed of 1.0f", Player->RotationSpeed, 1.0f);
	TestEqual("VRPlayer should default to translation (movement) speed of 2.0f", Player->TranslationSpeed, 2.0f);
	TestEqual("VRPlayer should default to max teleportation distance of 1000.0f", Player->MaxTeleportDistance, 1000.0f);
	
	// Components Added to Actor
	TestTrue("VRPlayer should create default USceneComponent for VRTrackingCentre", IsValid(Player->VRTrackingCentre));
	TestTrue("VRPlayer should create default UCameraComponent for Head", IsValid(Player->Head));
	TestTrue("VRPlayer should create default UTextRenderComponent for DebugOutputText", IsValid(Player->DebugOutputText));
	TestTrue("VRPlayer should create default UMotionControllerComponent for LeftController", IsValid(Player->LeftController));
	TestTrue("VRPlayer should create default UMotionControllerComponent for RightController", IsValid(Player->RightController));
	TestTrue("VRPlayer should create default UStaticMeshComponent for LeftHand", IsValid(Player->LeftHand));
	TestTrue("VRPlayer should create default UStaticMeshComponent for RightHand", IsValid(Player->RightHand));
	TestTrue("VRPlayer should create default USphereComponent for LeftBound", IsValid(Player->LeftBound));
	TestTrue("VRPlayer should create default USphereComponent for RightBound", IsValid(Player->RightBound));
	TestTrue("VRPlayer should create default UStaticMeshComponent for TeleportCylinder", IsValid(Player->TeleportCylinder));
	TestTrue("VRPlayer should create default UProcNavMeshComponent for ProcNavMesh", IsValid(Player->ProcNavMesh));

	// Default Attachments Set
	TestEqual("VRPlayer should have VRTrackingCentre as root component", Player->GetRootComponent(), Player->VRTrackingCentre);
	TestEqual("VRPlayer should have Head attached to VRTrackingCentre", Player->Head->GetAttachParent(), Player->VRTrackingCentre);
	TestTrue("VRPlayer should have DebugOutputText attached to Head", Player->DebugOutputText->GetAttachParent() == Player->Head);
	
	TestEqual("VRPlayer should have LeftController attached to VRTrackingCentre", Player->LeftController->GetAttachParent(), Player->VRTrackingCentre);
	TestTrue("VRPlayer should have LeftHand attached to LeftController", Player->LeftHand->GetAttachParent() == Player->LeftController);
	TestTrue("VRPlayer should have LeftBound attached to LeftController", Player->LeftBound->GetAttachParent() == Player->LeftController);
	
	TestEqual("VRPlayer should have RightController attached to VRTrackingCentre", Player->RightController->GetAttachParent(), Player->VRTrackingCentre);
	TestTrue("VRPlayer should have RightHand attached to RightController", Player->RightHand->GetAttachParent() == Player->RightController);
	TestTrue("VRPlayer should have RightBound attached to RightController", Player->RightBound->GetAttachParent() == Player->RightController);
	
	TestEqual("VRPlayer should have TeleportCylinder attached to VRTrackingCentre", Player->TeleportCylinder->GetAttachParent(), Player->VRTrackingCentre);

	// Default Meshes & Materials Set
	TestTrue("VRPlayer should have default TeleportCylinder visibility as False", Player->TeleportCylinder->bHiddenInGame);
	TestTrue("VRPlayer should have default ProcNavMesh visibility as False", Player->ProcNavMesh->bHiddenInGame);

	// Default Component Scales & Locations Set
	TestEqual("VRPlayer should have LeftBound default scale of (1.1f, 1.1f, 1.1f)", Player->LeftBound->GetRelativeScale3D(), FVector(1.1f, 1.1f, 1.1f));
	TestEqual("VRPlayer should have LeftBound default (relative) location of (0,0,0)", Player->LeftBound->GetRelativeLocation(), FVector(0,0,0));
	
	TestEqual("VRPlayer should have RightBound default scale of (1.1f, 1.1f, 1.1f)", Player->RightBound->GetRelativeScale3D(), FVector(1.1f, 1.1f, 1.1f));
	TestEqual("VRPlayer should have RightBound default (relative) location of (0,0,0)", Player->RightBound->GetRelativeLocation(), FVector(0, 0, 0));

	// Default Collision for Components Set
	
	// Defaults for Device Setup
	TestEqual("VRPlayer should have default DeviceType be DetectedHDM.", Player->DeviceType, EDeviceType::DetectedHMD);
	TestEqual("VRPlayer should have UseHandMeshes default to false.", Player->bUseHandMeshes, false);

	UStaticMesh* OcculusControllerMesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/VREditor/Devices/Oculus/OculusControllerMesh.OculusControllerMesh'"), NULL);
	TestEqual("VRPlayer should have default Occulus hand mesh be Occulus controller Mesh.", Player->OcculusController, OcculusControllerMesh);

	UStaticMesh* IndexControllerMesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/VREditor/Devices/STEM/STEMControllerMesh.STEMControllerMesh'"), NULL);
	TestEqual("VRPlayer should have default Index hand mesh be Index controller Mesh.", Player->IndexController, IndexControllerMesh);

	UStaticMesh* ViveControllerMesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/VREditor/Devices/Vive/VivePreControllerMesh.VivePreControllerMesh'"), NULL);
	TestEqual("VRPlayer should have default Vive hand mesh be Vive controller Mesh.", Player->ViveController, ViveControllerMesh);

	UStaticMesh* DefaultHandsMesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/VREditor/Devices/Oculus/OculusControllerMesh.OculusControllerMesh'"), NULL);
	TestEqual("VRPlayer should have default Hand mesh be Unreal Hand Mesh.", Player->DefaultHands, DefaultHandsMesh);



	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVRPlayerGripTest, "VRPlayerPawn.Grip", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FVRPlayerGripTest::RunTest(FString const& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	AVRPlayerPawn* Player = World->SpawnActor<AVRPlayerPawn>();
	ABaseGripObject* GripObject = World->SpawnActor<ABaseGripObject>();

	/*AGameModeBase* gamemode;
	World->SetGameMode();
	Player->controller
	GripObject->SetActorLocation(FVector(10, 10, 10));*/

	APlayerController* controller = UGameplayStatics::GetPlayerController(World, 0);

	if (controller)
	{
		controller->Possess(Player);
		controller->InputKey(EKeys::OculusTouch_Left_Grip_Click, EInputEvent::IE_Pressed, 0.0f, true);
	}


	TestEqual("VRPlayer gripping with left hand should move the object to the hand.", GripObject->GetActorLocation(), Player->LeftHand->GetComponentLocation());


	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVRPlayerDeviceBasedTest, "VRPlayerPawn.DeviceSetup", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FVRPlayerDeviceBasedTest::RunTest(FString const& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	AVRPlayerPawn* Player = World->SpawnActor<AVRPlayerPawn>();


	Player->DeviceType = EDeviceType::HTCVive;
	// Initiate Begin Play Setup (which includes device setup). 
	Player->DispatchBeginPlay();
	TestTrue("Device setup with HTC Vive device type should set hand meshes to visible.", Player->LeftHand->IsVisible() && Player->RightHand->IsVisible());
	TestEqual("Device setup with HTC Vive device type should set left hand mesh to vive controller mesh.", Player->ViveController, Player->LeftHand->GetStaticMesh());

	Player->DeviceType = EDeviceType::OcculusTouch;
	Player->DeviceSetup();
	TestTrue("Device setup with Occulus device type should set hand meshes to visible.", Player->LeftHand->IsVisible() && Player->RightHand->IsVisible());
	TestEqual("Device setup with Occulus device type should set left hand mesh to occulus controller mesh.", Player->OcculusController, Player->LeftHand->GetStaticMesh());

	Player->DeviceType = EDeviceType::ValveIndex;
	Player->DeviceSetup();
	TestTrue("Device setup with Valve Index device type should set hand meshes to visible.", Player->LeftHand->IsVisible() && Player->RightHand->IsVisible());
	TestEqual("Device setup with Valve Index device type should set left hand mesh to Index controller mesh.", Player->IndexController, Player->LeftHand->GetStaticMesh());

	Player->DeviceType = EDeviceType::Desktop;
	Player->DeviceSetup();
	TestFalse("Device setup with Desktop device type should hide the left controller.", Player->LeftHand->IsVisible());
	TestFalse("Device setup with Desktop device type should hide the right controller.", Player->RightHand->IsVisible());

	Player->DeviceType = EDeviceType::DetectedHMD;
	// Tests relative on current setup, which is unknown at point of testing. 

	return true;
}

//IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVRPlayer Test, "VRPlayerPawn.", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
//
//bool FVRPlayer Test::RunTest(FString const& Parameters)
//{
//
//	return true;
//}
//
