#pragma once
#include <QObject>
#include <QIODevice>
#include <QCryptographicHash>
#include "constants.h"

class BasicToken;

class Cass80Handler : public QObject
{
    Q_OBJECT
public:

    explicit Cass80Handler(QObject *parent = nullptr);

    bool isEmpty() const;
    bool isValid() const;

    machine_e machine() const;
    bool basic() const;
    QString hdr_name() const;
    QString hdr_author() const;
    QString hdr_copyright() const;
    QString hdr_description() const;
    quint8 sync() const;
    quint8 prefix() const;
    QString filename() const;
    quint16 blen() const;

    bool load(const QString& filename);
    bool load(QIODevice* device);

    bool has_lmoffset();
    bool undo_lmoffset();

    bool save(const QString& filename);

    int count() const;
    CasBlockList blocks() const;
    CasBlock block(int index) const;

    QStringList source() const;

public slots:
    bool set_blen(quint16 blen);

signals:
    void Info(QString message);
    void Error(QString message);

private:
    void reset();
    BasicToken* bas;
    int m_verbose;
    machine_e m_machine;
    uint8_t m_sync;
    QString m_hdr_name;
    QString m_hdr_author;
    QString m_hdr_copyright;
    QString m_hdr_description;
    QString m_filename;

    quint16 m_blen;
    quint16 m_addr;
    quint16 m_line;
    quint16 m_entry;
    quint16 m_size;
    quint8 m_prefix;
    quint8 m_csum;
    bool m_basic;

    QCryptographicHash m_sha1;
    QByteArray m_digest;
    QStringList m_source;
    CasBlockList  m_blocks;
    qint64 m_total_size;
};
