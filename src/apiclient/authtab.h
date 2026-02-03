#ifndef AUTHTAB_H
#define AUTHTAB_H

#include "requestoptions.h"
#include "../trv_defines.h"

class  DropDownTabWidget;
class QLineEdit;


enum AuthTypes {
    NO_AUTH=0,
    BASIC_AUTH=1,
    BEARER_TOKEN_AUTH=2,
    API_KEY_AUTH=3
};

class BasicAuth : public QWidget
{
    Q_OBJECT
public:
    explicit BasicAuth(QWidget *parent = nullptr);

    QString getUsername();
    QString getPassword();

private:
    QLineEdit *m_pUserEdit, *m_pPwdEdit;
};


class BearerTokenAuth : public QWidget
{
    Q_OBJECT
public:
    explicit BearerTokenAuth(QWidget *parent = nullptr);

    QString getToken();

private:
    QLineEdit *m_pTokenEdit;
};

class ApiKeyAuth : public QWidget
{
    Q_OBJECT
public:
    explicit ApiKeyAuth(QWidget *parent = nullptr);

    QString getKey();
    QString getValue();

private:
    QComboBox *m_pLocation;
    QLineEdit *m_pKeyEdit, *m_pValEdit;
};

class AuthTab : public RequestOptionTab
{
    Q_OBJECT
public:
    explicit AuthTab(QWidget *parent = nullptr);

    KeyValList getHeaders();

signals:

private:
    DropDownTabWidget *m_pDTabs;
};

#endif // AUTHTAB_H
