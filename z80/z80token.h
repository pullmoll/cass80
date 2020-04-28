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

#define	    DASMFLAG_SUPPORTED	(1u << 29)
#define	    DASMFLAG_SKIP	(1u << 30)
#define	    DASMFLAG_OVER	(1u << 31)

class z80Token
{
public:
    enum Mnemonics {
	zADC,
	zADD,
	zAND,
	zBIT,
	zCALL,
	zCCF,
	zCP,
	zCPD,
	zCPDR,
	zCPI,
	zCPIR,
	zCPL,
	zDAA,
	zDB,
	zDEC,
	zDEFB,
	zDEFW,
	zDEFD,
	zDEFM,
	zDEFS,
	zDI,
	zDJNZ,
	zEI,
	zEX,
	zEXX,
	zHLT,
	zIM,
	zIN,
	zINC,
	zIND,
	zINDR,
	zINI,
	zINIR,
	zJP,
	zJR,
	zLD,
	zLDD,
	zLDDR,
	zLDI,
	zLDIR,
	zNEG,
	zNOP,
	zOR,
	zOTDR,
	zOTIR,
	zOUT,
	zOUTD,
	zOUTI,
	zPOP,
	zPUSH,
	zRES,
	zRET,
	zRETI,
	zRETN,
	zRL,
	zRLA,
	zRLC,
	zRLCA,
	zRLD,
	zRR,
	zRRA,
	zRRC,
	zRRCA,
	zRRD,
	zRST,
	zSBC,
	zSCF,
	zSET,
	zSLA,
	zSLL,
	zSRA,
	zSRL,
	zSUB,
	zXOR
    };

    explicit z80Token(Mnemonics m = zDB, const char* p = nullptr);
    z80Token(const z80Token& other);

    Mnemonics mnemonic() const;
    const char* parameters() const;

    static QLatin1String string(Mnemonics m);
    static quint32 flags(Mnemonics m);
    static z80Token mnemonic_xx_cb(uchar op);
    static z80Token mnemonic_cb(uchar op);
    static z80Token mnemonic_ed(uchar op);
    static z80Token mnemonic_xx(uchar op);
    static z80Token mnemonic_main(uchar op);

    z80Token& operator= (const z80Token& other);

private:
    Mnemonics m_mnemonic;
    const char* m_parameters;
};
