//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Selectors/BaseTagSelectorWidget.h"

void UBaseTagSelectorWidget::SelectorCommitValue(FGameplayTagContainer NewSelectorValue)
{
	SelectedTag.Broadcast(NewSelectorValue, SelectorSlot);
}
