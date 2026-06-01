// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/ZMR_BuildMenuWidget.h"

#include "Building/FZMR_BuildUIMenuItem.h"
#include "Building/ZMR_BuildMenuItemWidget.h"
#include "Components/ScrollBox.h"

void UZMR_BuildMenuWidget::NativeConstruct()
{
  Super::NativeConstruct();
  BuildMenu();
}

void UZMR_BuildMenuWidget::NativeDestruct()
{
  Super::NativeDestruct();
}

void UZMR_BuildMenuWidget::OnBuildingClicked(FName RowName)
{
}

void UZMR_BuildMenuWidget::BuildMenu()
{
  UE_LOG(LogTemp, Warning, TEXT("Building Build Menu"));
  if (!BuildingDataTable)
  {
    return;
  }
  TArray<FZMR_BuildUIMenuItem*> Rows;
  BuildingDataTable->GetAllRows(TEXT("ZMR_BuildMenuWidget"), Rows);
  if (Rows.IsEmpty())
  {
    return;
  }
  for (const FZMR_BuildUIMenuItem* Row : Rows)
  {
    if (!Row)
    {
      continue;
    }
    UE_LOG(LogTemp, Warning, TEXT("Row: %s"), *Row->BuildingName.ToString());
    CreateButtonForItem(*Row);
  }
  Rows.Empty();
}

void UZMR_BuildMenuWidget::CreateButtonForItem(const FZMR_BuildUIMenuItem& Item)
{
  UE_LOG(LogTemp, Warning, TEXT("Creating button for: %s"), *Item.BuildingName.ToString());
  if (!BuildingScrollBox)
  {
    return;
  }
  if (!BuildingItemWidgetClass)
  {
    UE_LOG(LogTemp, Error, TEXT("BuildButtonClass is NULL! Assign a button subclass template in the editor."));
    return;
  }
  
  // 1. Create a live instance of your button widget template
  UZMR_BuildMenuItemWidget* NewButton = CreateWidget<UZMR_BuildMenuItemWidget>(this, BuildingItemWidgetClass);
  if (!NewButton)
  {
    return;
  }

  // 2. Initialize the button with your Data Table row struct config data 
  // (Ensure your child button widget has a method to consume this data, e.g., setting its text, icons, and click metadata)
  NewButton->SetItemData(Item);

  // 3. Append the physical widget into your UI layout hierarchy
  BuildingScrollBox->AddChild(NewButton);
  
}
