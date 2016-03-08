#include "ISAStat.h"
#include <iostream>

extern "C" {
// XXX: We define a dummy GetBits here to please the linker when using
// the ac_decoder library. We use the CreateDecoder function but we do not
// use the decoder per se, so we don't need a working GetBits().
void GetBits() __attribute__((weak));

void GetBits() {
  std::cerr << "Fatal error: GetBits() unimplemented.\n";
  exit(-1);
}

}

ISAStat::ISAStat(ac_dec_format *FormatList, ac_dec_instr *InstrList)
    : ISA(CreateDecoder(FormatList, InstrList)) {
  NumInsns = 0;

  for (ac_dec_instr *InstrPtr = InstrList; InstrPtr != nullptr;
       InstrPtr = InstrPtr->next) {
    analyzeInstruction(*InstrPtr);
    ++NumInsns;
  }
}

void ISAStat::analyzeInstruction(const ac_dec_instr &Instr) {
  ac_dec_format *FormatPtr = find_format(Instr.format);

  for (ac_dec_list *DecPtr = Instr.dec_list; DecPtr != nullptr;
       DecPtr = DecPtr->next) {
    ac_dec_field *FieldPtr = find_field(FormatPtr, DecPtr->name);

  }
}
