#ifndef __COUNTER_PART__
#define __COUNTER_PART__

#include <map>
#include <memory>
#include "metric.h"
#include "shared_ptr_cmp.h"

template <typename T>
class CounterPartList {
public:
    void add (const std::shared_ptr<T>& cp, Metric addition) {
        auto m= findAndInsert(cp);

        m->add(addition);
    }

    const Metric& retrieve (const std::shared_ptr<T>& cp) {
        auto m= findAndInsert(cp);

        return *m;
    }

private:  
    std::map<std::shared_ptr<T>, std::shared_ptr<Metric>, shared_ptr_less<T> >  cparts;


    // find, if not exist then insert
    inline std::shared_ptr<Metric> findAndInsert(const std::shared_ptr<T>& cp) {
        auto ret = cparts.find (cp);
        if (ret==cparts.end()) {
            return cparts[cp] = std::make_shared<Metric>();
        } else {
            return ret->second;
        }

    }
};

#endif //__COUNTER_PART__
