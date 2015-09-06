#include "setlistproxy.h"



SetlistProxy::SetlistProxy(QObject *parent) :
    DatabaseSortProxy(parent)
{
}

bool SetlistProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_row);
    Q_UNUSED(source_parent);
    return true;
}

bool acceptItem(const SetlistItem* item, const QString& filter)
{
    if (!filter.isEmpty())
    {
        for (const QString& token : item->textAttributes())
        {
            if (token.contains(filter, Qt::CaseInsensitive))
            {
                return true;
            }
        }
    }
    return false;
}


QVariant SetlistProxy::data(const QModelIndex &index, int role) const
{
    if (role == Qt::BackgroundRole)
    {
        if ( acceptItem(itemAtIndex(index), filter()) )
        {
            return QColor(255, 0, 0, 128);
        }
    }
    return DatabaseSortProxy::data(index, role);
}
