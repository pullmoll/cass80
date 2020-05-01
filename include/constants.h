#pragma once
#include <QtCore>

/** @brief machine type */
typedef enum {
    MACH_INVALID = -1,
    MACH_TRS80,
    MACH_EG2000
}   machine_e;
Q_ENUMS(machine_e);

/** @brief block type */
typedef enum {
    BT_INVALID = -1,
    BT_BASIC,
    BT_SYSTEM,
    BT_ENTRY,
    BT_RAW
}   block_type_e;
Q_ENUMS(block_type_e);

/** @brief decoder status */
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

typedef enum {
    CAS_SILENCE		    = 0x00,
    CAS_TRS80_SYNC          = 0xa5,
    CAS_TRS80_BASIC_HEADER  = 0xd3,
    CAS_CGENIE_SYNC         = 0x66,
    CAS_SYSTEM_HEADER       = 0x55,
    CAS_SYSTEM_DATA         = 0x3c,
    CAS_SYSTEM_ENTRY        = 0x78
}   cas_control_e;
Q_ENUMS(cas_control_e);

class CasBlock
{
public:
    CasBlock()
	: type(BT_INVALID), csum(0), addr(0), size(0), line(0), data()
    {}

    block_type_e  type;
    uchar  csum;
    quint16 addr;
    quint16 size;
    quint16 line;
    QByteArray data;
    bool operator== (const CasBlock& other)
    {
	return type == other.type &&
		csum == other.csum &&
		addr == other.addr &&
		size == other.size &&
		line == other.line &&
		data == other.data;
    }
};

typedef QList<CasBlock> CasBlockList;
Q_DECLARE_METATYPE(CasBlockList);
