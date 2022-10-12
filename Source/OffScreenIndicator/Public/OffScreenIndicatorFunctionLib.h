#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OffScreenIndicatorFunctionLib.generated.h"


UCLASS()
class OFFSCREENINDICATOR_API UOffScreenIndicatorFunctionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Game|Player")
	static FVector2D WorldLocationToScreenPosition(const FVector WorldLocation, const float Padding,const APlayerController* PlayerController, float& Angle, bool& IsOnScreen);
};
