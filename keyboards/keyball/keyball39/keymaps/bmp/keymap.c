/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "quantum.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  ,
    LCTL_T(KC_Z),KC_X   , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
    SCRL_TO  , _______  , _______  , _______  ,LT(2,KC_SPC),LSFT_T(KC_ESC),RCTL_T(KC_BSPC),LT(1,KC_ENT),RALT_T(KC_DEL),_______,_______,_______
  ),

  [1] = LAYOUT_universal(
    KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                            KC_6     , KC_7     , KC_8     , KC_9     , KC_0     ,
    KC_TAB   , _______  , _______  ,LCTL(KC_SPC),_______ ,                            KC_LEFT  , KC_DOWN  , KC_UP    , KC_RIGHT , KC_RGUI  ,
    KC_LCTL  , KC_BTN3  , KC_BTN2  , KC_BTN1  , KC_BTN4  ,                            _______  , KC_END   , KC_HOME  , _______  , _______  ,
    AML_TO   , _______  , _______  , _______  , MO(3)    , _______  ,      _______  , _______  ,RCTL_T(KC_RALT),_______,_______ , _______  
  ),

  [2] = LAYOUT_universal(
    S(KC_1)  , S(KC_2)  , S(KC_3)  , S(KC_4)  , S(KC_5)  ,                            S(KC_6)  , S(KC_7)  , S(KC_8)  , S(KC_EQL), KC_MINS  ,
    KC_TAB   ,S(KC_LBRC), KC_LBRC  , S(KC_9)  , KC_EQL   ,                           S(KC_MINS), S(KC_0)  , KC_RBRC  ,S(KC_RBRC),S(KC_BSLS),
    KC_LCTL  , KC_BTN3  , KC_BTN2  , KC_BTN1  , KC_BTN4  ,                            KC_GRV   , KC_QUOT  ,S(KC_QUOT), S(KC_GRV), KC_BSLS  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,      _______  , MO(3)    , KC_PSCR  , _______  , _______  , _______
  ),

  [3] = LAYOUT_universal(
    0x7e0a   , 0x7e02   , 0x7e03   , 0x7e04   , 0x7e05   ,                            _______  , CPI_D100 , CPI_I100 , _______  , _______  ,
    0x7e14   , _______  , _______  , _______  , _______  ,                            _______  , _______  , _______  , _______  , _______  ,
    SCRL_MO  , KC_BTN3  , KC_BTN2  , KC_BTN1  , KC_BTN4  ,                            _______  , _______  , _______  , _______  , _______  ,
    SCRL_TO  , _______  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is 3
    // keyball_set_scroll_mode(get_highest_layer(state) == 3);
    // keyball_set_scroll_mode(get_highest_layer(state) == 1 || get_highest_layer(state) == 3);
    keyball_set_scroll_mode(get_highest_layer(state) == 1);
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
