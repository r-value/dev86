// LIB86 - 80x86 library
// Operation classes

#pragma once

#include "op-common.h"


// Opcode helpers

#define OPCODE_MAX 6
#define OPNAME_MAX 8

extern char op_code_str [];
extern byte_t op_code_pos;

extern byte_t * op_code_base;
extern word_t op_code_seg;
extern word_t op_code_off;


// Register

#define RT_REG8  0
#define RT_REG16 1
#define RT_SEG   2


// Address

#define AF_DISP 0x01  // address with displacement value
#define AF_SI   0x02  // address indexed on SI
#define AF_DI   0x04  // address indexed on DI
#define AF_BX   0x08  // address indexed on BX
#define AF_BP   0x10  // address indexed on BP


// Operand

#define VT_REG   0  // operand is an ALU register
#define VT_SEG   1  // operand is a MMU register
#define VT_IMM   2  // operand is an immediate value
#define VT_INDEX 3  // operand is an indexed address (with index flags)
#define VT_NEAR  4  // operand is a relative near address
#define VT_FAR   5  // operand is an absolute far address

#define VS_BYTE 0  // operand is 8 bits
#define VS_WORD 1  // operand is 16 bits

union op_val_u
	{
	byte_t b;  // absolute byte
	word_t w;  // absolute word
	char   c;  // relative byte
	short  s;  // relative word
	};

typedef union op_val_u op_val_t;

struct op_var_s
	{
	byte_t type;   // operand type (VT_xxx)
	byte_t size;   // 8 or 16 bits
	byte_t flags;  // for relative address (AF_xxx)
	byte_t reg;    // register number
	op_val_t val;  // variable value
	word_t seg;    // segment value
	};

typedef struct op_var_s op_var_t;


// Operation

struct op_desc_s;  // forward declaration

typedef struct op_desc_s op_desc_t;

typedef void (* op_hand_t) (op_desc_t * op_desc);

struct op_desc_s
	{
	word_t    op_id;    // operation identifier

	byte_t   var_count; // operand count
	op_var_t var_to;    // optional single or 'to' operand
	op_var_t var_from;  // optional 'from' operand

	// opcode 1 byte fields

	byte_t v1;;         // variable flag (for in and out)
	byte_t w1;          // w flag 1
	byte_t seg1;        // segment number
	byte_t reg1;        // register number
	byte_t d;           // direction flag
	byte_t s;           // sign extension flag
	byte_t v2;          // variable flag (for rotate and shift)
	byte_t w2;          // w flag 2

	// opcode 2 bytes fields

	byte_t mod;         // mode number
	byte_t reg2;        // register number
	byte_t seg2;        // segment number
	byte_t rm;          // register number or displacement mode

	};

void op_print (op_desc_t * op_desc);


// Operation class

#define CF_1 0x01  // byte size
#define CF_2 0x02  // word size
#define CF_D 0x04  // direction bit
#define CF_S 0x08  // sign extent bit
#define CF_V 0x10  // variable bit
#define CF_A 0x20  // implicit accumulator
#define CF_W 0x40  // w bit

typedef int (* class_hand_t) (byte_t flags, op_desc_t * op_desc);

struct class_desc_s;  // forward declaration

typedef struct class_desc_s class_desc_t;

struct class_desc_s
	{
	byte_t mask;              // opcode mask
	byte_t code;              // opcode value

	byte_t len;               // opcode len

	class_desc_t * sub;       // sub-table

	class_hand_t class_hand;  // class handle
	byte_t class_flags;       // class flags

	word_t op_id;             // operation identifier
	};


int op_decode (op_desc_t * op_desc);
