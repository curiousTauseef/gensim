/* This file is Copyright University of Edinburgh 2018. For license details, see LICENSE. */

#include "arch/x86/X86Decoder.h"

extern "C" {
#include <xed/xed-interface.h>
}

using namespace archsim;
using namespace archsim::arch::x86;

static int get_register_index(xed_reg_enum_t reg)
{
	ASSERT(xed_reg_class(reg) == XED_REG_CLASS_GPR);
	return xed_get_largest_enclosing_register(reg) - XED_REG_RAX;
}

int X86Decoder::DecodeInstr(Address addr, int mode, MemoryInterface& interface)
{
	// read 15 bytes
	uint8_t data[15];
	interface.Read(addr, data, 15);

	// pass data to XED
	xed_state_t dstate;
	xed_decoded_inst_t xedd;
	xed_error_enum_t xed_error;

	xed_tables_init();
	xed_state_zero(&dstate);
	dstate.mmode = XED_MACHINE_MODE_LONG_64;

	xed_decoded_inst_zero_set_mode(&xedd, &dstate);
	xed_decoded_inst_set_input_chip(&xedd, XED_CHIP_HASWELL);

	xed_error = xed_decode(&xedd, XED_REINTERPRET_CAST(const xed_uint8_t*, data), 15);

	auto category = xed_decoded_inst_get_category(&xedd);
	auto iclass = xed_decoded_inst_get_iclass(&xedd);

	Instr_Length = xed_decoded_inst_get_length(&xedd);

	auto noperands = xed_decoded_inst_noperands(&xedd);
	auto inst = xed_decoded_inst_inst(&xedd);

	if(noperands > 0) {
		auto op0 = xed_inst_operand(inst, 0);
		auto op_name = xed_operand_name(op0);
		switch(op_name) {
			case XED_OPERAND_REG0:
			case XED_OPERAND_REG1:
			case XED_OPERAND_REG2:
			case XED_OPERAND_REG3:
			case XED_OPERAND_REG4:
			case XED_OPERAND_REG5:
			case XED_OPERAND_REG6:
			case XED_OPERAND_REG7:
			case XED_OPERAND_REG8: {
				auto reg = xed_decoded_inst_get_reg(&xedd, op_name);
				op0_is_reg = 1;
				op0_reg = get_register_index(reg);
				op0_size = xed_get_register_width_bits64(reg);
				break;
			}
			default:
				UNIMPLEMENTED;
		}
	}

	if(noperands > 1) {
		auto op = xed_inst_operand(inst, 1);
		auto op_name = xed_operand_name(op);
		switch(op_name) {
			case XED_OPERAND_REG0:
			case XED_OPERAND_REG1:
			case XED_OPERAND_REG2:
			case XED_OPERAND_REG3:
			case XED_OPERAND_REG4:
			case XED_OPERAND_REG5:
			case XED_OPERAND_REG6:
			case XED_OPERAND_REG7:
			case XED_OPERAND_REG8: {
				auto reg = xed_decoded_inst_get_reg(&xedd, op_name);
				op1_is_reg = 1;
				op1_reg = get_register_index(reg);
				op1_size = xed_get_register_width_bits64(reg);
				break;
			}
			default:
				UNIMPLEMENTED;
		}
	}

	switch(iclass) {
		case XED_ICLASS_XOR:
			Instr_Code = INST_x86_xor;
			break;
		default:
			UNIMPLEMENTED;
	}

	return 0;
}
