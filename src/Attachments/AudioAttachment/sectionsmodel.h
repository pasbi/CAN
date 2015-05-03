#ifndef SECTIONSMODEL_H
#define SECTIONSMODEL_H

#include <QAbstractTableModel>
#include <QDebug>
#include "global.h"

class Section
{
public:
    Section( const QString & caption, const int begin, const int end ) :
        m_caption( caption ),
        m_begin( begin ),
        m_end( end )
    {
        if (m_begin > m_end)
        {
            qSwap( m_begin, m_end );
            qWarning() << "Beginning is after ending. Swapped them.";
        }
    }

    QString caption() const { return m_caption; }
    int begin() const { return m_begin; }
    int end() const { return m_end; }


private:
    QString m_caption;
    int m_begin;
    int m_end;
};

class SectionsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SectionsModel(QObject *parent = 0);

    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    // creates a undo/redo command which will call setData_
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    void insertSection( const Section & section );

private:
    bool insertRows(int row, int count, const QModelIndex &parent);

    // this will actually set the data.
    bool setData_(const QModelIndex &index, const QVariant &value, int role) const;

    QList<Section> m_sections;

    friend class EditSectionCommand;


};

#endif // SECTIONSMODEL_H
