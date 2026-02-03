#include "trv_defines.h"
#include "aboutdialog.h"
#include <QPainter>
#include <QTextStream>
#include <QPixmap>
#include <QTextEdit>
#include <QFile>

//static QSvgRenderer testsvg;

AboutDialog::AboutDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    int nWidth = 500;
    int nHeight = 400;
    if (parent != Q_NULLPTR)
        setGeometry(parent->x() + parent->width()/2 - nWidth/2,
            parent->y() + parent->height()/2 - nHeight/2,
            nWidth, nHeight);
    else
        resize(nWidth, nHeight);

    m_logo = new QPixmap(":/images/traverse.png");
    m_pInfo = new AboutInfo(this);
    m_pInfo->setGeometry(QRect(0,90,width(),180));
    m_pInfo->setReadOnly(true);
    //m_pInfo->palette().setColor(QPalette::Window,QColor(0,0,0));
    QFile file(":/content/about.html");
    if (file.open(QIODevice::ReadOnly)) {
        QString html = QString::fromUtf8(file.readAll());
        m_pInfo->setText(html);
    }
    m_hue = (int)rand()%360;
    m_timer = startTimer(10000);

    //testsvg.load(QString(":/images/push_toolbar_icon.svg"));
}

AboutDialog::~AboutDialog()
{
    killTimer(m_timer);
    delete m_logo;
    delete m_pInfo;
//    delete m_qt_logo;
//    delete m_libgit_logo;
}

void AboutDialog::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void AboutDialog::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    close();
}

void AboutDialog::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    m_hue += 10;
    if (m_hue >= 360) {m_hue = 0; }
    update();
}

void AboutDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    //p.setRenderHint(QPainter::Antialiasing,true);
    //p.setRenderHint(QPainter::TextAntialiasing,true);
    QColor ltTxtClr(180,180,180);
    QRectF frame(QPointF(0,0), geometry().size());
    qreal w = m_logo->width();
    qreal h = m_logo->height();
    QPointF pixpos = frame.bottomRight() - QPointF(w,h/2);

    QLinearGradient linearGradient(0, 0, 0, height());
    linearGradient.setColorAt(0.0, QColor::fromHsl(m_hue,120,120));
    linearGradient.setColorAt(1.0, QColor::fromHsl(m_hue,100,20));
    //p.setBrush(linearGradient);
    p.fillRect(0, 0, width(), height(), linearGradient);
    QFont f("Helvetica", 28, QFont::Bold);
    p.setFont(f);
    p.setPen(QColor(0, 0, 0));
    p.setOpacity(1);
    p.drawText(QPointF(20,40), QString("Traverse"));
 #ifdef Q_OS_WIN
    int size = 10;
 #else
    int size = 13;
 #endif
    p.setFont(QFont("Helvetica", size));

    p.setPen(ltTxtClr);
    QString vers;
    QTextStream(&vers) << "version: " << TRV_APP_VERSION;

    p.setPen(QColor(0,0,0));
    p.drawText(QPointF(20,60), vers);
    p.setPen(ltTxtClr);
    p.setBrush(QColor(0,0,0));
    p.setOpacity(.4);
    //hmm reducing by 80% seems to produce the correct size
    p.drawPixmap(pixpos.x(), pixpos.y(), w*.8, h*.8, *m_logo);
}


AboutInfo::AboutInfo(QWidget *parent) : QTextEdit(parent)
{
    setStyleSheet("AboutInfo {background-color:rgba(0,0,0,100);color:#aaa;border:none;}");
    setWindowOpacity(0.5);
}
