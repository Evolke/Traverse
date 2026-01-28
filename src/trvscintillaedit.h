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

    void setContentType(QString sContentType, bool bFold=true);
    void setText(const char * text);
    QByteArray getText(sptr_t length);
    sptr_t textLength() const;

    void updateLineNumberMarginWidth(sptr_t zoom, QFont font, sptr_t line_count);
    int stringWidth(const QString& string, const QFontMetrics& metrics);

public slots:
    void onModified(Scintilla::ModificationFlags type, Scintilla::Position position, Scintilla::Position length, Scintilla::Position linesAdded,
                    const QByteArray &text, Scintilla::Position line, Scintilla::FoldLevel foldNow, Scintilla::FoldLevel foldPrev);
    void toggleFolding(Scintilla::Position position, Scintilla::KeyMod modifiers, int margin);

signals:

private:
    void setMargins(sptr_t margins);
    void setMarginWidthN(sptr_t margin, sptr_t pixelWidth);
    void styleSetFore(sptr_t style, sptr_t fore);
    void styleSetBack(sptr_t style, sptr_t back);
    void setSelFore(bool useSetting, sptr_t fore);
    void setSelBack(bool useSetting, sptr_t back);

    void styleClearAll();
    void styleSetFont(sptr_t style, const char * fontName);
    void styleSetSize(sptr_t style, sptr_t sizePoints);
    void setMarginBackN(sptr_t margin, sptr_t back);
    void setProperty(const char * key, const char * value);
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
    void setupFoldSettings();
    bool loadStyles();
    void setupKeywords(lang langType);


    void setStylesByName(QString sLangName);

    QDomDocument *m_pStyles;
    QFont m_font;
};

#endif // TRVSCINTILLAEDIT_H
