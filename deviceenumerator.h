#ifndef DEVICEENUMERATOR_H
#define DEVICEENUMERATOR_H

#pragma comment(lib, "mpr.lib")

#include <windows.h>
#include <stdio.h>
#include <winnetwk.h>
#include <QString>
#include <intsafe.h>
#include <minwindef.h>
#include <windows.h>
#include <winnetwk.h>
#include <tchar.h>
class DeviceEnumerator
{
public:
    DeviceEnumerator(LPNETRESOURCE lpnr);

};

#endif // DEVICEENUMERATOR_H
