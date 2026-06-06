// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/ZMR_BuildingBase.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AZMR_BuildingBase::AZMR_BuildingBase()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.bStartWithTickEnabled = true;

  BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
  RootComponent = BuildingMesh;
  // Optional: set collision / physics
  BuildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

  bIsPlaced = false;
  Level = 1;
}

// Called when the game starts or when spawned
void AZMR_BuildingBase::BeginPlay()
{
  Super::BeginPlay();
  TArray<AActor*> Buildings;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZMR_BuildingBase::StaticClass(), Buildings);
  int32 BuildingIdMax = 1;
  for (AActor* Actor : Buildings)
  {
    if (AZMR_BuildingBase* Building = Cast<AZMR_BuildingBase>(Actor))
    {
      if (Building->BuildingId >= BuildingIdMax)
      {
        BuildingIdMax = Building->BuildingId + 1;
      }
    }
  }
  BuildingId = BuildingIdMax;

  
  if (BuildingMesh)
  {
    if (NormalMaterial && bIsPlaced)
    {
      UE_LOG(LogTemp, Warning, TEXT("Setting Normal Material for Building ID: %i"), BuildingId);
      BuildingMesh->SetMaterial(0, NormalMaterial);
    } else if (!bIsPlaced && PlacementMaterial)
    {
      SetPreviewMode(bIsPlaced);;
    } 
  }

}

// Called every frame
void AZMR_BuildingBase::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (bDrawDebugInfo)
  {
    const FString Label = FString::Printf(
      TEXT("UID: %d\nID: %d\nName: %s"),
      GetUniqueID(),
      BuildingId,
      *GetName()
    );
    DrawDebugString(
      GetWorld(),
      GetActorLocation() + FVector(0,0,150),
       *Label,
      nullptr,
      FColor::FromHex("#A5CF83"),
      0.f,
      true
    );
  }

}

void AZMR_BuildingBase::OnPlaced()
{
  bIsPlaced = true;
  
}

void AZMR_BuildingBase::OnDestroyedBuilding()
{
}

void AZMR_BuildingBase::OnWalkerArrived(const AZMR_WalkerBase* Walker)
{
}

void AZMR_BuildingBase::UpgradeBuilding()
{
}

void AZMR_BuildingBase::SetGridPosition(FIntPoint NewGridPosition)
{
}

TArray<FIntPoint> AZMR_BuildingBase::GetOccupiedTiles() const
{
  TArray<FIntPoint> OccupiedTiles;
  for (int32 X = 0; X < FootprintSize.X; X++)
  {
    for (int32 Y = 0; Y < FootprintSize.Y; Y++)
    {
      OccupiedTiles.Add(FIntPoint(GridPosition.X + X, GridPosition.Y + Y));
    }
  }
  return OccupiedTiles;
}

void AZMR_BuildingBase::OnSelected_Implementation()
{
  IZMR_ObjectSelectInterface::OnSelected_Implementation();
  if (BuildingMesh && NormalMaterial)
  {
    BuildingMesh->SetMaterial(0, SelectedMaterial);
  }
  UE_LOG(LogTemp, Warning, TEXT("Building ID [ %i ] : Selected"), BuildingId);
}

void AZMR_BuildingBase::OnDeselected_Implementation()
{
  IZMR_ObjectSelectInterface::OnDeselected_Implementation();
  if (BuildingMesh && SelectedMaterial)
  {
    BuildingMesh->SetMaterial(0, NormalMaterial);
  }
  UE_LOG(LogTemp, Warning, TEXT("Building ID [ %i ] : DeSelected"), BuildingId);
}

void AZMR_BuildingBase::SetPreviewMode(const bool bPreview)
{ 
  bIsPlaced = !bPreview;
  
  if (bPreview)
  {
    BuildingMesh->SetCollisionEnabled(
        ECollisionEnabled::NoCollision);

    BuildingMesh->SetMaterial(0, PlacementMaterial);

    SetActorEnableCollision(false);
  }
  else
  {
    BuildingMesh->SetCollisionEnabled(
        ECollisionEnabled::QueryAndPhysics);

    BuildingMesh->SetMaterial(0, NormalMaterial);
    UE_LOG(LogTemp, Warning, TEXT("Building ID [ %i ] : Placed"), BuildingId);
    SetActorEnableCollision(true);
  }
}

