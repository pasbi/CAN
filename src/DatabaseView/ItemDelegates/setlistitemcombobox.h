#ifndef SETLISTITEMCOMBOBOX_H
#define SETLISTITEMCOMBOBOX_H

#include <QLineEdit>
#include <QListWidget>

class SetlistItemComboBox : public QLineEdit
{
    Q_OBJECT
public:
    explicit SetlistItemComboBox(QWidget *parent = 0);
    ~SetlistItemComboBox();

    void addItems(const QStringList &texts);

    void keyPressEvent(QKeyEvent *e);

private:
    QStringList m_items;
    QStringList m_sortedItems;
    QList<int> m_indexMap;
    void sortItems();
    QListWidget* m_popupWidget;
    enum ItemSelectionMode { SelectFirsItem, SelectNextItem, SelectPreviousItem };
    void selectItem(ItemSelectionMode mode);
    QModelIndexList visibleIndexes() const;
    int m_currentIndex;

protected:
    void showEvent(QShowEvent *e);
    bool eventFilter(QObject *o, QEvent *e);

public slots:
    void setCurrentIndex(int index);
public:
    int currentIndex() const;


private slots:
    void updatePopup();
    void showPopup();
    void updatePopupSize();


};

#endif // SETLISTITEMCOMBOBOX_H
