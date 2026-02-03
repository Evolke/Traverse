#include "authtab.h"
#include "../dropdowntabwidget.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>

AuthTab::AuthTab(QWidget *parent)
    : RequestOptionTab{parent}
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_pDTabs = new DropDownTabWidget(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_pDTabs);
    m_pDTabs->addTab(new QWidget(m_pDTabs),tr("No Auth"));
    m_pDTabs->addTab(new BasicAuth(m_pDTabs),tr("Basic Auth"));
    m_pDTabs->addTab(new BearerTokenAuth(m_pDTabs),tr("Bearer Token"));
    m_pDTabs->addTab(new ApiKeyAuth(m_pDTabs), tr("Api Key"));

    setLayout(layout);
}

/**
 * @brief AuthTab::getHeaders
 * @return
 */
KeyValList AuthTab::getHeaders()
{
    KeyValList ret;
    QWidget *pCurrentTab = m_pDTabs->currentTab();
    switch ((AuthTypes)m_pDTabs->currentIndex()) {
        case NO_AUTH:
            break;
        case BASIC_AUTH:
        {
            BasicAuth *pBasicAuth = qobject_cast<BasicAuth*>(pCurrentTab);
            QString sAuth = pBasicAuth->getUsername() + ":" + pBasicAuth->getPassword();
            sAuth = sAuth.toUtf8().toBase64();
            QString value = "Basic "+sAuth;
            ret.append(KeyValItem("Authorization", value));
        }
            break;
        case BEARER_TOKEN_AUTH:
        {
            BearerTokenAuth *pBTAuth = qobject_cast<BearerTokenAuth*>(pCurrentTab);
            QString value = "Bearer "+pBTAuth->getToken();
            ret.append(KeyValItem("Authorization",value));
        }
            break;
        case API_KEY_AUTH:
        {
            ApiKeyAuth *pApiKeyAuth = qobject_cast<ApiKeyAuth*>(pCurrentTab);
            ret.append(KeyValItem(pApiKeyAuth->getKey(), pApiKeyAuth->getValue()));
        }
            break;
    }

    return ret;
}

/**
 * @brief BasicAuth::BasicAuth
 * @param parent
 */
BasicAuth::BasicAuth(QWidget *parent)
    : QWidget(parent)
{
    QFormLayout *layout = new QFormLayout(this);
    m_pUserEdit = new QLineEdit(this);
    m_pPwdEdit = new QLineEdit(this);
    layout->addRow(tr("Username"), m_pUserEdit);
    layout->addRow(tr("Password"), m_pPwdEdit);
    setLayout(layout);
}

/**
 * @brief BasicAuth::getUsername
 * @return
 */
QString BasicAuth::getUsername()
{
    return m_pUserEdit->text().trimmed();
}

/**
 * @brief BasicAuth::getPassword
 * @return
 */
QString BasicAuth::getPassword()
{
    return m_pPwdEdit->text().trimmed();
}

/**
 * @brief BearerTokenAuth::BearerTokenAuth
 * @param parent
 */
BearerTokenAuth::BearerTokenAuth(QWidget *parent)
    : QWidget(parent)
{
    QFormLayout *layout = new QFormLayout(this);
    m_pTokenEdit = new QLineEdit(this);
    layout->addRow(tr("Token"), m_pTokenEdit);
    setLayout(layout);
}

/**
 * @brief BearerTokenAuth::getToken
 * @return
 */
QString BearerTokenAuth::getToken()
{
    return m_pTokenEdit->text().trimmed();
}

/**
 * @brief ApiKeyAuth::ApiKeyAuth
 * @param parent
 */
ApiKeyAuth::ApiKeyAuth(QWidget *parent)
    : QWidget(parent)
{
    QFormLayout *layout = new QFormLayout(this);
    m_pLocation = new QComboBox(this);
    m_pLocation->addItems({"Header", "Params"});
    m_pKeyEdit = new QLineEdit(this);
    m_pValEdit = new QLineEdit(this);
    layout->addRow(tr("Location"), m_pLocation);
    layout->addRow(tr("Key"), m_pKeyEdit);
    layout->addRow(tr("Value"), m_pValEdit);
    setLayout(layout);
}

/**
 * @brief ApiKeyAuth::getKey
 * @return
 */
QString ApiKeyAuth::getKey()
{
    return m_pKeyEdit->text().trimmed();
}

/**
 * @brief ApiKeyAuth::getValue
 * @return
 */
QString ApiKeyAuth::getValue()
{
    return m_pValEdit->text().trimmed();
}
