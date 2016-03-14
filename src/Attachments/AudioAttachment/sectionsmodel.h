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

    bool setData(const QModelIndex &index, const QVariant &value, int role);

    void removeSection( int i );
    void insertSection(Section section, int i);

    QList<Section> sections() const;

    int indexOf(Section section ) const;
private:

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
