#include "authtab.h"
#include "../dropdowntabwidget.h"

#include <QVBoxLayout>

AuthTab::AuthTab(QWidget *parent)
    : RequestOptionTab{parent}
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_pDTabs = new DropDownTabWidget(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_pDTabs);
    m_pDTabs->addTab(new QWidget(m_pDTabs),"No Auth");
    m_pDTabs->addTab(new QWidget(m_pDTabs),"Basic Auth");
    m_pDTabs->addTab(new QWidget(m_pDTabs),"Bearer");

    setLayout(layout);
}
