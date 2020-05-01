/****************************************************************************
 *
 * Cass80 tool - reading and writing cassette image files
 *
 * Copyright (C) 2020 Jürgen Buchmüller <pullmoll@t-online.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************/
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
    QByteArray digest() const;

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
