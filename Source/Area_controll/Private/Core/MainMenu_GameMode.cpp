// Copyright (c) Konstantin Pozdeev. All rights reserved.


#include "Core/MainMenu_GameMode.h"
#include "HUD/MainMenu.h"
#include "DeviceProfiles/DeviceProfileManager.h"
#include "DeviceProfiles/DeviceProfile.h"
#include "GameFramework/GameUserSettings.h"




void AMainMenu_GameMode::SetSettings(int Type)
{
    UGameUserSettings* Settings = GEngine->GetGameUserSettings();

    FString PlatformName = FPlatformProperties::PlatformName();
    if (PlatformName.Contains(TEXT("Android")))
    {
        UDeviceProfile* TargetProfile;

        FString LowName = "Android_Low";
        FString HighName = "Android_Mid";

        switch (Type)
        {
        case 0:
            TargetProfile = UDeviceProfileManager::Get().FindProfile(LowName);
            break;

        case 2:
            TargetProfile = UDeviceProfileManager::Get().FindProfile(HighName);
            break;

        default:
            return;
        }
        if (TargetProfile && GEngine)
        {
            UDeviceProfileManager::Get().SetOverrideDeviceProfile(TargetProfile);
        }
    }

    switch (Type)
    {
    case 0:
        Settings->SetResolutionScaleValueEx(85.0f);
        GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("r.DefaultFeature.Bloom 0"));
        break;

    case 2:
        Settings->SetResolutionScaleValueEx(100.0f);
        GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("r.DefaultFeature.Bloom 1"));
        break;

    default: return;
    }

    Settings->SetViewDistanceQuality(Type);
    Settings->SetAntiAliasingQuality(Type);
    Settings->SetPostProcessingQuality(Type);
    Settings->SetTextureQuality(Type);
    Settings->SetVisualEffectQuality(FMath::Max(0, Type - 1));
    Settings->SetShadingQuality(Type);

    Settings->SetShadowQuality(0);
    Settings->SetReflectionQuality(0);
    Settings->SetFoliageQuality(0);
    Settings->SetGlobalIlluminationQuality(0);
    Settings->SetFrameRateLimit(60.0f);

    GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("t.MaxFPS 60"));

    Settings->ApplySettings(false);
    Settings->SaveSettings();
}




void AMainMenu_GameMode::BeginPlay()
{
	Super::BeginPlay();

    //GameWidget create
    if (MainMenuClass)
    {
        if (!IsValid(MainMenu))
        {
            MainMenu = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
        }
        if (IsValid(MainMenu))
        {
            MainMenu->AddToViewport();
        }
    }
}
