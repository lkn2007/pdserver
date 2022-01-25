#include "check_black_list.h"

int FindInBlackList(const char *str)
{    
    FILE *conf = fopen(PATH_CONF, "r");
    if (conf == NULL)
    {
      perror("error opening conf file");
       exit(1);        
    }    

    char buf[32]={0}, tmp[32]={0};   

    while(!feof(conf))
    {
       fgets(buf, sizeof(buf), conf);
       size_t len = strlen(buf);
       if(len >0 && buf[len -1] == '\n')
       {
           buf[--len] = '\0';
       }
       sscanf(buf, "%s", tmp);                    
       if(strcmp(str, tmp) == 0)
       {
          fclose(conf);
          return 1;
       } 
    }      
    fclose(conf);
    return 0;
}