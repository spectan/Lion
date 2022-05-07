#include "incls.h"

int main() {
    DWORD dwExit = 0;
    bool isWaiting = true, bHealth = false, bAmmo = false, bNoRecoil = false, bRapidFire = false, bTrigger = false, bESP = false, bAim = false;

    while (1) {
        if(isWaiting) std::cout << "Assault cube not found. Waiting for process." << std::endl;
        
        if(!process->procHandle) { //If there is no process handle, try to create one.
            process->Attach(L"ac_client.exe"); //Create a process handle
            isWaiting = false;
        }
        while (GetExitCodeProcess(process->procHandle, &dwExit) && dwExit == STILL_ACTIVE) { //While process is running
            if (!isWaiting) std::cout << "\nCheat Activated: \nNum1 for invincibility. \nNum2 for infinite ammo. \nNum3 for no recoil/no spread. \nNum4 for rapidfire. \nNum5 for triggerbot. \nNum6 for ESP. \nNum7 for Aimbot. \n";
            isWaiting = true;
            //Infinite Health
            if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
                bHealth = !bHealth;
                std::cout << "Invincibility toggled.\n";
            }
            //Infinite Ammo
            if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
                bAmmo = !bAmmo;
                std::cout << "Infinite ammo toggled.\n";
            }
            //No Recoil
            if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
                bNoRecoil = !bNoRecoil;
                std::cout << "No recoil toggled.\n";
            }
            //Rapidfire
            if (GetAsyncKeyState(VK_NUMPAD4) & 1) {
                bRapidFire = !bRapidFire;
                std::cout << "Rapid fire toggled.\n";
            }
            //Triggerbot
            if (GetAsyncKeyState(VK_NUMPAD5) & 1) {
                bTrigger = !bTrigger;
                std::cout << "Triggerbot toggled.\n";
            }
            //ESP
            if (GetAsyncKeyState(VK_NUMPAD6) & 1) {
                bESP = !bESP;
                std::cout << "ESP toggled.\n";
            }
            //Aimbot
            if (GetAsyncKeyState(VK_NUMPAD7) & 1) {
                bAim = !bAim;
                std::cout << "Aimbot toggled.\n";
            }
        }
        process->Detach(); //If process closes, detach the previous process handle.
        Sleep(10);
    }
}
