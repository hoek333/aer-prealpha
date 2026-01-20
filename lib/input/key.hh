#pragma once
#include <cstdint>
#include <raylib.h>
namespace aer {


  /*
   * @class Key
   * @brief Keyboard Key keycode enum. The underlying values correspond to X11
   * physical keycodes. All keys except for the japanese and korean ones are
   * named according to a standard QWERTY US keyboard. All evdev keycodes except
   * for Solaris and Internet specific ones are supported. This enum struct is
   * based on my computer's evdev keycode file.
   *
   */
  enum struct Key : uint8_t {
    GRAVE = 0,
    ONE = 10,
    TWO = 11,
    THREE = 12,
    FOUR = 13,
    FIVE = 14,
    SIX = 15,
    SEVEN = 16,
    EIGHT = 17,
    NINE = 18,
    ZERO = 19,
    MINUS = 20,
    EQUAL = 21,
    BACKSPACE = 22,

    TAB = 23,
    Q = 24,
    W = 25,
    E = 26,
    R = 27,
    T = 28,
    Y = 29,
    U = 30,
    I = 31,
    O = 32,
    P = 33,
    BRACKET_LEFT = 34,
    BRACKET_RIGHT = 35,
    RETURN = 36,

    CAPS = 66,
    A = 38,
    S = 39,
    D = 40,
    F = 41,
    G = 42,
    H = 43,
    J = 44,
    K = 45,
    L = 46,
    SEMICOLON = 47,
    APOSTROPHE = 48,
    BACKSLASH = 49,

    LEFT_SHIFT = 50,
    LSGT = 94,
    Z = 52,
    X = 53,
    C = 54,
    V = 55,
    B = 56,
    N = 57,
    M = 58,
    COMMA = 59,
    PERIOD = 60,
    SLASH = 61,
    RIGHT_SHIFT = 62,

    LEFT_CONTROL = 37,
    LEFT_SUPER = 133,
    LEFT_ALT = 64,
    SPACE = 65,
    RIGHT_ALT = 108,
    RIGHT_SUPER = 134,
    MENU = 135,
    RIGHT_CONTROL = 105,

    ESC = 9,
    F1 = 67,
    F2 = 68,
    F3 = 69,
    F4 = 70,
    F5 = 71,
    F6 = 72,
    F7 = 73,
    F8 = 74,
    F9 = 75,
    F10 = 76,
    F11 = 77,
    F12 = 78,

    F13 = 191,
    F14 = 192,
    F15 = 193,
    F16 = 194,
    F17 = 195,
    F18 = 196,
    F19 = 197,
    F20 = 198,
    F21 = 199,
    F22 = 200,
    F23 = 201,
    F24 = 202,

    PRINT_SCREEN = 107,
    SCROLL_LOCK = 78,
    PAUSE = 127,

    INSERT = 118,
    HOME = 110,
    PAGE_UP = 112,
    DELETE = 119,
    END = 115,
    PAGE_DOWN = 117,

    UP = 111,
    LEFT = 113,
    DOWN = 116,
    RIGHT = 114,

    NUM_LOCK = 77,
    NUM_DIVIDE = 106,
    NUM_MULTIPLY = 63,
    NUM_SUBTRACT = 82,
    NUM_SEVEN = 79,
    NUM_EIGHT = 80,
    NUM_NINE = 81,
    NUM_ADD = 86,
    NUM_FOUR = 83,
    NUM_FIVE = 84,
    NUM_SIX = 85,
    NUM_ONE = 87,
    NUM_TWO = 88,
    NUM_THREE = 89,
    NUM_ENTER = 104,
    NUM_ZERO = 90,
    NUM_DELETE = 91,
    NUM_EQUAL = 125,

    HANKAKU_ZENKAKU = GRAVE,
    HIRAGANA_KATAKANA = 101,
    BACKSLASH_UNDERSCORE = 97,
    HENKAN = 100,
    MUHENKAN = 102,
    YEN = 132,
    KATAKANA = 98,
    HIRAGANA = 99,
    KP_JP_COMMA = 103,

    HANGUL_LATIN = 130,
    HANGUL_HANJA = 131,
  };


} // namespace aer
