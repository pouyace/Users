#include "netconnectionenumerator.h"
#include <windows.h>
#include <stdio.h>
#include <lm.h>
#include <QSysInfo>
#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "Netapi32.lib")
NetConnectionEnumerator::NetConnectionEnumerator(QString serverName,QString shareName)
{
   DWORD res, i, er = 0, tr = 0, resume = 0;
   PCONNECTION_INFO_0 p,b;
   LPTSTR lpszServer = NULL, lpszShare = NULL;

      //
      // The server is not the default local computer.
      //
      lpszServer=(LPTSTR)(serverName.data());
      //
      // ShareName is always the last argument.
      //
      lpszShare=(LPTSTR)(shareName.data());
      //
      // Call the NetConnectionEnum function,
      //  specifying information level 1.
      //
      res=NetConnectionEnum(NULL, lpszShare, 0, (LPBYTE *) &p, MAX_PREFERRED_LENGTH, &er, &tr, &resume);
      //
      // If no error occurred,
      //
      if(res == 0)
      {
         //
         // If there were any results,
         //
         if(er>0)
         {
            b=p;
            //
            // Loop through the entries; print user name and network name.
            //
            for(i=0;i<er;i++)
            {
               printf("%S\t%S\n", b->coni0_id);
               b++;
            }
            // Free the allocated buffer.
            //
            NetApiBufferFree(p);
         }
         // Otherwise, print a message depending on whether
         //  the qualifier parameter was a computer (\\ComputerName)
         //  or a share (ShareName).
         //
         else
         {
            if(lpszShare[0]=='\\')
               printf("No connection to %S from %S\n",
                  (lpszServer == NULL)?TEXT("LocalMachine"):lpszServer, lpszShare);
            else
               printf("No one connected to %S\\%S\n",
                  (lpszServer == NULL)?TEXT("\\\\LocalMachine"):lpszServer,lpszShare);
         }
      }
      //
      // Otherwise, print the error.
      //
      else
         printf("Error: %d\n",res);

}

