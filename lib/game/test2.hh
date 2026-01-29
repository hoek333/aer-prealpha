#pragma once
#include "core/fraction.hh"
#include <deque>
#include <optional>
#include <vector>
namespace aer {
  namespace chart {


    struct Note {
      Fraction time;
      int lane;
      std::optional<Fraction> length;
    };


    struct BPMMarker {
      Fraction time;
      std::optional<double> bpm;
    };


    struct Chart {
      int lanes;
      double offset;
      std::vector<Note> notes;
      std::vector<BPMMarker> bpms;
    };


    struct ChartPlayer {
      double epoch;
      double elapsed;
      std::deque<Note> notes;
      std::deque<BPMMarker> bpms;
    };


  }; // namespace chart
} // namespace aer
