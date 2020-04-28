/****************************************************************************
 *
 * Cass80 tool - Z80 disassembler
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

class bdfData;
class z80Defs;

class z80Dasm
{
public:
    z80Dasm(bool upper = false, const z80Defs* defs = nullptr, const bdfData* bdf = nullptr);
    QString dasm(quint32 pc, quint32 &bytes, quint32& flags, const quint8* oprom, const quint8* opram);
    QStringList list(const QByteArray& memory, quint32 pc_min, quint32 pc_max);
private:
    quint32 unicode(uchar ch) const;
    QString dasm_defb(quint32 pc, quint32& pos, const quint8* opram);
    QString dasm_defw(quint32 pc, quint32& pos, const quint8* opram);
    QString dasm_defd(quint32 pc, quint32& pos, const quint8* opram);
    QString dasm_defs(quint32 pc, quint32& pos, const quint8* opram);
    QString dasm_defm(quint32 pc, quint32& pos, const quint8* opram);
    QString dasm_token(quint32 pc, quint32& pos, const quint8* opram);
    QString dasm_code(quint32 pc, quint32& pos, quint32& flags, const quint8* oprom, const quint8* opram);
    bool m_uppercase;
    bool m_comment_glyphs;
    quint32 m_bytes_per_line;
    const z80Defs* m_defs;
    const bdfData* m_bdf;
    static inline QChar sign(qint8 offset);
    static inline QString hexb(quint32 val);
    static inline QString hexw(quint32 val);
    static inline QString hexd(quint32 val);
    static inline int offs(qint8 offset);
    static inline quint32 rd08(const quint8* mem);
    static inline quint32 rd16(const quint8* mem);
    static inline quint32 rd32(const quint8* mem);
    static inline void wr08(quint8* mem, quint32 val);
    static inline void wr16(quint8* mem, quint32 val);
    static inline void wr32(quint8* mem, quint32 val);
};
