#include "include/ascii.h"

void GetChar ( char chr, unsigned char *dst ) {
    // our bitmap font starts at ascii char 32 (decimal, space char). This ensures the input
    // ascii character is aligned with the start of the bitmap.
    chr -= 32;

    // loop through each byte of the array and store slice in dst[i] until entire char is built
    for ( int i = 0; i < 5; i++ ) {
        dst[ i ] = ascii[ ( chr * 5 ) + i ] ;
    }
}


