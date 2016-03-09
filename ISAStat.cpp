#include "ISAStat.h"
#include <iostream>
#include <set>
#include <stack>

extern "C" {
// XXX: We define a dummy GetBits here to please the linker when using
// the ac_decoder library. We use the CreateDecoder function but we do not
// use the decoder per se, so we don't need a working GetBits().
void GetBits() __attribute__((weak));

void GetBits() {
  std::cerr << "Fatal error: GetBits() unimplemented.\n";
  exit(-1);
}

ac_dec_field *FindDecField(ac_dec_field *fields, int id);

}

ISAStat::ISAStat(ac_dec_format *FormatList, ac_dec_instr *InstrList,
                 uint32_t Wordsize)
    : ISA(CreateDecoder(FormatList, InstrList)) {

  NumInsns = 0;

  Opcodes.resize(ISA->nFields);

  struct WorklistItem {
    ac_decoder *Item;
    ac_decoder *Parent;
  };

  std::vector<std::set<ac_decoder *>> Parents(ISA->nFields);
  std::stack<WorklistItem> Worklist;
  Worklist.push({ISA->decoder, nullptr});

  while (!Worklist.empty()) {
    WorklistItem WI = Worklist.top();
    ac_decoder *Cur = WI.Item;
    ac_decoder *Parent = WI.Parent;
    uint32_t Id = Cur->check->id;
    Worklist.pop();
    if (Cur->next)
      Worklist.push({Cur->next, Parent});
    if (Cur->subcheck)
      Worklist.push({Cur->subcheck, Cur});

    OpcodeFieldInfo &Opcode = Opcodes[Id];
    ac_dec_field *FieldPtr = FindDecField(ISA->fields, Id);

    if (Opcode.name == nullptr) {
      Opcode.name = FieldPtr->name;
      uint32_t UsedBits =
          Parent ? Wordsize - Opcodes[Parent->check->id].PayloadBits : 0;
      UsedBits += FieldPtr->size;
      Opcode.PayloadBits = Wordsize - UsedBits;
      Opcode.PossibleEncodings = 1 << FieldPtr->size;
      Parents[Id].insert(Parent);
    }
    if (Parents[Id].count(Parent) == 0) {
      Parents[Id].insert(Parent);
      Opcode.PossibleEncodings += 1 << FieldPtr->size;
    }
    ++Opcode.UsedEncodings;
  }

  for (ac_dec_instr *InstrPtr = InstrList; InstrPtr != nullptr;
       InstrPtr = InstrPtr->next) {
    ++NumInsns;
  }
}

