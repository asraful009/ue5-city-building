// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/ZMR_BuildingBase.h"

// Sets default values
AZMR_BuildingBase::AZMR_BuildingBase()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = false; // buildings don't need Tick usually

  BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
  RootComponent = BuildingMesh;

  bIsPlaced = false;
  Level = 1;
}

// Called when the game starts or when spawned
void AZMR_BuildingBase::BeginPlay()
{
  Super::BeginPlay();
}

// Called every frame
void AZMR_BuildingBase::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void AZMR_BuildingBase::OnPlaced()
{
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

FIntPoint AZMR_BuildingBase::GetGridPosition() const
{
  return FIntPoint::ZeroValue;
}
