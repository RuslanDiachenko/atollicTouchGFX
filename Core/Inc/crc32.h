//
// crc32.h
//
// Created on: January 18, 2019
// Author: MTK
//

#include <sys/types.h>

#ifndef CRC32_H
#define CRC32_H

#ifdef __cplusplus
 extern "C" {
#endif

uint32_t crc32_buffer(long int count, uint32_t crc, const void *buffer);
int crc32_bist(void);

enum
{
    ERR_CRC32_BIST=0,
    ERR_CRC32_NO_ERROR=1
};

#ifdef __cplusplus
}
#endif

#endif /* CRC32_H_ */
