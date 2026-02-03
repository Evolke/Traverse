#ifndef CONSOLEDOCK_H
#define CONSOLEDOCK_H

#include <QDockWidget>

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

class ConsoleDock : public QDockWidget
{
    Q_OBJECT
public:
    explicit ConsoleDock(const QString &title, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());

    void addText(const QString &text);

signals:

private:
    QTextEdit* m_pEdit;
};

#endif // CONSOLEDOCK_H
