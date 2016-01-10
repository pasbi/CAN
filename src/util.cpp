#include "util.h"
#include <QDir>
#include "global.h"



// see http://stackoverflow.com/questions/14009522/how-to-remove-accents-diacritic-marks-from-a-string-in-qt
QString Util::diacriticLetters()
{
    return QString::fromUtf8("ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ");
}

QStringList Util::nonDiacriticLetters()
{
   return QStringList() <<  "S" << "OE" << "Z" << "s" << "oe" << "z" << "Y" << "Y" << "u" << "A" << "A" << "A" <<
                            "A" << "A" << "A" << "AE" << "C" << "E" << "E" << "E" << "E" << "I" << "I" << "I" <<
                            "I" << "D" << "N" << "O" << "O" << "O" << "O" << "O" << "O" << "U" << "U" << "U" <<
                            "U" << "Y" << "s" << "a" << "a" << "a" << "a" << "ae" << "a" << "ae" << "c" << "e" << "e" <<
                            "e" << "e" << "i" << "i" << "i" << "i" << "o" << "n" << "o" << "o" << "o" << "o" << "oe" <<
                            "o" << "u" << "u" << "u" << "ue" << "y" << "y";
}


QString Util::replaceDiacritics(QString string)
{

    for (int i = 0; i < diacriticLetters().length(); ++i)
    {
        string.replace( diacriticLetters()[i], nonDiacriticLetters()[i] );
    }
    return string;
}

