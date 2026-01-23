#ifndef REQUESTOPTIONS_H
#define REQUESTOPTIONS_H

#include <QWidget>

class QTabWidget;
class QTableWidget;
class QComboBox;
class TrvCodeEditor;
class QHttpHeaders;
class KeyValTableView;
class DropDownTabWidget;


class RequestOptionTab : public QWidget
{
    Q_OBJECT
public:
    explicit RequestOptionTab(QWidget *parent = nullptr);

    KeyValTableView *getKeyValTable() { return m_pKeyValTable; }

    void buildKeyValTable();

public slots:
    void addRow();

protected:
    KeyValTableView *m_pKeyValTable;
    DropDownTabWidget *m_pDTabs;
};

class ParamsTab: public RequestOptionTab
{
    Q_OBJECT
public:
    explicit ParamsTab(QWidget *parent = nullptr);

private:
};

class HeadersTab: public RequestOptionTab
{
    Q_OBJECT
public:
    explicit HeadersTab(QWidget *parent = nullptr);

private:
};

class BodyTab: public RequestOptionTab
{
    Q_OBJECT
public:
    explicit BodyTab(QWidget *parent = nullptr);

    TrvCodeEditor *getEditor() { return m_pBodyEditor; }
    int getBodyType();

private:
    TrvCodeEditor *m_pBodyEditor;
};

class RequestOptions : public QWidget
{
    Q_OBJECT
public:
    explicit RequestOptions(QWidget *parent = nullptr);

    QHttpHeaders getHeaders();
    int getBodyType();
    QString getBodyString();

signals:

private:
    QTabWidget *m_pTabs;
};

#endif // REQUESTOPTIONS_H
