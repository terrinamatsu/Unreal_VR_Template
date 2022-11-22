// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University


#include "GrippableComponent_Tests.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"

#include "../Gripable/BaseGripObject.h"
#include "../VRPlayerPawn.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGripCtorTest, "GrippableComponent.Ctor", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FGripCtorTest::RunTest(FString const& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();

	//AVRPlayerPawn* Player = World->SpawnActor<AVRPlayerPawn>();
	ABaseGripObject* GripObject = World->SpawnActor<ABaseGripObject>();
	UGrippableComponent* GripComponent = GripObject->GripComponent;
	UStaticMeshComponent* GripObjectMesh = GripObject->Mesh;

	TestTrue("GripComponent must be created for default BaseGripObject.", IsValid(GripComponent));
	TestTrue("UStaticMeshComponent must be created for default Mesh component.", IsValid(GripObjectMesh));
	TestEqual("GripComponent must have default GripType as Snap grip.", GripComponent->GripType, EGripType::Snap);


	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGripGrippingSnapTest, "GrippableComponent.SnapGrip", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FGripGrippingSnapTest::RunTest(FString const& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	ABaseGripObject* GripObject = World->SpawnActor<ABaseGripObject>();
	UGrippableComponent* GripComponent = GripObject->GripComponent;

	FVector initialLocation = FVector(100, 100, 100);
	GripObject->SetActorLocation(initialLocation);
	GripComponent->GripType = EGripType::Snap;

	// Spawn VRPlayerPawn and grip object to left hand
	AVRPlayerPawn* TestPlayer = World->SpawnActor<AVRPlayerPawn>();
	GripComponent->Grip(TestPlayer->LeftHand);

	TestEqual("Gripping with GripComponent of grip type ::Snap should move the object to the new parent.", GripObject->GetActorLocation(), TestPlayer->LeftHand->GetComponentLocation());

	// Move the player & hand to test the grip object moves with it
	FVector newLocation = FVector(-100, -100, -100);
	TestPlayer->SetActorLocation(newLocation);

	TestEqual("Test object has moved.", TestPlayer->LeftHand->GetComponentLocation(), newLocation);
	TestEqual("GripObject should move with the player when gripped.", GripObject->GetActorLocation(), newLocation);

	// Move the player's hand to test the grip object moves with the individual component too. 
	FVector firstLocation = FVector(300, -300, 300);
	TestPlayer->LeftHand->SetWorldLocation(firstLocation);

	TestEqual("GripObject should move with the player hand when moved, from it's relative position.", GripObject->GetActorLocation(), firstLocation);

	// Ungrip the GripObject and test it no longer moves with the hand. 
	GripComponent->Drop();

	TestEqual(TEXT("GripObject should not move when dropped."), GripObject->GetActorLocation(), firstLocation);
	
	FVector secondLocation = FVector(200, 200, 200);
	TestPlayer->SetActorLocation(secondLocation);
	TestEqual(TEXT("GripObject should not move with the player when dropped."), GripObject->GetActorLocation(), firstLocation);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGripGrippingInPlaceTest, "GrippableComponent.InPlaceGrip", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FGripGrippingInPlaceTest::RunTest(FString const& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	ABaseGripObject* GripObject = World->SpawnActor<ABaseGripObject>();
	UGrippableComponent* GripComponent = GripObject->GripComponent;

	FVector initialLocation = FVector(100, 100, 100);
	GripObject->SetActorLocation(initialLocation);
	GripComponent->GripType = EGripType::InPlace;

	// Spawn VRPlayerPawn and grip object to left hand
	AVRPlayerPawn* TestPlayer = World->SpawnActor<AVRPlayerPawn>();
	GripComponent->Grip(TestPlayer->LeftHand);

	TestNotEqual("Gripping with GripComponent of grip type ::InPlace should not move the object to the new parent.", GripObject->GetActorLocation(), TestPlayer->LeftHand->GetComponentLocation());

	// Move the player & hand to test the grip object moves with it
	FVector newLocation = FVector(-100, -100, -100);
	TestPlayer->SetActorLocation(newLocation);

	TestEqual("Test object has moved.", TestPlayer->LeftHand->GetComponentLocation(), newLocation);
	TestEqual("GripObject should move with the player when gripped, from it's relative location.", GripObject->GetActorLocation(), newLocation + initialLocation);

	// Move the player's hand to test the grip object moves with the individual component too. 
	FVector firstLocation = FVector(300, -300, 300);
	TestPlayer->LeftHand->SetWorldLocation(firstLocation);

	TestEqual("GripObject should move with the player hand when moved, from it's relative position.", GripObject->GetActorLocation(), firstLocation + initialLocation);

	// Ungrip the GripObject and test it no longer moves with the hand. 
	GripComponent->Drop();

	TestEqual(TEXT("GripObject should not move when dropped."), GripObject->GetActorLocation(), firstLocation + initialLocation);

	FVector secondLocation = FVector(200, 200, 200);
	TestPlayer->SetActorLocation(secondLocation);
	TestEqual(TEXT("GripObject should not move with the player when dropped."), GripObject->GetActorLocation(), firstLocation + initialLocation);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGripGrippingNoAttachmentTest, "GrippableComponent.NoAttachmentGrip", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FGripGrippingNoAttachmentTest::RunTest(FString const& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	ABaseGripObject* GripObject = World->SpawnActor<ABaseGripObject>();
	UGrippableComponent* GripComponent = GripObject->GripComponent;

	FVector initialLocation = FVector(100, 100, 100);
	GripObject->SetActorLocation(initialLocation);
	GripComponent->GripType = EGripType::NoAttachment;

	// Spawn VRPlayerPawn and grip object to left hand
	AVRPlayerPawn* TestPlayer = World->SpawnActor<AVRPlayerPawn>();
	GripComponent->Grip(TestPlayer->LeftHand);

	TestNotEqual("Gripping with GripComponent of grip type ::NoAttachment should not move the object to the new parent.", GripObject->GetActorLocation(), TestPlayer->LeftHand->GetComponentLocation());

	// Move the player & hand to test the grip object doesn't move with it
	FVector newLocation = FVector(-100, -100, -100);
	TestPlayer->SetActorLocation(newLocation);

	TestEqual("Player hand has moved.", TestPlayer->LeftHand->GetComponentLocation(), newLocation);
	TestNotEqual("GripObject not should move with the player when gripped.", GripObject->GetActorLocation(), newLocation);

	// Move the player's hand to test the grip object doesn't move with the individual component too. 
	FVector firstLocation = FVector(300, -300, 300);
	TestPlayer->LeftHand->SetWorldLocation(firstLocation);

	TestEqual("GripObject shouldn't move with the player hand when moved, from it's relative position.", GripObject->GetActorLocation(), initialLocation);

	// Ungrip the GripObject and test it still doesn't move with the hand. 
	GripComponent->Drop();

	TestEqual(TEXT("GripObject should not move when dropped."), GripObject->GetActorLocation(), initialLocation);

	FVector secondLocation = FVector(200, 200, 200);
	TestPlayer->SetActorLocation(secondLocation);
	TestEqual(TEXT("GripObject should not move with the player when dropped."), GripObject->GetActorLocation(), initialLocation);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGripGrippedObjectTest, "GrippableComponent.GripGrippedObject", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FGripGrippedObjectTest::RunTest(FString const& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	ABaseGripObject* GripObject = World->SpawnActor<ABaseGripObject>();
	UGrippableComponent* GripComponent = GripObject->GripComponent;

	FVector initialLocation = FVector(100, 100, 100);
	GripObject->SetActorLocation(initialLocation);

	// Spawn VRPlayerPawn and grip object to left hand
	AVRPlayerPawn* TestPlayer = World->SpawnActor<AVRPlayerPawn>();

	FVector leftHandLocation = FVector(-30, -30, -30);
	TestPlayer->LeftHand->SetWorldLocation(leftHandLocation);
	FVector rightHandLocation = FVector(40, 40, 40);
	TestPlayer->RightHand->SetWorldLocation(rightHandLocation);

	GripComponent->Grip(TestPlayer->LeftHand);

	// Grip object to right hand, should no longer be gripped to left hand.
	GripComponent->Grip(TestPlayer->RightHand);

	TestEqual(TEXT("GripObject should move from gripping LeftHand to RightHand when RightHand tries to grip."), GripObject->GetActorLocation(), rightHandLocation);

	FVector leftHandSecondLocation = FVector(20, 20, 20);
	TestPlayer->LeftHand->SetWorldLocation(leftHandSecondLocation);
	TestEqual(TEXT("GripObject should no longer move with old hand after gripped to another object before it is dropped."), GripObject->GetActorLocation(), rightHandLocation);


	return true;
}
