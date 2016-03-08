#ifndef ISASTAT_H
#define ISASTAT_H

extern "C" {
#include "acpp.h"
}
#include <cstdint>

// Analyzes instructions of an ArchC model parsed by acpp. Supposes acpp is
// still loaded and its API is active.
class ISAStat {
  ac_dec_instr *InstrList;
public:
  ISAStat(ac_dec_instr *InstrList);

  void analyzeInstruction(const ac_dec_instr &Instr);

  uint32_t NumInsns;
};



#endif
