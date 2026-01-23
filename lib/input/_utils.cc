#include "input/_utils.hh"
#include <raylib.h>


// we have to define this function in a separate file to avoid naming conflicts
// between system libs and raylib
bool aer::is_raylib_window_focused() { return IsWindowFocused(); }
