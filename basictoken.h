/****************************************************************************
 *
 * Cass80 tool - BASIC token handling
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
