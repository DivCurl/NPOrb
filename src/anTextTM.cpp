#include "include/anTextTM.h"

using namespace std;
extern const int MAX_THETA_DIV;

anTextTM::anTextTM( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling ) 
: npAnimation( pDisplay, mode, frames, opts, customScaling ) { }

anTextTM::~anTextTM() { }

int anTextTM::Draw() {

    counter ctrShift( 50, ON );
    counter ctrColor( 100, ON );
    sprite TM_Text, TM_Logo, automationText;
    TM_Text.GetPixelVec().reserve( 64 );
    TM_Logo.GetPixelVec().reserve( 64 );
    automationText.GetPixelVec().reserve( 64 );
    
    char strTM[] = "TRANS-MARKET";
    char strAutomation[] = "Automation";
    
    unsigned char chr[ 5 ];   // stores the current ascii character
    uint32_t bmap[ 20 ];
    int xOffset;
   
    for ( int i = 0; i < strlen( strTM ); i++ ) {
        
        // Get the current character in the pointer and move it into chr array ( 5 bytes )
        GetChar( strTM[ i ], chr );
        // build the current character on the display
        for ( int x = 0; x < 5; x++ ) {   // 5 bytes per char
            for ( int y = 0; y < 8; y++ )	{
                if ( chr[ x ] & ( 0x80 >> y ) ) {
                    TM_Text.AddPixels( pixel( { x + xOffset, y + 12 }, rgbwGetByAngle( 150 ), 255 ) );
                }
            }
        }
        
        xOffset += 6;
    } 
        
    xOffset += 4;
    
    // Get the current character in the pointer and move it into chr array ( 5 bytes )
    GetBitmap( 0, bmap, 16 );
    // build the current character on the display
    for ( int x = 0; x < 16; x++ ) {   // 5 bytes per char
        for ( int y = 0; y < 16; y++ )	{
            if ( bmap[ x ] & ( 1 << y ) ) {          
                TM_Logo.AddPixels( pixel( { x + xOffset, y + 8 }, rgbwGetByAngle( 150 ), 255 ) );
            }
        }
    }
    
    xOffset = 0;
    
    for ( int i = 0; i < strlen( strAutomation ); i++ ) {
        
        // Get the current character in the pointer and move it into chr array ( 5 bytes )
        GetChar( strAutomation[ i ], chr );
        // build the current character on the display
        for ( int x = 0; x < 5; x++ ) {   // 5 bytes per char
            for ( int y = 0; y < 8; y++ )	{
                if ( chr[ x ] & ( 0x80 >> y ) ) {
                    automationText.AddPixels( pixel( { x + xOffset, y + 22 }, rgbwGetByAngle( angle ), 255 ) );
                }
            }
        }
        
        xOffset += 6;
    } 
        
    Blit( TM_Text.GetPixelVec() );
    Blit( TM_Logo.GetPixelVec() );     
    Blit( automationText.GetPixelVec() );     
   
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {     
        
        if ( ctrColor.Done() ) {
            ctrColor.Reset();
            automationText.SetColor( rgbwGetByAngle( angle ) );
            Blit( automationText.GetPixelVec() );  
            if ( ( angle += 3.0f ) > 360.0f ) { 
                angle -= 360.0f;                    
            } 
        }
         
        if ( ctrShift.Done() ) {
            ctrShift.Reset();
            automationText.Shift( -1, 0 );
            Clr();
            Blit( TM_Text.GetPixelVec() );
            Blit( TM_Logo.GetPixelVec() );     
            Blit( automationText.GetPixelVec() );  
            ++framesDrawn;
        }

    } // end main loop             
    
    return ( MODE_NEXT );
}