// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/ZMR_BuildMenuItemWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UZMR_BuildMenuItemWidget::SetItemData(const FZMR_BuildUIMenuItem& Item)
{

  CachedItem = Item;

  if (!CachedItem.BuildingName.IsEmpty())
  {
    BuildNameText->SetText(Item.BuildingName);
  }

  if (CachedItem.Icon)
  {
    BuildIconImage->SetBrushFromTexture(Item.Icon);
  }
}

void UZMR_BuildMenuItemWidget::NativeConstruct()
{
  Super::NativeConstruct();
  if (BuildButton)
  {
    BuildButton->OnClicked.AddDynamic(
      this, &UZMR_BuildMenuItemWidget::HandleBuildButtonClickEvent);
  }
}

void UZMR_BuildMenuItemWidget::NativeDestruct()
{
  if (BuildButton)
  {
    BuildButton->OnClicked.RemoveAll(this);
  }
  Super::NativeDestruct();
}

void UZMR_BuildMenuItemWidget::HandleBuildButtonClickEvent()
{
  int32 ID = CachedItem.BuildingID;

  UE_LOG(LogTemp, Warning, TEXT("BuildingID clicked: %d"), ID);
}
