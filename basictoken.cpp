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
#include "basictoken.h"

static const QLatin1String g_amp("&amp;");
static const QLatin1String g_lt("&lt;");
static const QLatin1String g_gt("&gt;");

BasicToken::BasicToken(bool esc_xml)
    : m_token()
    , m_esc_xml(esc_xml)
{
    m_token.insert(tEND,          "END");
    m_token.insert(tFOR,          "FOR");
    m_token.insert(tRESET,        "RESET");
    m_token.insert(tSET,          "SET");
    m_token.insert(tCLS,          "CLS");
    m_token.insert(tCMD,          "CMD");
    m_token.insert(tRANDOM,       "RANDOM");
    m_token.insert(tNEXT,         "NEXT");

    m_token.insert(tDATA,         "DATA");
    m_token.insert(tINPUT,        "INPUT");
    m_token.insert(tDIM,          "DIM");
    m_token.insert(tREAD,         "READ");
    m_token.insert(tLET,          "LET");
    m_token.insert(tGOTO,         "GOTO");
    m_token.insert(tRUN,          "RUN");
    m_token.insert(tIF,           "IF");

    m_token.insert(tRESTORE,      "RESTORE");
    m_token.insert(tGOSUB,        "GOSUB");
    m_token.insert(tRETURN,       "RETURN");
    m_token.insert(tREM,          "REM");
    m_token.insert(tSTOP,         "STOP");
    m_token.insert(tELSE,         "ELSE");
    m_token.insert(tTRON,         "TRON");
    m_token.insert(tTROFF,        "TROFF");

    m_token.insert(tDEFSTR,       "DEFSTR");
    m_token.insert(tDEFINT,       "DEFINT");
    m_token.insert(tDEFSNG,       "DEFSNG");
    m_token.insert(tDEFDBL,       "DEFDBL");
    m_token.insert(tLINE,         "LINE");
    m_token.insert(tEDIT,         "EDIT");
    m_token.insert(tERROR,        "ERROR");
    m_token.insert(tRESUME,       "RESUME");

    m_token.insert(tOUT,          "OUT");
    m_token.insert(tON,           "ON");
    m_token.insert(tOPEN,         "OPEN");
    m_token.insert(tFIELD,        "FIELD");
    m_token.insert(tGET,          "GET");
    m_token.insert(tPUT,          "PUT");
    m_token.insert(tCLOSE,        "CLOSE");
    m_token.insert(tLOAD,         "LOAD");

    m_token.insert(tMERGE,        "MERGE");
    m_token.insert(tNAME,         "NAME");
    m_token.insert(tKILL,         "KILL");
    m_token.insert(tLSET,         "LSET");
    m_token.insert(tRSET,         "RSET");
    m_token.insert(tSAVE,         "SAVE");
    m_token.insert(tSYSTEM,       "SYSTEM");
    m_token.insert(tLPRINT,       "LPRINT");

    m_token.insert(tDEF,          "DEF");
    m_token.insert(tPOKE,         "POKE");
    m_token.insert(tPRINT,        "PRINT");
    m_token.insert(tCONT,         "CONT");
    m_token.insert(tLIST,         "LIST");
    m_token.insert(tLLIST,        "LLIST");
    m_token.insert(tDELETE,       "DELETE");
    m_token.insert(tAUTO,         "AUTO");

    m_token.insert(tCLEAR,        "CLEAR");
    m_token.insert(tCLOAD,        "CLOAD");
    m_token.insert(tCSAVE,        "CSAVE");
    m_token.insert(tNEW,          "NEW");
    m_token.insert(tTAB_LPAREN,   "TAB(");
    m_token.insert(tTO,           "TO");
    m_token.insert(tFN,           "FN");
    m_token.insert(tUSING,        "USING");

    m_token.insert(tVARPTR,       "VARPTR");
    m_token.insert(tUSR,          "USR");
    m_token.insert(tERL,          "ERL");
    m_token.insert(tERR,          "ERR");
    m_token.insert(tSTRING_DOLLAR,"STRING$");
    m_token.insert(tINSTR,        "INSTR");
    m_token.insert(tCHECK,        "CHECK");
    m_token.insert(tTIME_DOLLAR,  "TIME$");

    m_token.insert(tMEM,          "MEM");
    m_token.insert(tINKEY_DOLLAR, "INKEY$");
    m_token.insert(tTHEN,         "THEN");
    m_token.insert(tNOT,          "NOT");
    m_token.insert(tSTEP,         "STEP");
    m_token.insert(tPLUS,         "+");
    m_token.insert(tMINUS,        "-");
    m_token.insert(tMULTIPLY,     "*");

    m_token.insert(tDIVIDE,       "/");
    m_token.insert(tLBRACKET,     "[");
    m_token.insert(tAND,          "AND");
    m_token.insert(tOR,           "OR");
    m_token.insert(tGT,           ">");
    m_token.insert(tEQ,           "=");
    m_token.insert(tLT,           "<");
    m_token.insert(tSGN,          "SGN");

    m_token.insert(tINT,          "INT");
    m_token.insert(tABS,          "ABS");
    m_token.insert(tFRE,          "FRE");
    m_token.insert(tINP,          "INP");
    m_token.insert(tPOS,          "POS");
    m_token.insert(tSQR,          "SQR");
    m_token.insert(tRND,          "RND");
    m_token.insert(tLOG,          "LOG");

    m_token.insert(tEXP,          "EXP");
    m_token.insert(tCOS,          "COS");
    m_token.insert(tSIN,          "SIN");
    m_token.insert(tTAN,          "TAN");
    m_token.insert(tATN,          "ATN");
    m_token.insert(tPEEK,         "PEEK");
    m_token.insert(tCVI,          "CVI");
    m_token.insert(tCVS,          "CVS");

    m_token.insert(tCVD,          "CVD");
    m_token.insert(tEOF,          "EOF");
    m_token.insert(tLOC,          "LOC");
    m_token.insert(tLOF,          "LOF");
    m_token.insert(tMKI_DOLLAR,   "MKI$");
    m_token.insert(tMKS_DOLLAR,   "MKS$");
    m_token.insert(tMKD_DOLLAR,   "MKD$");
    m_token.insert(tCINT,         "CINT");

    m_token.insert(tCSNG,         "CSNG");
    m_token.insert(tCDBL,         "CDBL");
    m_token.insert(tFIX,          "FIX");
    m_token.insert(tLEN,          "LEN");
    m_token.insert(tSTR_DOLLAR,   "STR$");
    m_token.insert(tVAL,          "VAL");
    m_token.insert(tASC,          "ASC");
    m_token.insert(tCHR_DOLLAR,   "CHR$");

    m_token.insert(tLEFT_DOLLAR,  "LEFT$");
    m_token.insert(tRIGHT_DOLLAR, "RIGHT$");
    m_token.insert(tMID_DOLLAR,   "MID$");
    m_token.insert(tQUOTE,        "'");
    m_token.insert(tOCT374,        "\\374");
    m_token.insert(tOCT375,        "\\375");
    m_token.insert(tOCT376,        "\\376");
    m_token.insert(tOCT377,        "\\377");

    /* Colour Genie specific strings (after 0xff) */
    m_token.insert(tCOLOUR,       "COLOUR");
    m_token.insert(tFCOLOU,       "FCOLOU");    /* (sic!) */
    m_token.insert(tKEYPAD,       "KEYPAD");
    m_token.insert(tJOY,          "JOY");
    m_token.insert(tPLOT,         "PLOT");
    m_token.insert(tFGR,          "FGR");
    m_token.insert(tLGR,          "LGR");
    m_token.insert(tFCLS,         "FCLS");

    m_token.insert(tPLAY,         "PLAY");
    m_token.insert(tCIRCLE,       "CIRCLE");
    m_token.insert(tSCALE,        "SCALE");
    m_token.insert(tSHAPE,        "SHAPE");
    m_token.insert(tNSHAPE,       "NSHAPE");
    m_token.insert(tXSHAPE,       "XSHAPE");
    m_token.insert(tPAINT,        "PAINT");
    m_token.insert(tCPOINT,       "CPOINT");

    m_token.insert(tNPLOT,        "NPLOT");
    m_token.insert(tSOUND,        "SOUND");
    m_token.insert(tCHAR,         "CHAR");
    m_token.insert(tRENUM,        "RENUM");
    m_token.insert(tSWAP,         "SWAP");
    m_token.insert(tFKEY,         "FKEY");
    m_token.insert(tCALL,         "CALL");
    m_token.insert(tVERIFY,       "VERIFY");

    m_token.insert(tBGRD,         "BGRD");
    m_token.insert(tNBGRD,        "NBGRD");

}

const QString BasicToken::detokenize(const void* source, int maxsize)
{
    const uchar* src = reinterpret_cast<const uchar *>(source);
    QStringList buff;
    int string = 0;

    for (/* */; *src && maxsize-- > 0; src++) {
        if (string) {
            if ('"' == *src)
                string = 0;
            switch (*src) {
            case '&':
		buff += m_esc_xml ? g_amp : QString(QChar(*src));
                break;
            case '<':
		buff += m_esc_xml ? g_lt : QString(QChar(*src));
		break;
            case '>':
		buff += m_esc_xml ? g_gt : QString(QChar(*src));
		break;
            default:
		buff += QString::fromLatin1(reinterpret_cast<const char *>(src), 1);
                break;
            }
            continue;
        }
        if (0xff == *src) {
            src++;
	    QString token = m_token.value(0xff00 | *src);
	    if (token.isEmpty())
		token = QString("\\377\\%1").arg(*src, 3, 8, QChar('0'));
	    buff += token;
            continue;
        }
        if (*src > 0x7f) {
	    if (m_token[*src] == QStringLiteral("&"))
		buff += m_esc_xml ? g_amp : m_token[*src];
	    else if (m_token[*src] == QStringLiteral("<"))
		buff += m_esc_xml ? g_lt : m_token[*src];
	    else if (m_token[*src] == QStringLiteral(">"))
		buff += m_esc_xml ? g_gt : m_token[*src];
	    else
		buff += m_token[*src];
            continue;
        }
        switch (*src) {
        case '&':
	    buff += m_esc_xml ? g_amp : QString(QChar(*src));
	    break;
        case '<':
	    buff += m_esc_xml ? g_lt : QString(QChar(*src));
	    break;
        case '>':
	    buff += m_esc_xml ? g_gt : QString(QChar(*src));
	    break;
        default:
	    buff += QString::fromLatin1(reinterpret_cast<const char *>(src), 1);
            break;
        }
        if ('"' == *src)
            string = 1;
    }
    return buff.join(QString());
}
