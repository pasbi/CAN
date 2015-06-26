#ifndef CONFLICTEDITOR_H
#define CONFLICTEDITOR_H

#include <QDialog>
#include "global.h"
#include <QListWidgetItem>
#include "Project/conflictfile.h"

namespace Ui {
class ConflictEditor;
}

class ConflictEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ConflictEditor(const QList<ConflictFile *> &conflictingFiles, QWidget *parent = 0);
    ~ConflictEditor();
    bool hasConflicts() const
    {
        return !m_files.isEmpty();
    }

public slots:
    void accept();
    void resolveConflicts();
    void resolveAllTheirs();
    void resolveAllMine();
    QList<ConflictFile*> files() const { return m_files; }

private:
    Ui::ConflictEditor *ui;
    QList<ConflictFile*> m_files;

    class Item: public QListWidgetItem
    {
    public:
        Item(QListWidget* parent, Conflict* conflict, int type = Type) :
            QListWidgetItem(parent, type),
            m_conflict(conflict)
        {
        }
        Conflict* conflict()
        {
            return m_conflict;
        }
        QVariant data(int role) const
        {
            if (role == Qt::DisplayRole)
            {
                return m_conflict->m_type;
            }
            else
            {
                return QVariant();
            }
        }

    private:
        Conflict* m_conflict;
    };

    QList<Item*> m_items;

    Conflict *currentConflict() const;
    QString m_defaultStyle;


private slots:
    void selectConflict();




    void on_buttonKeepRemote_clicked();
    void on_buttonKeepLocal_clicked();
};

#endif // CONFLICTEDITOR_H
