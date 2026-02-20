// Fill out your copyright notice in the Description page of Project Settings.


#include "Walker/ZMR_WalkerBase.h"

// Sets default values
AZMR_WalkerBase::AZMR_WalkerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZMR_WalkerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZMR_WalkerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZMR_WalkerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

