
#include <avr/io.h>

unsigned char counter = 0;

void delay_500us(void) {
    for (unsigned char i = 86; i > 0; i--)
        for (unsigned char j = 30; j > 0; j--);
}

void delay_333us(void) {
    for (unsigned char i = 57; i > 0; i--)
        for (unsigned char j = 30; j > 0; j--);
}

void delay_tenth_sec(void) {
    for (unsigned char i = 200; i > 0; i--)
        delay_500us();
}

void display_leds(void) {
    unsigned char out = 0xFF;                    // start all LEDs on (active low)

    if (counter & (1 << 0)) out &= 0b01111111;  // counter bit 0 -> LED bit 7
    if (counter & (1 << 1)) out &= 0b10111111;  // counter bit 1 -> LED bit 6
    if (counter & (1 << 2)) out &= 0b11011111;  // counter bit 2 -> LED bit 5
    if (counter & (1 << 3)) out &= 0b11101111;  // counter bit 3 -> LED bit 4
    if (counter & (1 << 4)) out &= 0b11110111;  // counter bit 4 -> LED bit 3

    PORTD = out;                             
}

void play_1khz_alarm(void) {
    for (unsigned char i = 250; i > 0; i--) {
        PORTE |=  (1 << 4);   
        delay_500us();         
        PORTE &= ~(1 << 4);  
        delay_500us();         
    }
}

void play_1_5khz_alarm(void) {
    for (unsigned char i = 250; i > 0; i--) {
        PORTE |=  (1 << 4);   // speaker on
        delay_333us();         // 333us high = 1.5khz
        PORTE &= ~(1 << 4);   // speaker off
        delay_333us();         // 333us low = 1.5khz
    }
}

void wait_for_release(void) {
    while (!(PINA & (1 << 0)) || !(PINA & (1 << 1)));  // wait until both buttons released
}

void count_up(void) {
    if (counter == 25) {        
        counter = 0;
        play_1khz_alarm();      // play alarm 
    } else {
        counter++;         
    }
    display_leds();             
    wait_for_release();      
}

void count_down(void) {
    if (counter == 0) {         
        counter = 25;
        play_1_5khz_alarm();    // play alarm
    } else {
        counter--;              
    }
    display_leds();             // display led’s
    wait_for_release(); 
}

void button3_task(void) {
    do {
        counter--;              
        display_leds();         // update LED display
        delay_tenth_sec();      
    } while (counter != 0);    
    play_1khz_alarm();          // play alarm 
}

int main(void) {
    DDRA  = 0b11111000;         // switches 1-3 as input
    PORTA = 0b00000111;         // enable pull-ups on switch pins
    DDRE  |= (1 << 4);         
    DDRD   = 0xFF;              // LEDs all output
    PORTD  = 0xFF;              // LEDs off (active low)

    while (1) {
        delay_tenth_sec();                     
        delay_tenth_sec();                       

        if (!(PINA & (1 << 0))) count_up();       // button 1 pressed
        if (!(PINA & (1 << 1))) count_down();     // button 2 pressed
        if (!(PINA & (1 << 2))) button3_task();   // button 3 pressed
    }
}
