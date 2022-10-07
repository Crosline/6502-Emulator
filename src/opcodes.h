#pragma once
#include "cpu_constants.h"

// Opcodes
static constexpr Byte // LDA
LDA_IMM = 0xA9,
LDA_ZP = 0xA5,
LDA_ZP_X = 0xB5,
LDA_ABS = 0xAD,
LDA_ABS_X = 0xBD,
LDA_ABS_Y = 0xB9,
LDA_ID_X = 0xA1,
LDA_ID_Y = 0xB1;

static constexpr Byte // JSR
JSR_ABS = 0x20;