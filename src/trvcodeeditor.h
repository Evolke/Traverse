#ifndef TRVCODEEDITOR_H
#define TRVCODEEDITOR_H

#include <public/QCodeEditor>

class QDomDocument;

class TrvCodeEditor : public QCodeEditor
{
    Q_OBJECT
public:
    explicit TrvCodeEditor(QWidget *parent = nullptr);

    void setJson(QJsonDocument &jdoc);
    void setXml(QDomDocument &xdoc);

signals:

private:
    void loadDraculaStyle();
};

#endif // TRVCODEEDITOR_H
