#pragma once
#include <QDomDocument>
#include <QDomElement>

class z80Def
{
public:
    enum EntryType {
	INVALID=-1,
	CODE,	    //!< z80 code
	DEFB,	    //!< define byte(s) (8 bit)
	DEFW,	    //!< define word(s) (16 bit)
	DEFD,	    //!< define dword(s) (32 bit)
	DEFS,	    //!< define space (unitialized)
	TEXT,	    //!< define text (char*)
	TOKEN,	    //!< array of tokens
    };

    explicit z80Def(const QDomElement& elm = QDomElement());
    z80Def(const z80Def& other);

    bool isValid() const;

    QString symbol() const;
    QString comment() const;
    quint32 addr() const;
    EntryType type() const;
    QString arg0() const;
    quint32 param() const;
    quint32 maxelem() const;

    z80Def operator= (const z80Def& other);

    void setSymbol(const QString& symbol = QString());
    void setComment(const QString& comment = QString());
    void setAddr(const quint32 addr = 0);
    void setType(const EntryType type = CODE);
    void setArg0(const QString& arg0 = QString());
    void setParam(const quint32 param = 0);
    void setMaxelem(const quint32 maxelem = 0);

    static z80Def fromDomElement(const QDomElement& elm);
    static QDomElement toDomElement(const QDomDocument& doc, const z80Def& def);
private:
    QString	m_symbol;
    QString	m_comment;
    quint32	m_addr;
    EntryType	m_type;
    QString	m_arg0;
    quint32	m_param;
    quint32	m_maxelem;
};
