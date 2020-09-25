#ifndef _FRAMER_H
#define _FRAMER_H

#include <stdio.h>
#include <stdint.h>

int GetNextMsg(FILE *in, uint8_t *buf, size_t bufSize);
int PutMsg(uint8_t buf[], size_t msgSize, FILE *out);

#endif /* ifndef _FRAMER_H */

