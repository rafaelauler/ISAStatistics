#ifndef ISASTAT_H
#define ISASTAT_H

extern "C" {
#include "acpp.h"
}
#include <cstdint>
#include <memory>
#include <vector>

struct OpcodeFieldInfo {
  const char *name = {0};
  uint32_t PayloadBits = {0};
  uint32_t PossibleEncodings = {0};
  uint32_t UsedEncodings = {0};
};

// Analyzes instructions of an ArchC model parsed by acpp. Supposes acpp is
// still loaded and its API is active.
class ISAStat {
public:
  ISAStat(ac_dec_format *FormatList, ac_dec_instr *InstrList,
          uint32_t Wordsize);

  // Maps a format field to the number of different known opcodes encoded
  // in this field.
  std::vector<OpcodeFieldInfo> Opcodes;

  // XXX: ArchC does not provide an interface to delete this object, allocate
  // it with unique_ptr here, but understand that this creates a memory leak
  // for internal objects of this structure
  std::unique_ptr<ac_decoder_full> ISA;

  uint32_t NumInsns;
};



#endif
