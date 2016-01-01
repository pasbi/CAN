#include "durationdelegate.h"
#include <QTime>
#include "Commands/DatabaseCommands/databaseeditcommand.h"


DurationDelegate::DurationDelegate(QObject* parent) :
    ItemDelegate<DurationEditor>(parent)
{
}

void DurationDelegate::setSpecificModelData(DurationEditor *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTime time = editor->time();
    QTime currentTime = index.model()->data(index, Qt::EditRole).toTime();
    if (time != currentTime)
    {
        pushCommand( new DatabaseEditCommand(model, index, time) );
    }
}

void DurationDelegate::setSpecificEditorData(DurationEditor *editor, const QModelIndex &index) const
{
    editor->setTime( index.model()->data(index, Qt::EditRole).toTime() );
}


QTime fromString(const QString& string)
{
    QStringList tokens = string.split(QRegExp(QString("%1|%2").arg(QRegExp::escape(":")).arg(QRegExp::escape(QLocale().decimalPoint()))));
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
