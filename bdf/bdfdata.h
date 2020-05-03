#pragma once
#include <QString>

#define	DEFAULT_BDF_PIXEL_SIZE	16

/**
 * @brief The bdfData class is a virtual class defining the
 * interface to embedded BDF font generators and translators.
 */
class bdfData
{
public:
    explicit bdfData(int pixelsize = 0);
    virtual quint32 toUnicode(uchar ch) const = 0;
    virtual uchar fromUnicode(quint32 uc) const = 0;
    virtual bool generate(const QString& fntname, const QString& bdfname) = 0;

protected:
    int m_pixelsize = 16;
};
