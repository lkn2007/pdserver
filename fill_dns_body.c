#include "fill_dns_body.h"

void FillInHeader(struct dns_header *dns_h, const uint8_t *buf)
{    
    dns_h->ID = buf[0] << 8 | buf[1];     
    
    dns_h->Flags.QR = (buf[2] & 128) >> 7;        
    dns_h->Flags.OPCODE = ((buf[2] & 64) | (buf[2] & 32)  | (buf[2] & 32) | (buf[2] & 16)) >> 3;    
    dns_h->Flags.AA = (buf[2] & 4) >> 2;    
    dns_h->Flags.TC = (buf[2] & 2) >> 1;    
    dns_h->Flags.RD = (buf[2] & 1);     
    dns_h->Flags.RA = (buf[3] & 128) >> 7;   
    dns_h->Flags.Z = ((buf[3] & 64) | (buf[3] & 32) | (buf[3] & 16)) >> 4;   
    dns_h->Flags.RCODE = (buf[2] & 8) | (buf[2] & 4) | (buf[2] & 2) | (buf[2] & 1);
    
    dns_h->QDCount = buf[4] << 8 | buf[5];
    dns_h->ANCount = buf[6] << 8 | buf[7];
    dns_h->NSCount = buf[8] << 8 | buf[9];
    dns_h->ARCount = buf[10] << 8 | buf[11];
}

void FillInQuestions(struct dns_questions *dns_q, const uint8_t *buf)
{
    size_t start = 12;   // Start section QUESTIONS  after HEADER              
    size_t marker_pos = start;
    size_t marker_val = 0 ;
    size_t j = 0, k=1;
               
    for (;;)
    {            
        marker_pos = marker_pos + j;
        marker_val = buf[marker_pos];
        if(marker_val == 0)
        {               
            dns_q->QNAME[k-2] = '\0';  // Removing the last dot 
            break;
        }
        for (j = 1; j <= marker_val; k++, j++)
        {               
            dns_q->QNAME[k-1] = buf[marker_pos + j];
            if(j == marker_val)
            {                               
               dns_q->QNAME[k] = '.';
               k++;
            }                                        
        }
    }        
    size_t size_qname = strlen(dns_q->QNAME) + 2;
    dns_q->QTYPE = buf[start + size_qname]<< 8 | buf[start + size_qname +1];   
    dns_q->QCLASS = buf[start + size_qname +2]<< 8 | buf[start + size_qname+3];   
}