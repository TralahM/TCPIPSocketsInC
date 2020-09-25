#ifndef _VoteEncoding_H
#define _VoteEncoding_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "VoteProtocol.h"

bool Decode(uint8_t *inBuf, size_t mSize, VoteInfo *v);
size_t Encode(VoteInfo *v, uint8_t *outBuf, size_t bufSize);


#endif /* ifndef _VoteEncoding_H */

