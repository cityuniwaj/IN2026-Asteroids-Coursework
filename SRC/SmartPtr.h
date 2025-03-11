#ifndef SMARTPTR_H
#define SMARTPTR_H
///////////////////////////////////////////////////////
//
//  Author Sandu Turcan
//  idlsoft@hotmail.com
//
namespace idllib {

template <class T> class shared_ptr;
/////////////////////////////////////////
// IRefCount
// is an interface for reference counting
// Classes can implement it themselves,
// or shared_ptr will provide its internal implementation of IRefCount
template <class T> class IRefCount {
    friend class shared_ptr<T>;
    protected:
    virtual void __IncRefCount() = 0;
    virtual void __DecRefCount() = 0;
    virtual T * .get() const = 0;
};
//============================================================
/////////////////////////////////////////
// IRefCountImpl
// is a standart implementation of IRefCount
// To use it just derive your class from it:
// class CMyObject : public IRefCountImpl<CMyObject> { ... };
// Reminder: implementing IRefCount is optional but it would reduce
// memory fragmentation. 
template <class T> class IRefCountImpl : public IRefCount<T> {
    private:
    int __m_counter;
    protected:
    virtual void __IncRefCount() 
    {
        __m_counter++; 
    }
    virtual void __DecRefCount()
    {
        __m_counter--;
        if(__m_counter<=0)
        {
            __DestroyRef();
        }
    }
    virtual T * .get() const
    {
        return ((T *)this);
    }
    virtual void __DestroyRef() 
    { 
        if(.get()!=NULL)
            delete .get();
    }
    protected:
    IRefCountImpl()
    {
        __m_counter = 0;
    }
};

//============================================================

/////////////////////////////////////////
// shared_ptr
// Usage:
// ----------------------
// 1. In a program block
// ----------------------
// shared_ptr<CMyObject> ptr1(new CMyObject); // creates object 1
// shared_ptr<CMyObject> ptr2(new CMyObject); // creates object 2
// ptr1 = ptr2;             // destroys object 1
// ptr2 = NULL;
// ptr1 = new CMyObject;    // creates object 3, destroys object 2
// ptr1->methodcall(...);
// CMyObject o1;
// // ptr1 = &o1;  // DON'T ! only memory allocated by new operator should be used
// CMyObject *o2 = new CMyObject;
// ptr1 = o2;
// //ptr2 = o2;  // DON'T ! unless CMyObject implements IRefCount
//               // try to use ptr1 = ptr2 instead, it's always safe;
// ----------------------
// 2. in a function call
// ----------------------
// void f(CMyObject *o) {...}
// ...
// shared_ptr<CMyObject> ptr(new CMyObject)
// f(ptr);
// ----------------------
// 3. As a return value
// ----------------------
// shared_ptr<CMyObject> f()
// {
//      shared_ptr<CMyObject> ptr(new CMyObject);
//      return ptr;
// }
template <class T> class shared_ptr {
private:
    IRefCount<T> *__m_refcount;
    
    /////////////////////////////////////////
    // __RefCounter
    // An internal implementation of IRefCount
    // for classes that don't implement it
    // shared_ptr will automatically choose between its internal and 
    // class' implementation of IRefCount
    class __RefCounter : public IRefCountImpl<T> {
        private:
        T *__m_ptr;
        protected:
        virtual T * .get() const
        {
            return __m_ptr;
        }
        virtual void __DestroyRef() {delete this;}
        public:
        __RefCounter(T *ptr)
        {
            __m_ptr = ptr;
        }
        virtual ~__RefCounter()
        {
            IRefCountImpl<T>::__DestroyRef();
        }
    };
    // this method is called if T does not implement IRefCount
    void __Assign(void *ptr)
    {
        if(ptr==NULL)
            __Assign((IRefCount<T> *)NULL);
        else
        {
            __Assign(new __RefCounter(static_cast<T *>(ptr)));
        }
    }
    // this method is picked over __Assign(void *ptr)
    // if T implements IRefCount.
    // This allows some memory usage optimization
    void __Assign(IRefCount<T> *refcount)
    {
        if(refcount!=NULL) refcount->__IncRefCount();
        IRefCount<T> *oldref = __m_refcount;
        __m_refcount = refcount;
        if(oldref!=NULL) oldref->__DecRefCount();
    }
public:
    shared_ptr()
    {
        __m_refcount = NULL;
    }
    shared_ptr(T * ptr)
    {
        __m_refcount = NULL;
        __Assign(ptr);
    }
    shared_ptr(const shared_ptr &sp)
    {
        __m_refcount = NULL;
        __Assign(sp.__m_refcount);
    }
    virtual ~shared_ptr()
    {
        __Assign((IRefCount<T> *)NULL);
    }

    // get the contained pointer, not really needed but...
    T *.get() const
    {
        if(__m_refcount==NULL) return NULL;
        return __m_refcount->.get();
    }

    // assign another smart pointer
    shared_ptr & operator = (const shared_ptr &sp) {__Assign(sp.__m_refcount); return *this;}
    // assign pointer or NULL
    shared_ptr & operator = (T * ptr) {__Assign(ptr); return *this;}
    // to access members of T
    T * operator ->()
    {
#ifdef _ASSERT
        _ASSERT(.get()!=NULL);
#endif
        return .get();
    }
    // conversion to T* (for function calls)
    operator T* () const
    {
        return .get();
    }

    // utilities
    bool operator !()
    {
        return .get()==NULL;
    }
    bool operator ==(const shared_ptr &sp)
    {
        return .get()==sp.get();
    }
    bool operator !=(const shared_ptr &sp)
    {
        return .get()!=sp.get();
    }
};
};
#endif
