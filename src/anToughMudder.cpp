#include "include/anToughMudder.h"

using namespace std;
extern const int MAX_THETA_DIV;

anToughMudder::anToughMudder( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling ) 
: npAnimation( pDisplay, mode, frames, opts, customScaling ) { }

anToughMudder::~anToughMudder() { }

int anToughMudder::Draw() {
    counter ctrShift( 50, ON );
    counter ctrColor( 100, ON );
    sprite TM_Text;
    sprite TM_Names;    
    int currName = 0;
    char* pName = NULL;
    
    TM_Text.GetPixelVec().reserve( 40 );
    TM_Names.GetPixelVec().reserve( 32 );      
        
    char mud[] = "TEAM MUD 2018!";    
    char name0[] = "Steven";
    char name1[] = "Jesus";
    char name2[] = "Adrienne";
    char name3[] = "Mike";
    char name4[] = "Danielle";
    char name5[] = "Nathan";
    
    // Set pointer to first name
    pName = name0;
    
    // stores the current ASCII character retrieved from lookup table
    unsigned char chr[ 5 ];   
    int xOffset;
    
    // Draw "Tough Mudder" Text
    for ( int i = 0; i < strlen( mud ); ++i ) {
        
        // Get the current character in the pointer and move it into chr array ( 5 bytes )
        GetChar( mud[ i ], chr );
        // build the current character on the display
        for ( int x = 0; x < 5; x++ ) {   // 5 bytes per char
            for ( int y = 0; y < 8; y++ )	{
                if ( chr[ x ] & ( 0x80 >> y ) ) {
                    TM_Text.AddPixels( pixel( { x + xOffset, y + 24 }, rgbwGetByAngle( 150 ), 255 ) );
                }
            }
        }
        
        xOffset += 6;
    } 

    xOffset = 0;
    
    // Draw names 
    for ( int i = 0; i < strlen( pName ); ++i ) {
        
        // Get the current character in the pointer and move it into chr array ( 5 bytes )
        GetChar( pName[ i ], chr );
        // build the current character on the display
        for ( int x = 0; x < 5; x++ ) {   // 5 bytes per char
            for ( int y = 0; y < 8; y++ )	{
                if ( chr[ x ] & ( 0x80 >> y ) ) {
                    TM_Names.AddPixels( pixel( { x + xOffset, y + 10 }, rgbwGetByAngle( angle ), 255 ) );
                }
            }
        }
        
        xOffset += 6;
    } 
   
    // Blit sprites to display
    Blit( TM_Text.GetPixelVec() );     
    Blit( TM_Names.GetPixelVec() );     
    
    // Draw decorative rings around top and bottom
    for ( int i = 0; i < MAX_THETA_DIV; ++i ) {
        Set( i, 38, rgbwGetByAngle( angle + ( i * 3 ) ) );
        Set( i, 37, rgbwGetByAngle( angle + ( i * 3 ) ) );
        Set( i, 36, rgbwGetByAngle( angle + ( i * 3 ) ) );
        Set( i, 4, rgbwGetByAngle( 360 - (angle + ( i * 3 ) ) ) );
        Set( i, 3, rgbwGetByAngle( 360 - (angle + ( i * 3 ) ) ) );
        Set( i, 2, rgbwGetByAngle( 360 - (angle + ( i * 3 ) ) ) );
    }
    
    // Main animation loop
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {     
        int shifts;
        
        if ( ctrColor.Done() ) {
            ctrColor.Reset();
            
            if ( ( angle += 3.0f ) > 360.0f ) { 
                angle -= 360.0f;                    
            } 
            TM_Text.SetColor( rgbwGetByAngle( angle ) );
            
            Blit( TM_Text.GetPixelVec() );
        }
         
        if ( ctrShift.Done() ) {
            ctrShift.Reset();            
            Clr();
            
            // Redraw rings at top and bottom
            for ( int i = 0; i < MAX_THETA_DIV; ++i ) {
                Set( i, 38, rgbwGetByAngle( angle + ( i * 3 ) ) );
                Set( i, 37, rgbwGetByAngle( angle + ( i * 3 ) ) );
                Set( i, 36, rgbwGetByAngle( angle + ( i * 3 ) ) );
                Set( i, 35, rgbwGetByAngle( angle + ( i * 3 ) ) );
                
                Set( i, 3, rgbwGetByAngle( 360 - (angle + ( i * 3 ) ) ) );
                Set( i, 2, rgbwGetByAngle( 360 - (angle + ( i * 3 ) ) ) );
                Set( i, 1, rgbwGetByAngle( 360 - (angle + ( i * 3 ) ) ) );
            }
            
            // Shift name sprite one position to left
            TM_Names.Shift( -1, 0 );  
            
            // Move to next name once we've shifted the name twice
            if ( ++shifts > MAX_THETA_DIV * 3 ) {                
                shifts = 0;
                
                switch ( currName ) {
                    case 0:
                        currName = 1;
                        pName = name1;                        
                        break;
                        
                    case 1:
                        currName = 2;
                        pName = name2;                        
                        break;
                        
                    case 2:
                        currName = 3;
                        pName = name3;                        
                        break;
                        
                    case 3:
                        currName = 4;
                        pName = name4;                        
                        break;
                        
                    case 4:
                        currName = 5;
                        pName = name5;
                        break;
                        
                    case 5:
                        return ( MODE_NEXT );
                        break;
                        
                        
                    default:
                        break;
                        
                }
                                
                TM_Names.PurgePixels();
                xOffset = 0;
                
                for ( int i = 0; i < strlen( pName ); ++i ) {        
                    // Get the current character in the pointer and move it into chr array ( 5 bytes )
                    GetChar( pName[ i ], chr );
                    // build the current character on the display
                    for ( int x = 0; x < 5; x++ ) {   // 5 bytes per char
                        for ( int y = 0; y < 8; y++ )	{
                            if ( chr[ x ] & ( 0x80 >> y ) ) {
                                TM_Names.AddPixels( pixel( { x + xOffset, y + 10 }, rgbwGetByAngle( angle ), 255 ) );
                            }
                        }
                    }

                    xOffset += 6;
                } 
            }
            
            Blit( TM_Text.GetPixelVec() );
            Blit( TM_Names.GetPixelVec() );   
            
            ++framesDrawn;
        }
    } // end main loop             
    
    return ( MODE_NEXT );
}