#include "trvcodeeditor.h"

#include <QFile>
#include <QSyntaxStyle>
#include <QJSONHighlighter>
#include <QXMLHighlighter>
#include <QJsonDocument>
#include <QtXml/QDomDocument>

TrvCodeEditor::TrvCodeEditor(QWidget *parent)
    : QCodeEditor{parent}
{
    loadDraculaStyle();
    setHighlighter(new QJSONHighlighter());
}

void TrvCodeEditor::loadDraculaStyle()
{
    QFile f(":/editor_styles/dracula.xml");
    if (!f.open(QIODevice::ReadOnly)) { return; }
    auto style = new QSyntaxStyle(this);

    if (!style->load(f.readAll()))
    {
        delete style;
        return;
    }

    setSyntaxStyle(style);
}

void TrvCodeEditor::setJson(QString &text)
{
    setPlainText(text);
    setHighlighter(new QJSONHighlighter);
}

void TrvCodeEditor::setXml(QString &text)
{
    setPlainText(text);
    setHighlighter(new QXMLHighlighter);
}

void TrvCodeEditor::setText(QString &text)
{
    setPlainText(text);
}
