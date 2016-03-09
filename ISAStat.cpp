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

void ISAStat::BumpPossibleEncodings(uint32_t FieldId, uint32_t ParentId,
                                    uint32_t PayloadBits) {
  std::vector<FieldUsage> &FieldUsages = Opcodes[FieldId];
  bool Found = false;
  uint32_t PossibleEncodings = 1 << FindDecField(ISA->fields, FieldId)->size;

  for (auto &Field : FieldUsages) {
    if (Field.ParentId != ParentId ||
        Field.PayloadBits != PayloadBits)
      continue;
    Found = true;
    Field.PossibleEncodings += PossibleEncodings;
  }

  if (!Found) {
    FieldUsages.push_back({ParentId, PayloadBits, PossibleEncodings,
                           /*UsedEncodings=*/0U, /*FinalEncodings=*/0U});
  }
}

const char *ISAStat::GetFieldName(uint32_t Id) {
  return FindDecField(ISA->fields, Id)->name;
}

ISAStat::ISAStat(ac_dec_format *FormatList, ac_dec_instr *InstrList,
                 uint32_t Wordsize)
    : ISA(CreateDecoder(FormatList, InstrList)) {

  NumInsns = 0;

  Opcodes.resize(ISA->nFields);

  struct WorklistItem {
    ac_decoder *Item;
    ac_decoder *Parent;
    uint32_t PayloadBits;
  };

  std::stack<WorklistItem> Worklist;
  Worklist.push({ISA->decoder, nullptr, 32U});

  while (!Worklist.empty()) {
    WorklistItem WI = Worklist.top();
    ac_decoder *Cur = WI.Item;
    ac_decoder *Parent = WI.Parent;
    uint32_t Id = Cur->check->id;
    ac_dec_field *FieldPtr = FindDecField(ISA->fields, Id);
    uint32_t PayloadBits = WI.PayloadBits - FieldPtr->size;
    uint32_t ParentId = Parent? Parent->check->id : 0;
    Worklist.pop();

    // Successors of the DFS
    if (Cur->next) // sibling
      Worklist.push({Cur->next, Parent, WI.PayloadBits});
    if (!Cur->found && Cur->subcheck) { // child
      ac_dec_field *ChildField =
          FindDecField(ISA->fields, Cur->subcheck->check->id);
      BumpPossibleEncodings(Cur->subcheck->check->id, Id,
                            PayloadBits - ChildField->size);
      Worklist.push({Cur->subcheck, Cur, PayloadBits});
    }

    // Find the entry for the tuple <Field id, Parent id, Used bits>. If it
    // does not exist, create one.
    std::vector<FieldUsage> &FieldUsages = Opcodes[Id];
    bool Found = false;
    for (auto &Field : FieldUsages) {
      if (Field.ParentId != ParentId ||
          Field.PayloadBits != PayloadBits)
        continue;
      Found = true;
      ++Field.UsedEncodings;
      if (Cur->found) {
        ++Field.FinalEncodings;
        Field.InstrNames.push_back(Cur->found->name);
      }
    }

    if (!Found) {
      FieldUsages.push_back({ParentId, PayloadBits, 1U << FieldPtr->size, 1,
                             Cur->found ? 1U : 0U});
      if (Cur->found)
        FieldUsages.back().InstrNames.push_back(Cur->found->name);
    }
  }

  // Aggregate everything by Payload size
  Payloads.resize(Wordsize);
  for (const auto &FieldUsages : Opcodes) {
    for (const auto &Field : FieldUsages) {
      Payloads[Field.PayloadBits].UsedEncodings += Field.UsedEncodings;
      Payloads[Field.PayloadBits].PossibleEncodings += Field.PossibleEncodings;
    }
  }

  for (ac_dec_instr *InstrPtr = InstrList; InstrPtr != nullptr;
       InstrPtr = InstrPtr->next) {
    ++NumInsns;
  }
}

