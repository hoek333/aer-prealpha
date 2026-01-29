#pragma once

#if defined(_WIN32)
#include "input/input.hh"
#include "input/key.hh"
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
namespace aer {


  class InputWindowsAdapter : public InputPlatformAdapter {
    struct Impl;
    std::unique_ptr<Impl> pimpl;
    std::wstring window_name;

  public:
    InputWindowsAdapter(std::wstring window_name);
    ~InputWindowsAdapter();

    void poll_input(rigtorp::SPSCQueue<InputEvent> &queue,
                    const std::atomic<double> &epoch);
  };

  /**
   * @brief Translate a Windows extended virtual scan code to a Key value.
   * @note I'm not sure if everything in here is mapped correctly; feedback
   * is welcome.
   * @note A most of the CJK Key values map to already used, more common Windows
   * scancode. We have prioritized the latter; the cases where this happens are
   * specified in the implementation's comments.
   */
  Key translate_windows_vscex(USHORT vscex);


} // namespace aer
#endif
