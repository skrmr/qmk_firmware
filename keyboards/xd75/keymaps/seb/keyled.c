

enum {
    off = 0,
    on
} keyled_status = 0;

uint16_t keyled_blink_status = 0;
uint16_t keyled_blink_time = 0;
const uint16_t keyled_blink_length = 100; // ms

void keyled_toggle(void);
void keyled_blink(uint16_t times);
void keyled_matrix_scan_user(void);

void keyled_toggle(void)
{
	keyled_status = !keyled_status;
	if (keyled_status) {
		keycaps_led_on();
	} else {
		keycaps_led_off();
	}
}

#define KEYLED_ON  if (!keyled_status) { keycaps_led_on();  } else { keycaps_led_off(); }
#define KEYLED_OFF if (!keyled_status) { keycaps_led_off(); } else { keycaps_led_on();  }


void keyled_matrix_scan_user(void)
{
    if (keyled_blink_status) {
        if (timer_elapsed(keyled_blink_time) > keyled_blink_length) {
            if (keyled_blink_status & 0x1) {
                KEYLED_OFF
            } else {
                KEYLED_ON
            }            
            keyled_blink_status--;            
            if (keyled_blink_status) {
                keyled_blink_time=timer_read();
            }
        }
    }
}

void keyled_blink(uint16_t times)
{
    keyled_blink_status = (times << 1) - 1;
    keyled_blink_time = timer_read();    
    KEYLED_ON
}
