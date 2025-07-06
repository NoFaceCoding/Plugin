//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Selectors/BaseVectorSelectorWidget.h"

void UBaseVectorSelectorWidget::SelectorCommitValue(FVector NewSelectorValue)
{
	SelectedVector.Broadcast(NewSelectorValue, SelectorSlot);
}
