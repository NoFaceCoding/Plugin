//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Selectors/BaseStringSelectorWidget.h"

void UBaseStringSelectorWidget::SelectorCommitValue(FString NewSelectorValue)
{
	SelectedString.Broadcast(NewSelectorValue, SelectorSlot);
}