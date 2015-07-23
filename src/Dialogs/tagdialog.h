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
    QStringList tags() const;

    static bool setTags(QWidget *parent, Taggable *taggable);

    void accept();

private:
    Ui::TagDialog* ui;
    QStringList m_tags;

};

#endif // TAGDIALOG_H
