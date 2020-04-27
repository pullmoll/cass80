#include "bdfglyph.h"
bdfGlyph::bdfGlyph(const uchar ch, const quint32 uc, const QString& name)
    : m_char(ch)
    , m_unicode(uc)
    , m_name(name)
{
}

uchar bdfGlyph::ch() const
{
    return m_char;
}

quint32 bdfGlyph::uc() const
{
    return m_unicode;
}

QString bdfGlyph::name() const
{
    return m_name;
}
