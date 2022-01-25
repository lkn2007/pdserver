#ifndef _FILL_DNS_BODY_H_
#define _FILL_DNS_BODY_H_

#include <stdio.h>       // for debugging
#include <string.h>
#include "dns_struct.h"

void FillInHeader(struct dns_header *dns_h, const unsigned char *buf);
void FillInQuestions(struct dns_questions *dns_q, const const unsigned char *buf);

#endif // _FILL_DNS_BODY_H_