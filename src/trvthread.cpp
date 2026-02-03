#include "trvthread.h"

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QtNetwork/QRestAccessManager>
#include <QtNetwork/QRestReply>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QtXml/QDomDocument>

TrvThread::TrvThread(QObject *parent)
    : QThread{parent}
{
    m_bRestart = false;
    m_bAbort = false;
}

TrvThread::~TrvThread()
{
    m_mutex.lock();
    m_bAbort = true;
    m_condition.wakeOne();
    m_mutex.unlock();

    wait();

}


void TrvThread::startThread()
{
    QMutexLocker locker(&m_mutex);

    if (!isRunning()) {
        start(LowPriority);
    } else {
        m_bRestart = true;
        m_condition.wakeOne();
    }
}

void TrvThread::stopThread()
{
    if (isRunning())
        wait();
    quit();
}


TrvFormatThread::TrvFormatThread(QObject *parent)
    : TrvThread(parent)
{
}

void TrvFormatThread::format(QByteArray data, QString contentType)
{
    stopThread();
    m_mutex.lock();
    m_data = data;
    m_contentType = contentType;
    m_mutex.unlock();
    startThread();
}

void TrvFormatThread::run()
{
    m_mutex.lock();
    if (m_contentType.contains(QRegularExpression("(application|text)\\/json"))) {
        QJsonDocument jdoc = QJsonDocument::fromJson(m_data);
        m_sFormattedText = jdoc.toJson();
    } else if (m_contentType.contains(QRegularExpression("application\\/(?:soap\\+)*xml"))) {
        QDomDocument xdoc;
        xdoc.setContent(m_data);
        m_sFormattedText = xdoc.toString();
    } else {
        m_sFormattedText = m_data;
    }
    m_mutex.unlock();

    emit formatFinished(&m_sFormattedText, &m_contentType);
}

