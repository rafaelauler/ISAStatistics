# ISAStatistics
Read an ArchC model and extract info about the opcode space.

This is a standalone tool that uses the ArchC library to read an ISA model
and print useful information about this ISA.

License
-------
 - GNU GPLv3

Build
-----

```bash
git clone http://github.com/rafaelauler/ISAStatistics
mkdir isastat-build
cd isastat-build
cmake ../ISAStatistics
make
```

Sample output
-------------

This is the statistics generated after reading the MIPS-I ArchC model:
  
```bash
$ isastat mips.ac

  Decoding tree information:

  Field op
          Usage rate: 43.75%
          Payload Bits: 26
          Possible encodings: 64
          Used encodings: 28
          Final encodings: 23 (lb, lbu, lh, lhu, lw, lwl, lwr, sb, sh, sw, swl, swr, addi, addiu, slti, sltiu, andi, ori, xori, j, jal, beq, bne)
          ---------------------------
  ===========================
  Field rs
          Usage rate: 3.125%
          Payload Bits: 21
          Possible encodings: 32
          Used encodings: 1
          Final encodings: 1 (lui)
          ---------------------------
  ===========================
  Field rt
          Usage rate: 6.25%
          Payload Bits: 21
          Possible encodings: 96
          Used encodings: 6
          Final encodings: 6 (blez, bgtz, bltz, bgez, bltzal, bgezal)
          ---------------------------
===========================
  Field rd
          Usage rate: 3.125%
          Payload Bits: 21
          Possible encodings: 32
          Used encodings: 1
          Final encodings: 0
          ---------------------------
  ===========================
  Field func
          Usage rate: 43.75%
          Payload Bits: 20
          Possible encodings: 64
          Used encodings: 28
          Final encodings: 28 (add, addu, sub, subu, slt, sltu, instr_and, instr_or, instr_xor, instr_nor, sll, srl, sra, sllv, srlv, srav, mult, multu, div, divu, mfhi, mthi, mflo, mtlo, jr, jalr, sys_call, instr_break)
          ---------------------------
          Usage rate: 1.5625%
          Payload Bits: 15
          Possible encodings: 64
          Used encodings: 1
          Final encodings: 1 (nop)
          ---------------------------
  ===========================

  Summary:
          Payload: 15 bits.
                  Usage rate: 1.5625%
                  Possible encodings: 64
                  Used encodings: 1
          Payload: 20 bits.
                  Usage rate: 43.75%
                  Possible encodings: 64
                  Used encodings: 28
          Payload: 21 bits.
                  Usage rate: 5%
                  Possible encodings: 160
                  Used encodings: 8
          Payload: 26 bits.
                  Usage rate: 43.75%
                  Possible encodings: 64
                  Used encodings: 28

  Model has 59 instructions.
  Model has 13 fields.

  Total opcode space utilization is 18.4659% (65 out of 352)
```