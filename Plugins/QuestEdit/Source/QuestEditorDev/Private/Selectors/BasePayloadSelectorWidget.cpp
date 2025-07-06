//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Selectors/BasePayloadSelectorWidget.h"

#include "Selectors/BaseSelectorWidget.h"


UBasePayloadSelectorWidget::UBasePayloadSelectorWidget() : Super()
{
}

void UBasePayloadSelectorWidget::HandleWidgetSelectorsStart(TArray<UWidget*> SelectorWidgets)
{
	for (int i = 0; i < SelectorWidgets.Num(); i++)
	{
		UBaseSelectorWidget* BaseSelector = Cast <UBaseSelectorWidget>(SelectorWidgets[i]);
		if (BaseSelector)
		{
			if (!BaseSelector->SelectedFloat.IsAlreadyBound(this, &UBasePayloadSelectorWidget::SelectedFloatCallback))
			{
				BaseSelector->SelectedFloat.AddDynamic(this, &UBasePayloadSelectorWidget::SelectedFloatCallback);
			}

			if (!BaseSelector->SelectedString.IsAlreadyBound(this, &UBasePayloadSelectorWidget::SelectedStringCallback))
			{
				BaseSelector->SelectedString.AddDynamic(this, &UBasePayloadSelectorWidget::SelectedStringCallback);
			}

			if (!BaseSelector->SelectedObjectClass.IsAlreadyBound(this, &UBasePayloadSelectorWidget::SelectedObjectClassCallback))
			{
				BaseSelector->SelectedObjectClass.AddDynamic(this, &UBasePayloadSelectorWidget::SelectedObjectClassCallback);
			}

			if (!BaseSelector->SelectedVector.IsAlreadyBound(this, &UBasePayloadSelectorWidget::SelectedVectorCallback))
			{
				BaseSelector->SelectedVector.AddDynamic(this, &UBasePayloadSelectorWidget::SelectedVectorCallback);
			}

			if (!BaseSelector->SelectedTag.IsAlreadyBound(this, &UBasePayloadSelectorWidget::SelectedTagCallback))
			{
				BaseSelector->SelectedTag.AddDynamic(this, &UBasePayloadSelectorWidget::SelectedTagCallback);
			}			
		}	
	}
}

void UBasePayloadSelectorWidget::SelectedFloatCallback(float NewValueIn, int SlotIn)
{
	switch (SlotIn)
	{
	case 0:
	{
		AmountChanged.Broadcast(NewValueIn);
	}
	break;
	case 1:
	{
		TimesTocheckChanged.Broadcast(NewValueIn);
	}
	break;
	default:
		break;
	}
}

void UBasePayloadSelectorWidget::SelectedStringCallback(FString NewValueIn, int SlotIn)
{
	switch (SlotIn)
	{
	case 0:
	{
		StringAChanged.Broadcast(NewValueIn);
	}
	break;
	case 1:
	{
		StringBChanged.Broadcast(NewValueIn);
	}
	break;
	default:
		break;
	}
}

void UBasePayloadSelectorWidget::SelectedObjectClassCallback(TSoftClassPtr<UObject> NewValueIn, int SlotIn)
{

	ObjectClassChanged.Broadcast(NewValueIn);	
}

void UBasePayloadSelectorWidget::SelectedVectorCallback(FVector NewValueIn, int SlotIn)
{
	VectorChanged.Broadcast(NewValueIn);
}

void UBasePayloadSelectorWidget::SelectedTagCallback(FGameplayTagContainer NewValueIn, int SlotIn)
{
	switch (SlotIn)
	{
	case 0:
	{
		RequiredTagsChanged.Broadcast(NewValueIn);
	}
	break;
	case 1:
	{
		IgnoredTagsChanged.Broadcast(NewValueIn);
	}
	break;
	default:
		break;
	}
}
