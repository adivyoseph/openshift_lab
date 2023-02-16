/*
 
 
 
 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define ETHPORT_MAX   4                                                  // maximum base stsem ethernet ports
#define STR_LEN_MAX 64                                                   // maximum system name string length + 1
typedef struct systemInfo_s {
    char  ethPorts[ETHPORT_MAX][STR_LEN_MAX];
    int      ethPortCnt;                                                                // number of ethernet ports found
    char wifiPort[STR_LEN_MAX];


}  systemInfo_t;
systemInfo_t     g_systemInfo;

#define SYS_CLASS_NET  "/sys/class/net"

int main( int argc, char** argv ) {
	    DIR *                      p_dir;
        struct dirent *   p_dirEntry;
        //struct stat            s;
        //char pathname[1024];
        int i;


        p_dir = opendir(SYS_CLASS_NET);
        g_systemInfo.ethPortCnt = 0;
        if (p_dir) { /*if it's a directory*/
                 while (( p_dirEntry = readdir(p_dir)) != NULL) { 
                     if(strcmp( p_dirEntry->d_name, ".") == 0)  continue;
                     if(strcmp( p_dirEntry->d_name, "..") == 0)  continue;
                     if(strcmp( p_dirEntry->d_name, "lo") == 0)  continue;
                     if(strncmp( p_dirEntry->d_name, "wlo", 3) == 0)  {
                       // assume WiFi
                         strcpy(g_systemInfo.wifiPort, p_dirEntry->d_name );
                         continue;
                     }
                     else {
                         // assume an eth port
                         if (g_systemInfo.ethPortCnt < ETHPORT_MAX ) {
                             //printf("%d %s\n", g_systemInfo.ethPortCnt, p_dirEntry->d_name);
                             strcpy(&g_systemInfo.ethPorts[g_systemInfo.ethPortCnt][0], p_dirEntry->d_name);

                             g_systemInfo.ethPortCnt++;
                         }
                     }
/*
          sprintf(pathname, "./%s",  p_dirEntry->d_name); //makes pathname
          if (lstat(pathname, &s) == 0 && S_ISDIR(s.st_mode)) { //if the file is a directory
 
*/
                 }
                 closedir(p_dir);
        }

        printf("WiFi port : %s\n", g_systemInfo.wifiPort);
        for (i = 0; i < g_systemInfo.ethPortCnt ; i++) {
            printf("Eth  port_%d: %s\n", i, &g_systemInfo.ethPorts[i][0]);
        }
        //Todo add checks


    return 0;
}
