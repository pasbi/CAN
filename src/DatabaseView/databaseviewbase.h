#ifndef DATABASEVIEWBASE_H
#define DATABASEVIEWBASE_H


#include "global.h"
#include "freezetableview.h"

class OverlayDecorator;
class DatabaseViewBase : public FreezeTableView
{
    typedef FreezeTableView Super;
    Q_OBJECT
protected:
    DatabaseViewBase(QWidget* parent = 0);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);
    virtual void setFilter(const QString& filter);
    virtual QString filter() const = 0;
    void focusOutEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void enterEvent(QEvent *event);

    void setModel(QAbstractItemModel *model);
    virtual int numFilteredItems() const = 0;
    QString itemName(int n) const;

private:
    OverlayDecorator* m_hud;
    enum FilterEditMode { OverwriteFilter, AppendFilter };
    FilterEditMode m_filterEditMode;

signals:
    void clicked();
    void changed(); // anything in the database changed (insert, delete, move or edit)
};

#endif // DATABASEVIEWBASE_H
