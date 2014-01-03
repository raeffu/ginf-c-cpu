/*
  6502-decode.c
  bho1 10.7.2011

  GPL applies
*/

#include <stdio.h>
#include <search.h>

#include "register.h"
#include "memory.h"

#include "cpu-util.h"
#include "cpu.h"

//#include "6502-opcodes.h"
#include "decode.h"

#include "mu-code.h"

/*
  array of structures containing the essentials:
  - text describing the command,
  - and int encoding the command,
  - function pointer executing the command

  
  ATTENTION: When adding new entries, behold of the comma between entries (but the last)
  When adding new entries don't forget to implement them (in mu-code.c and declare them in mu_code.h
  
*/

/*
  structure generated by script
  ./milking.bash | sort -k3
  and some hand edit
*/
struct cpu_operator operations[OPS] =
  {
	
    /* 
       Logical and arithmetic instructions with addressing modes: 
       Addressing modes: Register direkt, Register indirekt, Memory direkt, Immediate
    */


{ "BRK", 0x00, cpu_6502_brk_imp, "imp", "interrupt", "BRK impl", "(S)-=:PC,P PC:=($FFFE) ", "--1-1--" },
{ "ORA", 0x01, cpu_6502_ora_izx, "izx", "or with accumulator", "ORA X,ind", "A:=A or {adr} ", "*----*-" },
{ "ORA", 0x05, cpu_6502_ora_zp, "zp", "or with accumulator", "ORA zpg", "A:=A or {adr} ", "*----*-" },
{ "ASL", 0x06, cpu_6502_asl_zp, "zp", "arithmetic shift left", "ASL zpg", "{adr}:={adr}*2 ", "*----**" },
{ "PHP", 0x08, cpu_6502_php_imp, "imp", "push processor status (SR)", "PHP impl", "(S)-:=P ", "-------" },
{ "ORA", 0x09, cpu_6502_ora_imm, "imm", "or with accumulator", "ORA #", "A:=A or {adr} ", "*----*-" },
{ "ASL", 0x0A, cpu_6502_asl_imp, "imp", "arithmetic shift left", "ASL A", "{adr}:={adr}*2 ", "*----**" },
{ "ORA", 0x0D, cpu_6502_ora_abs, "abs", "or with accumulator", "ORA abs", "A:=A or {adr} ", "*----*-" },
{ "ASL", 0x0E, cpu_6502_asl_abs, "abs", "arithmetic shift left", "ASL abs", "{adr}:={adr}*2 ", "*----**" },
{ "BPL", 0x10, cpu_6502_bpl_rel, "rel", "branch on plus (negative clear)", "BPL rel", "branch on N=0 ", "-------" },
{ "ORA", 0x11, cpu_6502_ora_izy, "izy", "or with accumulator", "ORA ind,Y", "A:=A or {adr} ", "*----*-" },
{ "ORA", 0x15, cpu_6502_ora_zpx, "zpx", "or with accumulator", "ORA zpg,X", "A:=A or {adr} ", "*----*-" },
{ "ASL", 0x16, cpu_6502_asl_zpx, "zpx", "arithmetic shift left", "ASL zpg,X", "{adr}:={adr}*2 ", "*----**" },
{ "CLC", 0x18, cpu_6502_clc_imp, "imp", "clear carry", "CLC impl", "C:=0 ", "------0" },
{ "ORA", 0x19, cpu_6502_ora_aby, "aby", "or with accumulator", "ORA abs,Y", "A:=A or {adr} ", "*----*-" },
{ "ORA", 0x1D, cpu_6502_ora_abx, "abx", "or with accumulator", "ORA abs,X", "A:=A or {adr} ", "*----*-" },
{ "ASL", 0x1E, cpu_6502_asl_abx, "abx", "arithmetic shift left", "ASL abs,X", "{adr}:={adr}*2 ", "*----**" },
{ "JSR", 0x20, cpu_6502_jsr_abs, "abs", "jump subroutine", "JSR abs", "(S)-:=PC PC:={adr} ", "-------" },
{ "AND", 0x21, cpu_6502_and_izx, "izx", "and (with accumulator)", "AND X,ind", "A:=A&{adr} ", "*----*-" },
{ "BIT", 0x24, cpu_6502_bit_zp, "zp", "bit test", "BIT zpg", "N:=b7 V:=b6 Z:=A&{adr} ", "**---*-" },
{ "AND", 0x25, cpu_6502_and_zp, "zp", "and (with accumulator)", "AND zpg", "A:=A&{adr} ", "*----*-" },
{ "ROL", 0x26, cpu_6502_rol_zp, "zp", "rotate left", "ROL zpg", "{adr}:={adr}*2+C ", "*----**" },
{ "PLP", 0x28, cpu_6502_plp_imp, "imp", "pull processor status (SR)", "PLP impl", "P:=+(S) ", "**-****" },
{ "AND", 0x29, cpu_6502_and_imm, "imm", "and (with accumulator)", "AND #", "A:=A&{adr} ", "*----*-" },
{ "ROL", 0x2A, cpu_6502_rol_imp, "imp", "rotate left", "ROL A", "{adr}:={adr}*2+C ", "*----**" },
{ "BIT", 0x2C, cpu_6502_bit_abs, "abs", "bit test", "BIT abs", "N:=b7 V:=b6 Z:=A&{adr} ", "**---*-" },
{ "AND", 0x2D, cpu_6502_and_abs, "abs", "and (with accumulator)", "AND abs", "A:=A&{adr} ", "*----*-" },
{ "ROL", 0x2E, cpu_6502_rol_abs, "abs", "rotate left", "ROL abs", "{adr}:={adr}*2+C ", "*----**" },
{ "BMI", 0x30, cpu_6502_bmi_rel, "rel", "branch on minus (negative set)", "BMI rel", "branch on N=1 ", "-------" },
{ "AND", 0x31, cpu_6502_and_izy, "izy", "and (with accumulator)", "AND ind,Y", "A:=A&{adr} ", "*----*-" },
{ "AND", 0x35, cpu_6502_and_zpx, "zpx", "and (with accumulator)", "AND zpg,X", "A:=A&{adr} ", "*----*-" },
{ "ROL", 0x36, cpu_6502_rol_zpx, "zpx", "rotate left", "ROL zpg,X", "{adr}:={adr}*2+C ", "*----**" },
{ "SEC", 0x38, cpu_6502_sec_imp, "imp", "set carry", "SEC impl", "C:=1 ", "------1" },
{ "AND", 0x39, cpu_6502_and_aby, "aby", "and (with accumulator)", "AND abs,Y", "A:=A&{adr} ", "*----*-" },
{ "AND", 0x3D, cpu_6502_and_abx, "abx", "and (with accumulator)", "AND abs,X", "A:=A&{adr} ", "*----*-" },
{ "ROL", 0x3E, cpu_6502_rol_abx, "abx", "rotate left", "ROL abs,X", "{adr}:={adr}*2+C ", "*----**" },
{ "RTI", 0x40, cpu_6502_rti_imp, "imp", "return from interrupt", "RTI impl", "P,PC:=+(S) ", "**-****" },
{ "EOR", 0x41, cpu_6502_eor_izx, "izx", "exclusive or (with accumulator)", "EOR X,ind", "A:=A exor {adr} ", "*----*-" },
{ "EOR", 0x45, cpu_6502_eor_zp, "zp", "exclusive or (with accumulator)", "EOR zpg", "A:=A exor {adr} ", "*----*-" },
{ "LSR", 0x46, cpu_6502_lsr_zp, "zp", "logical shift right", "LSR zpg", "{adr}:={adr}/2 ", "*----**" },
{ "PHA", 0x48, cpu_6502_pha_imp, "imp", "push accumulator", "PHA impl", "(S)-:=A ", "-------" },
{ "EOR", 0x49, cpu_6502_eor_imm, "imm", "exclusive or (with accumulator)", "EOR #", "A:=A exor {adr} ", "*----*-" },
{ "LSR", 0x4A, cpu_6502_lsr_imp, "imp", "logical shift right", "LSR A", "{adr}:={adr}/2 ", "*----**" },
{ "JMP", 0x4C, cpu_6502_jmp_abs, "abs", "jump", "JMP abs", "PC:={adr} ", "-------" },
{ "EOR", 0x4D, cpu_6502_eor_abs, "abs", "exclusive or (with accumulator)", "EOR abs", "A:=A exor {adr} ", "*----*-" },
{ "LSR", 0x4E, cpu_6502_lsr_abs, "abs", "logical shift right", "LSR abs", "{adr}:={adr}/2 ", "*----**" },
{ "BVC", 0x50, cpu_6502_bvc_rel, "rel", "branch on overflow clear", "BVC rel", "branch on V=0 ", "-------" },
{ "EOR", 0x51, cpu_6502_eor_izy, "izy", "exclusive or (with accumulator)", "EOR ind,Y", "A:=A exor {adr} ", "*----*-" },
{ "EOR", 0x55, cpu_6502_eor_zpx, "zpx", "exclusive or (with accumulator)", "EOR zpg,X", "A:=A exor {adr} ", "*----*-" },
{ "LSR", 0x56, cpu_6502_lsr_zpx, "zpx", "logical shift right", "LSR zpg,X", "{adr}:={adr}/2 ", "*----**" },
{ "CLI", 0x58, cpu_6502_cli_imp, "imp", "clear interrupt disable", "CLI impl", "I:=0 ", "----0--" },
{ "EOR", 0x59, cpu_6502_eor_aby, "aby", "exclusive or (with accumulator)", "EOR abs,Y", "A:=A exor {adr} ", "*----*-" },
{ "EOR", 0x5D, cpu_6502_eor_abx, "abx", "exclusive or (with accumulator)", "EOR abs,X", "A:=A exor {adr} ", "*----*-" },
{ "LSR", 0x5E, cpu_6502_lsr_abx, "abx", "logical shift right", "LSR abs,X", "{adr}:={adr}/2 ", "*----**" },
{ "RTS", 0x60, cpu_6502_rts_imp, "imp", "return from subroutine", "RTS impl", "PC:=+(S) ", "-------" },
{ "ADC", 0x61, cpu_6502_adc_izx, "izx", "add with carry", "ADC X,ind", "A:=A+{adr} ", "**---**" },
{ "ADC", 0x65, cpu_6502_adc_zp, "zp", "add with carry", "ADC zpg", "A:=A+{adr} ", "**---**" },
{ "ROR", 0x66, cpu_6502_ror_zp, "zp", "rotate right", "ROR zpg", "{adr}:={adr}/2+C*128 ", "*----**" },
{ "PLA", 0x68, cpu_6502_pla_imp, "imp", "pull accumulator", "PLA impl", "A:=+(S) ", "*----*-" },
{ "ADC", 0x69, cpu_6502_adc_imm, "imm", "add with carry", "ADC #", "A:=A+{adr} ", "**---**" },
{ "ROR", 0x6A, cpu_6502_ror_imp, "imp", "rotate right", "ROR A", "{adr}:={adr}/2+C*128 ", "*----**" },
{ "JMP", 0x6C, cpu_6502_jmp_ind, "ind", "jump", "JMP ind", "PC:={adr} ", "-------" },
{ "ADC", 0x6D, cpu_6502_adc_abs, "abs", "add with carry", "ADC abs", "A:=A+{adr} ", "**---**" },
{ "ROR", 0x6E, cpu_6502_ror_abs, "abs", "rotate right", "ROR abs", "{adr}:={adr}/2+C*128 ", "*----**" },
{ "BVS", 0x70, cpu_6502_bvs_rel, "rel", "branch on overflow set", "BVS rel", "branch on V=1 ", "-------" },
{ "ADC", 0x71, cpu_6502_adc_izy, "izy", "add with carry", "ADC ind,Y", "A:=A+{adr} ", "**---**" },
{ "ADC", 0x75, cpu_6502_adc_zpx, "zpx", "add with carry", "ADC zpg,X", "A:=A+{adr} ", "**---**" },
{ "ROR", 0x76, cpu_6502_ror_zpx, "zpx", "rotate right", "ROR zpg,X", "{adr}:={adr}/2+C*128 ", "*----**" },
{ "SEI", 0x78, cpu_6502_sei_imp, "imp", "set interrupt disable", "SEI impl", "I:=1 ", "----1--" },
{ "ADC", 0x79, cpu_6502_adc_aby, "aby", "add with carry", "ADC abs,Y", "A:=A+{adr} ", "**---**" },
{ "ADC", 0x7D, cpu_6502_adc_abx, "abx", "add with carry", "ADC abs,X", "A:=A+{adr} ", "**---**" },
{ "ROR", 0x7E, cpu_6502_ror_abx, "abx", "rotate right", "ROR abs,X", "{adr}:={adr}/2+C*128 ", "*----**" },
{ "STA", 0x81, cpu_6502_sta_izx, "izx", "store accumulator", "STA X,ind", "{adr}:=A ", "-------" },
{ "STY", 0x84, cpu_6502_sty_zp, "zp", "store Y", "STY zpg", "{adr}:=Y ", "-------" },
{ "STA", 0x85, cpu_6502_sta_zp, "zp", "store accumulator", "STA zpg", "{adr}:=A ", "-------" },
{ "STX", 0x86, cpu_6502_stx_zp, "zp", "store X", "STX zpg", "{adr}:=X ", "-------" },
{ "DEY", 0x88, cpu_6502_dey_imp, "imp", "decrement Y", "DEY impl", "Y:=Y-1 ", "*----*-" },
{ "TXA", 0x8A, cpu_6502_txa_imp, "imp", "transfer X to accumulator", "TXA impl", "A:=X ", "*----*-" },
{ "STY", 0x8C, cpu_6502_sty_abs, "abs", "store Y", "STY abs", "{adr}:=Y ", "-------" },
{ "STA", 0x8D, cpu_6502_sta_abs, "abs", "store accumulator", "STA abs", "{adr}:=A ", "-------" },
{ "STX", 0x8E, cpu_6502_stx_abs, "abs", "store X", "STX abs", "{adr}:=X ", "-------" },
{ "BCC", 0x90, cpu_6502_bcc_rel, "rel", "branch on carry clear", "BCC rel", "branch on C=0 ", "-------" },
{ "STA", 0x91, cpu_6502_sta_izy, "izy", "store accumulator", "STA ind,Y", "{adr}:=A ", "-------" },
{ "STY", 0x94, cpu_6502_sty_zpx, "zpx", "store Y", "STY zpg,X", "{adr}:=Y ", "-------" },
{ "STA", 0x95, cpu_6502_sta_zpx, "zpx", "store accumulator", "STA zpg,X", "{adr}:=A ", "-------" },
{ "STX", 0x96, cpu_6502_stx_zpy, "zpy", "store X", "STX zpg,Y", "{adr}:=X ", "-------" },
{ "TYA", 0x98, cpu_6502_tya_imp, "imp", "transfer Y to accumulator", "TYA impl", "A:=Y ", "*----*-" },
{ "STA", 0x99, cpu_6502_sta_aby, "aby", "store accumulator", "STA abs,Y", "{adr}:=A ", "-------" },
{ "TXS", 0x9A, cpu_6502_txs_imp, "imp", "transfer X to stack pointer", "TXS impl", "S:=X ", "-------" },
{ "STA", 0x9D, cpu_6502_sta_abx, "abx", "store accumulator", "STA abs,X", "{adr}:=A ", "-------" },
{ "LDY", 0xA0, cpu_6502_ldy_imm, "imm", "load Y", "LDY #", "Y:={adr} ", "*----*-" },
{ "LDA", 0xA1, cpu_6502_lda_izx, "izx", "load accumulator", "LDA X,ind", "A:={adr} ", "*----*-" },
{ "LDX", 0xA2, cpu_6502_ldx_imm, "imm", "", "LDX #", "X:={adr} ", "*----*-" },
{ "LDY", 0xA4, cpu_6502_ldy_zp, "zp", "load Y", "LDY zpg", "Y:={adr} ", "*----*-" },
{ "LDA", 0xA5, cpu_6502_lda_zp, "zp", "load accumulator", "LDA zpg", "A:={adr} ", "*----*-" },
{ "LDX", 0xA6, cpu_6502_ldx_zp, "zp", "", "LDX zpg", "X:={adr} ", "*----*-" },
{ "TAY", 0xA8, cpu_6502_tay_imp, "imp", "transfer accumulator to Y", "TAY impl", "Y:=A ", "*----*-" },
{ "LDA", 0xA9, cpu_6502_lda_imm, "imm", "load accumulator", "LDA #", "A:={adr} ", "*----*-" },
{ "TAX", 0xAA, cpu_6502_tax_imp, "imp", "transfer accumulator to X", "TAX impl", "X:=A ", "*----*-" },
{ "LDY", 0xAC, cpu_6502_ldy_abs, "abs", "load Y", "LDY abs", "Y:={adr} ", "*----*-" },
{ "LDA", 0xAD, cpu_6502_lda_abs, "abs", "load accumulator", "LDA abs", "A:={adr} ", "*----*-" },
{ "LDX", 0xAE, cpu_6502_ldx_abs, "abs", "", "LDX abs", "X:={adr} ", "*----*-" },
{ "BCS", 0xB0, cpu_6502_bcs_rel, "rel", "branch on carry set", "BCS rel", "branch on C=1 ", "-------" },
{ "LDA", 0xB1, cpu_6502_lda_izy, "izy", "load accumulator", "LDA ind,Y", "A:={adr} ", "*----*-" },
{ "LDY", 0xB4, cpu_6502_ldy_zpx, "zpx", "load Y", "LDY zpg,X", "Y:={adr} ", "*----*-" },
{ "LDA", 0xB5, cpu_6502_lda_zpx, "zpx", "load accumulator", "LDA zpg,X", "A:={adr} ", "*----*-" },
{ "LDX", 0xB6, cpu_6502_ldx_zpy, "zpy", "", "LDX zpg,Y", "X:={adr} ", "*----*-" },
{ "CLV", 0xB8, cpu_6502_clv_imp, "imp", "clear overflow", "CLV impl", "V:=0 ", "-0-----" },
{ "LDA", 0xB9, cpu_6502_lda_aby, "aby", "load accumulator", "LDA abs,Y", "A:={adr} ", "*----*-" },
{ "TSX", 0xBA, cpu_6502_tsx_imp, "imp", "transfer stack pointer to X", "TSX impl", "X:=S ", "*----*-" },
{ "LDY", 0xBC, cpu_6502_ldy_abx, "abx", "load Y", "LDY abs,X", "Y:={adr} ", "*----*-" },
{ "LDA", 0xBD, cpu_6502_lda_abx, "abx", "load accumulator", "LDA abs,X", "A:={adr} ", "*----*-" },
{ "LDX", 0xBE, cpu_6502_ldx_aby, "aby", "", "LDX abs,Y", "X:={adr} ", "*----*-" },
{ "CPY", 0xC0, cpu_6502_cpy_imm, "imm", "compare with Y", "CPY #", "Y-{adr} ", "*----**" },
{ "CMP", 0xC1, cpu_6502_cmp_izx, "izx", "compare (with accumulator)", "CMP X,ind", "A-{adr} ", "*----**" },
{ "CPY", 0xC4, cpu_6502_cpy_zp, "zp", "compare with Y", "CPY zpg", "Y-{adr} ", "*----**" },
{ "CMP", 0xC5, cpu_6502_cmp_zp, "zp", "compare (with accumulator)", "CMP zpg", "A-{adr} ", "*----**" },
{ "DEC", 0xC6, cpu_6502_dec_zp, "zp", "decrement", "DEC zpg", "{adr}:={adr}-1 ", "*----*-" },
{ "INY", 0xC8, cpu_6502_iny_imp, "imp", "increment Y", "INY impl", "Y:=Y+1 ", "*----*-" },
{ "CMP", 0xC9, cpu_6502_cmp_imm, "imm", "compare (with accumulator)", "CMP #", "A-{adr} ", "*----**" },
{ "DEX", 0xCA, cpu_6502_dex_imp, "imp", "decrement X", "DEX impl", "X:=X-1 ", "*----*-" },
{ "CPY", 0xCC, cpu_6502_cpy_abs, "abs", "compare with Y", "CPY abs", "Y-{adr} ", "*----**" },
{ "CMP", 0xCD, cpu_6502_cmp_abs, "abs", "compare (with accumulator)", "CMP abs", "A-{adr} ", "*----**" },
{ "DEC", 0xCE, cpu_6502_dec_abs, "abs", "decrement", "DEC abs", "{adr}:={adr}-1 ", "*----*-" },
{ "BNE", 0xD0, cpu_6502_bne_rel, "rel", "branch on not equal (zero clear)", "BNE rel", "branch on Z=0 ", "-------" },
{ "CMP", 0xD1, cpu_6502_cmp_izy, "izy", "compare (with accumulator)", "CMP ind,Y", "A-{adr} ", "*----**" },
{ "CMP", 0xD5, cpu_6502_cmp_zpx, "zpx", "compare (with accumulator)", "CMP zpg,X", "A-{adr} ", "*----**" },
{ "DEC", 0xD6, cpu_6502_dec_zpx, "zpx", "decrement", "DEC zpg,X", "{adr}:={adr}-1 ", "*----*-" },
{ "CLD", 0xD8, cpu_6502_cld_imp, "imp", "clear decimal", "CLD impl", "D:=0 ", "---0---" },
{ "CMP", 0xD9, cpu_6502_cmp_aby, "aby", "compare (with accumulator)", "CMP abs,Y", "A-{adr} ", "*----**" },
{ "CMP", 0xDD, cpu_6502_cmp_abx, "abx", "compare (with accumulator)", "CMP abs,X", "A-{adr} ", "*----**" },
{ "DEC", 0xDE, cpu_6502_dec_abx, "abx", "decrement", "DEC abs,X", "{adr}:={adr}-1 ", "*----*-" },
{ "CPX", 0xE0, cpu_6502_cpx_imm, "imm", "compare with X", "CPX #", "X-{adr} ", "*----**" },
{ "SBC", 0xE1, cpu_6502_sbc_izx, "izx", "subtract with carry", "SBC X,ind", "A:=A-{adr} ", "**---**" },
{ "CPX", 0xE4, cpu_6502_cpx_zp, "zp", "compare with X", "CPX zpg", "X-{adr} ", "*----**" },
{ "SBC", 0xE5, cpu_6502_sbc_zp, "zp", "subtract with carry", "SBC zpg", "A:=A-{adr} ", "**---**" },
{ "INC", 0xE6, cpu_6502_inc_zp, "zp", "increment", "INC zpg", "{adr}:={adr}+1 ", "*----*-" },
{ "INX", 0xE8, cpu_6502_inx_imp, "imp", "increment X", "INX impl", "X:=X+1 ", "*----*-" },
{ "SBC", 0xE9, cpu_6502_sbc_imm, "imm", "subtract with carry", "SBC #", "A:=A-{adr} ", "**---**" },
{ "NOP", 0xEA, cpu_6502_nop_imp, "imp", "no operation", "NOP impl", "  ", "-------" },
{ "CPX", 0xEC, cpu_6502_cpx_abs, "abs", "compare with X", "CPX abs", "X-{adr} ", "*----**" },
{ "SBC", 0xED, cpu_6502_sbc_abs, "abs", "subtract with carry", "SBC abs", "A:=A-{adr} ", "**---**" },
{ "INC", 0xEE, cpu_6502_inc_abs, "abs", "increment", "INC abs", "{adr}:={adr}+1 ", "*----*-" },
{ "BEQ", 0xF0, cpu_6502_beq_rel, "rel", "branch on equal (zero set)", "BEQ rel", "branch on Z=1 ", "-------" },
{ "SBC", 0xF1, cpu_6502_sbc_izy, "izy", "subtract with carry", "SBC ind,Y", "A:=A-{adr} ", "**---**" },
{ "SBC", 0xF5, cpu_6502_sbc_zpx, "zpx", "subtract with carry", "SBC zpg,X", "A:=A-{adr} ", "**---**" },
{ "INC", 0xF6, cpu_6502_inc_zpx, "zpx", "increment", "INC zpg,X", "{adr}:={adr}+1 ", "*----*-" },
{ "SED", 0xF8, cpu_6502_sed_imp, "imp", "set decimal", "SED impl", "D:=1 ", "---1---" },
{ "SBC", 0xF9, cpu_6502_sbc_aby, "aby", "subtract with carry", "SBC abs,Y", "A:=A-{adr} ", "**---**" },
{ "SBC", 0xFD, cpu_6502_sbc_abx, "abx", "subtract with carry", "SBC abs,X", "A:=A-{adr} ", "**---**" },
{ "INC", 0xFE, cpu_6502_inc_abx, "abx", "increment", "INC abs,X", "{adr}:={adr}+1 ", "*----*-" }
};

/* void init_operations(){ */
/*   operations_array_size = sizeof(operations); */
/* } */

//operations_array_size = sizeof(operations);

/* comparison function used for sorting and searching. */
int op_code_cmp (const int *key, const struct cpu_operator *c2){
  return (*key - c2->opcode);
}

struct cpu_operator* operation_lookup(int op_code){
  operations_array_size = sizeof(operations);

  size_t count = OPS; 
  struct cpu_operator *opp;
  opp = lfind(&op_code,
			  operations,
			  &count,
			  sizeof (struct cpu_operator),
			  (int (*)(const void *, const void *)) op_code_cmp);
  return opp;
}

void (*lookup_operation(int op_code))(){
  //number of elements
  operations_array_size = sizeof(operations);
  size_t count = OPS;
  struct cpu_operator *opp;
  opp=lfind(&op_code, operations, &count, sizeof (struct cpu_operator), (int (*)(const void *,
      const void *)) op_code_cmp);
  //print_op(opp);
  void (*fp)() = opp->opfunction;
  return fp;
}


/*

  Print information about cpu-op 

   void print_op (struct cpu_operator *c)
   {
   //char message[40];
   //snprintf (40, message, "Operator %s\n", c->name);
   }
   
*/


