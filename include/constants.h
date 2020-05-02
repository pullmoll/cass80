#pragma once
#include <QtCore>

/** @brief machine type */
typedef enum {
    MACH_INVALID = -1,
    MACH_TRS80,
    MACH_EG2000
}   Cass80Machine;
Q_ENUMS(Cass80Machine);

/** @brief block type */
typedef enum {
    BT_INVALID = -1,
    BT_BASIC,
    BT_SYSTEM,
    BT_ENTRY,
    BT_RAW
}   Cass80BlockType;
Q_ENUMS(Cass80BlockType);

/** @brief States of the decoder status machine */
typedef enum {
    ST_INVALID = -1,
    ST_COMMENT,
    ST_EOF,
    ST_SILENCE,
    ST_NUL,
    ST_HEADER,
    ST_SYSTEM_BLOCKTYPE,
    ST_SYSTEM_COUNT,
    ST_SYSTEM_ADDR_LSB,
    ST_SYSTEM_ADDR_MSB,
    ST_SYSTEM_DATA,
    ST_SYSTEM_CSUM,
    ST_SYSTEM_ENTRY_LSB,
    ST_SYSTEM_ENTRY_MSB,
    ST_BASIC_ADDR_LSB,
    ST_BASIC_ADDR_MSB,
    ST_BASIC_LINE_LSB,
    ST_BASIC_LINE_MSB,
    ST_BASIC_DATA,
    ST_AFTER_ENTRY,
    ST_IGNORE
}   decoder_status_e;
Q_ENUMS(decoder_status_e);

/** @brief Special byte values in cassette data stream */
typedef enum {
    CAS_SILENCE		    = 0x00,	//!< TRS-80 tapes start with silence
    CAS_TRS80_SYNC          = 0xa5,	//!< TRS-80 synchronization pattern (binary 10100101)
    CAS_TRS80_BASIC_HEADER  = 0xd3,	//!< TRS-80 BASIC first byte
    CAS_CGENIE_PRELUDE	    = 0xaa,	//!< Colour-Genie tapes start with 1/0 patterns (binary 10101010)
    CAS_CGENIE_SYNC         = 0x66,	//!< Colour-Genie synchronization pattern (binary 01100110)
    CAS_SYSTEM_HEADER       = 0x55,	//!< Both machine's SYSTEM formats first byte
    CAS_SYSTEM_DATA         = 0x3c,	//!< Both machine's SYSTEM data block byte
    CAS_SYSTEM_ENTRY        = 0x78	//!< Both machine's SYSTEM entry block byte
}   cas_control_e;
Q_ENUMS(cas_control_e);

class Cass80Block
{
public:
    Cass80Block()
	: type(BT_INVALID), csum(0), addr(0), size(0), line(0), data()
    {}

    Cass80BlockType  type;
    uchar  csum;
    quint16 addr;
    quint16 size;
    quint16 line;
    QByteArray data;
    bool operator== (const Cass80Block& other)
    {
	return type == other.type &&
		csum == other.csum &&
		addr == other.addr &&
		size == other.size &&
		line == other.line &&
		data == other.data;
    }
};

typedef QList<Cass80Block> CasBlockList;
Q_DECLARE_METATYPE(CasBlockList);
