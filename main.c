#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h> // for debugging

#include "dns_struct.h"
#include "find_upserver.h"
#include "check_black_list.h"
#include "fill_dns_body.h"

#define A     1
#define NS    2
#define CNAME 5
#define SOA   6
#define PTR   12
#define MX    15
#define AAAA  28

#define IP_SERVER "127.0.0.1"
#define PORT 53
#define BUF_LENGTH 128

int main(int argc, char *argv[])
{   
    
    int ListenSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if(ListenSocket == -1)
    {
        perror("Error: create listen socket\n");
        exit(1);
    }
        
    struct sockaddr_in LSockAddr;
    LSockAddr.sin_family = AF_INET;
    LSockAddr.sin_port = htons(PORT);
    LSockAddr.sin_addr.s_addr = inet_addr(IP_SERVER);
    
    if(bind(ListenSocket, (struct sockaddr *)&LSockAddr, sizeof(LSockAddr)) == -1)
    {
        perror("Error: bind listen socket");
        exit(1);
    } 
    
    struct sockaddr_in CSockAddr;
    socklen_t c_len = sizeof(CSockAddr);

    unsigned char IP_UPSERVER[16];
    FindUpServer("upserver", IP_UPSERVER);    
    
    struct sockaddr_in RSockAddr;
    RSockAddr.sin_family = AF_INET;
    RSockAddr.sin_port = htons(53);
    RSockAddr.sin_addr.s_addr = inet_addr(IP_UPSERVER);      
    socklen_t r_len = sizeof(RSockAddr);

    struct dns_header DnsRequest;
    struct dns_questions DnsRequestQuestions; 
      
    while(1)
    {    
       uint8_t buf[BUF_LENGTH];
       if((recvfrom(ListenSocket, buf, BUF_LENGTH, 0, (struct sockaddr *)&CSockAddr, &c_len)) == -1)
       {
           perror("Error: recvfrom client\n");            
       }
       
       FillInHeader(&DnsRequest, buf);       
       FillInQuestions(&DnsRequestQuestions, buf);

       if (DnsRequestQuestions.QTYPE == A)
       {
           if(FindInBlackList(DnsRequestQuestions.QNAME))
           {             
              buf[2] = 0b10000000;           
              buf[3] = 0b00100000 | 0b00000101;
              if(sendto(ListenSocket, buf, BUF_LENGTH, 0, (struct sockaddr *)&CSockAddr, c_len) == -1)
              {
                  perror("Error: sendto client\n");
              }
           }
           else
           {
              int RedirectSocket = socket(AF_INET, SOCK_DGRAM, 0);
              if(sendto(RedirectSocket, buf, BUF_LENGTH, 0, (struct sockaddr *)&RSockAddr, r_len) == -1)
              {
                 perror("Error: redirect sendto\n");            
              }        
              if((recvfrom(RedirectSocket, buf, BUF_LENGTH, 0, (struct sockaddr *)&RSockAddr, &r_len)) == -1)
              {
                 perror("Error: redirect recvfrom\n");            
              }            
              close(RedirectSocket);
              if(sendto(ListenSocket, buf, BUF_LENGTH, 0, (struct sockaddr *)&CSockAddr, c_len) == -1)
              {
                perror("Error: sendto client\n");
              }                
           }
        }
        else
        {       
            int RedirectSocket = socket(AF_INET, SOCK_DGRAM, 0);
            if(sendto(RedirectSocket, buf, BUF_LENGTH, 0, (struct sockaddr *)&RSockAddr, r_len) == -1)
            {
                perror("Error: redirect sendto\n");            
            }        
            if(recvfrom(RedirectSocket, buf, BUF_LENGTH, 0, (struct sockaddr *)&RSockAddr, &r_len) == -1)
            {
                perror("Error: redirect recvfrom\n");            
            }            
            close(RedirectSocket);
            if(sendto(ListenSocket, buf, BUF_LENGTH, 0, (struct sockaddr *)&CSockAddr, c_len) == -1)
            {
            perror("Error: sendto client\n");
            }                
        }
    }
    return 0;
}