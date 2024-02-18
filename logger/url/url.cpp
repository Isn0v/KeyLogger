#include "url.h"

CComPtr<IUIAutomationElement> getRoot(CComPtr<IUIAutomation> uia)
{

    CComPtr<IUIAutomationElement> root;

    if SUCCEEDED (uia->GetRootElement(&root))
    {
        return root;
    }

    return nullptr;
}

bool find_url(IUIAutomation *uia, IUIAutomationElement *root, std::string &retUrl)
{
    CComPtr<IUIAutomationElement> pane;
    CComPtr<IUIAutomationCondition> pane_cond;
    uia->CreatePropertyCondition(UIA_ControlTypePropertyId,
                                 CComVariant(UIA_PaneControlTypeId), &pane_cond);

    CComPtr<IUIAutomationElementArray> arr;
    if FAILED (root->FindAll(TreeScope_Children, pane_cond, &arr))
        return false;

    int count = 0;
    arr->get_Length(&count);
    for (int i = 0; i < count; i++)
    {
        CComBSTR name;
        if SUCCEEDED (arr->GetElement(i, &pane))
            if SUCCEEDED (pane->get_CurrentName(&name))
            {
                std::wstring ws(name, SysStringLen(name));
                if (ws.find(L"Google Chrome") != std::wstring::npos)
                    break;
            }

        pane.Release();
    }

    if (!pane)
        return false;

    CComPtr<IUIAutomationElement> url;
    CComPtr<IUIAutomationCondition> url_cond;
    uia->CreatePropertyCondition(UIA_ControlTypePropertyId,
                                 CComVariant(UIA_EditControlTypeId), &url_cond);
    if FAILED (pane->FindFirst(TreeScope_Descendants, url_cond, &url))
        return false;

    CComVariant var;
    if FAILED (url->GetCurrentPropertyValue(UIA_ValueValuePropertyId, &var))
        return false;
    if (!var.bstrVal)
        return false;

    std::wstring wStr(var.bstrVal, SysStringLen(var.bstrVal));
    retUrl = std::string(wStr.begin(), wStr.end());

    // set new address ...
    // IValueProvider *pattern = nullptr;
    // if (FAILED(url->GetCurrentPattern(UIA_ValuePatternId, (IUnknown **)&pattern)))
    //     return false;
    // // pattern->SetValue(L"somewhere.com");
    // pattern->Release();

    // INPUT input[2] = {INPUT_KEYBOARD};
    // input[0].ki.wVk = VK_RETURN;
    // input[1] = input[0];
    // input[1].ki.dwFlags |= KEYEVENTF_KEYUP;
    // SendInput(2, input, sizeof(INPUT));

    return true;
}

void getChromeUrl(std::string &retUrl)
{

    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    CComPtr<IUIAutomation> uia;
    if FAILED (uia.CoCreateInstance(CLSID_CUIAutomation))
    {
        return;
    }

    CComPtr<IUIAutomationElement> root = getRoot(uia);
    find_url(uia, root, retUrl);
    CoUninitialize();
}
