
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

  ISAStat Stats(format_ins_list, instr_list);

  std::cout << "Model has " << Stats.NumInsns << " instructions.\n";
  acppUnload();
  return 0;
}
