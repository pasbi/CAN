#include "audioattachmentmergewidget.h"

#include <QTableWidget>

#include "Attachments/AudioAttachment/audioattachment.h"
#include "Attachments/AudioAttachment/sectionsmodel.h"
#include <QHeaderView>

QString labelSection(const Section& section)
{
    return QString("%1: %2 to %3").arg(section.caption(),
                                       SectionsModel::timeToString(section.begin()),
                                       SectionsModel::timeToString(section.end()));
}

AudioAttachmentMergeWidget::AudioAttachmentMergeWidget(MergeItem *mergeItem, QWidget *parent) :
    IndexedFileAttachmentMergeWidget(mergeItem, parent)
{
    AudioAttachment* aaMaster = static_cast<AudioAttachment*>(mergeItem->master());
    AudioAttachment* aaSlave  = static_cast<AudioAttachment*>(mergeItem->slave());
    if (aaMaster->attribute("sections") != aaSlave->attribute("sections"))
    {
        m_masterSections = new QTableWidget(this);
        m_slaveSections = new QTableWidget(this);

        m_masterSections->insertColumn(0);
        m_slaveSections->insertColumn(0);

        m_masterSections->horizontalHeader()->hide();
        m_masterSections->verticalHeader()->hide();
        m_masterSections->horizontalHeader()->setStretchLastSection(true);
        m_slaveSections->horizontalHeader()->hide();
        m_slaveSections->verticalHeader()->hide();
        m_slaveSections->horizontalHeader()->setStretchLastSection(true);

        for (int row = 0; row < aaMaster->sectionsModel()->rowCount(); ++row)
        {
            m_masterSections->insertRow(row);
            m_masterSections->setItem(row, 0, new QTableWidgetItem(labelSection(aaMaster->sectionsModel()->sections()[row])));
        }
        for (int row = 0; row < aaSlave->sectionsModel()->rowCount(); ++row)
        {
            m_slaveSections->insertRow(row);
            m_slaveSections->setItem(row, 0, new QTableWidgetItem(labelSection(aaSlave->sectionsModel()->sections()[row])));
        }

        m_masterSections->setSelectionMode(QAbstractItemView::NoSelection);
        m_slaveSections->setSelectionMode(QAbstractItemView::NoSelection);

        addEditorWidget(m_masterSections, m_slaveSections, "sections");
        connectDecisions("sections", "hash");
    }
    else
    {
        m_masterSections = nullptr;
        m_slaveSections = nullptr;
    }
}
