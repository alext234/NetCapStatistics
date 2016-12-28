#ifndef __COUNTER_PART__
#define __COUNTER_PART__

#include <map>
#include <memory>
#include "metric.h"
#include "shared_ptr_cmp.h"

template <typename T>
class CounterPartList {
public:
    void add (const std::shared_ptr<T>& cp, Metric addition);

    const Metric& retrieve (const std::shared_ptr<T>& cp);

private:  
    std::map<std::shared_ptr<T>, std::shared_ptr<Metric>, shared_ptr_less<T> >  cparts;


    // find, if not exist then insert
    inline std::shared_ptr<Metric> findAndInsert(const std::shared_ptr<T>& cp) ;
};

#include "counter_part.ipp"



#endif //__COUNTER_PART__
