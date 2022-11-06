// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemNameWidget.h"

#include "Components/EditableText.h"

/* UI 위젯 가져오는데에 문제가 없는지 확인하는 함수, 초기 설정*/
bool UItemNameWidget::Initialize()
{
	bool bIsSuccess = Super::Initialize();

	if (!bIsSuccess)
	{
		return false;
	}

	ItemName->SetVisibility(ESlateVisibility::Hidden);
	return true;
}

/* Item의 이름을 플레이어가 가까이 다가왔을때 보여지도록 설정 */
void UItemNameWidget::ToggleVisibility()
{
	if (ItemName->GetVisibility() == ESlateVisibility::Hidden)
	{
		ItemName->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ItemName->SetVisibility(ESlateVisibility::Hidden);
	}
}

/* 아이템의 이름을 설정하도록 함. */
void UItemNameWidget::SetItemName(FString& _ItemName)
{
	ItemName->SetText(FText::FromString(_ItemName));
}