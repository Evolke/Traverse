#ifndef AUTHTAB_H
#define AUTHTAB_H

#include "requestoptions.h"

class  DropDownTabWidget;

class AuthTab : public RequestOptionTab
{
    Q_OBJECT
public:
    explicit AuthTab(QWidget *parent = nullptr);

signals:

private:
    DropDownTabWidget *m_pDTabs;
};

#endif // AUTHTAB_H
