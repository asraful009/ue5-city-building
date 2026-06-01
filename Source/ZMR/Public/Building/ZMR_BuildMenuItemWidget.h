// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FZMR_BuildUIMenuItem.h"
#include "Blueprint/UserWidget.h"
#include "ZMR_BuildMenuItemWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class ZMR_API UZMR_BuildMenuItemWidget : public UUserWidget
{
  GENERATED_BODY()
public:
  void SetItemData(const FZMR_BuildUIMenuItem& Item);

protected:
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UButton> BuildButton;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> BuildNameText;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UImage> BuildIconImage;
  
private:
  UPROPERTY()
  FName RowName;
  UPROPERTY()
  FZMR_BuildUIMenuItem CachedItem;
  
protected:
  virtual void NativeConstruct() override;
  virtual void NativeDestruct() override;
  
private:
  UFUNCTION()
  void HandleBuildButtonClickEvent();
};
