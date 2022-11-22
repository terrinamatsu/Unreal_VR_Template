// By Jacob Worgan, for MSc CAVE Pipeline & TD Assignment 2022 @ Bournemouth University


#include "ProcNavMeshComponent.h"

// Sets default values
UProcNavMeshComponent::UProcNavMeshComponent(const FObjectInitializer &ObjectInitialiser) : Super(ObjectInitialiser)
{
    // Set to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryComponentTick.bCanEverTick = true;

    // Set Physics & Collision
    this->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    this->SetGenerateOverlapEvents(false);
    this->CanCharacterStepUpOn = ECB_No;
    this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    static ConstructorHelpers::FObjectFinder<UMaterial> NavMat(TEXT("Material'/Game/VRTemplate/Materials/M_VRCursor.M_VRCursor'"));
    this->SetMaterial(0, NavMat.Object);

    NavMesh = dynamic_cast<ARecastNavMesh*>(UGameplayStatics::GetActorOfClass(GetWorld(), ARecastNavMesh::StaticClass()));
}

// Called every frame
void UProcNavMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (this->IsVisible())
    {
        if (!NavMesh)
        {
            NavMesh = dynamic_cast<ARecastNavMesh*>(UGameplayStatics::GetActorOfClass(GetWorld(), ARecastNavMesh::StaticClass()));
        }
        this->SetWorldLocation(FVector(0, 0, 0));
        this->SetWorldRotation(FRotator(0, 0, 0));
        GenerateNavMesh();
    }
}

void UProcNavMeshComponent::GenerateNavMesh()
{
    Vertices.Reset();
    Triangles.Reset();
    Normals.Reset();
    Tangents.Reset();
    UVs.Reset();
    Colours.Reset();

    int32 TriangleIndexCount = 0;
    FVector DefinedShape[8];
    FProcMeshTangent TangentSetup;



    // TESTING FOR RENDERING THE NAVMESH
    FBox b = FBox(this->GetComponentLocation() - FVector(-1000, -1000, -1000), this->GetComponentLocation() - FVector(1000, 1000, 1000)); // (later could adapt this so it gets a box centred on the player)
    TArray<FNavPoly> pol;

    if (NavMesh)
    {
        NavMesh->GetPolysInBox(b, pol);

        for (auto& poly : pol)
        {
            FOccluderVertexArray verts;
            NavMesh->GetPolyVerts(poly.Ref, verts);
            if (verts.Num() >= 3)
            {
                //GLog->Log(FString::Printf(TEXT("Nav Verts: %f, %f, %f, num : %d"), verts[0].X, verts[0].Y, verts[0].Z, verts.Num()));

                for (auto i = 0; i < (verts.Num() - 2); ++i)
                {
                    FVector v = FVector(verts[0] - verts[i + 1]);
                    AddTriangleMesh(verts[0], verts[i + 1], verts[i + 2], TriangleIndexCount, FProcMeshTangent(v.X, v.Y, v.Z));
                }
            }
        }

        this->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colours, Tangents, true);
    }
}

void UProcNavMeshComponent::AddTriangleMesh(FVector TopRight, FVector BottomRight, FVector BottomLeft, int32& TriIndex, FProcMeshTangent Tangent)
{
    int32 Point1 = TriIndex++;
    int32 Point2 = TriIndex++;
    int32 Point3 = TriIndex++;

    Vertices.Add(TopRight);
    Vertices.Add(BottomRight);
    Vertices.Add(BottomLeft);

    Triangles.Add(Point1);
    Triangles.Add(Point2);
    Triangles.Add(Point3);

    FVector ThisNorm = FVector::CrossProduct(BottomRight, BottomLeft).GetSafeNormal();
    if (ThisNorm.Z < 0) ThisNorm = -ThisNorm;
    for (int i = 0; i < 3; i++)
    {
        Normals.Add(ThisNorm);
        Tangents.Add(Tangent);
        Colours.Add(FLinearColor::Green);
    }

    UVs.Add(FVector2D(0.0f, 1.0f)); //Top Right
    UVs.Add(FVector2D(0.0f, 0.0f)); //Bottom Right
    UVs.Add(FVector2D(1.0f, 0.0f)); //Bottom Left
}