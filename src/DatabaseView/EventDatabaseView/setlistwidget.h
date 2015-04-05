#ifndef SETLISTWIDGET_H
#define SETLISTWIDGET_H

#include <QWidget>
#include "Database/EventDatabase/setlist.h"
#include <QListView>
#include "setlistitemselector.h"

namespace Ui {
class SetlistWidget;
}

class SetlistWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SetlistWidget(QWidget *parent = 0);
    ~SetlistWidget();

    void setSetlist( Setlist* Setlist );
    QListView* listView() const;
    SetlistItem* currentItem() const;


private slots:
    void on_buttonSortUp_clicked();
    void on_buttonSortDown_clicked();
    void on_buttonDelete_clicked();
    void on_buttonAdd_clicked();

    void on_buttonShowSongs_clicked();
    void updateButtonsEnabled();

protected:
    void hideEvent(QHideEvent *e);
    void showEvent(QShowEvent *e);

private:
    Ui::SetlistWidget *ui;
    Setlist* m_currentSetlist = NULL;

    bool m_selectorIsVisible = false;
    QRect m_selectorGeometry;
    QPoint m_selectorPosition;
    SetlistItemSelector* const m_selector;
};

#endif // SETLISTWIDGET_H
