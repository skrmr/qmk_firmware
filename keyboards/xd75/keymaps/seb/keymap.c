#include QMK_KEYBOARD_H
#include "keyled.c"

#define BLINK 100
uint16_t blink_status = 0;
uint16_t blink_timer = 0;


enum {
	_QWERTY,
	_NUMPAD,
	_FN	
};

enum {
	KEYLED_TOG = SAFE_RANGE,
	LAYOUT
};

enum  {
	DEFAULT = 0, // Normal US intl layout
	ALTGR = 1 // Simulate dead keys on AltGr for US intl layout
} accent_layout = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_ortho_5x15(    
	 	KC_ESC     , KC_1    , KC_2      , KC_3      , KC_4      , KC_5    , KC_MINS   , KC_GRV  , KC_EQL  , KC_6    , KC_7    , KC_8    , KC_9    , KC_0    , KC_BSPC , 
	 	KC_TAB     , KC_Q    , KC_W      , KC_E      , KC_R      , KC_T    , KC_LBRC   , KC_BSLS , KC_RBRC , KC_Y    , KC_U    , KC_I    , KC_O    , KC_P    , KC_QUOT , 
		TO(_NUMPAD), KC_A    , KC_S      , KC_D      , KC_F      , KC_G    , KC_HOME   , KC_DEL  , KC_PGUP , KC_H    , KC_J    , KC_K    , KC_L    , KC_SCLN , KC_ENT  , 
		KC_LSFT    , KC_Z    , KC_X      , KC_C      , KC_V      , KC_B    , KC_END    , KC_UP   , KC_PGDN , KC_N    , KC_M    , KC_COMM , KC_DOT  , KC_SLSH , KC_RSFT , 
		KC_LCTL    , KC_LGUI , MO(_FN)   , KC_LALT   , KC_SPC    , KC_SPC  , KC_LEFT   , KC_DOWN , KC_RGHT , KC_SPC  , KC_SPC  , KC_RALT , MO(_FN) , KC_RGUI , KC_RCTL
	)
	,  
	[_NUMPAD] = LAYOUT_ortho_5x15(   
		XXXXXXX    , XXXXXXX , XXXXXXX   , XXXXXXX   , XXXXXXX   , XXXXXXX , XXXXXXX   , KC_SLSH , KC_ASTR , KC_MINS , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , _______     , 
		_______    , XXXXXXX , KC_UP     , XXXXXXX   , XXXXXXX   , XXXXXXX , KC_P7     , KC_P8   , KC_P9   , KC_PLUS , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX     , 
		TO(_QWERTY), KC_LEFT , KC_DOWN   , KC_RGHT   , XXXXXXX   , XXXXXXX , KC_P4     , KC_P5   , KC_P6   , KC_PLUS , XXXXXXX , XXXXXXX , XXXXXXX , _______ , _______     , 
		_______    , XXXXXXX , LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), KC_COMM , KC_P1     , KC_P2   , KC_P3   , KC_PENT , XXXXXXX , _______ , _______ , _______ , _______     , 
		_______    , _______ , _______   , _______   , _______   , _______ , KC_P0     , KC_P0   , KC_PDOT , KC_PENT , XXXXXXX , _______ , _______ , _______ , _______
	)		
	,
	[_FN] = LAYOUT_ortho_5x15(
		KC_SLEP    , KC_F1   , KC_F2     , KC_F3     , KC_F4     , KC_F5   , RGB_HUI   , RGB_VAI , RGB_SAI , XXXXXXX , XXXXXXX , XXXXXXX , KC_MINS , KC_EQL  , XXXXXXX    ,
		XXXXXXX    , KC_F6   , KC_F7     , KC_F8     , KC_F9     , KC_F10  , RGB_HUD   , RGB_VAD , RGB_SAD , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX    , 
		XXXXXXX    , KC_F11  , KC_F12    , XXXXXXX   , XXXXXXX   , XXXXXXX , RGB_MOD   , KC_INS  , RGB_RMOD, XXXXXXX , XXXXXXX , RESET   , XXXXXXX , XXXXXXX , KC_WAKE    ,
		_______    , XXXXXXX , XXXXXXX   , XXXXXXX   , XXXXXXX   , XXXXXXX , KEYLED_TOG, KC_CALC , RGB_TOG , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , _______    , 
		_______    , XXXXXXX , XXXXXXX   , _______   , XXXXXXX   , XXXXXXX , KC_VOLD   , KC_MUTE , KC_VOLU , XXXXXXX , XXXXXXX , XXXXXXX , _______ ,  XXXXXXX , LAYOUT    
	)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	int mods = get_mods();

	switch(keycode) {	
		case RESET:
			if (record->event.pressed) {
				for (int i=0; i<10; i++) {
					keycaps_led_on();
					_delay_ms(50);
					keycaps_led_off();
					_delay_ms(50);
				}
				reset_keyboard();
			}	
			return false;
		case KEYLED_TOG:
			if (record->event.pressed) {
				keyled_toggle();
			}
			return false;
	}

	if ((keycode == LAYOUT) && (record->event.pressed)) {
		switch (accent_layout) {
			default:
			case DEFAULT:
				accent_layout = ALTGR;
				break;
			case ALTGR:
				accent_layout = DEFAULT;
				break;
		}
		keyled_blink(accent_layout+1);
		return false;
	}

	switch(accent_layout) {
		case DEFAULT:
			break;
			// return true;
		case ALTGR:
			switch(keycode) {
				//
				// Accents
				//
				case KC_QUOT:
				case KC_GRV:
					if (record->event.pressed) {				
						if (mods & MOD_BIT(KC_RALT))	{
							unregister_code(KC_RALT);
							tap_code(keycode);
							register_code(KC_RALT);
						} else {		
							tap_code(keycode);
							tap_code(KC_SPC);
						}
					}
					return false;
				case KC_6:
					if (record->event.pressed) {
						if (mods & (MOD_BIT(KC_RALT))) {
							unregister_code(KC_RALT);
							register_code(KC_LSFT);
							tap_code(KC_6);
							unregister_code(KC_LSFT);
							register_code(KC_RALT);
						} else {
							tap_code(keycode);
							tap_code(KC_SPC);
						}
						return false;
					}
					 // else {
					//	return true;
					//}
			}
			break;
	}			
	return true;
}

layer_state_t layer_state_set_user(layer_state_t state)
{
	if (layer_state_cmp(state, _NUMPAD)) {		
		capslock_led_on();
	} else {
		capslock_led_off();
	}
	if (layer_state_cmp(state, _FN)) {
		gp100_led_on();
	} else {
		gp100_led_off();
	}
	return state;
}


void matrix_scan_user(void)
{
	keyled_matrix_scan_user();
}

