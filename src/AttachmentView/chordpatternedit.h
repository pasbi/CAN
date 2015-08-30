#ifndef CHORDPATTERNEDIT_H
#define CHORDPATTERNEDIT_H

#include <QTextEdit>
#include "looselines.h"

class ChordPatternEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit ChordPatternEdit(QWidget *parent = 0);

public slots:

protected:
    void contextMenuEvent(QContextMenuEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void insertFromMimeData(const QMimeData *source);
    bool canInsertFromMimeData(const QMimeData *source) const;
    QMimeData* createMimeDataFromSelection() const;
    void mousePressEvent(QMouseEvent *e);

private:
    QMenu* m_contextMenu = NULL;
    QList<int> m_selectedLines;



signals:
    void pasted();

public:
    void setExternalExtraSelections( const QList<ExtraSelection>& selections );
private:
    void setInternalExtraSelections( const QList<ExtraSelection>& selections );
    QList<ExtraSelection> m_internalExtraSelections;
    QList<ExtraSelection> m_externalExtraSelections;
    void setExtraSelections(const QList<ExtraSelection> &selections);


    /**
     * @brief updateHighlights update line highlightings (@see m_selectedLines);
     */
    void updateHighlights();
    void clearHighlights();

    QString pasteLooseLines(const QString& base, const LooseLines& looseLines, int currentLineNumber, int &newCursorPosition);

    QAction* m_pasteAction;
    QAction* m_copyAction;
};

#endif // CHORDPATTERNEDIT_H
