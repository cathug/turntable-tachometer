#include "nokiaLCD.h"
#include "tachometer.h"

//------------------------------------------------------------------------------
// variables
//------------------------------------------------------------------------------

volatile byte m_strobeMarkCount = 0;  // if count > 255, change data type
volatile double m_currentRPM = 0.0;
double m_averageRPM = 0.0;
double m_sumRPM = 0.0;
unsigned long int elapsedTime, currentTime;
unsigned long int lastUpdatedTime = 0;
const byte numStrobeMarks = 1;   // 77 = number of 33rpm strobe marks on
                                 // Garrard 401 50 Hz platter
                                 // 1 = paper strip if no strobe marks
const double millisecondsPerMinute = 60000.0;

// create a circular array
const byte sizeof_array_RPM = 10;
byte currentPos = 0;
bool calculate_average_flag = 0;
double array_RPM[sizeof_array_RPM];
char buf_currentRPM[BUFFER_SIZE];
char buf_averageRPM[BUFFER_SIZE];

//------------------------------------------------------------------------------
// handlers
//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////

// function to find the speed in rpm (called by timer interrupt)
void checkCurrentRPM()
{       
    currentTime = millis();
    elapsedTime = currentTime - lastUpdatedTime;
    lastUpdatedTime = currentTime;

    //Serial.println( elapsedTime );

    m_currentRPM = millisecondsPerMinute * 
        m_strobeMarkCount / elapsedTime / numStrobeMarks;

    array_RPM[currentPos] = m_currentRPM;
    currentPos++; 
    
    if ( currentPos >= sizeof_array_RPM )
    {
        calculate_average_flag = 1;
        currentPos = 0; 
    }
    

    if ( calculate_average_flag == 1 )
    {
        // find total sum of array
        for ( byte i = 0; i < sizeof_array_RPM; i++ )
        {
            m_sumRPM += array_RPM[i];
        }

        // find average
        m_averageRPM = m_sumRPM / sizeof_array_RPM;
        m_sumRPM = 0; 
    }
        
    m_strobeMarkCount = 0;  // reset count
}


////////////////////////////////////////////////////////////////////////////////

void ISR_CountStrobeMarks()
{
    m_strobeMarkCount++;
}


////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// end of handlers
//------------------------------------------------------------------------------






//------------------------------------------------------------------------------
// ISRs
//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////

// timer1 interrupt
ISR( TIMER1_COMPA_vect )
{
    noInterrupts();   // critical section - no interrupts executed
    
    checkCurrentRPM();
    
    interrupts();   // end of critical section
}


//------------------------------------------------------------------------------
// end of ISRs
//------------------------------------------------------------------------------




void setup()
{
    LcdInitialise();
    LcdClear();

    noInterrupts();

    // set timer 1 to CTC mode, prescaler = 1024
    TCCR1A = 0;
    TCCR1B = 0;  // reset
    TCNT1  = 0;   // initialize counter value = 0
    TCCR1A |= _BV( WGM12 );
    TCCR1B |= _BV( CS12 ) | _BV( CS10 );
    OCR1A = F_CPU / 1024 / 2 / 8  - 1;
                    // execute timer interrupt at this match point ( 1 second )
    TIMSK1 |= _BV( OCIE1A );
    
    interrupts();

    // set pins
    pinMode( IR_EMITTER, OUTPUT );
    pinMode( IR_DETECTOR, INPUT );
    digitalWrite( IR_EMITTER, HIGH );   
    attachInterrupt( digitalPinToInterrupt( IR_DETECTOR ), 
        ISR_CountStrobeMarks, FALLING );

    // print static text and text box once
    LCDDrawFrame();
    LCDSetCursor( 4, 1 );
            
    LcdString( "Speed" );
}



/*
The Nokia LCD code came from
http://playground.arduino.cc/Code/PCD8544.
It has been modified to improve memory usage.
*/
void loop()
{
    String currentRPM_string, averageRPM_string;  

    LCDSetCursor( 10, 2 );
        // avr function dtostrf replaces snprintf in stdlib.h
    LcdString( dtostrf( m_currentRPM, BUFFER_SIZE, 1,  buf_currentRPM ) );
            
    LCDSetCursor( 10, 3 );
    LcdString( dtostrf( m_averageRPM, BUFFER_SIZE, 1, buf_averageRPM ) ); 
}
