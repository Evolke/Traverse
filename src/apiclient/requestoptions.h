#ifndef REQUESTOPTIONS_H
#define REQUESTOPTIONS_H

#include <QWidget>
#include "../trv_defines.h"

class QTabWidget;
class QTableWidget;
class QComboBox;
class TrvScintillaEdit;
class QHttpHeaders;
class KeyValTableView;
class DropDownTabWidget;

enum RequestOptionTabs {
    AUTH_REQUEST_OPTION=0,
    BODY_REQUEST_OPTION=1,
    HEADERS_REQUEST_OPTION=2,
    PARAMS_REQUEST_OPTION=3
};

enum BodyTypes {
    NONE_BODY_TYPE=0,
    RAW_BODY_TYPE=1,
    FORM_DATA_BODY_TYPE=2
};

enum RawTypes {
    JSON_RAW_TYPE=0,
    XML_RAW_TYPE=1
};

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

class RawBodyTab: public QWidget
{
    Q_OBJECT
public:
    explicit RawBodyTab(QWidget *parent = nullptr);

    TrvScintillaEdit *getEditor() { return m_pBodyEditor; }
    int getRawBodyType();

public slots:
    void onRawTypeChanged(int index);

private:
    TrvScintillaEdit *m_pBodyEditor;
    QComboBox *m_pRawTypeCombo;
};

class BodyTab: public RequestOptionTab
{
    Q_OBJECT
public:
    explicit BodyTab(QWidget *parent = nullptr);

    TrvScintillaEdit *getEditor();
    int getBodyType();
    int getRawBodyType();
    KeyValList getFormDataList();

private:
};

class RequestOptions : public QWidget
{
    Q_OBJECT
public:
    explicit RequestOptions(QWidget *parent = nullptr);

    QHttpHeaders getHeaders();
    int getBodyType();
    QString getBodyString();
    QString getRawBodyContentType();
    KeyValList getBodyFormDataList();

signals:

private:
    QTabWidget *m_pTabs;
};

#endif // REQUESTOPTIONS_H
