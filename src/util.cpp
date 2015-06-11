#include "util.h"
#include <QDir>
#include "global.h"

namespace Util
{

QString replaceDiacritics(QString string)
{
    // see http://stackoverflow.com/questions/14009522/how-to-remove-accents-diacritic-marks-from-a-string-in-qt
    const QString DIACRITIC_LETTERS = QString::fromUtf8("ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ");
    const QStringList NON_DIACRITIC_LETTERS = QStringList() <<  "S" << "OE" << "Z" << "s" << "oe" << "z" << "Y" << "Y" << "u" << "A" << "A" << "A" <<
                                                                "A" << "A" << "A" << "AE" << "C" << "E" << "E" << "E" << "E" << "I" << "I" << "I" <<
                                                                "I" << "D" << "N" << "O" << "O" << "O" << "O" << "O" << "O" << "U" << "U" << "U" <<
                                                                "U" << "Y" << "s" << "a" << "a" << "a" << "a" << "ae" << "a" << "ae" << "c" << "e" << "e" <<
                                                                "e" << "e" << "i" << "i" << "i" << "i" << "o" << "n" << "o" << "o" << "o" << "o" << "oe" <<
                                                                "o" << "u" << "u" << "u" << "ue" << "y" << "y";

    for (int i = 0; i < DIACRITIC_LETTERS.length(); ++i)
    {
        QChar c = DIACRITIC_LETTERS[i];
        string.replace( c, NON_DIACRITIC_LETTERS[i] );
    }
    return string;

}
}

