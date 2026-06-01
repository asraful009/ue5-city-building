// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/ZMR_BuildMenuWidget.h"

#include "Building/FZMR_BuildUIMenuItem.h"

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
}
