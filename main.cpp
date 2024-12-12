#include "Includes.h"



int main()
{
    DWORD ProccessID, baseModule, LocalPlayerOffset, RifleAmmoOffset, HealthOffset, CurrentPistolAmmo, RifleReloadAnimationOffset;


 
    ProccessID = GetProcessID(L"ac_client.exe");

    baseModule = GetModuleBaseAddress(ProccessID, L"ac_client.exe");

    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, NULL, ProccessID);
    ReadProcessMemory(handle, (LPCVOID)(baseModule + 0x17E0A8), &LocalPlayerOffset, sizeof(LocalPlayerOffset), nullptr);
    
    //Stores the current for repeating
    int CurrentHealth;
    int CurrentRifleAmmo;
    int RifleReloadAnimation;


    //offsets
    RifleAmmoOffset = LocalPlayerOffset + 0x140;
    HealthOffset = LocalPlayerOffset + 0xEC;
    CurrentPistolAmmo = LocalPlayerOffset + 0x12c;
    RifleReloadAnimationOffset = LocalPlayerOffset + 0x78;

    //Cheated Variables
    int CheatedHealth = 100;
    int CheatedRifleAmmo = 20;
    int RemoveRifleReloadAnimation = 0;
    while (true)
    {
        ReadProcessMemory(handle, (LPCVOID)(HealthOffset), &CurrentHealth, sizeof(CurrentHealth), nullptr);
        ReadProcessMemory(handle, (LPCVOID)(RifleAmmoOffset), &CurrentRifleAmmo, sizeof(CurrentRifleAmmo), nullptr);
        ReadProcessMemory(handle, (LPCVOID)(RifleReloadAnimationOffset), &RifleReloadAnimation, sizeof(RifleReloadAnimation), nullptr);

        std::cout << RifleReloadAnimation << std::endl;
        if (RifleReloadAnimation > 0)
        {   
            std::cout << "Reloading" << std::endl;
            WriteProcessMemory(handle, (LPVOID)(RifleReloadAnimationOffset), &RemoveRifleReloadAnimation, sizeof(RemoveRifleReloadAnimation), nullptr);
        }
        if (CurrentRifleAmmo < 10)
        {
            WriteProcessMemory(handle, (LPVOID)(RifleAmmoOffset), &CheatedRifleAmmo, sizeof(CheatedRifleAmmo), nullptr);
        }
        if (CurrentHealth < 100)
        {
            WriteProcessMemory(handle, (LPVOID)(HealthOffset), &CheatedHealth, sizeof(CheatedHealth), nullptr);
        }
    }

    CloseHandle(handle); // Clean up
    return 0;
}
