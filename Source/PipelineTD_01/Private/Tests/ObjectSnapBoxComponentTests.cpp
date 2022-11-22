// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University


#include "Tests/ObjectSnapBoxComponentTests.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"

#include "../../Gripable/BaseGripObject.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FObjectSnapBoxComponentCtorTest, "Components.ObjectSnapBoxComponent.Constructor", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FObjectSnapBoxComponentCtorTest::RunTest(FString const& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	AActor* BasicActor = World->SpawnActor<AActor>();

	// Spawn & Get ObjectSnapBoxComponent from the BasicActor
	BasicActor->AddComponentByClass(UObjectSnapBoxComponent::StaticClass(), true, FTransform(), false);
	UObjectSnapBoxComponent* TestSnapper = dynamic_cast<UObjectSnapBoxComponent*>(BasicActor->GetComponentByClass(UObjectSnapBoxComponent::StaticClass()));

	if (TestSnapper)
	{
		TestEqual(TEXT("Default ESnapMode for ObjectSnapBoxComponent should be ::OnDrop"), TestSnapper->SnapMode, ESnapMode::OnDrop);
		TestTrue(TEXT("Default SnappedObject should be nullptr."), TestSnapper->SnappedObject == nullptr);
		TestTrue(TEXT("Default SnappedGripComponent should be nullptr."), TestSnapper->SnappedGripComponent == nullptr);
		TestTrue(TEXT("Default bSnapOrientation should be true."), TestSnapper->bSnapOrientation);
		TestEqual(TEXT("Default size should be (10.0f, 10.0f, 10.0f)"), TestSnapper->GetComponentScale(), FVector(10.0f, 10.0f, 10.0f));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Actor either didn't spawn or return the ObjectSnapBoxComponent. "));
		return false;
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FObjectSnapBoxComponentSnapTest, "Components.ObjectSnapBoxComponent.SnapObject", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FObjectSnapBoxComponentSnapTest::RunTest(FString const& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	AActor* BasicActor = World->SpawnActor<AActor>();
	ABaseGripObject* GrippableObject = World->SpawnActor<ABaseGripObject>();

	GrippableObject->SetActorLocation(FVector(100.0f, 100.0f, 100.0f));

	// Spawn & Get ObjectSnapBoxComponent from the BasicActor
	BasicActor->AddComponentByClass(UObjectSnapBoxComponent::StaticClass(), true, FTransform(), false);
	UObjectSnapBoxComponent* TestSnapper = dynamic_cast<UObjectSnapBoxComponent*>(BasicActor->GetComponentByClass(UObjectSnapBoxComponent::StaticClass()));
	TestSnapper->SnapMode = ESnapMode::OnEnter;

	if (TestSnapper)
	{
		GrippableObject->SetActorLocation(FVector(10.0f, 10.0f, 10.0f));
		TestEqual(TEXT("Grippable Object entering Box while SnapMode is ::OnEnter should move the object to the Box's location."), GrippableObject->GetActorLocation(), TestSnapper->GetComponentLocation());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Actor either didn't spawn or return the ObjectSnapBoxComponent. "));
		return false;
	}

	return true;
}