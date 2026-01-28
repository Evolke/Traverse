#ifndef RESPONSEVIEW_H
#define RESPONSEVIEW_H

#include <QWidget>

class QTabWidget;
class QTableWidget;
class QLabel;
class TrvScintillaEdit;
class QHttpHeaders;

class ResponseStatus : public QWidget
{
    Q_OBJECT
public:
    explicit ResponseStatus(QWidget *parent = nullptr);

    void setStatus(int status);
    void setExecTime(qint64 time);
    void setSize(qsizetype size);

private:
    QLabel *m_pStatusLabel;
    QLabel *m_pSizeLabel;
    QLabel *m_pExecTimeLabel;
};

class ResponseView : public QWidget
{
    Q_OBJECT
public:
    explicit ResponseView(QWidget *parent = nullptr);

    void setDataWithHeaders(QString &formattedText, QString &contentType);
    void setStatus(int status);
    void setExecTime(qint64 time);
    void setHeaders(QHttpHeaders &headers);
    void setSize(qsizetype size);

signals:

private:
    TrvScintillaEdit *m_pEditor;
    QTableWidget *m_pHeadersTable;
    QTabWidget *m_pTabs;
    ResponseStatus *m_pStatus;

};

#endif // RESPONSEVIEW_H
