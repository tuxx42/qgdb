#include <QtWidgets>
#include <QDebug>

#include "codeviewer.h"


CodeViewer::CodeViewer(QWidget *parent) : QPlainTextEdit(parent), m_top(-1), m_bottom(-1)
{
	lineNumberArea = new LineNumberArea(this);
	numVisibleBlock = 0;

	setCenterOnScroll(true);
	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
	connect(lineNumberArea, SIGNAL(emitMouseDoubleClickEvent(QMouseEvent *)), this, SLOT(addBreakpoint(QMouseEvent *)));

	updateLineNumberAreaWidth(0);
}

int CodeViewer::lineNumberAreaWidth()
{
	int digits = 1;
	int max;
	for (max = qMax(1, blockCount()); max >= 10; max /= 10)
		++digits;

	int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
}

void CodeViewer::updateLineNumberAreaWidth(int /* newBlockCount */)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeViewer::updateLineNumberArea(const QRect &rect, int dy)
{
	if (dy)
		lineNumberArea->scroll(0, dy);
	else
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

	if (rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}

void CodeViewer::scrollToBlock(int line)
{
	QTextBlock block = firstVisibleBlock();
	int blockNumber;
       	
	while (block.isValid()) {
		blockNumber = block.blockNumber();

		if (block.isVisible() && blockNumber == (line - 1)) {
			QTextCursor cursor = textCursor();
			cursor.setPosition(block.position());
			centerCursor();
			setTextCursor(cursor);
			break;
		}
		block = block.next();
	}
}


void CodeViewer::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();
	lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeViewer::setHighlightLine(int line)
{
	QTextBlock block;
	int blockNumber;
	QTextEdit::ExtraSelection selection;
	QColor lineColor = QColor(Qt::blue).lighter(160);
	QList<QTextEdit::ExtraSelection> extraSelections;

	for (block = firstVisibleBlock(); block.isValid(); block = block.next()) {
		blockNumber = block.blockNumber();
		if (blockNumber == (line - 1)) {
			selection.format.setBackground(lineColor);
			selection.format.setProperty(QTextFormat::FullWidthSelection, true);
			selection.cursor = textCursor();
			selection.cursor.clearSelection();
			selection.cursor.setPosition(block.position());

			extraSelections.append(selection);
			setExtraSelections(extraSelections);
			scrollToBlock(line);
			break;
		}
	}
}

void CodeViewer::setBreakpoints(QVector<int> numList)
{
	breakpoints = numList;
	update();
}

void CodeViewer::lineNumberAreaPaintEvent(QPaintEvent *event)
{
	QPainter painter(lineNumberArea);
	QColor color;
	painter.fillRect(event->rect(), Qt::lightGray);

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int) blockBoundingRect(block).height();

	m_top = blockNumber;
	while (block.isValid() && top <= event->rect().bottom()) {
		if (block.isVisible() && bottom >= event->rect().top()) {
			QString number = QString::number(blockNumber + 1);
			if (breakpoints.contains(blockNumber + 1))
				color = Qt::red;
			else
				color = Qt::black;
			painter.setPen(color);
			painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + (int) blockBoundingRect(block).height();
		++blockNumber;
	}
	m_bottom = blockNumber;

	numVisibleBlock = blockNumber - firstVisibleBlock().blockNumber();
}

void LineNumberArea::mouseDoubleClickEvent(QMouseEvent *event)
{
	emit emitMouseDoubleClickEvent(event);
}

void CodeViewer::addBreakpoint(QMouseEvent *event)
{
	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int) blockBoundingRect(block).height();

	while (block.isValid() && block.isVisible()) {
		if (event->y() >= top && event->y() < bottom)
			break;

		block = block.next();
		top = bottom;
		bottom = top + (int) blockBoundingRect(block).height();
		blockNumber++;
	}

	if (m_inf)
		m_inf->ICodeView_onRowDoubleClick(blockNumber + 1);
}
