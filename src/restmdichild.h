#ifndef RESTMDICHILD_H
#define RESTMDICHILD_H

#include <QMdiSubWindow>

QT_BEGIN_NAMESPACE
class QSplitter;
class QTextEdit;
class QComboBox;
class QLineEdit;
class QTabWidget;
class QNetworkAccessManager;
class QRestAccessManager;
QT_END_NAMESPACE

class RequestHeader : public QWidget
{
    Q_OBJECT
public:
    explicit RequestHeader(QWidget *parent = nullptr);

    QString getURL();
    QString getMethod();

public slots:

private:
    QComboBox *m_pMethodCB;
    QLineEdit *m_pUrlEdit;
    QToolButton *m_pSendBtn;
};

class ParamsTab: public QWidget
{
    Q_OBJECT
public:
    explicit ParamsTab(QWidget *parent = nullptr);

};

class AuthTab: public QWidget
{
    Q_OBJECT
public:
    explicit AuthTab(QWidget *parent = nullptr);

};

class HeadersTab: public QWidget
{
    Q_OBJECT
public:
    explicit HeadersTab(QWidget *parent = nullptr);

};

class BodyTab: public QWidget
{
    Q_OBJECT
public:
    explicit BodyTab(QWidget *parent = nullptr);

};

class RequestForm : public QWidget
{
    Q_OBJECT
public:
    explicit RequestForm(QWidget *parent = nullptr);

private:
    QTabWidget *m_pTabs;
};

class RestMdiChild : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit RestMdiChild(QWidget *parent = nullptr);

public slots:
    void sendRequest();

signals:

private:
    QSplitter *m_pSplit;
    QSplitter *m_pSubSplit;
    RequestHeader *m_pRequestHeader;
    RequestForm *m_pRequestForm;
    QTextEdit *m_pResults;
    QRestAccessManager *m_pRestMan;
    QNetworkAccessManager *m_pNetMan;
};

#endif // RESTMDICHILD_H
