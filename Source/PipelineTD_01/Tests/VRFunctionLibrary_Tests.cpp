// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University

#include "VRFunctionLibrary_Tests.h"

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"

#include "../Gripable/BaseGripObject.h"
#include "../VRFunctionLibrary.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FClosestByCompVoidInputTest, "VRFunctionLibrary.GetClosestObjectByComponent.VoidInput", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FClosestByCompVoidInputTest::RunTest(FString const& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	
	TArray<FHitResult> hits;
	AActor* gotActor = UVRFunctionLibrary::GetClosestObjectByComponent(hits, ABaseGripObject::StaticClass(), FVector(0.0f, 0.0f, 0.0f));

	TestFalse("GetClosestObjectByComponent should return nothing with empty hit results.", IsValid(gotActor));

	return true;
}