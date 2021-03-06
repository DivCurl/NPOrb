#include "./include/fft.h"

// global so ISR can access
int16c sampleBuffer[ N ]; //initialize buffer to collect samples
volatile int sampleIndex;
volatile bool FFTBufferReady;
bool analyzerRun;
int16c din[ N ];       // buffer to hold old samples
int16c dout[ N ];      // holds computed FFT
int16c scratch[ N ];
int16c twiddle [ N / 2 ];
long int freqVector [ N ];
short singleSidedFFT[ N ];

#ifdef FFT_1024
    int log2N = 9;  // log2(1024) = 9
#endif

#ifdef FFT_256
    int log2N = 8;  // log2(256) = 8
#endif

#ifdef FFT_128
    int log2N = 7;  // log2(128) = 7
#endif

#ifdef FFT_64
    int log2N = 6;  // log2(64) = 6
#endif

// fast log (dB) lookup table
const char dbLUT[ 1024 ] = {
-60,-60,-54,-51,-48,-46,-45,-43,-42,-41,-40,-39,-39,-38,-37,-37,-36,-36,-35,-35,-34,-34,-33,-33,-33,-32,-32,-32,-31,-31,-31,-30,-30,-30,-30,-29,-29,
-29,-29,-28,-28,-28,-28,-28,-27,-27,-27,-27,-27,-26,-26,-26,-26,-26,-26,-25,-25,-25,-25,-25,-25,-24,-24,-24,-24,-24,-24,-24,-24,-23,-23,-23,-23,-23,
-23,-23,-23,-22,-22,-22,-22,-22,-22,-22,-22,-22,-22,-21,-21,-21,-21,-21,-21,-21,-21,-21,-21,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-20,-19,-19,
-19,-19,-19,-19,-19,-19,-19,-19,-19,-19,-19,-18,-18,-18,-18,-18,-18,-18,-18,-18,-18,-18,-18,-18,-18,-18,-17,-17,-17,-17,-17,-17,-17,-17,-17,-17,-17,
-17,-17,-17,-17,-17,-17,-16,-16,-16,-16,-16,-16,-16,-16,-16,-16,-16,-16,-16,-16,-16,-16,-16,-16,-15,-15,-15,-15,-15,-15,-15,-15,-15,-15,-15,-15,-15,
-15,-15,-15,-15,-15,-15,-15,-15,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-13,-13,-13,-13,-13,
-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,
-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,
-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,
-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,
-9,-9,-9,-9,-9,-9,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,
-8,-8,-8,-8,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,
-7,-7,-7,-7,-7,-7,-7,-7,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,
-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,
-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,
-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,
-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,
-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-2,-2,-2,
-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,
-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

void InitFFT() {
    FFTBufferReady = 0;
    // copy twiddle factors from flash to RAM for slight performance boost
    memcpy( twiddle, fftc, sizeof( twiddle ) );
    // Start with clean buffers
    memset( singleSidedFFT, 0, sizeof( singleSidedFFT ) );
    memset( sampleBuffer, 0, sizeof( sampleBuffer ) );
    memset( din, 0, sizeof( din ) );
    memset( dout, 0, sizeof( dout ) );
    // Regenerate frequency vector
	for ( int i = 0; i < N / 2; i++ ) {
		freqVector[ i ] = i * ( ADC_SAMPLE_FREQ / 2 ) / ( ( N / 2 ) - 1 );
	}

}

__attribute__ ( ( optimize( "unroll-loops" ) ) )
void ComputeFFT() {
    mT4IntEnable( 0 );
    // Pull sampled values out of ADC sample buffer and store in din
    for ( int i = 0; i < N; i++ ) {
        if ( i < sampleIndex ) {
            din[ i + ( N - sampleIndex ) ] = sampleBuffer[ i ];
        }
        else {
            din[ i - sampleIndex ] = sampleBuffer[ i ];
        }
    }

    mips_fft16( dout, din, twiddle, scratch, log2N );

    for ( int i = 1; i < N / 2; i++ ) { // skip [0] because it contains garbage
        float re_sqr = ( dout[ i ].re * dout[ i ].re );
        float im_sqr = ( dout[ i ].im * dout[ i ].im );        
        singleSidedFFT[ i - 1 ] = (short)dbLUT[ (int)( sqrt( re_sqr + im_sqr ) ) ]; // scaled to dB using lookup table for speed
    }

    FFTBufferReady = 0;
    mT4IntEnable( 1 );
}

