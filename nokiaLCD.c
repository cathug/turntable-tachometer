#include "nokiaLCD.h"

/*
The Nokia LCD code came from
http://playground.arduino.cc/Code/PCD8544.
It has been modified to improve memory usage.
*/


void LcdInitialise()
{
    pinMode( LCD_SCE, OUTPUT );
    pinMode( LCD_RESET, OUTPUT );
    pinMode( LCD_DC, OUTPUT );
    pinMode( LCD_SDIN, OUTPUT );
    pinMode( LCD_SCLK, OUTPUT );
    //pinMode( LCD_BL, OUTPUT );
  
    digitalWrite( LCD_RESET, LOW );
    //digitalWrite( LCD_BL, HIGH );
   // delay( 1 );
    digitalWrite( LCD_RESET, HIGH );
  
    LcdWrite( LCD_CMD, 0x21 );  // LCD Extended Commands.
    LcdWrite( LCD_CMD, 0xBf );  // Set LCD Vop (Contrast). //B1
    LcdWrite( LCD_CMD, 0x04 );  // Set Temp coefficent. //0x04
    LcdWrite( LCD_CMD, 0x14 );  // LCD bias mode 1:48. //0x13
    LcdWrite( LCD_CMD, 0x0c );  // LCD in normal mode. 0x0d for inverse
    LcdWrite(LCD_C, 0x20);
    LcdWrite(LCD_C, 0x0C);
}


void LcdWrite( byte dc, byte data )
{
    digitalWrite( LCD_DC, dc );
    digitalWrite( LCD_SCE, LOW );
    shiftOut( LCD_SDIN, LCD_SCLK, MSBFIRST, data );
    digitalWrite( LCD_SCE, HIGH );
}


void LcdCharacter( char character )
{
    LcdWrite( LCD_D, 0x00 );
    
    for( int index = 0; index < 5; index++ )
    {
        LcdWrite( LCD_D, pgm_read_byte( &ASCII[character - 0x20][index] ) );
    }
    
    LcdWrite( LCD_D, 0x00 );
}



void LcdClear()
{
    for ( int index = 0; index < LCD_X * LCD_Y / 8; index++ )
    {
        LcdWrite(LCD_D, 0x00);
    }
}


void LcdString( char *characters )
{
    while ( *characters )
    {
        LcdCharacter( *characters++ );
    }
}



// LCDSetCursor routine to position cursor 
// x - range: 0 to 84
// y - range: 0 to 5
void LCDSetCursor( byte x, byte y )
{
    LcdWrite( 0, 0x80 | x );  // Column.
    LcdWrite( 0, 0x40 | y );  // Row.  
}



void LCDDrawFrame()
{
    unsigned char j;  
    for( j = 0; j < LCD_X; j++ ) // top
    {
        LCDSetCursor ( j, 0 );
        LcdWrite ( 1, 0x01 );
    }   
    
    for( j = 0; j < LCD_X; j++ ) //Bottom
    {
        LCDSetCursor( j, 5 );
        LcdWrite( 1, 0x80 );
    }   
  
    for( j = 0; j < 6; j++ ) // Right
    {
        LCDSetCursor( LCD_X - 1, j );
        LcdWrite( 1, 0xff );
    }   
    
    for( j = 0; j < 6; j++ ) // Left
    {
        LCDSetCursor( 0, j );
        LcdWrite( 1, 0xff );
    }
}

