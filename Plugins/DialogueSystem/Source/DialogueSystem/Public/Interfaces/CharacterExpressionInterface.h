#pragma once

#include "UObject/Interface.h"
#include "CharacterExpressionInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UCharacterExpressionInterface : public UInterface
{
	GENERATED_BODY()
};

class ICharacterExpressionInterface
{
	GENERATED_BODY()

public:
	// Muss im implementierenden Blueprint überschrieben werden
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dialogue")
	void PlayExpression(FName ExpressionID);
};