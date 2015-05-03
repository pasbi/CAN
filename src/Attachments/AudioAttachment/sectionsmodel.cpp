#include "sectionsmodel.h"
#include "application.h"

SectionsModel::SectionsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int SectionsModel::columnCount(const QModelIndex &parent) const
{
    assert( !parent.isValid() );
    return 3; // caption, begin, end
}

int SectionsModel::rowCount(const QModelIndex &parent) const
{
    assert( !parent.isValid() );
    return m_sections.length();
}

QVariant SectionsModel::data(const QModelIndex &index, int role) const
{
    const Section& section = m_sections[ index.row() ];
    switch (role)
    {
    case Qt::EditRole:
    case Qt::DisplayRole:
        switch (index.column())
        {
        case 0:
            return section.caption();
        case 1:
            return section.begin();
        case 2:
            return section.end();
        default:
            assert( false );
            return QVariant();
        }
    default:
        return QVariant();
    }
}

QVariant SectionsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED( section );
    Q_UNUSED( orientation );
    Q_UNUSED( role );
    return QVariant(); // no headers!
}

Qt::ItemFlags SectionsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    if (index.column() == 0)
    {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

#include "Commands/AttachmentCommands/AudioAttachmentCommands/editsectioncommand.h"
bool SectionsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() != 0)
    {
        qWarning() << "SectionsModel columns > 0 are not editable";
        return false;
    }

    if (role != Qt::EditRole)
    {
        return false;
    }

    app().pushCommand( new EditSectionCommand( this, value, index, role) );
    return true;
}

bool SectionsModel::setData_(const QModelIndex &index, const QVariant &value, int role) const
{
    Q_UNUSED( index );
    Q_UNUSED( value );
    Q_UNUSED( role );
    return false;
}

bool SectionsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED( row );
    Q_UNUSED( count );
    Q_UNUSED( parent );
    return false;
}
