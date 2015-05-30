#ifndef ADDFILEINDEXSOURCEDIALOG_H
#define ADDFILEINDEXSOURCEDIALOG_H

#include <QFileDialog>
#include <QLineEdit>


class AddFileIndexSourceDialog : public QFileDialog
{
    Q_OBJECT

public:
    explicit AddFileIndexSourceDialog( const QString & filter, QWidget *parent = 0);
    ~AddFileIndexSourceDialog();

    QStringList filterList() const { return m_filterLineEdit->text().split("|"); }
    QString filter() const { return m_filterLineEdit->text(); }

private:
    void initFilterWidget();
    QWidget* m_filterWidget = NULL;
    QLineEdit* m_filterLineEdit = NULL;

};

#endif // ADDFILEINDEXSOURCEDIALOG_H
