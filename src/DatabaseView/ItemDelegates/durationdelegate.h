#ifndef DURATIONDELEGATE_H
#define DURATIONDELEGATE_H

#include "itemdelegate.h"
#include "Commands/DatabaseCommands/databaseeditcommand.h"
#include <QLineEdit>
#include <QTime>
#include <QValidator>
#include "application.h"

class DurationEditor : public QLineEdit
{
    Q_OBJECT
public:
    explicit DurationEditor(QWidget* parent = nullptr);
    void setTime(const QTime& time);
    QTime time() const;

    void increase();
    void decrease();

protected:
    void keyPressEvent(QKeyEvent *e);
    void wheelEvent(QWheelEvent *e);
};

class DurationDelegate : public ItemDelegate<DurationEditor>
{
public:
    DurationDelegate(QObject* parent = 0) :
        ItemDelegate<DurationEditor>(parent)
    {
    }

private:
    void setModelData(DurationEditor *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        QTime time = editor->time();
        QTime currentTime = index.model()->data(index, Qt::EditRole).toTime();
        if (time != currentTime)
        {
            app().pushCommand( new DatabaseEditCommand(model, index, time) );
        }
    }

    void setEditorData(DurationEditor *editor, const QModelIndex &index) const
    {
        editor->setTime( index.model()->data(index, Qt::EditRole).toTime() );
    }

private:
    static QTime fromString(const QString& string)
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

};

#endif // DURATIONDELEGATE_H
