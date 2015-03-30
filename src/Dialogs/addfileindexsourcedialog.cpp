#include "addfileindexsourcedialog.h"
#include <QLayout>
#include "global.h"
#include <QCheckBox>

AddFileIndexSourceDialog::AddFileIndexSourceDialog(const QString &filter, QWidget *parent) :
    QFileDialog(parent)
{

    QGridLayout* gridLayout = qobject_assert_cast<QGridLayout*>(layout());
    assert( gridLayout );

    initFilterWidget();
    m_filterLineEdit->setText( filter );
    gridLayout->addWidget( m_filterWidget, gridLayout->rowCount(), 0, 1, gridLayout->columnCount() );
}

AddFileIndexSourceDialog::~AddFileIndexSourceDialog()
{
}

void AddFileIndexSourceDialog::initFilterWidget()
{
    m_filterWidget = new QWidget( this );

    QStringList endings = QStringList() << "PDF" << "mp3" << "ogg";

    QMap<QString, QCheckBox*> checkBoxes;
    QHBoxLayout* hlayout = new QHBoxLayout(m_filterWidget);
    for (const QString & ending : endings)
    {
        QCheckBox* cb = new QCheckBox(m_filterWidget);
        checkBoxes.insert(ending, cb);
        cb->setText(tr("*.%1").arg(ending));
        hlayout->addWidget( cb );
    }
    QCheckBox* allCheckBox = new QCheckBox(m_filterWidget);
    allCheckBox->setText(tr("all files"));
    hlayout->addWidget( allCheckBox );


    m_filterLineEdit = new QLineEdit(m_filterWidget);
    m_filterLineEdit->setPlaceholderText(tr("< No files >"));
    hlayout->addWidget(m_filterLineEdit);

    m_filterWidget->setLayout(hlayout);

    for (QCheckBox* cb : checkBoxes.values())
    {
        connect( cb, &QCheckBox::toggled, [allCheckBox](bool c) { if (!c) allCheckBox->setChecked(false); });
    }
    connect( allCheckBox, &QCheckBox::toggled, [checkBoxes](bool c) { if (c)
        {
            for (QCheckBox* cb : checkBoxes.values())
            {
                cb->setChecked( true );
            }
        }
    });

    auto updateLineEdit = [checkBoxes, allCheckBox, this]()
    {
        m_filterLineEdit->blockSignals(true);

        QString text;
        if (allCheckBox->isChecked())
        {
            text = "*";
        }
        else
        {
            QStringList tokens;
            for (const QString& ending : checkBoxes.keys())
            {
                if (checkBoxes[ending]->isChecked())
                {
                    tokens << QString("*.%1").arg(ending.toLower());
                }
            }
            text = tokens.join("|");
        }
        m_filterLineEdit->setText(text);
        m_filterLineEdit->blockSignals(false);
    };

    auto updateCheckBoxes = [checkBoxes, allCheckBox, this]()
    {
        for (QCheckBox* cb : checkBoxes.values())
        {
            cb->blockSignals(true);
            cb->setChecked(false);
        }
        allCheckBox->blockSignals(true);
        allCheckBox->setChecked(false);

        QStringList tokens = m_filterLineEdit->text().split("|");
        for (QString token : tokens)
        {
            token = token.trimmed();
            if (token == "*")
            {
                for (QCheckBox* cb : checkBoxes.values())
                {
                    cb->setChecked(true);
                }
                allCheckBox->setChecked(true);
                break;
            }
            for (const QString & key : checkBoxes.keys())
            {
                if (token.toLower() == QString("*.%1").arg(key.toLower()))
                {
                    checkBoxes[key]->setChecked(true);
                }
            }
        }

        for (QCheckBox* cb : checkBoxes.values())
        {
            cb->blockSignals(false);
        }
        allCheckBox->blockSignals(false);
    };

    for (QCheckBox* cb : checkBoxes)
    {
        connect( cb, &QCheckBox::toggled, updateLineEdit);
    }
    connect( allCheckBox, &QCheckBox::toggled, updateLineEdit);
    connect( m_filterLineEdit, &QLineEdit::textChanged, updateCheckBoxes);

    allCheckBox->setChecked(true);

}
