#include "ratio.h"
#include <qmath.h>
#include <QString>
#include <QDebug>

Ratio::Ratio(double d, double n)
    : d(d)
    , n(n)
{

}

Ratio::Ratio(double d)
    : Ratio(d, 1)
{

}

Ratio::Ratio()
    : Ratio(0, 0)
{

}

Ratio::operator double()
{
    double ratio = d/n;
    Q_ASSERT(ratio >= 0.0 && ratio <= 1.0);

    // n can be interpreted as confidence level,
    // d/n ~ similarity.
    // make the ratio more extrem if the confidence is greater.
    if (ratio < 0.5)
    {
        ratio = qPow(ratio * 2.0, n)/2.0;
    }
    else
    {
        ratio = 1.0 - qPow((1.0 - ratio) * 2, n)/2.0;
    }

    return ratio;
}

Ratio& Ratio::operator+=(const Ratio& other)
{
    d += other.d;
    n += other.n;
    return (*this);
}

QDebug& operator<<(QDebug& out, const Ratio& ratio)
{
    out << QString("Ratio(%1, %2)").arg(ratio.d).arg(ratio.n).toStdString().c_str();
    return out;
}

Ratio operator+(Ratio& a, Ratio& b)
{
    return Ratio(a.d + b.d, a.n + b.n);
}

