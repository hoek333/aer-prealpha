#pragma once
#include "core/fraction.hh"
#include <raylib.h>
#include <string>
#include <vector>
namespace aer {
  namespace chart {


    /**
     * @class Note
     * @brief Represents a note
     *
     */
    struct Note {
      // single note
      Fraction time; // hit time
      int lane;      // lane

      // additional data: hold
      Fraction length{}; // note length
    };


    /**
     * @class BPM
     * @brief BPM change
     *
     */
    struct Marker {
      Fraction time;
      double bpm;
    };


    /**
     * @class ChartVariant
     * @brief A single playable chart variant
     *
     */
    struct Variant {
      // meta
      std::string name;
      int lanes;
      double offset;

      // data
      std::vector<Marker> markers;
      std::vector<Note> notes;
    };


    /**
     * @class Chart
     * @brief A chart with multiple difficulties (called variants here)
     *
     */
    struct Chart {
      // meta
      std::string title;
      std::string artist;
      std::string author;

      // data
      std::vector<Variant> variants;
    };


    struct ChartPlayer {
      Chart *chart;
      double elapsed;
    };


  } // namespace chart


} // namespace aer
