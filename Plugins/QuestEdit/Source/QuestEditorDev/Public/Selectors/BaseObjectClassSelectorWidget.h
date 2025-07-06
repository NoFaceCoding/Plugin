//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Selectors/BaseSelectorWidget.h"
#include "BaseObjectClassSelectorWidget.generated.h"

/**
 * Base widget that contains logic for all object class payload selectors
 */
UCLASS(Abstract, Blueprintable, Meta = (DisplayName = "ObjectClassSelector"))
class QUESTEDITORDEV_API UBaseObjectClassSelectorWidget : public UBaseSelectorWidget
{
	GENERATED_BODY()
	
public:

	/**
	*	Selector value commit function.
	*	Must be called when the selected variable changes.
	*/
	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectorCommitValue(TSoftClassPtr<UObject> NewSelectorValue);

};
