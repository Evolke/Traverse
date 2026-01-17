#include "trvscintillaedit.h"

TrvScintillaEdit::TrvScintillaEdit(QWidget *parent)
    : ScintillaEdit{parent}
{
    setMargins(SC_MAX_MARGIN + 2);
    int width = 10*3;
    setMarginWidthN(MARGIN_LINE_NUMBERS, qMax(MARGIN_LINE_NUMBERS_MIN_WIDTH + MARGIN_PADDING_LINE_NUMBERS, width));
    styleSetFore(STYLE_DEFAULT,RGB_TO_SPRT(255,0,0));
    styleSetBack(STYLE_DEFAULT,RGB_TO_SPRT(52,52,52));
    setMarginBackN(MARGIN_LINE_NUMBERS, RGB_TO_SPRT(0,0,0));
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

void TrvScintillaEdit::setMarginBackN(sptr_t margin, sptr_t back) {
    send(SCI_SETMARGINBACKN, margin, back);
}
