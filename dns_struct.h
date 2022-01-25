#ifndef _DNS_STRUCT_H_
#define _DNS_STRUCT_H_

#include <inttypes.h>

struct dns_header_flags
{
    uint8_t QR;
    uint8_t OPCODE;
    uint8_t AA;
    uint8_t TC;
    uint8_t RD;
    uint8_t RA;
    uint8_t Z;
    uint8_t RCODE;
};

struct dns_header
{
   uint16_t ID;
   struct dns_header_flags Flags;     
   uint16_t QDCount;
   uint16_t ANCount;
   uint16_t NSCount;
   uint16_t ARCount;  
};

struct dns_questions
{
    uint8_t QNAME[30];
    uint16_t QTYPE;
    uint16_t QCLASS;
};

#endif // _DNS_STRUCT_H_
