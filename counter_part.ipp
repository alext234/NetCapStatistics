
#ifndef __COUNTER_PART_IPP__
#define __COUNTER_PART_IPP__

class CounterPartNotFoundException: public std::exception {
public:
    CounterPartNotFoundException (std::string r): _reason(r) {}
    const char* what() const noexcept {
        return _reason.c_str();
    }
private:
    std::string _reason;
};


template <typename T>
void CounterPartList<T>::add (const std::shared_ptr<T>& cp, Metric addition) {
        auto m= findAndInsert(cp);

        m->add(addition);
    }

template <typename T>
const Metric& CounterPartList<T>::retrieve (const std::shared_ptr<T>& cp) {
        auto m= find(cp);

        return *m;
    }

template <typename T>
inline std::shared_ptr<Metric> CounterPartList<T>::findAndInsert(const std::shared_ptr<T>& cp) {
        auto ret = cparts.find (cp);
        if (ret==cparts.end()) {
            return cparts[cp] = std::make_shared<Metric>();
        } else {
            return ret->second;
        }

    }
template <typename T>
inline std::shared_ptr<Metric> CounterPartList<T>::find(const std::shared_ptr<T>& cp) {
        auto ret = cparts.find (cp);
        if (ret==cparts.end()) {
            throw CounterPartNotFoundException("counterpart not found");
        } else {
            return ret->second;
        }

    }

#endif // __COUNTER_PART_IPP__    
