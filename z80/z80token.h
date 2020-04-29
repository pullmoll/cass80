/****************************************************************************
 *
 * Cass80 tool - Z80 token (mnemonics) and flags
 *
 * Copyright (C) 2020 Jürgen Buchmüller <pullmoll@t-online.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************/
#pragma once
#include <QVector>

class z80Token
{
public:
    enum z80Flags {
	DASMFLAG_NONE = 0,
	DASMFLAG_FINAL = (1u << 28),
	DASMFLAG_OVER = (1u << 29),
	DASMFLAG_SKIP = (1u << 30),
	DASMFLAG_SUPPORTED = (1u << 31)
    };

    enum z80Mnemonic {
	z80ADC,	    //!< addition with carry
	z80ADD,	    //!< addition
	z80AND,	    //!< logical AND
	z80BIT,	    //!< bit test
	z80CALL,    //!< subroutine call
	z80CCF,	    //!< complement carry flag
	z80CP,	    //!< compare
	z80CPD,	    //!< compare A with (HL) and decrement HL, BC
	z80CPDR,    //!< compare A with (HL) and decrement HL, BC until found or BC is zero
	z80CPI,	    //!< compare A with (HL) and increment HL, decrement BC
	z80CPIR,    //!< compare A with (HL) and increment HL, decrement BC until found or BC is zero
	z80CPL,	    //!< complement accumulator
	z80DAA,	    //!< decimal adjust accumulator after addition / subtraction
	z80DB,	    //!< alternate for for define byte
	z80DEC,	    //!< decrement
	z80DEFB,    //!< define byte
	z80DEFW,    //!< define word
	z80DEFD,    //!< define dword
	z80DEFM,    //!< define message
	z80DEFS,    //!< define space
	z80DI,	    //!< disable interrupts
	z80DJNZ,    //!< decrement B and jump if non zero
	z80EI,	    //!< enable interrupts
	z80EX,	    //!< exchange register with memory
	z80EXX,	    //!< exchange register sets (BC, DE, HL, AF)
	z80HLT,	    //!< halt
	z80IM,	    //!< set interrupt mode
	z80IN,	    //!< input from port
	z80INC,	    //!< increment
	z80IND,	    //!< input from port ([B]C) to (HL), decrement HL, B
	z80INDR,    //!< input from port ([B]C) to (HL), decrement HL, B, repeat until B is zero
	z80INI,	    //!< input from port ([B]C) to (HL), increment HL, decrement B
	z80INIR,    //!< input from port ([B]C) to (HL), increment HL, decrement B until B is zero
	z80JP,	    //!< jump absolute
	z80JR,	    //!< jump relative to PC
	z80LD,	    //!< load register or memory
	z80LDD,	    //!< load (DE) from (HL), decrement HL, DE, BC
	z80LDDR,    //!< load (DE) from (HL), decrement HL, DE, BC until BC is zero
	z80LDI,	    //!< load (DE) from (HL), increment HL, DE, decrement BC
	z80LDIR,    //!< load (DE) from (HL), increment HL, DE, decrement BC until BC is zero
	z80NEG,	    //!< negate A (A = 0 - A)
	z80NOP,	    //!< no operation
	z80OR,	    //!< logical OR
	z80OTDR,    //!< output (HL) to port ([B]C), decrement HL, B until B is zero
	z80OTIR,    //!< output (HL) to port ([B]C), increment HL, decrement B until B is zero
	z80OUT,	    //!< output A to to port (nn)
	z80OUTD,    //!< output (HL) to port ([B]C), decrement HL, B
	z80OUTI,    //!< output (HL) to port ([B]C), increment HL, decrement B
	z80POP,	    //!< pop register pair off stack (SP++)
	z80PUSH,    //!< push register pair to stack (--SP)
	z80RES,	    //!< reset bit
	z80RET,	    //!< return from subroutine
	z80RETI,    //!< return from interrupt subroutine
	z80RETN,    //!< return from non-maskable-interrupt subroutine
	z80RL,	    //!< rotate left through carry
	z80RLA,	    //!< rotate accumulator (A) left
	z80RLC,	    //!< rotate left circular
	z80RLCA,    //!< rotate accumulator (A) left circular
	z80RLD,	    //!< rotate left digit between A and (HL)
	z80RR,	    //!< rotate right through carry
	z80RRA,	    //!< rotate accumulator (A) right
	z80RRC,	    //!< rotate right circular
	z80RRCA,    //!< rotate accumulator (A) right
	z80RRD,	    //!< rotate right digit between A and (HL)
	z80RST,	    //!< restart vector
	z80SBC,	    //!< subtract with carry
	z80SCF,	    //!< set carry flag
	z80SET,	    //!< set bit
	z80SLA,	    //!< shift left arithmetic
	z80SLL,	    //!< shift left logic (1 bit shifted in)
	z80SRA,	    //!< shift right arithmetic (keep bit 7)
	z80SRL,	    //!< shift right logic
	z80SUB,	    //!< subtract
	z80XOR	    //!< logical XOR
    };

    explicit z80Token(z80Mnemonic m = z80DB, const char* p = nullptr, const z80Flags flags = DASMFLAG_NONE);
    z80Token(const z80Token& other);

    z80Mnemonic mnemonic() const;
    const char* parameters() const;
    z80Flags flags() const;

    static QLatin1String string(z80Mnemonic m);
    static z80Token mnemonic_xx_cb(uchar op);
    static z80Token mnemonic_cb(uchar op);
    static z80Token mnemonic_ed(uchar op);
    static z80Token mnemonic_xx(uchar op);
    static z80Token mnemonic_main(uchar op);

    z80Token& operator= (const z80Token& other);

private:
    z80Mnemonic m_mnemonic;
    const char* m_parameters;
    z80Flags m_flags;
};
