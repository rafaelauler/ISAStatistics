
extern "C" {
#include "acpp.h"
}

#include "ISAStat.h"
#include <iostream>
#include <cstdint>

int
main(int argc, char **argv) {
  acppInit(0);

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <modelname.ac>\n";
    return -1;
  }

  if (!acppLoad(argv[1])) {
    std::cerr << "Could not open ISA input file: " << argv[1] << "\n";
    std::cerr << "Parser terminated unsuccessfully.\n";
    return -1;
  }

  if (acppRun()) {
    std::cerr << "Parser terminated unsuccessfully.\n";
    return -1;
  }
  acppUnload();

  if (isa_filename == nullptr) {
    std::cerr << "No ISA file defined";
  }

  if (!acppLoad(isa_filename)) {
    std::cerr << "Could not open ISA input file: " << isa_filename << "\n";
    std::cerr << "Parser terminated unsuccessfully.\n";
    return -1;
  }

  if (acppRun()) {
    std::cerr << "Parser terminated unsuccessfully for " << isa_filename
              << "\n";
  }

  if (wordsize == 0) {
    std::cerr << "Warning: No wordsize defined. Default value is 32 bits.\n";
    wordsize = 32;
  }

  ISAStat Stats(format_ins_list, instr_list, wordsize);

  std::cout << "Model has " << Stats.NumInsns << " instructions.\n";

  std::cout << "Model has " << Stats.ISA->nFields << " fields.\n";

  for (uint32_t I = 0, E = Stats.Opcodes.size(); I != E; ++I) {
    const auto &FieldUsages = Stats.Opcodes[I];
    if (FieldUsages.size() == 0)
      continue;
    std::cout << "Field " << Stats.GetFieldName(I) << "\n";
    for (const auto &Field : FieldUsages) {
      const double Usage =
          Field.UsedEncodings / (double)Field.PossibleEncodings * 100.0;
      std::cout << "\tUsage rate: " << Usage << "%\n";
      std::cout << "\tPayload Bits: " << Field.PayloadBits << "\n";
      std::cout << "\tPossible encodings: " << Field.PossibleEncodings << "\n";
      std::cout << "\tUsed encodings: " << Field.UsedEncodings << "\n";
      std::cout << "\tFinal encodings: " << Field.FinalEncodings << " ";
      if (Field.FinalEncodings) {
        std::cout << "(";
        auto Sep = "";
        for (auto &Name : Field.InstrNames) {
          std::cout << Sep << Name;
          Sep = ", ";
        }
        std::cout << ")";
      }
      std::cout << "\n\t---------------------------\n";
    }
    std::cout << "===========================\n";
  }

  acppUnload();
  return 0;
}
