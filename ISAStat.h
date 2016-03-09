#ifndef ISASTAT_H
#define ISASTAT_H

extern "C" {
#include "acpp.h"
}
#include <cstdint>
#include <memory>
#include <vector>

// Aggregates information about all encodings that use this field
struct FieldUsage {
  FieldUsage(uint32_t ParentId, uint32_t PayloadBits,
             uint32_t PossibleEncodings, uint32_t UsedEncodings,
             uint32_t FinalEncodings)
      : ParentId(ParentId), PayloadBits(PayloadBits),
        PossibleEncodings(PossibleEncodings), UsedEncodings(UsedEncodings),
        FinalEncodings(FinalEncodings) {}
  // Field id of the parent opcode checked to arrive at this encoding
  uint32_t ParentId = {0};
  // Number of bits available in the word, deducing the necessary to encode
  // this opcode
  uint32_t PayloadBits = {0};
  // Number of different instructions that can be encoded in this field, with
  // this given parent, with this payload
  uint32_t PossibleEncodings = {0};
  // Number of actual instructions encoded in this field
  uint32_t UsedEncodings = {0};
  // Number of instructions that use this field as a final check to establish
  // their id
  uint32_t FinalEncodings = {0};
};

// Analyzes instructions of an ArchC model parsed by acpp. Supposes acpp is
// still loaded and its API is active.
class ISAStat {
  void BumpPossibleEncodings(uint32_t FieldId, uint32_t ParentId,
                             uint32_t PayloadBits);

public:
  ISAStat(ac_dec_format *FormatList, ac_dec_instr *InstrList,
          uint32_t Wordsize);

  const char *GetFieldName(uint32_t Id);

  // Maps a format field to the number of different known opcodes encoded
  // in this field. First vector is indexed by field id, second vector is not
  // indexed, but simply a container of multiple OpcodeFieldInfo related to
  // the same field.
  std::vector<std::vector<FieldUsage>> Opcodes;

  // XXX: ArchC does not provide an interface to delete this object, allocate
  // it with unique_ptr here, but understand that this creates a memory leak
  // for internal objects of this structure
  std::unique_ptr<ac_decoder_full> ISA;

  uint32_t NumInsns;
};



#endif
