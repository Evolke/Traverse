#include "dropdowntabwidget.h"

#include <QVBoxLayout>

DropDownTabWidget::DropDownTabWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    m_pTabsCombo = new QComboBox(this);
    m_pTabs = new QTabWidget(this);
    m_pTabs->tabBar()->hide();
    layout->addWidget(m_pTabsCombo);
    layout->addWidget(m_pTabs);
    setLayout(layout);
    connect(m_pTabsCombo, &QComboBox::currentIndexChanged, this, &DropDownTabWidget::switchActiveTab);
}

void DropDownTabWidget::switchActiveTab(int index)
{
    m_pTabs->setCurrentIndex(index);
}

int DropDownTabWidget::addTab(QWidget *page, const QString &label)
{
    m_pTabsCombo->addItem(label);
    return m_pTabs->addTab(page, label);
}

int DropDownTabWidget::currentIndex()
{
    return m_pTabs->currentIndex();
}

QWidget *DropDownTabWidget::currentTab()
{
    return m_pTabs->currentWidget();
}

QWidget *DropDownTabWidget::getTab(int index)
{
    return m_pTabs->widget(index);
}
