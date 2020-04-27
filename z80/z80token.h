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

    static quint32 flags(Mnemonics m);
    static QLatin1String string(Mnemonics m);
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
