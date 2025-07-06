//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Selectors/BaseSelectorWidget.h"
#include "BaseVectorSelectorWidget.generated.h"

/**
 * Base widget that contains logic for all vector payload selectors
 */
UCLASS(Abstract, Blueprintable, Meta = (DisplayName = "VectorSelector"))
class QUESTEDITORDEV_API UBaseVectorSelectorWidget : public UBaseSelectorWidget
{
	GENERATED_BODY()
	

public:

	/**
	*	Selector value commit function.
	*	Must be called when the selected variable changes.
	*/
	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectorCommitValue(FVector NewSelectorValue);
};
