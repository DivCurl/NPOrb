#include "./include/main.h"

/*
extern int16c sampleBuffer[];  
extern bool analyzerRun;
extern volatile bool FFTBufferReady;
extern int sampleIndex;
*/

#pragma config POSCMOD      = HS            // Primary oscillator using high speed crystal mode
#pragma config FNOSC        = PRIPLL        // Internal Fast RC oscillator (4 MHz) w/ PLL
#pragma config FPLLIDIV     = DIV_1         // Divide FRC before PLL (now 4 MHz)
#pragma config FPLLMUL      = MUL_20        // PLL Multiply (now 80 MHz)
#pragma config FPLLODIV     = DIV_2         // Divide After PLL (now 40 MHz)
#pragma config FPBDIV       = DIV_1         // Divide core clock by 1 for peripheral bus (=40MHz Fpb)
#pragma config FWDTEN       = OFF           // Watchdog Timer Disabled
#pragma config ICESEL       = ICS_PGx1      // ICE/ICD Comm Channel Select
#pragma config JTAGEN       = OFF           // Disable JTAG
#pragma config FSOSCEN      = OFF           // Disable Secondary Oscillator

using namespace std;

// calibrated with motor at "high" speed
#define MOTOR_RPM           495         
#define MOTOR_HZ            ( MOTOR_RPM / 60.0 ) // ~ 8.3 RPS -> 1 Rotation / .120s ; 64 DIVS / Rotation -> 
#define REFRESH_HZ          ( MOTOR_HZ * MAX_THETA_DIV )


int T23Period = 48425; // hand-tuned for proper timing of refresh interrupt
int T4Period = 40000;  // 1 ms per tick @ 40Mhz FPB
volatile long msTicks;
volatile long tmrTicks;
uint32_t _1_ms_tick;
// Needs to be global for ISR access
npDisplay display( RGB );   
// DMA buffer - contains a single animation frame for SPI output transfer
uint32_t txSpiDmaBuff[ FR_LED_COUNT + 1 ];  
   
int main() {                    
    SYSTEMConfigPerformance( SYS_FREQ );    
    _1_ms_tick = ( uint32_t )round( ( PB_FREQ / T4Period ) * 0.001f - 1 );   
    mJTAGPortEnable( DEBUG_JTAGPORT_OFF );
    srand(ReadCoreTimer());
    LATBCLR = 0xFFFF;    
    /* Pin Setup for SPI and so on */
    // Note 1: SPI1 CLK = Pin 25 (SCK1), SPI2 CLK = Pin 26 (SCK2)
    // Note 2: DotStar Neopixels do not require a slave select
    TRISBbits.TRISB5 = 1;       // set RB5 (Pin 14) as input
    TRISBbits.TRISB7 = 0;       // set RB7 (Pin 16) as output
    TRISBbits.TRISB8 = 0;       // set RB8 (Pin 17) as output
    TRISBbits.TRISB13 = 0;      // set RB13 (Pin 24) as output - SPI SDO
    PPSUnLock;
    PPSOutput( 3, RPB13, SDO1 );    // CLK = YEL wire, SDI = GRN wire (subject to change per Adafruit's wiring whimsy)
    PPSInput( 2, SDI1, RPB5 );
    PPSLock;    
    // display refresh
    OpenTimer23( T23_ON | T23_SOURCE_INT | T23_PS_1_1, T23Period );  
    ConfigIntTimer23( T23_INT_ON | T23_INT_PRIOR_5 );     
    // millisecond counter
    OpenTimer4( T4_ON | T4_SOURCE_INT | T4_PS_1_1, T4Period );  
    ConfigIntTimer4( T4_INT_ON | T4_INT_PRIOR_4 ); 
    ANSELA = 0;
    ANSELB = 0;    
    // Initialize SPI
    SpiChnOpen( SPI_CHANNEL1, (SpiOpenFlags) ( SPI_OPEN_MSTEN | SPI_OPEN_MODE32 ), 2 );  // with clkDiv = 2 -> 20 Mhz    	
    // Initialize DMA. I'm using DMA for SPI transfers here since tying up the MCU with the usual SPI macros is too expensive
    // timewise to achieve the interrupt latency needed
	DmaChnOpen( DMA_CHANNEL1, DMA_CHN_PRI3, DMA_OPEN_DEFAULT);
	// SPITBE (SPI buffery empty) interrupt event triggers DMA transfer
	DmaChnSetEventControl( DMA_CHANNEL1, (DmaEvCtrlFlags) (DMA_EV_START_IRQ_EN | DMA_EV_START_IRQ(_SPI1_TX_IRQ) ) );    	
    // DMA 1, source is 1 division of the total display, destination is the SPI register, 4 cells per xfer, 4 bytes/cell
	DmaChnSetTxfer( DMA_CHANNEL1, txSpiDmaBuff, (void*)&SPI1BUF, sizeof( txSpiDmaBuff ), 4 , 4 );        
    INTEnableSystemMultiVectoredInt();
    
    // dwell for startup
    counter startup( 5000, ON );
    while ( !startup.Done() );
    
    	    
    npAnimation* pAnim;  
    int currAnim = ID_AN_TEXT_MUDDER;  
    // int currAnim = ID_AN_TEXT_TM;
                    
    while ( 1 ) { 
        if ( currAnim == ID_AN_NULL ) {
            currAnim = ID_AN_MAX - 1;
        } 
        else if ( currAnim >= ID_AN_MAX ) {
            currAnim = 1;
        }
        
        switch ( currAnim ) {  
                       
            case ( ID_AN_TEST ):
                pAnim = new anTest( &display, MODE_REPEAT );
                
                if ( pAnim->Draw() == MODE_PREV ) {
                    currAnim--;
                } 
                else {
                    currAnim++;
                }
                
                delete pAnim;
                break;     
                
            /*
            case ( ID_AN_TEXT_TM ):
                pAnim = new anTextTM( &display, MODE_NULL, 750 );
                
                if ( pAnim->Draw() == MODE_PREV ) {
                    currAnim--;
                } 
                else  {
                    currAnim++;
                }
                
                delete pAnim;
                break;  
             */
                
            case ( ID_AN_TEXT_MUDDER ):
                pAnim = new anToughMudder( &display, MODE_REPEAT );
                
                if ( pAnim->Draw() == MODE_PREV ) {
                    currAnim--;
                } 
                else  {
                    currAnim++;
                }
                
                delete pAnim;
                break;          
                
            case ( ID_AN_RAIN ):
                pAnim = new anRain( &display, MODE_NULL, 800 );
                
                if ( pAnim->Draw() == MODE_PREV ) {
                    currAnim--;
                } 
                else  {
                    currAnim++;
                }
                
                delete pAnim;
                break;    
                
            case ( ID_AN_COMETS ):
                pAnim = new anComets( &display, MODE_NULL, 500 );
                
                if ( pAnim->Draw() == MODE_PREV ) {
                    currAnim--;
                } 
                else {
                    currAnim++;
                }
                
                delete pAnim;
                break;      
                
            case ( ID_AN_RANDOM_FILL ):
                pAnim = new anRandomFill( &display, MODE_REPEAT );
                
                if ( pAnim->Draw() == MODE_PREV ) {
                    currAnim--;
                } 
                else  {
                    currAnim++;
                }
                
                delete pAnim;
                break; 
                
            case ( ID_AN_RINGS ):
                pAnim = new anRings( &display, MODE_NULL );
                
                if ( pAnim->Draw() == MODE_PREV ) {
                    currAnim--;
                } 
                else  {
                    currAnim++;
                }
                
                delete pAnim;
                break;    
                
            
            default : 
                currAnim++;
                delete pAnim;
                break;                        
        }
    }
    
    
    return ( EXIT_SUCCESS );
}

// Timer4 interrupt for accurate timing
extern "C"
void __ISR ( _TIMER_23_VECTOR, IPL5SOFT ) TMR23IntHandler( void ) {        
    // Refresh one divison of display    
    display.Refresh();        
    
    mT23ClearIntFlag();     
} 

// Timer4 interrupt for accurate general timing
extern "C"
void __ISR ( _TIMER_4_VECTOR, IPL4SOFT ) TMR4IntHandler( void ) {        
    mT4ClearIntFlag();   
    
    ++msTicks;  
    tmrTicks = 0;
      
}
