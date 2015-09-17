#include "sectionsmodel.h"
#include "application.h"
#include "persistentobject.h"
#include "section.h"
#include "Commands/AttachmentCommands/AudioAttachmentCommands/deletesectioncommand.h"

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

bool SectionsModel::setData_(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        assert( index.column() == 0 );
        m_sections[index.row()].setCaption( value.toString() );
        return true;
    }
    else
    {
        return false;
    }
}

bool SectionsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    assert( count == m_sectionsToBeInserted.length() );
    assert( !parent.isValid() );

    beginInsertRows(parent, row, row + count - 1);
    for (int i = row; i < row + count; ++i)
    {
        m_sections.insert(i, m_sectionsToBeInserted.takeFirst());
    }
    endInsertRows();
    assert( m_sectionsToBeInserted.isEmpty() );
    return true;
}

#include "Commands/AttachmentCommands/AudioAttachmentCommands/insertsectioncommand.h"
void SectionsModel::insertSection(const Section &section, int index)
{
    app().pushCommand( new InsertSectionCommand( this, section, index ) );
}

bool SectionsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows( parent, row, row + count -1 );
    for (int i = row + count - 1; i >= row; --i)
    {
        m_sections.removeAt(i);
    }
    endRemoveRows();
    return true;
}

const Section* SectionsModel::section(int index) const
{
    return &m_sections[index];
}

int SectionsModel::indexOf(const Section *section) const
{
    for (int i = 0; i < m_sections.length(); ++i)
    {
        if (&m_sections[i] == section)
        {
                return i;
        }
    }
    return -1;
}

void SectionsModel::removeSection(const Section *section)
{
    int index = -1;
    for (int i = 0; i < m_sections.length(); ++i)
    {
        if (&m_sections[i] == section)
        {
            index = i;
        }
    }

    if (index >= 0)
    {
        removeSection( index );
    }
}


void SectionsModel::removeSection(int i)
{
    app().pushCommand( new DeleteSectionCommand( this, i ) );
}

QDataStream& operator<<(QDataStream& out, const SectionsModel* model)
{
    out << model->m_sections;
    return out;
}

QDataStream& operator>>(QDataStream& in, SectionsModel* model)
{
    in >> model->m_sections;
    return in;
}







