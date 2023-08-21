// https://gamedevelopment.tutsplus.com/positioning-on-screen-indicators-to-point-to-off-screen-targets--gamedev-6644t

#include "OffScreenIndicatorFunctionLib.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"

FVector2D UOffScreenIndicatorFunctionLib::WorldLocationToScreenPosition(const FVector WorldLocation,
	const float Padding, const APlayerController* PlayerController, float& Angle, bool& IsOnScreen)
{
	IsOnScreen = false;
	Angle = 0.f;
	if(!PlayerController) return {0.f, 0.f};
	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(PlayerController->GetWorld());
	const FVector2D ViewportCenter = ViewportSize/2.f;

	FVector2D ScreenPosition;
	PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenPosition);
	
	if(ScreenPosition.X > 0.01f && ScreenPosition.Y > 0.01f && ScreenPosition.X <= ViewportSize.X && ScreenPosition.Y <= ViewportSize.Y)
	{
		IsOnScreen = true;
		Angle = UKismetMathLibrary::DegAtan2(ScreenPosition.Y - ViewportCenter.Y, ScreenPosition.X - ViewportCenter.X) + 90.f;
		return ScreenPosition;
	}
	
	const auto CameraManager = PlayerController->PlayerCameraManager;
	if (!CameraManager) return {0.f, 0.f};

	const auto CameraLocation = CameraManager->GetCameraLocation();
	const auto VectorLocation = CameraLocation + CameraManager->K2_GetActorRotation().Vector() * 100.f + UKismetMathLibrary::GetDirectionUnitVector(CameraLocation, WorldLocation);
	PlayerController->ProjectWorldLocationToScreen(VectorLocation, ScreenPosition);
	
	const FVector2D PaddingViewportSize = ViewportSize-Padding;
	ScreenPosition -= ViewportCenter;
	const float Slope = ScreenPosition.Y/ScreenPosition.X;
	
	if(ScreenPosition.Y<0) ScreenPosition = FVector2D{-PaddingViewportSize.Y/2.f/Slope, -PaddingViewportSize.Y/2.f};
	else ScreenPosition = FVector2D{PaddingViewportSize.Y/2.f/Slope, PaddingViewportSize.Y/2.f};

	if(ScreenPosition.X<-PaddingViewportSize.X/2.f) ScreenPosition = FVector2D{-PaddingViewportSize.X/2.f, Slope*(-PaddingViewportSize.X/2.f)};
	else if(ScreenPosition.X>PaddingViewportSize.X/2.f) ScreenPosition = FVector2D{PaddingViewportSize.X/2.f, Slope*(PaddingViewportSize.X/2.f)};
	
	Angle = UKismetMathLibrary::DegAtan2(ScreenPosition.Y, ScreenPosition.X) + 90.f;
	
	return ScreenPosition+ViewportCenter;
}
