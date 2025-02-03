#include <iostream>
#include <windows.h>
#include <Xinput.h>

#pragma comment(lib, "xinput.lib")


HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Convert stick value to percentage
float StickPercentage(SHORT value) {
    return (value / 32767.0f) * 100.0f; // Normalize to percentage
}

// Moves console cursor to top-left
void SetCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Sets vibration for controller
void SetVibration(WORD leftMotor, WORD rightMotor) {
    XINPUT_VIBRATION vibration;
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
    vibration.wLeftMotorSpeed = leftMotor;
    vibration.wRightMotorSpeed = rightMotor;
    XInputSetState(0, &vibration);
}

// Display ASCII header
void DisplayHeader() {
    SetConsoleTextAttribute(hConsole, 5);
    std::cout << R"( 
 ________  ___  _________  ________  ___  ___  ________    _______  ___  __       
|\_____  \|\  \|\___   ___\\   __  \|\  \|\  \|\   ____\  /  ___  \|\  \|\  \     
 \|___/  /\ \  \|___ \  \_\ \  \|\  \ \  \\\  \ \  \___|_/__/|_/  /\ \  \/  /|_   
     /  / /\ \  \   \ \  \ \ \   _  _\ \  \\\  \ \_____  \__|//  / /\ \   ___  \  
    /  /_/__\ \  \   \ \  \ \ \  \\  \\ \  \\\  \|____|\  \  /  /_/__\ \  \\ \  \ 
   |\________\ \__\   \ \__\ \ \__\\ _\\ \_______\____\_\  \|\________\ \__\\ \__\
    \|_______|\|__|    \|__|  \|__|\|__|\|_______|\_________\\|_______|\|__| \|__|
                                                 \|_________|                     
                                                                                  
    )" << "\n";
}

int main() {

    SetConsoleTitleA("zitrus2k's Controller Tester");

    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    // Hide cursor for cleaner look
    CONSOLE_CURSOR_INFO cursorInfo;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    DisplayHeader();
    std::cout << "Controller Tester - Press 'END' to exit\n\n";

    WORD leftMotorStrength = 0;
    WORD rightMotorStrength = 0;

    while (true) {
        SetCursorPosition(0, 10);

        if (XInputGetState(0, &state) == ERROR_SUCCESS) {
            SetConsoleTextAttribute(hConsole, 5);
            std::cout << "Buttons:  ";
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) std::cout << "[A] ";
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) std::cout << "[B] ";
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) std::cout << "[X] ";
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) std::cout << "[Y] ";
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) std::cout << "[D-Pad Up] ";
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) std::cout << "[D-Pad Down] ";
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) std::cout << "[D-Pad Left] ";
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) std::cout << "[D-Pad Right] ";
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) std::cout << "[LB] ";
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) std::cout << "[RB] ";
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) std::cout << "[Start] ";
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) std::cout << "[Back] ";
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) std::cout << "[L-Stick Click] ";
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) std::cout << "[R-Stick Click] ";

            std::cout << "\n\n";

            std::cout << "Left Stick:  X = " << StickPercentage(state.Gamepad.sThumbLX)
                << "%, Y = " << StickPercentage(state.Gamepad.sThumbLY) << "%  \n";
            std::cout << "Right Stick: X = " << StickPercentage(state.Gamepad.sThumbRX)
                << "%, Y = " << StickPercentage(state.Gamepad.sThumbRY) << "%  \n";


            std::cout << "Left Trigger:  " << (int)state.Gamepad.bLeftTrigger << "/255   \n";
            std::cout << "Right Trigger: " << (int)state.Gamepad.bRightTrigger << "/255   \n";
            SetConsoleTextAttribute(hConsole, 13);

            std::cout << "\n[Vibration Tester]\n";
            std::cout << "  1 - Increase Low Motor (Current: " << leftMotorStrength << "/65535)\n";
            std::cout << "  2 - Decrease Low Motor\n";
            std::cout << "  3 - Increase High Motor (Current: " << rightMotorStrength << "/65535)\n";
            std::cout << "  4 - Decrease High Motor\n";
            std::cout << "  0 - Stop Vibration\n";
        }
        else {
            std::cout << "No controller detected!         \n";
        }


        if (GetAsyncKeyState('1') & 0x8000) {
            leftMotorStrength = min(leftMotorStrength + 5000, 65535);
            SetVibration(leftMotorStrength, rightMotorStrength);
        }
        if (GetAsyncKeyState('2') & 0x8000) {
            leftMotorStrength = max(leftMotorStrength - 5000, 0);
            SetVibration(leftMotorStrength, rightMotorStrength);
        }
        if (GetAsyncKeyState('3') & 0x8000) {
            rightMotorStrength = min(rightMotorStrength + 5000, 65535);
            SetVibration(leftMotorStrength, rightMotorStrength);
        }
        if (GetAsyncKeyState('4') & 0x8000) {
            rightMotorStrength = max(rightMotorStrength - 5000, 0);
            SetVibration(leftMotorStrength, rightMotorStrength);
        }
        if (GetAsyncKeyState('0') & 0x8000) {
            leftMotorStrength = 0;
            rightMotorStrength = 0;
            SetVibration(0, 0);
        }

        if (GetAsyncKeyState(VK_END) & 0x8000) break;

        Sleep(100);
    }

    SetVibration(0, 0);

    return 0;
}
