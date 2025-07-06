//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Selectors/BaseSelectorWidget.h"
#include "BaseTagSelectorWidget.generated.h"

/**
 *  Base widget that contains logic for all tag payload selectors
 */
UCLASS(Abstract, Blueprintable, Meta = (DisplayName = "TagSelector"))
class QUESTEDITORDEV_API UBaseTagSelectorWidget : public UBaseSelectorWidget
{
	GENERATED_BODY()
	

public:

	/**
	*	Selector value commit function.
	*	Must be called when the selected variable changes.
	*/
	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectorCommitValue(FGameplayTagContainer NewSelectorValue);
};
