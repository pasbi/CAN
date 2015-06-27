#ifndef TAGEDITOR_H
#define TAGEDITOR_H

#include <QWidget>
#include "taggable.h"

namespace Ui {
class TagEditor;
}

class TagEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TagEditor( QWidget *parent = 0 );
    ~TagEditor();
    QStringList tags() const { return m_tags; }
    void setTags( const QStringList& tags );

protected:
    bool eventFilter(QObject *o, QEvent *e);

private:
    Ui::TagEditor *ui;
    QStringList m_tags;
    void updateEdits();
    void addTag( const QString& tag );

private slots:
    void on_pushButton_clicked();
    void removeTag( QString tag );

public:
    static bool setTags( QWidget* parent, Taggable* taggable );
};

#endif // TAGEDITOR_H
