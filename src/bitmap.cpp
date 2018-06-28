#include "include/bitmap.h"

void GetBitmap( short index, uint32_t *dst, short len ) {
    
    // loop through each byte of the array and store slice in dst[i] until entire char is built
    for ( int i = 0; i < len; i++ ) {
        dst[ i ] = bitmap[ index ] [ i ] ;
    }
}


