#ifndef SETLISTWIDGET_H
#define SETLISTWIDGET_H

#include <QWidget>
#include "configurable.h"

namespace Ui {
class SetlistWidget;
}


class SetlistItemSelector;
class SetlistItem;
class SetlistView;
class Setlist;
template<typename T> class Database;

// SetlistWidget does not inherit DatabaseWidget because it is too different.
class SetlistWidget : public QWidget
{
    Q_OBJECT
    DECL_CONFIG( SetlistWidget )
public:
    explicit SetlistWidget(QWidget *parent = 0);
    ~SetlistWidget();

    void setSetlist( Database<SetlistItem>* setlist );
    SetlistView *setlistView() const;
    QList<SetlistItem*> currentItems() const;
    Setlist *setlist() const;


private slots:
    void on_buttonAdd_clicked();
    void on_buttonShowSongs_clicked();
    void on_buttonExportPDF_clicked();
    void updateInfoLabel();
    void updateTagComboBox();
    void updateButtonsEnabled();

protected:
    void hideEvent(QHideEvent *e);
    void showEvent(QShowEvent *e);

private:
    Ui::SetlistWidget *ui;

    bool m_selectorIsVisible = false;
    SetlistItemSelector* const m_selector;
    QPixmap m_attentionPixmap;
    QPixmap m_emptyPixmap;
};

#endif // SETLISTWIDGET_H
