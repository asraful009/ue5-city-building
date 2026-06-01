// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZMR_BuildMenuWidget.generated.h"

class UZMR_BuildMenuItemWidget;
class UScrollBox;
class UUserWidget;
class UDataTable;
struct FZMR_BuildUIMenuItem;

/**
 * 
 */
UCLASS()
class ZMR_API UZMR_BuildMenuWidget : public UUserWidget
{
  GENERATED_BODY()
public:
  
protected:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UDataTable> BuildingDataTable;
  
  // UI container (bind in UMG)
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UScrollBox> BuildingScrollBox;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
  TSubclassOf<UZMR_BuildMenuItemWidget> BuildingItemWidgetClass;

  virtual void NativeConstruct() override;
  virtual void NativeDestruct() override;
  
  UFUNCTION()
  void OnBuildingClicked(FName RowName);
  
private:
  UFUNCTION()
  void BuildMenu();
  
  UFUNCTION()
  void CreateButtonForItem(const FZMR_BuildUIMenuItem& Item);
  
};
