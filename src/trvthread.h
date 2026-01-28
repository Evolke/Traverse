#ifndef TRVTHREAD_H
#define TRVTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QRestReply>

class TrvThread : public QThread
{
    Q_OBJECT
public:
    TrvThread(QObject *parent = nullptr);
    ~TrvThread();

signals:

protected:
    void startThread();
    void stopThread();

    QMutex m_mutex;
    QWaitCondition m_condition;

    bool m_bRestart;
    bool m_bAbort;
    QString m_sError;
    qint64 m_startTime;

};

class QNetworkAccessManager;
class QRestAccessManager;
class QNetworkRequest;

struct RequestOutput
{
    QHttpHeaders* m_pHeaders;
    int m_nStatus;
    qint64 m_execTime;
    QString m_formattedOutput;
};

class TrvFormatThread : public TrvThread
{
    Q_OBJECT
public:
    TrvFormatThread(QObject *parent=nullptr);

    void format(QByteArray data, QString contentType);

signals:
    void formatFinished(QString *formattedText, QString *contentType);

protected:
    void run() override;

private:
    QByteArray m_data;
    QString m_contentType;
    QString m_sFormattedText;
};

#endif // TRVTHREAD_H
