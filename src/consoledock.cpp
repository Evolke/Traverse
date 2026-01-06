#include "consoledock.h"

#include <QTextEdit>

ConsoleDock::ConsoleDock(const QString &title, QWidget *parent, Qt::WindowFlags flags)
    : QDockWidget(title, parent, flags)
{
    m_pEdit = new QTextEdit(this);
    QString empty = "";
    m_pEdit->setText(empty);
    m_pEdit->setReadOnly(true);
    setWidget(m_pEdit);
}

void ConsoleDock::addText(const QString &text)
{
    m_pEdit->append(text);
}

