#ifndef DURATIONDELEGATE_H
#define DURATIONDELEGATE_H

#include "itemdelegate.h"
#include "Commands/DatabaseCommands/databaseedititemcommand.h"
#include <QLineEdit>
#include <QTime>
#include <QValidator>

template<typename T>
class DurationDelegate : public ItemDelegate<T, QLineEdit>
{
public:
    DurationDelegate(QObject* parent = 0) :
        ItemDelegate<T, QLineEdit>(parent)
    {
    }

private:
    void setModelData(QLineEdit *editor, Database<T> *database, const QModelIndex &index) const
    {
        QTime time = fromString(editor->text().replace("-", ""));
        QTime currentTime = index.model()->data(index, Qt::EditRole).toTime();
        if (time != currentTime)
        {
            app().pushCommand( new DatabaseEditItemCommand<T>(database, index, time) );
        }
    }

    void setEditorData(QLineEdit *editor, const QModelIndex &index) const
    {
        editor->setInputMask("00:00;-");
        editor->setValidator(new QRegExpValidator(QRegExp("([0-5]|-)([0-9]|-):([0-5]|-)([0-9]|-)"), editor));
        editor->setText(index.model()->data(index, Qt::EditRole).toTime().toString("mm:ss"));
    }

private:
    static QTime fromString(const QString& string)
    {
        QStringList tokens = string.split(":");
        if (tokens.length() == 2)
        {
            bool minutesOk, secondsOk;
            int minutes = tokens[0].toInt(&minutesOk);
            int seconds = tokens[1].toInt(&secondsOk);

            if (minutesOk && secondsOk)
            {
                return QTime(0, minutes, seconds, 0);
            }
            else
            {
                return QTime();
            }
        }
        else if (tokens.length() > 2)
        {
            int n = tokens.length();
            tokens = QStringList({tokens[n-2], tokens[n-1]});
            return fromString(tokens.join(":"));
        }
        else
        {
            return QTime();
        }


    }

};

#endif // DURATIONDELEGATE_H
