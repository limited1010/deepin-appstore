/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#include "common.h"
#include <QtGlobal>
#include <QFile>
#include <QTimer>
#include "LocalFileSystemReply.h"


LocalFileSystemReply::LocalFileSystemReply(QNetworkAccessManager::Operation UNUSED(op),
                                           const QNetworkRequest& req,
                                           QObject* parent,
                                           QIODevice* UNUSED(outgoingData)) : QNetworkReply(parent) {
    auto urlStr = req.url().path();
    if (urlStr == "/") {
        urlStr = "/index.html";
    }
    req.url().setPath(urlStr);
    this->setUrl(req.url());

    QFile file("/usr/share/deepin-appstore/webapp" + urlStr);
    const auto opened = file.open(ReadOnly | Unbuffered);
    if (opened) {
        this->content = file.readAll();
        file.close();

        this->open(ReadOnly | Unbuffered);
        if (urlStr.endsWith(".html")) {
            this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/html"));
        } else if (urlStr.endsWith(".appcache")) {
            this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/cache-manifest"));
        } else if (urlStr.endsWith(".js")) {
            this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/javascript"));
        } else if (urlStr.endsWith(".css")) {
            this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/css"));
        } else if (urlStr.endsWith(".svg")) {
            this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/svg+xml"));
        } else if (urlStr.endsWith(".gif")) {
            this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/gif"));
        } else if (urlStr.endsWith(".png")) {
            this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
        } else if (urlStr.endsWith(".ico")) {
            this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/x-icon"));
        } else if (urlStr.endsWith(".otf")) {
            this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/font-sfnt"));
        } else {
            qWarning() << "ContentType didn't specify for " << urlStr;
        }
    } else {
        QFile indexFile("/usr/share/deepin-appstore/webapp" + QString("/index.html"));
        const auto indexOpened = indexFile.open(ReadOnly | Unbuffered);
        if (indexOpened) {
            this->content = indexFile.readAll();
            indexFile.close();

            this->open(ReadOnly | Unbuffered);
            this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/html"));
        } else {
            qWarning() << "Cannot redirect to index.html";
        }
    }


    this->setHeader(QNetworkRequest::ContentLengthHeader, QVariant(content.size()));
    this->setOperation(QNetworkAccessManager::Operation::GetOperation);
    this->setAttribute(QNetworkRequest::CacheSaveControlAttribute, 0);
    this->setAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);

    this->timer = new QTimer(this);
    QObject::connect(this->timer, &QTimer::timeout, [this]() {
        Q_EMIT this->finished();
    });
    this->timer->setSingleShot(true);
    this->timer->start(0);
}

LocalFileSystemReply::~LocalFileSystemReply() {
    if (this->timer) {
        this->timer->stop();
        delete this->timer;
        this->timer = nullptr;
    }
}

qint64 LocalFileSystemReply::readData(char *data, qint64 maxlen) {
    if (this->offset >= this->content.size())
        return -1;

    qint64 number = qMin(maxlen, this->content.size() - this->offset);
    memcpy(data, this->content.constData() + this->offset, number);
    this->offset += number;

    return number;
}

qint64 LocalFileSystemReply::bytesAvailable() const {
    return this->content.size() - this->offset;
}


qint64 LocalFileSystemReply::writeData(const char* UNUSED(data), qint64 UNUSED(len)) {
    throw std::logic_error("Not Implemented(writeData)");
}

void LocalFileSystemReply::close() {
    throw std::logic_error("Not Implemented(close)");
}

void LocalFileSystemReply::abort() {

}

bool LocalFileSystemReply::isSequential() const {
    return true;
}

void LocalFileSystemReply::sslConfigurationImplementation(QSslConfiguration& UNUSED(configuration)) const {
    throw std::logic_error("Not Implemented(sslConfigurationImplementation)");
}

void LocalFileSystemReply::setSslConfigurationImplementation(const QSslConfiguration& UNUSED(configuration)) {
    throw std::logic_error("Not Implemented(setSslConfigurationImplementation)");
}

void LocalFileSystemReply::ignoreSslErrorsImplementation(const QList<QSslError>& UNUSED(list)) {
    throw std::logic_error("Not Implemented(ignoreSslErrorsImplementation)");
}

void LocalFileSystemReply::ignoreSslErrors() {
    throw std::logic_error("Not Implemented(ignoreSslErrors)");
}
