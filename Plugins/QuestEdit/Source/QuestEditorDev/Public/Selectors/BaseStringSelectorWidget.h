//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Selectors/BaseSelectorWidget.h"
#include "BaseStringSelectorWidget.generated.h"

/**
 *	This is the basic widget for making a custom string selector widget for the quest editor
 */
UCLASS(Abstract, Blueprintable, Meta = (DisplayName = "StringSelector"))
class QUESTEDITORDEV_API UBaseStringSelectorWidget : public UBaseSelectorWidget
{
	GENERATED_BODY()
	
public:

	/**
	*	Selector value commit function.
	*	Must be called when the selected variable changes.
	*/
	UFUNCTION(BlueprintCallable, Category = "Selector")
		void SelectorCommitValue(FString NewSelectorValue);
};
