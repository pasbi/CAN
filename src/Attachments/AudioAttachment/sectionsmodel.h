#ifndef SECTIONSMODEL_H
#define SECTIONSMODEL_H

#include <QAbstractTableModel>
#include "section.h"

class SectionsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SectionsModel(QObject *parent = 0);

    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    int rowCount() const { return rowCount( QModelIndex() ); }
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    // creates a undo/redo command which will call setData_
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    void insertSection( const Section& section , int index);
    void appendSection( const Section& section ) { insertSection( section, m_sections.length() ); }
    void removeSection( int i );
    void removeSection( const Section* section );

    const Section* section( int index ) const;

    int indexOf( const Section* section ) const;
private:
    bool insertRows(int row, int count, const QModelIndex &parent);
    QList<Section> m_sectionsToBeInserted;

    bool removeRows(int row, int count, const QModelIndex &parent);

    // this will actually set the data.
    bool setData_(const QModelIndex &index, const QVariant &value, int role);

    QList<Section> m_sections;

    friend class EditSectionCommand;
    friend class InsertSectionCommand;
    friend class DeleteSectionCommand;

    friend QDataStream& operator<<(QDataStream& out, const SectionsModel* model);
    friend QDataStream& operator>>(QDataStream& in, SectionsModel* model);

};

QDataStream& operator<<(QDataStream& out, const SectionsModel* model);
QDataStream& operator>>(QDataStream& in, SectionsModel* model);



#endif // SECTIONSMODEL_H
