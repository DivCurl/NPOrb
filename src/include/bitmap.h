#ifndef BITMAP_H
#define	BITMAP_H

#include "sprite.h"

const uint16_t bitmap[ 1 ] [ 16 ] = {
    { 4112, 12312, 16344, 16344, 12312, 14328, 14328, 13312, 12544, /* 12384, */  12352, 12544, 13312, 14328, 14328, 12312, 4112 }
};

void GetBitmap( short index, uint32_t *dst, short len );
    
#endif	

