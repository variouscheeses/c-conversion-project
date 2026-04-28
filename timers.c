// 500 microsecond delay function
void delay_500us(void) {
    volatile unsigned char r21, r20;

    for (r21 = 86; r21 > 0; r21--) {
        for (r20 = 30; r20 > 0; r20--) {
        }
    }
}

// 333 microsecond delay function
void delay_333us(void) {
    volatile unsigned char r21, r20;

    for (r21 = 57; r21 > 0; r21--) {
        for (r20 = 30; r20 > 0; r20--) {
        }
    }
}

// 0.1 second delay function
void delay_tenth_sec(void) {
    volatile unsigned char r16;

    for (r16 = 200; r16 > 0; r16--) {
        delay_500us();
    }
}
