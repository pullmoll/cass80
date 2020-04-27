#pragma once
#include <QtCore>

class bdfData;
class z80Defs;

class z80Dasm
{
public:
    z80Dasm(bool upper = false, const z80Defs* defs = nullptr, const bdfData* bdf = nullptr);
    QString dasm(quint32 pc, quint32 &bytes, const quint8* oprom, const quint8* opram);
    QStringList list(const QByteArray& memory, quint32 pc_min, quint32 pc_max);
private:
    quint32 unicode(uchar ch) const;
    QString dasm_defb(quint32 pc, quint32& pos, const quint8* opram);
    QString dasm_defw(quint32 pc, quint32& pos, const quint8* opram);
    QString dasm_defd(quint32 pc, quint32& pos, const quint8* opram);
    QString dasm_defs(quint32 pc, quint32& pos, const quint8* opram);
    QString dasm_defm(quint32 pc, quint32& pos, const quint8* opram);
    QString dasm_code(quint32 pc, quint32& pos, const quint8* oprom, const quint8* opram);
    bool m_upper;
    const z80Defs* m_defs;
    const bdfData* m_bdf;
};
