#include "deviceenumerator.h"
#ifndef UNICODE
#define UNICODE
#endif

#include <QDebug>
BOOL WINAPI EnumerateFunc(LPNETRESOURCE lpnr);
void DisplayStruct(int i, LPNETRESOURCE lpnrLocal);

DeviceEnumerator::DeviceEnumerator(       LPNETRESOURCE lpnr)
{

    if (EnumerateFunc(lpnr) == FALSE) {
        qDebug()<<"Call to EnumerateFunc failed\n";
    }
    else{
        qDebug()<<"Hello";
    }
}



BOOL WINAPI EnumerateFunc(LPNETRESOURCE lpnr)
{
    DWORD dwResult, dwResultEnum;
    HANDLE hEnum;
    DWORD cbBuffer = 16384;     // 16K is a good size
    DWORD cEntries = -1;        // enumerate all possible entries
    LPNETRESOURCE lpnrLocal;    // pointer to enumerated structures
    DWORD i;
    //
    // Call the WNetOpenEnum function to begin the enumeration.
    //
    dwResult = WNetOpenEnumW(RESOURCE_GLOBALNET, // all network resources
                            RESOURCETYPE_ANY,   // all resources
                            0,  // enumerate all resources
                            lpnr,       // NULL first time the function is called
                            &hEnum);    // handle to the resource

    if (dwResult != NO_ERROR) {
        qDebug()<<"WnetOpenEnum failed with error %d\n"<<dwResult;
        return FALSE;
    }
    //
    // Call the GlobalAlloc function to allocate resources.
    //
    lpnrLocal = (LPNETRESOURCE) GlobalAlloc(GPTR, cbBuffer);
    if (lpnrLocal == NULL) {
        qDebug()<<"WnetOpenEnum failed with error %d\n", dwResult;
//      NetErrorHandler(hwnd, dwResult, (LPSTR)"WNetOpenEnum");
        return FALSE;
    }

    do {
        //
        // Initialize the buffer.
        //
        ZeroMemory(lpnrLocal, cbBuffer);
        //
        // Call the WNetEnumResource function to continue
        //  the enumeration.
        //
        dwResultEnum = WNetEnumResource(hEnum,  // resource handle
                                        &cEntries,      // defined locally as -1
                                        lpnrLocal,      // LPNETRESOURCE
                                        &cbBuffer);     // buffer size
        //
        // If the call succeeds, loop through the structures.
        //
        if (dwResultEnum == NO_ERROR) {
            for (i = 0; i < cEntries; i++) {
                // Call an application-defined function to
                //  display the contents of the NETRESOURCE structures.
                //
                DisplayStruct(i, &lpnrLocal[i]);

                // If the NETRESOURCE structure represents a container resource,
                //  call the EnumerateFunc function recursively.

                if (RESOURCEUSAGE_CONTAINER == (lpnrLocal[i].dwUsage
                                                & RESOURCEUSAGE_CONTAINER))
//          if(!EnumerateFunc(hwnd, hdc, &lpnrLocal[i]))
                    if (!EnumerateFunc(&lpnrLocal[i]))
                        qDebug()<<"EnumerateFunc returned FALSE\n";
//            TextOut(hdc, 10, 10, "EnumerateFunc returned FALSE.", 29);
            }
        }
        // Process errors.
        //
        else if (dwResultEnum != ERROR_NO_MORE_ITEMS) {
            qDebug()<<"WNetEnumResource failed with error %d\n", dwResultEnum;

//      NetErrorHandler(hwnd, dwResultEnum, (LPSTR)"WNetEnumResource");
            break;
        }
    }
    //
    // End do.
    //
    while (dwResultEnum != ERROR_NO_MORE_ITEMS);
    //
    // Call the GlobalFree function to free the memory.
    //
    GlobalFree((HGLOBAL) lpnrLocal);
    //
    // Call WNetCloseEnum to end the enumeration.
    //
    dwResult = WNetCloseEnum(hEnum);

    if (dwResult != NO_ERROR) {
        //
        // Process errors.
        //
        qDebug()<<"WNetCloseEnum failed with error %d\n", dwResult;
//    NetErrorHandler(hwnd, dwResult, (LPSTR)"WNetCloseEnum");
        return FALSE;
    }

    return TRUE;
}

void DisplayStruct(int i, LPNETRESOURCE lpnrLocal)
{
    qDebug()<<"NETRESOURCE[%d] Scope: "<< i;
    switch (lpnrLocal->dwScope) {
    case (RESOURCE_CONNECTED):
        qDebug()<<"connected\n";
        break;
    case (RESOURCE_GLOBALNET):
        qDebug()<<"all resources\n";
        break;
    case (RESOURCE_REMEMBERED):
        qDebug()<<"remembered\n";
        break;
    default:
        qDebug()<<"unknown scope %d\n"<< lpnrLocal->dwScope;
        break;
    }

    qDebug()<<"NETRESOURCE[%d] Type: "<< i;
    switch (lpnrLocal->dwType) {
    case (RESOURCETYPE_ANY):
        qDebug()<<"any\n";
        break;
    case (RESOURCETYPE_DISK):
        qDebug()<<"disk\n";
        break;
    case (RESOURCETYPE_PRINT):
        qDebug()<<"print\n";
        break;
    default:
        qDebug()<<"unknown type %d\n", lpnrLocal->dwType;
        break;
    }

    qDebug()<<"NETRESOURCE[%d] DisplayType: " << i;
    switch (lpnrLocal->dwDisplayType) {
    case (RESOURCEDISPLAYTYPE_GENERIC):
        qDebug()<<"generic\n";
        break;
    case (RESOURCEDISPLAYTYPE_DOMAIN):
        qDebug()<<"domain\n";
        break;
    case (RESOURCEDISPLAYTYPE_SERVER):
        qDebug()<<"server\n";
        break;
    case (RESOURCEDISPLAYTYPE_SHARE):
        qDebug()<<"share\n";
        break;
    case (RESOURCEDISPLAYTYPE_FILE):
        qDebug()<<"file\n";
        break;
    case (RESOURCEDISPLAYTYPE_GROUP):
        qDebug()<<"group\n";
        break;
    case (RESOURCEDISPLAYTYPE_NETWORK):
        qDebug()<<"network\n";
        break;
    default:
        qDebug()<<"unknown display type %d\n", lpnrLocal->dwDisplayType;
        break;
    }

    qDebug()<<"NETRESOURCE[%d] Usage: 0x%x = " << i << lpnrLocal->dwUsage;
    if (lpnrLocal->dwUsage & RESOURCEUSAGE_CONNECTABLE)
        qDebug()<<"connectable ";
    if (lpnrLocal->dwUsage & RESOURCEUSAGE_CONTAINER)
        qDebug()<<"container ";
    qDebug()<<"\n";

    qDebug()<<"NETRESOURCE[%d] Localname: %S\n" << i << lpnrLocal->lpLocalName;
    qDebug()<<"NETRESOURCE[%d] Remotename: %S\n" << i << lpnrLocal->lpRemoteName;
    qDebug()<<"NETRESOURCE[%d] Comment: %S\n" <<  i << lpnrLocal->lpComment;
    qDebug()<<"NETRESOURCE[%d] Provider: %S\n" << i << lpnrLocal->lpProvider;
    qDebug()<<"\n";
}
