//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Selectors/BaseObjectClassSelectorWidget.h"

void UBaseObjectClassSelectorWidget::SelectorCommitValue(TSoftClassPtr<UObject> NewSelectorValue)
{
	SelectedObjectClass.Broadcast(NewSelectorValue, SelectorSlot);
}