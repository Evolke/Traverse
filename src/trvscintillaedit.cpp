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
    setTabWidth(4);

    setupGlobalStyles();

    updateLineNumberMarginWidth(1, m_font, lineCount());


    setScrollWidthTracking(true);
    setScrollWidth(1);
    //setWrapVisualFlags(SC_WRAPVISUALFLAG_MARGIN);

    connect(this, &TrvScintillaEdit::modified, this, &TrvScintillaEdit::onModified);
    connect(this, &TrvScintillaEdit::marginClicked, this, &TrvScintillaEdit::toggleFolding);
}

/**
 * @brief TrvScintillaEdit::setupGlobalStyles
 */
void TrvScintillaEdit::setupGlobalStyles()
{
    QColor foreClr = QColor::fromRgb(200,200,200);
    QColor backClr = QColor::fromRgb(52,52,52);
    QColor marginBackClr = QColor::fromRgb(48,48,48);
    QColor selClr = QColor::fromRgb(32,32,32);

    styleSetFore(STYLE_DEFAULT,QCOLOR_TO_SPRT(foreClr));
    styleSetBack(STYLE_DEFAULT,QCOLOR_TO_SPRT(backClr));
    styleSetFont(STYLE_DEFAULT, m_font.family().toUtf8().constData());
    styleSetSize(STYLE_DEFAULT, 10);
    styleClearAll();
    styleSetBack(SC_MARGIN_BACK,QCOLOR_TO_SPRT(backClr));
    styleSetBack(SC_MARGIN_FORE,QCOLOR_TO_SPRT(backClr));
    setCaretFore(QCOLOR_TO_SPRT(foreClr));

    styleSetBack(STYLE_LINENUMBER,QCOLOR_TO_SPRT(marginBackClr));
    styleSetFore(STYLE_LINENUMBER,QCOLOR_TO_SPRT(foreClr));
    setMarginBackN(MARGIN_LINE_NUMBERS, QCOLOR_TO_SPRT(marginBackClr));
    setMarginBackN(SC_MARGIN_COLOUR, QCOLOR_TO_SPRT(marginBackClr));
    //setSelFore(true, QCOLOR_TO_SPRT(foreClr));
    setSelBack(true, QCOLOR_TO_SPRT(selClr));

}

/**
 * @brief TrvScintillaEdit::stringWidth
 * @param string
 * @param metrics
 * @return
 */
int TrvScintillaEdit::stringWidth(const QString& string, const QFontMetrics& metrics) {
    const QStringList lines = string.split(QLatin1Char('\n'));
    int width = 0;

    auto nbWidth = textWidth(STYLE_LINENUMBER, "8");

    foreach (const QString& line, lines) {
        int line_width = metrics.boundingRect(line).width();

        if (line_width > width) {
            width = line_width;
        }
    }

    nbWidth *= 3;

    if (width < nbWidth) {
        width = nbWidth;
    }
    return width;
}

/**
 * @brief TrvScintillaEdit::onModified
 * @param type
 * @param position
 * @param length
 * @param linesAdded
 * @param text
 * @param line
 * @param foldNow
 * @param foldPrev
 */
void TrvScintillaEdit::onModified(Scintilla::ModificationFlags type, Scintilla::Position position, Scintilla::Position length, Scintilla::Position linesAdded,
                                  const QByteArray &text, Scintilla::Position line, Scintilla::FoldLevel foldNow, Scintilla::FoldLevel foldPrev)
{
    if (linesAdded != 0) {
        updateLineNumberMarginWidth(1, m_font, lineCount());
    }}

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

/**
 * @brief TrvScintillaEdit::updateLineNumberMarginWidth
 * @param zoom
 * @param font
 * @param line_count
 */
void TrvScintillaEdit::updateLineNumberMarginWidth(sptr_t zoom, QFont font, sptr_t line_count) {
    // Set point size and add some padding.
    font.setPointSize(font.pointSize() + int(zoom));

    QFontMetrics metr(font);
    int width = stringWidth(QString::number(line_count), metr) + MARGIN_PADDING_LINE_NUMBERS;

    setMarginWidthN(MARGIN_LINE_NUMBERS, width);
    setMarginWidthN(MARGIN_LINE_NUMBERS_RIGHT_SPACE, MARGIN_PADDING_LINE_NUMBERS);
}

/**
 * @brief TrvScintillaEdit::setScrollWidth
 * @param pixelWidth
 */
void TrvScintillaEdit::setScrollWidth(sptr_t pixelWidth) {
    send(SCI_SETSCROLLWIDTH, pixelWidth, 0);
}

/**
 * @brief TrvScintillaEdit::setScrollWidthTracking
 * @param tracking
 */
void TrvScintillaEdit::setScrollWidthTracking(bool tracking) {
    send(SCI_SETSCROLLWIDTHTRACKING, tracking, 0);
}

/**
 * @brief TrvScintillaEdit::setMargins
 * @param margins
 */
void TrvScintillaEdit::setMargins(sptr_t margins)
{
    send(SCI_SETMARGINS, margins, 0);
}

/**
 * @brief TrvScintillaEdit::setMarginWidthN
 * @param margin
 * @param pixelWidth
 */
void TrvScintillaEdit::setMarginWidthN(sptr_t margin, sptr_t pixelWidth)
{
    send(SCI_SETMARGINWIDTHN, margin, pixelWidth);
}

/**
 * @brief TrvScintillaEdit::textWidth
 * @param style
 * @param text
 * @return
 */
sptr_t TrvScintillaEdit::textWidth(sptr_t style, const char * text) {
    return send(SCI_TEXTWIDTH, style, (sptr_t)text);
}

/**
 * @brief TrvScintillaEdit::linesOnScreen
 * @return
 */
sptr_t TrvScintillaEdit::linesOnScreen() const {
    return send(SCI_LINESONSCREEN, 0, 0);
}

/**
 * @brief TrvScintillaEdit::firstVisibleLine
 * @return
 */
sptr_t TrvScintillaEdit::firstVisibleLine() const {
    return send(SCI_GETFIRSTVISIBLELINE, 0, 0);
}

/**
 * @brief TrvScintillaEdit::visibleFromDocLine
 * @param docLine
 * @return
 */
sptr_t TrvScintillaEdit::visibleFromDocLine(sptr_t docLine) {
    return send(SCI_VISIBLEFROMDOCLINE, docLine, 0);
}

/**
 * @brief TrvScintillaEdit::docLineFromVisible
 * @param displayLine
 * @return
 */
sptr_t TrvScintillaEdit::docLineFromVisible(sptr_t displayLine) {
    return send(SCI_DOCLINEFROMVISIBLE, displayLine, 0);
}

/**
 * @brief TrvScintillaEdit::setMarginLeft
 * @param pixelWidth
 */
void TrvScintillaEdit::setMarginLeft(sptr_t pixelWidth) {
    send(SCI_SETMARGINLEFT, 0, pixelWidth);
}

/**
 * @brief TrvScintillaEdit::styleSetFore
 * @param style
 * @param fore
 */
void TrvScintillaEdit::styleSetFore(sptr_t style, sptr_t fore) {
    send(SCI_STYLESETFORE, style, fore);
}

/**
 * @brief TrvScintillaEdit::styleSetBack
 * @param style
 * @param back
 */
void TrvScintillaEdit::styleSetBack(sptr_t style, sptr_t back) {
    send(SCI_STYLESETBACK, style, back);
}

/**
 * @brief TrvScintillaEdit::setSelFore
 * @param useSetting
 * @param fore
 */
void TrvScintillaEdit::setSelFore(bool useSetting, sptr_t fore) {
    send(SCI_SETSELFORE, useSetting, fore);
}

/**
 * @brief TrvScintillaEdit::setSelBack
 * @param useSetting
 * @param back
 */
void TrvScintillaEdit::setSelBack(bool useSetting, sptr_t back) {
    send(SCI_SETSELBACK, useSetting, back);
}

/**
 * @brief TrvScintillaEdit::styleClearAll
 */
void TrvScintillaEdit::styleClearAll() {
    send(SCI_STYLECLEARALL, 0, 0);
}

/**
 * @brief TrvScintillaEdit::markerDefine
 * @param markerNumber
 * @param markerSymbol
 */
void TrvScintillaEdit::markerDefine(sptr_t markerNumber, sptr_t markerSymbol) {
    send(SCI_MARKERDEFINE, markerNumber, markerSymbol);
}

/**
 * @brief TrvScintillaEdit::markerSetFore
 * @param markerNumber
 * @param fore
 */
void TrvScintillaEdit::markerSetFore(sptr_t markerNumber, sptr_t fore) {
    send(SCI_MARKERSETFORE, markerNumber, fore);
}

/**
 * @brief TrvScintillaEdit::markerSetBack
 * @param markerNumber
 * @param back
 */
void TrvScintillaEdit::markerSetBack(sptr_t markerNumber, sptr_t back) {
    send(SCI_MARKERSETBACK, markerNumber, back);
}

/**
 * @brief TrvScintillaEdit::markerSetAlpha
 * @param markerNumber
 * @param alpha
 */
void TrvScintillaEdit::markerSetAlpha(sptr_t markerNumber, sptr_t alpha) {
    send(SCI_MARKERSETALPHA, markerNumber, alpha);
}

/**
 * @brief TrvScintillaEdit::setMarginBackN
 * @param margin
 * @param back
 */
void TrvScintillaEdit::setMarginBackN(sptr_t margin, sptr_t back) {
    send(SCI_SETMARGINBACKN, margin, back);
}

/**
 * @brief TrvScintillaEdit::setFoldMarginColour
 * @param useSetting
 * @param back
 */
void TrvScintillaEdit::setFoldMarginColour(bool useSetting, sptr_t back) {
    send(SCI_SETFOLDMARGINCOLOUR, useSetting, back);
}

/**
 * @brief TrvScintillaEdit::setFoldMarginHiColour
 * @param useSetting
 * @param fore
 */
void TrvScintillaEdit::setFoldMarginHiColour(bool useSetting, sptr_t fore) {
    send(SCI_SETFOLDMARGINHICOLOUR, useSetting, fore);
}

/**
 * @brief TrvScintillaEdit::setCaretFore
 * @param fore
 */
void TrvScintillaEdit::setCaretFore(sptr_t fore) {
    send(SCI_SETCARETFORE, fore, 0);
}

/**
 * @brief TrvScintillaEdit::setMarginSensitiveN
 * @param margin
 * @param sensitive
 */
void TrvScintillaEdit::setMarginSensitiveN(sptr_t margin, bool sensitive) {
    send(SCI_SETMARGINSENSITIVEN, margin, sensitive);
}

/**
 * @brief TrvScintillaEdit::setMarginMaskN
 * @param margin
 * @param mask
 */
void TrvScintillaEdit::setMarginMaskN(sptr_t margin, sptr_t mask) {
    send(SCI_SETMARGINMASKN, margin, mask);
}

/**
 * @brief TrvScintillaEdit::markerEnableHighlight
 * @param enabled
 */
void TrvScintillaEdit::markerEnableHighlight(bool enabled) {
    send(SCI_MARKERENABLEHIGHLIGHT, enabled, 0);
}

/**
 * @brief TrvScintillaEdit::setIdleStyling
 * @param idleStyling
 */
void TrvScintillaEdit::setIdleStyling(sptr_t idleStyling) {
    send(SCI_SETIDLESTYLING, idleStyling, 0);
}

/**
 * @brief TrvScintillaEdit::indicSetStyle
 * @param indicator
 * @param indicatorStyle
 */
void TrvScintillaEdit::indicSetStyle(sptr_t indicator, sptr_t indicatorStyle) {
    send(SCI_INDICSETSTYLE, indicator, indicatorStyle);
}

/**
 * @brief TrvScintillaEdit::allocateSubStyles
 * @param styleBase
 * @param numberStyles
 * @return
 */
sptr_t TrvScintillaEdit::allocateSubStyles(sptr_t styleBase, sptr_t numberStyles) {
    return send(SCI_ALLOCATESUBSTYLES, styleBase, numberStyles);
}

/**
 * @brief TrvScintillaEdit::setIdentifiers
 * @param style
 * @param identifiers
 */
void TrvScintillaEdit::setIdentifiers(sptr_t style, const char * identifiers) {
    send(SCI_SETIDENTIFIERS, style, (sptr_t)identifiers);
}

/**
 * @brief TrvScintillaEdit::indicSetFore
 * @param indicator
 * @param fore
 */
void TrvScintillaEdit::indicSetFore(sptr_t indicator, sptr_t fore) {
    send(SCI_INDICSETFORE, indicator, fore);
}

/**
 * @brief TrvScintillaEdit::setProperty
 * @param key
 * @param value
 */
void TrvScintillaEdit::setProperty(const char * key, const char * value) {
    send(SCI_SETPROPERTY, (sptr_t)key, (sptr_t)value);
}

/**
 * @brief TrvScintillaEdit::setTabWidth
 * @param tabWidth
 */
void TrvScintillaEdit::setTabWidth(sptr_t tabWidth) {
    send(SCI_SETTABWIDTH, tabWidth, 0);
}

/**
 * @brief TrvScintillaEdit::setWrapMode
 * @param wrapMode
 */
void TrvScintillaEdit::setWrapMode(sptr_t wrapMode) {
    send(SCI_SETWRAPMODE, wrapMode, 0);
}

/**
 * @brief TrvScintillaEdit::wrapMode
 * @return
 */
sptr_t TrvScintillaEdit::wrapMode() const {
    return send(SCI_GETWRAPMODE, 0, 0);
}

/**
 * @brief TrvScintillaEdit::setWrapVisualFlags
 * @param wrapVisualFlags
 */
void TrvScintillaEdit::setWrapVisualFlags(sptr_t wrapVisualFlags) {
    send(SCI_SETWRAPVISUALFLAGS, wrapVisualFlags, 0);
}

/**
 * @brief TrvScintillaEdit::wrapVisualFlags
 * @return
 */
sptr_t TrvScintillaEdit::wrapVisualFlags() const {
    return send(SCI_GETWRAPVISUALFLAGS, 0, 0);
}

/**
 * @brief TrvScintillaEdit::setText
 * @param text
 */
void TrvScintillaEdit::setText(const char * text)
{
    bool bReadOnly = readOnly();
    if (bReadOnly) { setReadOnly(false); }
    send(SCI_SETTEXT, 0, (sptr_t)text);
    QFont f(DEFAULT_FONT);
    updateLineNumberMarginWidth(1, m_font, lineCount());
    if (bReadOnly) { setReadOnly(true); }
}

/**
 * @brief TrvScintillaEdit::setReadOnly
 * @param readOnly
 */
void TrvScintillaEdit::setReadOnly(bool readOnly) {
    send(SCI_SETREADONLY, readOnly, 0);
}

/**
 * @brief TrvScintillaEdit::readOnly
 * @return
 */
bool TrvScintillaEdit::readOnly() const {
    return send(SCI_GETREADONLY, 0, 0);
}

/**
 * @brief TrvScintillaEdit::getText
 * @param length
 * @return
 */
QByteArray TrvScintillaEdit::getText(sptr_t length) {
    return TextReturner(SCI_GETTEXT, length);
}

/**
 * @brief TrvScintillaEdit::textLength
 * @return
 */
sptr_t TrvScintillaEdit::textLength() const {
    return send(SCI_GETTEXTLENGTH, 0, 0);
}

/**
 * @brief TrvScintillaEdit::styleSetFont
 * @param style
 * @param fontName
 */
void TrvScintillaEdit::styleSetFont(sptr_t style, const char * fontName) {
    send(SCI_STYLESETFONT, style, (sptr_t)fontName);
}

/**
 * @brief TrvScintillaEdit::styleSetSize
 * @param style
 * @param sizePoints
 */
void TrvScintillaEdit::styleSetSize(sptr_t style, sptr_t sizePoints) {
    send(SCI_STYLESETSIZE, style, sizePoints);
}

/**
 * @brief TrvScintillaEdit::lineCount
 * @return
 */
sptr_t TrvScintillaEdit::lineCount() const {
    return send(SCI_GETLINECOUNT, 0, 0);
}

/**
 * @brief TrvScintillaEdit::lineFromPosition
 * @param pos
 * @return
 */
sptr_t TrvScintillaEdit::lineFromPosition(sptr_t pos) {
    return send(SCI_LINEFROMPOSITION, pos, 0);
}

/**
 * @brief TrvScintillaEdit::toggleFold
 * @param line
 */
void TrvScintillaEdit::toggleFold(sptr_t line) {
    send(SCI_TOGGLEFOLD, line, 0);
}

/**
 * @brief TrvScintillaEdit::setILexer
 * @param ilexer
 */
void TrvScintillaEdit::setILexer(sptr_t ilexer) {
    send(SCI_SETILEXER, 0, ilexer);
}

/**
 * @brief TrvScintillaEdit::setKeyWords
 * @param keyWordSet
 * @param keyWords
 */
void TrvScintillaEdit::setKeyWords(sptr_t keyWordSet, const char * keyWords) {
    send(SCI_SETKEYWORDS, keyWordSet, (sptr_t)keyWords);
}

/**
 * @brief TrvScintillaEdit::clearDocumentStyle
 */
void TrvScintillaEdit::clearDocumentStyle() {
    send(SCI_CLEARDOCUMENTSTYLE, 0, 0);
}

/**
 * @brief TrvScintillaEdit::docPointer
 * @return
 */
sptr_t TrvScintillaEdit::docPointer() const {
    return send(SCI_GETDOCPOINTER, 0, 0);
}

/**
 * @brief TrvScintillaEdit::setDocPointer
 * @param doc
 */
void TrvScintillaEdit::setDocPointer(sptr_t doc) {
    send(SCI_SETDOCPOINTER, 0, doc);
}

/**
 * @brief TrvScintillaEdit::colourise
 * @param start
 * @param end
 */
void TrvScintillaEdit::colourise(sptr_t start, sptr_t end) {
    send(SCI_COLOURISE, start, end);
}

/**
 * @brief TrvScintillaEdit::setFoldFlags
 * @param flags
 */
void TrvScintillaEdit::setFoldFlags(sptr_t flags) {
    send(SCI_SETFOLDFLAGS, flags, 0);
}

/**
 * @brief TrvScintillaEdit::setupFoldSettings
 */
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

    markerDefine(SC_MARKNUM_FOLDER, SC_MARK_ARROW);
    markerDefine(SC_MARKNUM_FOLDEROPEN, SC_MARK_ARROWDOWN);
    markerDefine(SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY);
    markerDefine(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY);
    markerDefine(SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY);
    markerDefine(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY);
    markerDefine(SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY);

    markerSetBack(SC_MARKNUM_FOLDER, QCOLOR_TO_SPRT(foreClr));
    markerSetFore(SC_MARKNUM_FOLDER, QCOLOR_TO_SPRT(marginBackClr));
    markerSetBack(SC_MARKNUM_FOLDEROPEN, QCOLOR_TO_SPRT(foreClr));
    markerSetFore(SC_MARKNUM_FOLDEROPEN, QCOLOR_TO_SPRT(marginBackClr));
    //markerSetBack(SC_MARKNUM_FOLDEROPENMID, QCOLOR_TO_SPRT(foreClr));

    //markerSetFore(SC_MARKNUM_FOLDEROPEN, QCOLOR_TO_SPRT(foreClr));

    // Lines.
    //markerSetBack(SC_MARKNUM_FOLDERSUB, QCOLOR_TO_SPRT(foreClr));
    //markerSetFore(SC_MARKNUM_FOLDERSUB, QCOLOR_TO_SPRT(foreClr));
    //markerSetBack(SC_MARKNUM_FOLDERMIDTAIL, QCOLOR_TO_SPRT(foreClr));
    //markerSetFore(SC_MARKNUM_FOLDERMIDTAIL, QCOLOR_TO_SPRT(foreClr));
    //markerSetBack(SC_MARKNUM_FOLDERTAIL, QCOLOR_TO_SPRT(foreClr));
    //markerSetFore(SC_MARKNUM_FOLDERTAIL, QCOLOR_TO_SPRT(foreClr));
    setFoldFlags(SC_FOLDFLAG_LINEBEFORE_CONTRACTED | SC_FOLDFLAG_LINEAFTER_CONTRACTED);

    setMarginSensitiveN(MARGIN_FOLDING, true);
    setFoldMarginColour(true, QCOLOR_TO_SPRT(marginBackClr));
    setFoldMarginHiColour(true, QCOLOR_TO_SPRT(marginBackClr));
    styleSetBack(SC_MARGIN_BACK,QCOLOR_TO_SPRT(marginBackClr));
    //styleSetBack(SC_MARGIN_FORE,QCOLOR_TO_SPRT(foreClr));
    markerEnableHighlight(true);
}

/**
 * @brief TrvScintillaEdit::setupKeywords
 * @param langType
 */
void TrvScintillaEdit::setupKeywords(lang langType)
{
    QStringList keywords;
    int subStyleCount = 0;
    int baseSubStyleId = 9;
    //int baseStyleId = 0;

    switch (langType) {
        case JSON:
            keywords = {
                "false null true",
                "@id @context @type @value @language @container @list @set @reverse @index @base @vocab @graph"
            };
            break;
        case XML:
            keywords = {"ATTLIST DOCTYPE ELEMENT ENTITY NOTATION"};
            subStyleCount = 8;
            baseSubStyleId = SCE_H_ATTRIBUTE;
            break;
        case HTML:
            keywords = {
                "charset accesskey acronym action address align alink alt applet archive area article aside async audio autocomplete autofocus axis b background base basefont bdi bdo bgcolor bgsound big blink blockquote body border br button canvas caption cellpadding cellspacing center char charoff charset checkbox checked cite class classid clear code codebase codetype col colgroup color cols colspan command compact content contenteditable contextmenu coords data datafld dataformatas datalist datapagesize datasrc datetime dd declare defer del details dfn dialog dir disabled div dl draggable dropzone dt element em embed enctype event face fieldset figcaption figure file font footer for form formaction formenctype formmethod formnovalidate formtarget frame frameborder frameset h1 h2 h3 h4 h5 h6 head header headers height hgroup hidden hr href hreflang hspace html http-equiv i id iframe image img input ins isindex ismap kbd keygen label lang language leftmargin legend li link list listing longdesc main manifest map marginheight marginwidth mark marquee max maxlength media menu menuitem meta meter method min minlength multicol multiple name nav nobr noembed noframes nohref noresize noscript noshade novalidate nowrap object ol onabort onafterprint onautocomplete onautocompleteerror onbeforeonload onbeforeprint onblur oncancel oncanplay oncanplaythrough onchange onclick onclose oncontextmenu oncuechange ondblclick ondrag ondragend ondragenter ondragleave ondragover ondragstart ondrop ondurationchange onemptied onended onerror onfocus onhashchange oninput oninvalid onkeydown onkeypress onkeyup onload onloadeddata onloadedmetadata onloadstart onmessage onmousedown onmouseenter onmouseleave onmousemove onmouseout onmouseover onmouseup onmousewheel onoffline ononline onpagehide onpageshow onpause onplay onplaying onpointercancel onpointerdown onpointerenter onpointerleave onpointerlockchange onpointerlockerror onpointermove onpointerout onpointerover onpointerup onpopstate onprogress onratechange onreadystatechange onredo onreset onresize onscroll onseeked onseeking onselect onshow onsort onstalled onstorage onsubmit onsuspend ontimeupdate ontoggle onundo onunload onvolumechange onwaiting optgroup option output p param password pattern picture placeholder plaintext pre profile progress prompt public q radio readonly rel required reset rev reversed role rows rowspan rp rt rtc ruby rules s samp sandbox scheme scope scoped script seamless section select selected shadow shape size sizes small source spacer span spellcheck src srcdoc srcset standby start step strike strong style sub submit summary sup svg svg:svg tabindex table target tbody td template text textarea tfoot th thead time title topmargin tr track tt type u ul usemap valign value valuetype var version video vlink vspace wbr width xml xmlns xmp",
                "ATTLIST DOCTYPE ELEMENT ENTITY NOTATION"
            };
            break;
    }

    if (keywords.length()) {
        for (int i=0; i < keywords.length(); i++) {
            QByteArray data = keywords[i].toUtf8();
            setKeyWords(i, data.constData());
        }
    }

    if (subStyleCount > 0) {
        int firstID = allocateSubStyles(baseSubStyleId, subStyleCount) & 0xFF;
        int styleID = 0;
        for (int j=0; j < subStyleCount; j++) {
            styleID = firstID + j;
            setIdentifiers(styleID, "");
        }
    }
}

/**
 * @brief TrvScintillaEdit::loadStyles
 * @return
 */
bool TrvScintillaEdit::loadStyles()
{
    //QFile f(":/editor_styles/nppdarkmode.xml");
    //QFile f(":/editor_styles/nppmaterial.xml");
    QFile f(":/editor_styles/nppmonokai.xml");

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

/**
 * @brief TrvScintillaEdit::setStylesByName
 * @param sLangName
 */
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
                    QColor foreClr = QColor::fromString(sFgColor);
                    QColor backClr = QColor::fromString(sBgColor);
                    if (styleId == 0) {
                        styleSetFore(STYLE_DEFAULT,QCOLOR_TO_SPRT(foreClr));
                        styleSetBack(STYLE_DEFAULT,QCOLOR_TO_SPRT(backClr));
                    }

                    styleSetFore(styleId, QCOLOR_TO_SPRT(foreClr));
                    styleSetBack(styleId, QCOLOR_TO_SPRT(backClr));
                }
                break;
            }
        }
    }
}

/**
 * @brief TrvScintillaEdit::setContentType
 * @param sContentType
 * @param bFold
 */
void TrvScintillaEdit::setContentType(QString sContentType, bool bFold)
{
    if (!loadStyles()) { return; }
    // sptr_t doc = docPointer();
    // setDocPointer(0);
    clearDocumentStyle();

    if (sContentType.contains(QRegularExpression("(application|text)\\/json"))) {
        setILexer((sptr_t)CreateLexer("json"));
        setupKeywords(JSON);
        setStylesByName("json");
        setProperty("lexer.json.escape.sequence","1");
    } else if (sContentType.contains(QRegularExpression("(application|text)\\/(?:soap\\+)*xml"))) {
        setILexer((sptr_t)CreateLexer("xml"));
        setupKeywords(XML);
        setStylesByName("xml");
        setProperty("lexer.xml.allow.scripts", "0");
    } else {
        setILexer((sptr_t)CreateLexer("hypertext"));
        setupKeywords(HTML);
        setStylesByName("html");
    }
    if (bFold) {
        setupFoldSettings();
    }
    colourise(0, -1);
    //setDocPointer(doc);
}
