#ifndef TRVSCINTILLAEDIT_H
#define TRVSCINTILLAEDIT_H

#include <ScintillaEdit.h>

constexpr auto MARGIN_LINE_NUMBERS_MIN_WIDTH = 10;
constexpr auto MARGIN_LINE_NUMBERS = 0;
constexpr auto MARGIN_FOLDING = 2;
constexpr auto MARGIN_WIDTH_FOLDING = 15;
constexpr auto MARGIN_LINE_NUMBERS_RIGHT_SPACE = 4 + 1; // SC_MAX_MARGIN = 4
constexpr auto MARGIN_PADDING_LINE_NUMBERS = 7;

#define QCOLOR_TO_SPRT(col) ((((col).blue()) << 16) | (((col).green()) << 8) | ((col).red()))
#define RGB_TO_SPRT(r, g, b) (((r) << 16) | ((g) << 8) | (b))

enum lang {
    JSON = 1,
    XML = 2,
    HTML = 3
};

class QDomDocument;

class TrvScintillaEdit : public ScintillaEdit
{
    Q_OBJECT
public:
    explicit TrvScintillaEdit(QWidget *parent = nullptr);

    void setupGlobalStyles();
    void setContentType(QString sContentType, bool bFold=true);
    void setText(const char * text);
    QByteArray getText(sptr_t length);
    sptr_t textLength() const;
    void setReadOnly(bool readOnly);
    bool readOnly() const;

    void updateLineNumberMarginWidth(sptr_t zoom, QFont font, sptr_t line_count);
    int stringWidth(const QString& string, const QFontMetrics& metrics);

public slots:
    void onModified(Scintilla::ModificationFlags type, Scintilla::Position position, Scintilla::Position length, Scintilla::Position linesAdded,
                    const QByteArray &text, Scintilla::Position line, Scintilla::FoldLevel foldNow, Scintilla::FoldLevel foldPrev);
    void toggleFolding(Scintilla::Position position, Scintilla::KeyMod modifiers, int margin);

signals:

private:
    void setScrollWidth(sptr_t pixelWidth);
    void setScrollWidthTracking(bool tracking);
    void setMargins(sptr_t margins);
    void setMarginWidthN(sptr_t margin, sptr_t pixelWidth);
    void setMarginLeft(sptr_t pixelWidth);
    void styleSetFore(sptr_t style, sptr_t fore);
    void styleSetBack(sptr_t style, sptr_t back);
    void setSelFore(bool useSetting, sptr_t fore);
    void setSelBack(bool useSetting, sptr_t back);

    void styleClearAll();
    void styleSetFont(sptr_t style, const char * fontName);
    void styleSetSize(sptr_t style, sptr_t sizePoints);
    void setMarginBackN(sptr_t margin, sptr_t back);
    sptr_t linesOnScreen() const;
    sptr_t firstVisibleLine() const;
    sptr_t visibleFromDocLine(sptr_t docLine);
    sptr_t docLineFromVisible(sptr_t displayLine);
    void setProperty(const char * key, const char * value);
    sptr_t textWidth(sptr_t style, const char * text);

    void setTabWidth(sptr_t tabWidth);
    void setWrapMode(sptr_t wrapMode);
    sptr_t wrapMode() const;
    void setWrapVisualFlags(sptr_t wrapVisualFlags);
    sptr_t wrapVisualFlags() const;

    void setFoldMarginColour(bool useSetting, sptr_t back);
    void setFoldMarginHiColour(bool useSetting, sptr_t fore);
    void setCaretFore(sptr_t fore);
    void setMarginSensitiveN(sptr_t margin, bool sensitive);
    void setMarginMaskN(sptr_t margin, sptr_t mask);
    void setIdleStyling(sptr_t idleStyling);
    void setFoldFlags(sptr_t flags);
    void setKeyWords(sptr_t keyWordSet, const char * keyWords);
    void indicSetStyle(sptr_t indicator, sptr_t indicatorStyle);
    void indicSetFore(sptr_t indicator, sptr_t fore);
    void setILexer(sptr_t ilexer);
    void colourise(sptr_t start, sptr_t end);
    void markerDefine(sptr_t markerNumber, sptr_t markerSymbol);
    void markerSetFore(sptr_t markerNumber, sptr_t fore);
    void markerSetBack(sptr_t markerNumber, sptr_t back);
    void markerSetAlpha(sptr_t markerNumber, sptr_t alpha);
    void markerEnableHighlight(bool enabled);
    sptr_t lineCount() const;
    sptr_t lineFromPosition(sptr_t pos);
    void toggleFold(sptr_t line);

    void clearDocumentStyle();
    sptr_t docPointer() const;
    void setDocPointer(sptr_t doc);

    void setupFoldSettings();
    bool loadStyles();
    void setupKeywords(lang langType);


    void setStylesByName(QString sLangName);

    QDomDocument *m_pStyles;
    QFont m_font;
};

#endif // TRVSCINTILLAEDIT_H
