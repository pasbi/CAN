#ifndef ADDFILEINDEXSOURCEDIALOG_H
#define ADDFILEINDEXSOURCEDIALOG_H

#include <QFileDialog>
#include <QLineEdit>


class AddFileIndexSourceDialog : public QFileDialog
{
    Q_OBJECT

public:
    explicit AddFileIndexSourceDialog(QWidget *parent = 0);
    ~AddFileIndexSourceDialog();

    QStringList filter() const { return m_filterLineEdit->text().split("|"); }

private:
    void initFilterWidget();
    QWidget* m_filterWidget = NULL;
    QLineEdit* m_filterLineEdit = NULL;

};

#endif // ADDFILEINDEXSOURCEDIALOG_H
