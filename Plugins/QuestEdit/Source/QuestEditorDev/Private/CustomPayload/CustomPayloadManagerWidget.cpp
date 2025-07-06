//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "CustomPayload/CustomPayloadManagerWidget.h"

#include "CustomPayload/CustomPayloadSelectorWidget.h"


void UCustomPayloadManagerWidget::HandleWidgetSelectorsStart(TArray<UWidget*> SelectorWidgets)
{
	for (int i = 0; i < SelectorWidgets.Num(); i++)
	{
		UCustomPayloadSelectorWidget* BaseSelector = Cast <UCustomPayloadSelectorWidget>(SelectorWidgets[i]);
		if (BaseSelector)
		{
			if (!BaseSelector->OnStructDataCommited.IsAlreadyBound(this, &UCustomPayloadManagerWidget::SelectedDataCallback))
			{
				BaseSelector->OnStructDataCommited.AddDynamic(this, &UCustomPayloadManagerWidget::SelectedDataCallback);
			}
		}
	}
}

void UCustomPayloadManagerWidget::SelectedDataCallback(FString NewValueIn, int SlotIn)
{
	//Dispatch event for data changed in one of the custom selectors
	CustomPayloadDataChanged.Broadcast(NewValueIn, SlotIn);
}
