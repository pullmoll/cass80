#pragma once
#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

#include "constants.h"

class Cass80Handler;

class CasXml : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QByteArray sha1_digest MEMBER m_sha1_digest READ sha1_digest WRITE set_sha1_digest NOTIFY sha1_digest_changed)
    Q_PROPERTY(machine_e machine MEMBER m_machine READ machine WRITE set_machine NOTIFY machine_changed)
    Q_PROPERTY(bool basic MEMBER m_basic READ basic WRITE set_basic NOTIFY basic_changed)
    Q_PROPERTY(QString hdr_name MEMBER m_hdr_name READ hdr_name WRITE set_hdr_name NOTIFY hdr_name_changed)
    Q_PROPERTY(QString hdr_author MEMBER m_hdr_author READ hdr_author WRITE set_hdr_author NOTIFY hdr_author_changed)
    Q_PROPERTY(QString hdr_copyright MEMBER m_hdr_copyright READ hdr_copyright WRITE set_hdr_copyright NOTIFY hdr_copyright_changed)
    Q_PROPERTY(QString hdr_description MEMBER m_hdr_description READ hdr_description WRITE set_hdr_description NOTIFY hdr_description_changed)
    Q_PROPERTY(quint8 sync MEMBER m_sync READ sync WRITE set_sync NOTIFY sync_changed)
    Q_PROPERTY(quint8 prefix MEMBER m_prefix READ prefix WRITE set_prefix NOTIFY prefix_changed)
    Q_PROPERTY(QString filename MEMBER m_filename READ filename WRITE set_filename NOTIFY filename_changed)
    Q_PROPERTY(CasBlockList blocks MEMBER m_blocks READ blocks WRITE set_blocks NOTIFY blocks_changed)
public:
    CasXml(QObject* parent = nullptr);

    QByteArray sha1_digest() const;
    machine_e machine() const;
    bool basic() const;
    QString hdr_name() const;
    QString hdr_author() const;
    QString hdr_copyright() const;
    QString hdr_description() const;
    quint8 sync() const;
    quint8 prefix() const;
    QString filename() const;
    CasBlockList blocks() const;

    QString toXml() const;

public slots:
    void set_data(const Cass80Handler* h);

    void set_sha1_digest(const QByteArray& digest);
    void set_machine(machine_e machine);
    void set_basic(bool basic);
    void set_hdr_name(const QString& hdr_name);
    void set_hdr_author(const QString& hdr_author);
    void set_hdr_copyright(const QString& hdr_copyright);
    void set_hdr_description(const QString& hdr_description);
    void set_sync(quint8 sync);
    void set_prefix(quint8 prefix);
    void set_filename(const QString& filename);
    void set_blocks(const CasBlockList& blocks);

signals:
    void sha1_digest_changed(const QByteArray& digest);
    void machine_changed(machine_e machine);
    void basic_changed(bool basic);
    void hdr_name_changed(const QString& hdr_name);
    void hdr_author_changed(const QString& hdr_author);
    void hdr_copyright_changed(const QString& hdr_copyright);
    void hdr_description_changed(const QString& hdr_description);
    void sync_changed(quint8 sync);
    void prefix_changed(quint8 prefix);
    void filename_changed(const QString& filename);
    void blocks_changed(CasBlockList& blocks);

private:
    QDomDocument cas_document() const;
    QDomElement block_element(QDomDocument& doc, int index) const;
    QByteArray m_sha1_digest;
    machine_e m_machine;
    bool m_basic;
    QString m_hdr_name;
    QString m_hdr_author;
    QString m_hdr_copyright;
    QString m_hdr_description;
    quint8 m_sync;
    quint8 m_prefix;
    QString m_filename;
    CasBlockList m_blocks;
};