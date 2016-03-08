#ifndef ISASTAT_H
#define ISASTAT_H

extern "C" {
#include "acpp.h"
}
#include <cstdint>
#include <memory>

// Analyzes instructions of an ArchC model parsed by acpp. Supposes acpp is
// still loaded and its API is active.
class ISAStat {
  // XXX: ArchC does not provide an interface to delete this object, allocate
  // it with unique_ptr here, but understand that this creates a memory leak
  // for internal objects of this structure
  std::unique_ptr<ac_decoder_full> ISA;
public:
  ISAStat(ac_dec_format *FormatList, ac_dec_instr *InstrList);

  void analyzeInstruction(const ac_dec_instr &Instr);

  uint32_t NumInsns;
};



#endif
