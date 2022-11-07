// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemNameWidget.h"

#include "Components/EditableText.h"

/* UI ���� �������µ��� ������ ������ Ȯ���ϴ� �Լ�, �ʱ� ����*/
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

/* Item�� �̸��� �÷��̾ ������ �ٰ������� ���������� ���� */
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

/* �������� �̸��� �����ϵ��� ��. */
void UItemNameWidget::SetItemName(FString& _ItemName)
{
	ItemName->SetText(FText::FromString(_ItemName));
}