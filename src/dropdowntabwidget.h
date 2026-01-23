#ifndef DROPDOWNTABWIDGET_H
#define DROPDOWNTABWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QTabWidget>

class DropDownTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DropDownTabWidget(QWidget *parent = nullptr);

    int addTab(QWidget *page, const QString &label);
    int currentIndex();

public slots:
    void switchActiveTab(int index);

signals:

private:
    QComboBox *m_pTabsCombo;
    QTabWidget *m_pTabs;
};

#endif // DROPDOWNTABWIDGET_H
