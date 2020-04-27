#pragma once
#include <QString>

class bdfGlyph
{
public:
    bdfGlyph(const uchar ch, const quint32 uc, const QString& name);

    uchar ch() const;
    quint32 uc() const;
    QString name() const;

private:
    uchar m_char;
    uchar m_unicode;
    QString m_name;
};
