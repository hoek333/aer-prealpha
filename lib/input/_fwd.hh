#pragma once
namespace aer {

  // input.hh
  enum struct InputControllerKind;
  enum struct InputEventKind;
  struct InputEvent;
  class InputHandler;

#ifdef AER_HAS_LIB_X11
  class InputX11Adapter;
#endif

  class InputFallbackAdapter;

} // namespace aer
