#include "sectionsmodel.h"
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

QString timeToString(const QTime& time)
{
    if (time.hour() > 0)
    {
        return time.toString(QString("hh:mm:ss%1zzz").arg(QLocale().decimalPoint()));
    }
    else
    {
        return time.toString(QString("mm:ss%1zzz").arg(QLocale().decimalPoint()));
    }
}

QVariant SectionsModel::data(const QModelIndex &index, int role) const
{
    Section section = m_sections[ index.row() ];
    switch (role)
    {
    case Qt::EditRole:
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
    case Qt::DisplayRole:
        switch (index.column())
        {
        case 0:
            return section.caption();
        case 1:
            return timeToString(section.begin());
        case 2:
            return timeToString(section.end());
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
    Q_UNUSED(index);
    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool SectionsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        switch (index.column())
        {
        case 0:
            m_sections[index.row()].setCaption(value.toString());
            break;
        case 1:
            m_sections[index.row()].setBegin(value.toTime());
            break;
        case 2:
            m_sections[index.row()].setEnd(value.toTime());
            break;
        }
        emit dataChanged( index, index );
        return true;
    }
    else
    {
        return false;
    }
}

void SectionsModel::insertSection(Section section, int i)
{
    beginInsertRows(QModelIndex(), i, i);
    m_sections.insert(i, section);
    endInsertRows();
}

Section SectionsModel::section(int index) const
{
    return m_sections[index];
}

int SectionsModel::indexOf(Section section) const
{
    for (int i = 0; i < m_sections.length(); ++i)
    {
        if (m_sections[i] == section)
        {
                return i;
        }
    }
    return -1;
}

void SectionsModel::removeSection(int i)
{
    beginRemoveRows(QModelIndex(), i, i);
    m_sections.removeAt(i);
    endRemoveRows();
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







