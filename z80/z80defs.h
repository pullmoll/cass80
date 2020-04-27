#pragma once
#include <QMap>
#include "z80def.h"

class z80Defs
{
public:
    z80Defs(const QString& filename = nullptr);

    bool load(const QString& filename = nullptr);

    QString filename() const;
    QString system() const;
    QMap<quint32,z80Def> defs() const;
    z80Def def(quint32 addr) const;
    z80Def::EntryType type(quint32 addr) const;

private:
    QString m_filename;
    QString m_system;
    QDomDocument m_doc;
    QMap<quint32,z80Def> m_defs;
    z80Def::EntryType m_last_type;
};
