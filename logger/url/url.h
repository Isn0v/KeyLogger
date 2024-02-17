#pragma once

#ifdef _WIN32
#include <windows.h>
#include <oleacc.h>
#endif

#include <stdio.h>
#include <regex>

#include <atlbase.h>
#include <atlcom.h>
#include <UIAutomation.h>

// void hookUrl();

// void unhookUrl();

bool find_url(IUIAutomation *uia, IUIAutomationElement *root, std::string &retUrl);

void getChromeUrl(std::string &retUrl);
