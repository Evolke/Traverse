#include "trvscintillaedit.h"

#include <QRegularExpression>
#include <ILexer.h>
#include <include/SciLexer.h>
#include <include/lexilla.h>
#include <QColor>
#include <QLatin1Char>
#include <QFontMetrics>
#include <QFile>
#include <QDomDocument>

#define DEFAULT_FONT "Courier New"

TrvScintillaEdit::TrvScintillaEdit(QWidget *parent)
    : ScintillaEdit{parent}, m_font(DEFAULT_FONT)
{
    m_pStyles = nullptr;
    setMargins(SC_MAX_MARGIN + 2);

    QColor foreClr = QColor::fromRgb(200,200,200);
    QColor backClr = QColor::fromRgb(52,52,52);
    QColor marginBackClr = QColor::fromRgb(48,48,48);
    QColor selClr = QColor::fromRgb(32,32,32);

    styleSetFore(STYLE_DEFAULT,QCOLOR_TO_SPRT(foreClr));
    styleSetBack(STYLE_DEFAULT,QCOLOR_TO_SPRT(backClr));
    styleSetBack(SC_MARGIN_BACK,QCOLOR_TO_SPRT(backClr));
    styleSetBack(SC_MARGIN_FORE,QCOLOR_TO_SPRT(backClr));
    setCaretFore(QCOLOR_TO_SPRT(foreClr));
    styleSetFont(STYLE_DEFAULT, m_font.family().toUtf8().constData());
    styleSetSize(STYLE_DEFAULT, 10);
    updateLineNumberMarginWidth(1, m_font, lineCount());

    styleClearAll();
    styleSetBack(STYLE_LINENUMBER,QCOLOR_TO_SPRT(marginBackClr));
    styleSetFore(STYLE_LINENUMBER,QCOLOR_TO_SPRT(foreClr));
    setMarginBackN(MARGIN_LINE_NUMBERS, QCOLOR_TO_SPRT(marginBackClr));
    setMarginBackN(SC_MARGIN_COLOUR, QCOLOR_TO_SPRT(marginBackClr));
    setSelFore(true, QCOLOR_TO_SPRT(foreClr));
    setSelBack(true, QCOLOR_TO_SPRT(selClr));

    connect(this, &TrvScintillaEdit::modified, this, &TrvScintillaEdit::onModified);
    connect(this, &TrvScintillaEdit::marginClicked, this, &TrvScintillaEdit::toggleFolding);
}

int TrvScintillaEdit::stringWidth(const QString& string, const QFontMetrics& metrics) {
    const QStringList lines = string.split(QLatin1Char('\n'));
    int width = 0;

    foreach (const QString& line, lines) {
        int line_width = metrics.boundingRect(line).width();

        if (line_width > width) {
            width = line_width;
        }
    }

    return width;
}

void TrvScintillaEdit::onModified(Scintilla::ModificationFlags type, Scintilla::Position position, Scintilla::Position length, Scintilla::Position linesAdded,
                                  const QByteArray &text, Scintilla::Position line, Scintilla::FoldLevel foldNow, Scintilla::FoldLevel foldPrev)
{
    if (linesAdded != 0) {
        updateLineNumberMarginWidth(1, m_font, lineCount());
    }
}

void TrvScintillaEdit::toggleFolding(Scintilla::Position position, Scintilla::KeyMod modifiers, int margin) {
    Q_UNUSED(modifiers)

    const sptr_t line_number = lineFromPosition(position);

    switch (margin) {
    case MARGIN_FOLDING:
        toggleFold(line_number);
        break;

    default:
        break;
    }
}


void TrvScintillaEdit::updateLineNumberMarginWidth(sptr_t zoom, QFont font, sptr_t line_count) {
    // Set point size and add some padding.
    font.setPointSize(font.pointSize() + int(zoom));

    QFontMetrics metr(font);
    int width = stringWidth(QString::number(line_count), metr) + MARGIN_PADDING_LINE_NUMBERS +5;

    setMarginWidthN(MARGIN_LINE_NUMBERS, qMax(MARGIN_LINE_NUMBERS_MIN_WIDTH + MARGIN_PADDING_LINE_NUMBERS, width));
    setMarginWidthN(MARGIN_LINE_NUMBERS_RIGHT_SPACE, MARGIN_PADDING_LINE_NUMBERS);
}

void TrvScintillaEdit::setMargins(sptr_t margins)
{
    send(SCI_SETMARGINS, margins, 0);
}

void TrvScintillaEdit::setMarginWidthN(sptr_t margin, sptr_t pixelWidth)
{
    send(SCI_SETMARGINWIDTHN, margin, pixelWidth);
}

void TrvScintillaEdit::styleSetFore(sptr_t style, sptr_t fore) {
    send(SCI_STYLESETFORE, style, fore);
}

void TrvScintillaEdit::styleSetBack(sptr_t style, sptr_t back) {
    send(SCI_STYLESETBACK, style, back);
}

void TrvScintillaEdit::setSelFore(bool useSetting, sptr_t fore) {
    send(SCI_SETSELFORE, useSetting, fore);
}

void TrvScintillaEdit::setSelBack(bool useSetting, sptr_t back) {
    send(SCI_SETSELBACK, useSetting, back);
}

void TrvScintillaEdit::styleClearAll() {
    send(SCI_STYLECLEARALL, 0, 0);
}

void TrvScintillaEdit::markerDefine(sptr_t markerNumber, sptr_t markerSymbol) {
    send(SCI_MARKERDEFINE, markerNumber, markerSymbol);
}

void TrvScintillaEdit::markerSetFore(sptr_t markerNumber, sptr_t fore) {
    send(SCI_MARKERSETFORE, markerNumber, fore);
}

void TrvScintillaEdit::markerSetBack(sptr_t markerNumber, sptr_t back) {
    send(SCI_MARKERSETBACK, markerNumber, back);
}

void TrvScintillaEdit::markerSetAlpha(sptr_t markerNumber, sptr_t alpha) {
    send(SCI_MARKERSETALPHA, markerNumber, alpha);
}

void TrvScintillaEdit::setMarginBackN(sptr_t margin, sptr_t back) {
    send(SCI_SETMARGINBACKN, margin, back);
}

void TrvScintillaEdit::setFoldMarginColour(bool useSetting, sptr_t back) {
    send(SCI_SETFOLDMARGINCOLOUR, useSetting, back);
}

void TrvScintillaEdit::setFoldMarginHiColour(bool useSetting, sptr_t fore) {
    send(SCI_SETFOLDMARGINHICOLOUR, useSetting, fore);
}

void TrvScintillaEdit::setCaretFore(sptr_t fore) {
    send(SCI_SETCARETFORE, fore, 0);
}

void TrvScintillaEdit::setMarginSensitiveN(sptr_t margin, bool sensitive) {
    send(SCI_SETMARGINSENSITIVEN, margin, sensitive);
}

void TrvScintillaEdit::setMarginMaskN(sptr_t margin, sptr_t mask) {
    send(SCI_SETMARGINMASKN, margin, mask);
}

void TrvScintillaEdit::markerEnableHighlight(bool enabled) {
    send(SCI_MARKERENABLEHIGHLIGHT, enabled, 0);
}

void TrvScintillaEdit::setIdleStyling(sptr_t idleStyling) {
    send(SCI_SETIDLESTYLING, idleStyling, 0);
}

void TrvScintillaEdit::indicSetStyle(sptr_t indicator, sptr_t indicatorStyle) {
    send(SCI_INDICSETSTYLE, indicator, indicatorStyle);
}

void TrvScintillaEdit::indicSetFore(sptr_t indicator, sptr_t fore) {
    send(SCI_INDICSETFORE, indicator, fore);
}

void TrvScintillaEdit::setProperty(const char * key, const char * value) {
    send(SCI_SETPROPERTY, (sptr_t)key, (sptr_t)value);
}

void TrvScintillaEdit::setText(const char * text) {
    send(SCI_SETTEXT, 0, (sptr_t)text);
    QFont f(DEFAULT_FONT);
    updateLineNumberMarginWidth(1, m_font, lineCount());
}

QByteArray TrvScintillaEdit::getText(sptr_t length) {
    return TextReturner(SCI_GETTEXT, length);
}

sptr_t TrvScintillaEdit::textLength() const {
    return send(SCI_GETTEXTLENGTH, 0, 0);
}

void TrvScintillaEdit::styleSetFont(sptr_t style, const char * fontName) {
    send(SCI_STYLESETFONT, style, (sptr_t)fontName);
}

void TrvScintillaEdit::styleSetSize(sptr_t style, sptr_t sizePoints) {
    send(SCI_STYLESETSIZE, style, sizePoints);
}

sptr_t TrvScintillaEdit::lineCount() const {
    return send(SCI_GETLINECOUNT, 0, 0);
}

sptr_t TrvScintillaEdit::lineFromPosition(sptr_t pos) {
    return send(SCI_LINEFROMPOSITION, pos, 0);
}

void TrvScintillaEdit::toggleFold(sptr_t line) {
    send(SCI_TOGGLEFOLD, line, 0);
}


void TrvScintillaEdit::setILexer(sptr_t ilexer) {
    send(SCI_SETILEXER, 0, ilexer);
}

void TrvScintillaEdit::setKeyWords(sptr_t keyWordSet, const char * keyWords) {
    send(SCI_SETKEYWORDS, keyWordSet, (sptr_t)keyWords);
}


void TrvScintillaEdit::clearDocumentStyle() {
    send(SCI_CLEARDOCUMENTSTYLE, 0, 0);
}

void TrvScintillaEdit::colourise(sptr_t start, sptr_t end) {
    send(SCI_COLOURISE, start, end);
}

void TrvScintillaEdit::setFoldFlags(sptr_t flags) {
    send(SCI_SETFOLDFLAGS, flags, 0);
}

void TrvScintillaEdit::setupFoldSettings()
{
    QColor foreClr = QColor::fromRgb(120,120,120);
    QColor marginBackClr = QColor::fromRgb(48,48,48);

    setProperty("fold", "1");
    setProperty("fold.compact", "1");
    setProperty("fold.html", "1");
    setMarginWidthN(MARGIN_FOLDING, MARGIN_WIDTH_FOLDING);
    setMarginSensitiveN(MARGIN_FOLDING, true);
    setMarginMaskN(MARGIN_FOLDING, SC_MASK_FOLDERS);

    markerDefine(SC_MARKNUM_FOLDER, SC_MARK_PLUS);
    markerDefine(SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS);
    markerDefine(SC_MARKNUM_FOLDEREND, SC_MARK_CIRCLEPLUSCONNECTED);
    markerDefine(SC_MARKNUM_FOLDEROPENMID, SC_MARK_CIRCLEMINUSCONNECTED);
    markerDefine(SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
    markerDefine(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER);
    markerDefine(SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNER);

    markerSetBack(SC_MARKNUM_FOLDER, QCOLOR_TO_SPRT(foreClr));
    //markerSetFore(SC_MARKNUM_FOLDER, QCOLOR_TO_SPRT(foreClr));
    markerSetBack(SC_MARKNUM_FOLDEROPEN, QCOLOR_TO_SPRT(foreClr));
    markerSetBack(SC_MARKNUM_FOLDEREND, QCOLOR_TO_SPRT(foreClr));
    markerSetBack(SC_MARKNUM_FOLDEROPENMID, QCOLOR_TO_SPRT(foreClr));

    //markerSetFore(SC_MARKNUM_FOLDEROPEN, QCOLOR_TO_SPRT(foreClr));

    // Lines.
    markerSetBack(SC_MARKNUM_FOLDERSUB, QCOLOR_TO_SPRT(foreClr));
    //markerSetFore(SC_MARKNUM_FOLDERSUB, QCOLOR_TO_SPRT(foreClr));
    markerSetBack(SC_MARKNUM_FOLDERMIDTAIL, QCOLOR_TO_SPRT(foreClr));
    //markerSetFore(SC_MARKNUM_FOLDERMIDTAIL, QCOLOR_TO_SPRT(foreClr));
    markerSetBack(SC_MARKNUM_FOLDERTAIL, QCOLOR_TO_SPRT(foreClr));
    //markerSetFore(SC_MARKNUM_FOLDERTAIL, QCOLOR_TO_SPRT(foreClr));
    setFoldFlags(SC_FOLDFLAG_LINEAFTER_CONTRACTED | SC_FOLDFLAG_LINEBEFORE_CONTRACTED);

    setMarginSensitiveN(MARGIN_FOLDING, true);
    setFoldMarginColour(true, QCOLOR_TO_SPRT(marginBackClr));
    setFoldMarginHiColour(true, QCOLOR_TO_SPRT(marginBackClr));
    styleSetBack(SC_MARGIN_BACK,QCOLOR_TO_SPRT(marginBackClr));
    //styleSetBack(SC_MARGIN_FORE,QCOLOR_TO_SPRT(foreClr));
    //markerEnableHighlight(true);
}

void TrvScintillaEdit::setupKeywords(lang langType)
{
    QStringList keywords;

    switch (langType) {
        case JSON:
            keywords = {
                "false null true",
                "@id @context @type @value @language @container @list @set @reverse @index @base @vocab @graph"
            };
            break;
        case XML:
            keywords = {"ATTLIST DOCTYPE ELEMENT ENTITY NOTATION"};
            break;
        case HTML:
            break;
    }

    if (keywords.length()) {
        for (int i=0; i < keywords.length(); i++) {
            QByteArray data = keywords[i].toUtf8();
            setKeyWords(i, data.constData());
        }
    }
}

bool TrvScintillaEdit::loadStyles()
{
    QFile f(":/editor_styles/nppmaterial.xml");
    if (!f.open(QIODevice::ReadOnly)) { return false; }
    if (m_pStyles == nullptr) {
        m_pStyles = new QDomDocument();
    }
    QByteArray data = f.readAll();
    if (data.length() > 0) {
        m_pStyles->setContent(data);
        return true;
    }

    return false;
}

void TrvScintillaEdit::setStylesByName(QString sLangName)
{
    QDomNodeList list = m_pStyles->elementsByTagName("LexerType");
    if (list.length() > 0) {
        for (int i=0; i < list.length(); i++) {
            QDomElement elem = list.at(i).toElement();
            if (elem.attribute("name") == sLangName) {
                QDomNodeList styles = elem.elementsByTagName("WordsStyle");
                for (int j=0; j < styles.length(); j++) {
                    elem = styles.at(j).toElement();
                    int styleId = elem.attribute("styleID").toInt();
                    QString sFgColor = "#" + elem.attribute("fgColor");
                    QString sBgColor = "#" + elem.attribute("bgColor");
                    styleSetFore(styleId, QCOLOR_TO_SPRT(QColor::fromString(sFgColor)));
                    //styleSetBack(styleId, QCOLOR_TO_SPRT(QColor::fromString(sBgColor)));
                }
                break;
            }
        }
    }
}

void TrvScintillaEdit::setContentType(QString sContentType, bool bFold)
{
    if (!loadStyles()) { return; }

    if (sContentType.contains(QRegularExpression("(application|text)\\/json"))) {
        setILexer((sptr_t)CreateLexer("json"));
        setStylesByName("json");
        setProperty("lexer.json.escape.sequence","1");
        setupKeywords(JSON);
    } else if (sContentType.contains(QRegularExpression("application\\/(?:soap\\+)*xml"))) {
        setILexer((sptr_t)CreateLexer("xml"));
        setStylesByName("xml");
        setupKeywords(XML);
    } else {
        setILexer((sptr_t)CreateLexer("hypertext"));
        setStylesByName("html");
    }
    clearDocumentStyle();
    if (bFold) {
        setupFoldSettings();
    }
    colourise(0, -1);
}
