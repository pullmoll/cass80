#pragma once
#include <QtCore>

class BasicToken
{

public:
    enum basic_token {
        tEND = 128,
        tFOR,
        tRESET,
        tSET,
        tCLS,
        tCMD,
        tRANDOM,
        tNEXT,

        tDATA,
        tINPUT,
        tDIM,
        tREAD,
        tLET,
        tGOTO,
        tRUN,
        tIF,

        tRESTORE,
        tGOSUB,
        tRETURN,
        tREM,
        tSTOP,
        tELSE,
        tTRON,
        tTROFF,

        tDEFSTR,
        tDEFINT,
        tDEFSNG,
        tDEFDBL,
        tLINE,
        tEDIT,
        tERROR,
        tRESUME,

        tOUT,
        tON,
        tOPEN,
        tFIELD,
        tGET,
        tPUT,
        tCLOSE,
        tLOAD,

        tMERGE,
        tNAME,
        tKILL,
        tLSET,
        tRSET,
        tSAVE,
        tSYSTEM,
        tLPRINT,

        tDEF,
        tPOKE,
        tPRINT,
        tCONT,
        tLIST,
        tLLIST,
        tDELETE,
        tAUTO,

        tCLEAR,
        tCLOAD,
        tCSAVE,
        tNEW,
        tTAB_LPAREN,
        tTO,
        tFN,
        tUSING,

        tVARPTR,
        tUSR,
        tERL,
        tERR,
        tSTRING_DOLLAR,
        tINSTR,
        tCHECK,
        tTIME_DOLLAR,

        tMEM,
        tINKEY_DOLLAR,
        tTHEN,
        tNOT,
        tSTEP,
        tPLUS,
        tMINUS,
        tMULTIPLY,

        tDIVIDE,
        tLBRACKET,
        tAND,
        tOR,
        tGT,
        tEQ,
        tLT,
        tSGN,

        tINT,
        tABS,
        tFRE,
        tINP,
        tPOS,
        tSQR,
        tRND,
        tLOG,

        tEXP,
        tCOS,
        tSIN,
        tTAN,
        tATN,
        tPEEK,
        tCVI,
        tCVS,

        tCVD,
        tEOF,
        tLOC,
        tLOF,
        tMKI_DOLLAR,
        tMKS_DOLLAR,
        tMKD_DOLLAR,
        tCINT,

        tCSNG,
        tCDBL,
        tFIX,
        tLEN,
        tSTR_DOLLAR,
        tVAL,
        tASC,
        tCHR_DOLLAR,

        tLEFT_DOLLAR,
        tRIGHT_DOLLAR,
        tMID_DOLLAR,
        tQUOTE,
        tOCT374,
        tOCT375,
        tOCT376,
        tOCT377,

	tCOLOUR = 0xff80,
        tFCOLOU,    /* (sic!) */
        tKEYPAD,
        tJOY,
        tPLOT,
        tFGR,
        tLGR,
        tFCLS,

        tPLAY,
        tCIRCLE,
        tSCALE,
        tSHAPE,
        tNSHAPE,
        tXSHAPE,
        tPAINT,
        tCPOINT,

        tNPLOT,
        tSOUND,
        tCHAR,
        tRENUM,
        tSWAP,
        tFKEY,
        tCALL,
        tVERIFY,

        tBGRD,
        tNBGRD
    };

    explicit BasicToken(bool esc_xml = false);

    const QString detokenize(const void* source, int maxsize);

private:
    QMap<int, QString> m_token;
    bool m_esc_xml;
};
