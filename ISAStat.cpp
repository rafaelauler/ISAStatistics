#include "ISAStat.h"

ISAStat::ISAStat(ac_dec_instr *InstrList) : InstrList(InstrList) {
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
