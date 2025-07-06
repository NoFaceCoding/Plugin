//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Selectors/BaseFloatSelectorWidget.h"

void UBaseFloatSelectorWidget::SelectorCommitValue(float NewSelectorValue)
{
	SelectedFloat.Broadcast(NewSelectorValue, SelectorSlot);
}