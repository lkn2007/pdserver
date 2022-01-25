#include "find_upserver.h"

void FindUpServer(const char* str, char* res)
{
    FILE *conf = fopen(PATH_CONF, "r");
    if (conf == NULL)
    {
       perror("Error opening conf file");
       exit(1);        
    }
    
    char buf[32], left[16], right[16];    
    int flag = 1;   
    
    while(!feof(conf))
    { 
       fgets(buf, sizeof(buf), conf);
       sscanf(buf, "%s%s", left, right);       
       if(strcmp(left, str) == 0)
       {
          strcpy(res, right);
          flag = 0;          
          break;
       }       
    }    
    fclose(conf);
    if(flag)
    {
        printf("Error config file\n");
        exit(1);
    }  
}