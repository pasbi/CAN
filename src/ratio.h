#ifndef RATIO_H
#define RATIO_H

#include <QMultiMap>

class Ratio
{
public:
    Ratio(double d, double n);
    Ratio(double d);
    Ratio();
    double d, n;
    operator double();
    Ratio& operator+=(const Ratio& other);

    template<class T> static QMultiMap<double, QPair<T*, T*>> sortSimilar(QList<T *> masterItems, QList<T *> slaveItems)
    {
        // sort the <master, slave> pairs by their similarity.

        QMultiMap<double, QPair<T*, T*>> ratioMasterSlaveMap;

        // first, we remove all the equal-pairs from the list, since
        // - usually most items belong to an equal-pair
        // - checking for equality is super fast compared to computing similarity
        for (auto mit = masterItems.begin(); mit != masterItems.end();)
        {
            bool match = false;
            for (auto sit = slaveItems.begin(); sit != slaveItems.end() && !match;)
            {
                T* master = *mit;
                T* slave = *sit;
                if (*master == *slave)
                {
                    ratioMasterSlaveMap.insert(1.0, qMakePair(master, slave));
                    /*sit = */ slaveItems.erase(sit);
                    mit = masterItems.erase(mit);
                    match = true;
                }

                if (!match)
                {
                    ++sit;
                }
            }
            if (!match)
            {
                ++mit;
            }
        }

        // now we check for similarity among the remaining items.
        for (T* master : masterItems)
        {
            ratioMasterSlaveMap.insert(0, qMakePair(master, nullptr));
            for (T* slave : slaveItems)
            {
                Q_ASSERT(*master != *slave);    // equal pairs shall have been filtered out.
                Ratio sim = master->similarity(slave);
                ratioMasterSlaveMap.insert(sim, qMakePair(master, slave));
            }
        }
        for (T* slave : slaveItems)
        {
            ratioMasterSlaveMap.insert(0, qMakePair(nullptr, slave));
        }


        // inserting similarity produced duplicates.
        // here we remove those duplicates (keeping the best pair (greedy, should be almost optimal))
        // we could skip the equal pairs however the performance boost probably does not justifies the complexity boost.
        auto it = ratioMasterSlaveMap.end();
        while (it != ratioMasterSlaveMap.begin())
        {
            it--;

            T* master = it.value().first;
            T* slave = it.value().second;
            auto deleteIt = ratioMasterSlaveMap.begin();
            while (deleteIt != it)
            {
                if ( deleteIt.value().first == master  ||  deleteIt.value().second == slave )
                {
                    deleteIt = ratioMasterSlaveMap.erase(deleteIt);
                }
                else
                {
                    ++deleteIt;
                }
            }
        }

        return ratioMasterSlaveMap;
    }

    template<class T> static Ratio similarMapToRatio(const QMultiMap<double, QPair<T*, T*>>& map, double threshold)
    {
        Ratio r;
        for (double s : map.keys())
        {
            if (s >= threshold) // protect against NaN
            {
                r.d += s;
            }
            else
            {
                r.d += 0.0;
            }
            r.n += 1;
        }
        return r;
    }
};
Ratio operator+(const Ratio& a, const Ratio& b);


#endif // RATIO_H
