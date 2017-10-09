#ifndef TACHOMETER_H
#define TACHOMETER_H


#include <Arduino.h>

#define BUFFER_SIZE 7

extern volatile byte m_strobeMarkCount;
extern volatile double m_currentRPM;


#ifdef __cplusplus
extern "C" 
{
#endif

    void LcdWrite( byte dc, byte data );
    void LcdCharacter( char character );
    void LcdClear();
    void LcdInitialise();
    void LcdString( char *characters );
    void LCDSetCursor( byte x, byte y );
    void LCDDrawFrame();

    void checkCurrentRPM();
    void ISR_CountStrobeMarks();


#ifdef __cplusplus
}
#endif


#endif // tachometer.h
