#ifndef RESTMDICHILD_H
#define RESTMDICHILD_H

#include <QFrame>

class QSplitter;
class QTextEdit;
class QComboBox;
class QLineEdit;
class QTabWidget;
class QNetworkAccessManager;
class QRestAccessManager;
class ResponseView;
class RequestOptions;
class TrvFormatThread;

class RequestHeader : public QWidget
{
    Q_OBJECT
public:
    explicit RequestHeader(QWidget *parent = nullptr);

    QString getURL();
    QString getMethod();
    void toggleEnableSendButton();

public slots:

private:
    QComboBox *m_pMethodCB;
    QLineEdit *m_pUrlEdit;
    QToolButton *m_pSendBtn;
};



class RestMdiChild : public QFrame
{
    Q_OBJECT
public:
    explicit RestMdiChild(QWidget *parent = nullptr);
    ~RestMdiChild();

public slots:
    void sendRequest();
    void formatFinished(QString *formattedText, QString *contentType);

signals:

private:
    QSplitter *m_pSplit;
    QSplitter *m_pSubSplit;
    RequestHeader *m_pRequestHeader;
    RequestOptions *m_pRequestOptions;
    ResponseView *m_pResponse;
    QRestAccessManager *m_pRestMan;
    QNetworkAccessManager *m_pNetMan;
    qint64 m_startTime;
    TrvFormatThread *m_pFormatThread;
};

#endif // RESTMDICHILD_H
