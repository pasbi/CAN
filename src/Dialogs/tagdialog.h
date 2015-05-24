#ifndef TAGDIALOG_H
#define TAGDIALOG_H

#include <QDialog>
#include "taggable.h"

namespace Ui {
class TagDialog;
}

class TagDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagDialog(const QStringList &tags, QWidget *parent = 0);
    ~TagDialog();
    QStringList tags() const { return m_tags; }

private:
    Ui::TagDialog *ui;
    QStringList m_tags;
    void updateTextEdit();
    void addTag( const QString& tag );

private slots:
    void on_pushButton_clicked();
    void removeTag( QString tag );

public:
    static bool setTags( QWidget* parent, Taggable* taggable );
};

#endif // TAGDIALOG_H
