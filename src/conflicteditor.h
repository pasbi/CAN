#ifndef CONFLICTEDITOR_H
#define CONFLICTEDITOR_H

#include <QDialog>
#include "global.h"
#include "../../ZipGit/src/file.h"
#include <QListWidgetItem>

namespace Ui {
class ConflictEditor;
}

class ConflictEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ConflictEditor(const QList<File> & conflictingFiles, QWidget *parent = 0);
    ~ConflictEditor();
    bool hasConflicts() const
    {
        return !m_files.isEmpty();
    }

public slots:
    void accept();
    void resolveConflicts();
    QList<File> files() const { return m_files; }

private:
    Ui::ConflictEditor *ui;
    QList<File> m_files;

    class Item: public QListWidgetItem
    {
    public:
        Item(QListWidget* parent, const Conflict& conflict, int type = Type) :
            QListWidgetItem(parent, type),
            m_conflict(conflict)
        {
        }
        const Conflict& conflict() const
        {
            return m_conflict;
        }
        QVariant data(int role) const
        {
            if (role == Qt::DisplayRole)
            {
                return conflict().m_type;
            }
            else
            {
                return QVariant();
            }
        }

    private:
        const Conflict& m_conflict;
    };

    QList<Item> m_items;

    const Conflict& currentConflict() const;
    QString m_defaultStyle;


private slots:
    void selectConflict();




    void on_buttonKeepRemote_clicked();
    void on_buttonKeepLocal_clicked();
};

#endif // CONFLICTEDITOR_H
