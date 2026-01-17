#ifndef TRVSCINTILLAEDIT_H
#define TRVSCINTILLAEDIT_H

#include <ScintillaEdit.h>

constexpr auto MARGIN_LINE_NUMBERS_MIN_WIDTH = 10;
constexpr auto MARGIN_LINE_NUMBERS = 0;
constexpr auto MARGIN_LINE_NUMBERS_RIGHT_SPACE = 4 + 1; // SC_MAX_MARGIN = 4
constexpr auto MARGIN_PADDING_LINE_NUMBERS = 7;

#define QCOLOR_TO_SPRT(col) ((((col).blue()) << 16) | (((col).green()) << 8) | ((col).red()))
#define RGB_TO_SPRT(r, g, b) (((r) << 16) | ((g) << 8) | (b))

class TrvScintillaEdit : public ScintillaEdit
{
    Q_OBJECT
public:
    explicit TrvScintillaEdit(QWidget *parent = nullptr);

signals:

private:
    void setMargins(sptr_t margins);
    void setMarginWidthN(sptr_t margin, sptr_t pixelWidth);
    void styleSetFore(sptr_t style, sptr_t fore);
    void styleSetBack(sptr_t style, sptr_t back);
    void setMarginBackN(sptr_t margin, sptr_t back);

};

#endif // TRVSCINTILLAEDIT_H
