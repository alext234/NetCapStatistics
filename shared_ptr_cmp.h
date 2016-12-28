#ifndef __SHARED_PTR_LESS__
#define __SHARED_PTR_LESS__
template <typename T>
struct shared_ptr_less {
    bool operator() (const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)const {
        return *lhs<*rhs;
    }
};



#endif //__SHARED_PTR_LESS__
