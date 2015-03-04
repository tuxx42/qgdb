#ifndef CODEVIEWER_H
#define CODEVIEWER_H

#include <QPlainTextEdit>
#include <QObject>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberArea;

class ICodeView {
public:
	ICodeView(){};

	virtual void ICodeView_onRowDoubleClick(int lineNo) = 0;
	virtual void ICodeView_onContextMenu(QPoint pos, int lineNo, QStringList text) = 0;
};

class CodeViewer : public QPlainTextEdit {
	Q_OBJECT

public:
	CodeViewer(QWidget *parent = 0);

	void lineNumberAreaPaintEvent(QPaintEvent *event);
	int lineNumberAreaWidth();
	void setHighlightLine(int);
	void setBreakpoints(QVector<int>);
	void scrollToBlock(int);
	void setInterface(ICodeView *inf) { m_inf = inf; };

protected:
	void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
	void addBreakpoint(QMouseEvent *);
	void updateLineNumberAreaWidth(int newBlockCount);
	void updateLineNumberArea(const QRect &, int);

private:
	QWidget *lineNumberArea;
	QVector<int> breakpoints;
	int numVisibleBlock;
	ICodeView *m_inf;
	int m_top, m_bottom;
};


class LineNumberArea : public QWidget {
	Q_OBJECT
public:
	LineNumberArea(CodeViewer *editor) : QWidget(editor) {
		codeEditor = editor;
	}

	QSize sizeHint() const Q_DECL_OVERRIDE {
		return QSize(codeEditor->lineNumberAreaWidth(), 0);
	}

protected:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE {
		codeEditor->lineNumberAreaPaintEvent(event);
	}

signals:
	void emitMouseDoubleClickEvent(QMouseEvent * event);

private:
	void mouseDoubleClickEvent(QMouseEvent *event);
	CodeViewer *codeEditor;
};
#endif
