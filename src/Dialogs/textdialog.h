#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QDialog>

namespace Ui {
class TextDialog;
}

class TextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextDialog(QWidget *parent = 0);
    ~TextDialog();
    QString text() const { return m_text; }
    void setText(const QString& text);

    void closeEvent(QCloseEvent *e);

public slots:
    void accept();

private:
    Ui::TextDialog *ui;
    QString m_text;
};

#endif // TEXTDIALOG_H
