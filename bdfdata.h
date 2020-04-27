#pragma once
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QVariantList>
#include <QFile>
#include <QTextStream>

#define	DEFAULT_BDF_PIXEL_SIZE	16

class bdfGlyph;

class bdfData
{
public:
    bdfData(int pixelsize = DEFAULT_BDF_PIXEL_SIZE);
    quint32 cgenie2unicode(uchar ch) const;
    uchar unicode2cgenie(quint32 uc) const;

    bool generate(const QString& fntname, const QString& bdfname);

private:
    static constexpr char const *FOUNDRY = "EACA";
    static constexpr char const *FONTNAME_REGISTRY = "ColourGenie";
    static constexpr char const *FAMILY_NAME = "ColourGenie";
    static constexpr char const *WEIGHT_NAME = "regular";
    static constexpr char const *SLANT = "upright";
    static constexpr char const *SETWIDTH_NAME = "normal";
    static constexpr char const *ADD_STYLE_NAME = "Ancient";
    static constexpr int RESOLUTION_X = 75;
    static constexpr int RESOLUTION_Y = 75;
    static constexpr char const *SPACING = "monospaced";
    static constexpr char const *CHARSET_REGISTRY = "ISO8859";
    static constexpr char const *CHARSET_ENCODING = "1";

    static constexpr int DEFAULT_CHAR = 0;
    static constexpr int FONT_DESCENT = 0;
    static constexpr int FONT_ASCENT = DEFAULT_BDF_PIXEL_SIZE;
    static constexpr char const *COPYRIGHT = "EACA Ltd. Hong Kong (1982)";

    int m_pixelsize = 16;
    int m_font_w = 8;
    int m_font_h = 8;

    int PIXEL_SIZE = DEFAULT_BDF_PIXEL_SIZE;
    int POINT_SIZE = DEFAULT_BDF_PIXEL_SIZE * 10;
    int AVERAGE_WIDTH = DEFAULT_BDF_PIXEL_SIZE * 10;

    int CAP_HEIGHT = DEFAULT_BDF_PIXEL_SIZE;
    int X_HEIGHT = DEFAULT_BDF_PIXEL_SIZE * 3 / 4;

    int SCALABLE_WIDTH = 1000 * RESOLUTION_X / 72;
    int SCALABLE_HEIGHT = 0;   // default
    int DEVICE_WIDTH = 8;
    int DEVICE_HEIGHT = 0;   // default

    int BDF_BBOX_W = DEFAULT_BDF_PIXEL_SIZE;
    int BDF_BBOX_H = DEFAULT_BDF_PIXEL_SIZE;
    int BDF_BBOX_X = 0;
    int BDF_BBOX_Y = 0;

    QHash<uchar,bdfGlyph*> m_glyphs;
    QStringList m_properties;

    void setup_properties();
    int bit_chk(const QByteArray& buff, int x, int y);
    void bit_set(QByteArray& bitmap, int x);
    void emit_bitmap(QTextStream& bdf, const QByteArray& buff);
    QHash<uchar,quint32> m_cgenie_to_unicode;
    QHash<quint32,uchar> m_unicode_to_cgenie;
};
