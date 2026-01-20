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

void TrvCodeEditor::setJson(QJsonDocument &jdoc)
{
    QString sText = jdoc.toJson();
    setPlainText(sText);
    setHighlighter(new QJSONHighlighter);
}

void TrvCodeEditor::setXml(QDomDocument &xdoc)
{
    QString sText = xdoc.toString();
    setPlainText(sText);
    setHighlighter(new QXMLHighlighter);
}

