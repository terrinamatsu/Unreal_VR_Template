
// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University

#pragma once

#include "Engine.h"
#include "Core.h"
#include "ProceduralMeshComponent.h"

#include "NavMesh/RecastNavMesh.h"

#include "ProcNavMeshComponent.generated.h"

/**
 * A Procedural Mesh Component that creates a mesh from the current level's Nav Mesh. 
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class PIPELINETD_01_API UProcNavMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
	
public: 
	// Sets default values for this component's properties
	UProcNavMeshComponent(const FObjectInitializer& ObjectInitialiser);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Reference to the RecastNavMesh in the current level to be drawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
		ARecastNavMesh* NavMesh;

protected:
	/** Generates the Mesh from the current NavMesh. */
	void GenerateNavMesh();

private:
	TArray<FVector>			Vertices;
	TArray<int32>			Triangles;
	TArray<FVector>			Normals;
	TArray<FProcMeshTangent>Tangents;
	TArray<FVector2D>		UVs;
	TArray<FLinearColor>	Colours;


	void AddTriangleMesh(FVector TopRight, FVector BottomRight, FVector BottomLeft, int32& TriIndex, FProcMeshTangent Tangent);
};
