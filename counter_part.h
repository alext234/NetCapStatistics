#ifndef __COUNTER_PART__
#define __COUNTER_PART__

#include <map>
#include <memory>
#include "metric.h"

template <typename T>
class CounterPartList {
public:
    void add (const T& cp, Metric addition) {
        auto m= findAndInsert(cp);

        m->add(addition);
    }

    const Metric& retrieve (const T& cp) {
        auto m= findAndInsert(cp);

        return *m;
    }

private:  
    std::map<T, std::shared_ptr<Metric>>  cparts;


    // find, if not exist then insert
    inline std::shared_ptr<Metric> findAndInsert(const T& cp) {
        auto ret = cparts.find (cp);
        if (ret==cparts.end()) {
            return cparts[cp] = std::make_shared<Metric>();
        } else {
            return ret->second;
        }

    }
};

#endif //__COUNTER_PART__
