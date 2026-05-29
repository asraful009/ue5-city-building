// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZMR_WalkerBaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class ZMR_API AZMR_WalkerBaseAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
    
	UFUNCTION()
	void OnPathFailed(FAIRequestID RequestID, EPathFollowingResult::Type Result);
};
