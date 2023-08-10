#pragma once
#include "Utility.h"
#include "TypeInfo.h"

#include <atomic>

CUSTOM_BEGIN


template<class Ty, class DeleterNoref, class = void>
struct _GetDeleterPointerType { using Type = Ty*; };

template<class Ty, class DeleterNoref>
struct _GetDeleterPointerType<Ty, DeleterNoref, Void_t<typename DeleterNoref::Pointer>> { using Type = typename DeleterNoref::Pointer; };

template<class Deleter>
using _UniquePtrEnableDefault_t = EnableIf_t<Conjunction_v<
                                                Negation<IsPointer<Deleter>>,
                                                IsDefaultConstructible<Deleter>>, bool>;


template<class Type>
struct DefaultDelete                                        // default deleter for UniquePtr
{
    DefaultDelete() = default;

    template<class Type2, EnableIf_t<IsConvertible_v<Type2*, Type*>, bool> = true>
    DefaultDelete(const DefaultDelete<Type2>&) { /*Empty*/ }

    void operator()(Type* ptr) const noexcept
    {
        static_assert(0 < sizeof(Type), "can't delete an incomplete type");
        delete ptr;
    }
}; // END DefaultDelete


template<class Type>
struct DefaultDelete<Type[]>                                // default deleter for unique_ptr to array of unknown size
{
    DefaultDelete() = default;

    template<class Type2, EnableIf_t<IsConvertible_v<Type2(*)[], Type(*)[]>, bool> = true>
    DefaultDelete(const DefaultDelete<Type2[]>&) { /*Empty*/ }

    template<class Type2, EnableIf_t<IsConvertible_v<Type2(*)[], Type(*)[]>, bool> = true>
    void operator()(Type2* ptr) const noexcept
    {
        static_assert(0 < sizeof(Type2), "can't delete an incomplete type");
        delete[] ptr;
    }
}; // END DefaultDelete[]


// UniquePtr implementation
template<class Type, class Deleter = DefaultDelete<Type>>
class UniquePtr                                             // non-copyable pointer to an object
{
public:
    using ValueType     = Type;
    using DeleterType   = Deleter;
    using Pointer       = typename _GetDeleterPointerType<Type, RemoveReference_t<Deleter>>::Type;

private:
    Pointer _data;
    Deleter _deleter;

public:
    // Constructors

    template<class Deleter2 = Deleter, _UniquePtrEnableDefault_t<Deleter2> = true>
    UniquePtr() { /*Empty*/ }

    template<class Deleter2 = Deleter, _UniquePtrEnableDefault_t<Deleter2> = true>
    UniquePtr(std::nullptr_t) { /*Empty*/ }

    template<class Deleter2 = Deleter, _UniquePtrEnableDefault_t<Deleter2> = true>
    explicit UniquePtr(Pointer ptr)
        : _data(ptr) { /*Empty*/ }

    template<class Deleter2 = Deleter,
    EnableIf_t<IsConstructible_v<Deleter2, const Deleter2&>, bool> = true>
    UniquePtr(Pointer ptr, const Deleter& del)
        : _data(ptr), _deleter(del) { /*Empty*/ }

    template<class Deleter2 = Deleter,
    EnableIf_t<Conjunction_v<
                    Negation<IsReference<Deleter2>>,
                    IsConstructible<Deleter2, Deleter2>>, bool> = true>
    UniquePtr(Pointer ptr, Deleter&& del)
        : _data(ptr), _deleter(custom::move(del)) { /*Empty*/ }

    template<class Deleter2 = Deleter,
    EnableIf_t<Conjunction_v<
                    IsReference<Deleter2>,
                    IsConstructible<Deleter2, RemoveReference_t<Deleter2>>>, bool> = true>
    UniquePtr(Pointer, RemoveReference_t<Deleter>&&) = delete;

    template<class Deleter2 = Deleter,
    EnableIf_t<IsMoveConstructible_v<Deleter2>, bool> = true>
    UniquePtr(UniquePtr&& other) noexcept
        : _data(other.release()), _deleter(custom::forward<Deleter>(other.get_deleter())) { /*Empty*/ }

    template<class Type2, class Deleter2,
    EnableIf_t<Conjunction_v<
                    Negation<IsArray<Type2>>,
                    IsConvertible<typename UniquePtr<Type2, Deleter2>::Pointer, Pointer>,
                    Conditional_t<IsReference_v<Deleter>, IsSame<Deleter2, Deleter>, IsConvertible<Deleter2, Deleter>>>,
    bool> = true>
    UniquePtr(UniquePtr<Type2, Deleter2>&& other) noexcept
        : _data(other.release()), _deleter(custom::forward<Deleter2>(other.get_deleter())) { /*Empty*/ }

    UniquePtr(const UniquePtr&) = delete;

    ~UniquePtr() {
        if (_data != nullptr)
            _deleter(_data);
    }

public:
    // Operators

    UniquePtr& operator=(std::nullptr_t) noexcept {
        reset();
        return *this;
    }

    template<class Type2, class Deleter2,
    EnableIf_t<Conjunction_v<
                    Negation<IsArray<Type2>>,
                    IsAssignable<Deleter&, Deleter2>,
                    IsConvertible<typename UniquePtr<Type2, Deleter2>::Pointer, Pointer>>,
    bool> = true>
    UniquePtr& operator=(UniquePtr<Type2, Deleter2>&& other) noexcept {
        reset(other.release());
        _deleter = custom::forward<Deleter2>(other._deleter);

        return *this;
    }

    template<class Deleter2 = Deleter,
    EnableIf_t<IsMoveAssignable_v<Deleter2>, bool> = true>
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (_data != other._data) {
            reset(other.release());
            _deleter = custom::forward<Deleter>(other._deleter);
        }

        return *this;
    }

    UniquePtr& operator=(const UniquePtr&)  = delete;

    AddLvalueReference_t<Type> operator*() const noexcept {
       return *_data;
    }

    Pointer operator->() const noexcept {
        return _data;
    }

    explicit operator bool() const noexcept {
        return _data != nullptr;
    }

public:
    // Main functions

    Deleter& get_deleter() noexcept {
        return _deleter;
    }

    const Deleter& get_deleter() const noexcept {
        return _deleter;
    }

    Pointer get() const noexcept {
        return _data;
    }

    Pointer release() noexcept {                            // Releases the ownership of the managed object, get() returns nullptr after this
        return custom::exchange(_data, nullptr);
    }

    void reset(Pointer ptr = nullptr) noexcept {            // Replaces the managed object and deletes old
        Pointer old = custom::exchange(_data, ptr);

        if (old)
            _deleter(old);
    }
}; // END UniquePtr


template<class Type, class Deleter>
class UniquePtr<Type[], Deleter>                            // non-copyable pointer to an array object
{
public:
    using ValueType     = Type;
    using DeleterType   = Deleter;
    using Pointer       = typename _GetDeleterPointerType<Type, RemoveReference_t<Deleter>>::Type;

    template<class Type2, class IsNullptr = IsSame<Type2, std::nullptr_t>>
    using _EnableConstructorReset = EnableIf_t<Disjunction_v<
                                                    IsSame<Type2, Pointer>,
                                                    IsNullptr,
                                                    Conjunction<
                                                        IsSame<Pointer, ValueType*>,
                                                        IsPointer<Type2>,
                                                        IsConvertible<RemovePointer_t<Type2>(*)[], ValueType(*)[]>>>,
                                    bool>;

    template<class Type2, class Del, class More,
    class UpPointer         = typename UniquePtr<Type2, Del>::Pointer,
    class UpValueType       = typename UniquePtr<Type2, Del>::ValueType>
    using _EnableConversion =   EnableIf_t<Conjunction_v<
                                                IsArray<Type2>,
                                                IsSame<Pointer, ValueType*>,
                                                IsSame<UpPointer, UpValueType*>,
                                                IsConvertible<UpValueType(*)[], ValueType(*)[]>,
                                                More>,  // More is used for additional enable traits
                                bool>;  // TODO: check

private:
    Pointer _data;
    Deleter _deleter;

public:
    // Constructors

    template<class Deleter2 = Deleter,
    _UniquePtrEnableDefault_t<Deleter2> = true>
    UniquePtr() { /*Empty*/ }

    template<class Type2, class Deleter2 = Deleter,
    _UniquePtrEnableDefault_t<Deleter2> = true,
    _EnableConstructorReset<Type2> = true>
    explicit UniquePtr(Type2 ptr)
        : _data(ptr) { /*Empty*/ }

    template<class Type2, class Deleter2 = Deleter, 
    EnableIf_t<IsCopyConstructible_v<Deleter2>, bool> = true,
    _EnableConstructorReset<Type2> = true>
    UniquePtr(Type2 ptr, const Deleter& del)
        : _data(ptr), _deleter(del) { /*Empty*/ }

    template<class Type2, class Deleter2 = Deleter,
    EnableIf_t<Conjunction_v<
                    Negation<IsReference<Deleter2>>,
                    IsMoveConstructible<Deleter2>>, bool> = true,
    _EnableConstructorReset<Type2> = true>
    UniquePtr(Type2 ptr, Deleter&& del) noexcept
        : _data(ptr), _deleter(custom::move(del)) { /*Empty*/ }

    template<class Type2, class Deleter2 = Deleter,
    EnableIf_t<Conjunction_v<
                    IsReference<Deleter2>,
                    IsConstructible<Deleter2, RemoveReference_t<Deleter2>>>, bool> = true>
    UniquePtr(Type2, RemoveReference_t<Deleter>&&) = delete;

    template<class Deleter2 = Deleter,
    EnableIf_t<IsMoveConstructible_v<Deleter2>, bool> = true>
    UniquePtr(UniquePtr&& other) noexcept
        : _data(other.release()), _deleter(custom::forward<Deleter>(other.get_deleter())) { /*Empty*/ }

    UniquePtr(const UniquePtr&) = delete;

    ~UniquePtr() {
        if (_data != nullptr)
            _deleter(_data);
    }

public:
    // Operators

    UniquePtr& operator=(std::nullptr_t) noexcept {
        reset();
        return *this;
    }

    template<class Type2, class Deleter2,
    _EnableConversion<Type2, Deleter2, IsAssignable<Deleter&, Deleter2>> = true>
    UniquePtr& operator=(UniquePtr<Type2, Deleter2>&& other) noexcept {
        reset(other.release());
        _deleter = custom::forward<Deleter2>(other._deleter);

        return *this;
    }

    template<class Deleter2 = Deleter,
    EnableIf_t<IsMoveAssignable_v<Deleter2>, bool> = true>
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (_data != other._data) {
            reset(other.release());
            _deleter = custom::move(other._deleter);
        }

        return *this;
    }

    UniquePtr& operator=(const UniquePtr&) = delete;

    Type& operator[](size_t index) const noexcept {
        return _data[index];
    }

    explicit operator bool() const noexcept {
        return _data != nullptr;
    }

public:
    // Main functions

    Deleter& get_deleter() noexcept {
        return _deleter;
    }

    const Deleter& get_deleter() const noexcept {
        return _deleter;
    }

    Pointer get() const noexcept {
        return _data;
    }

    Pointer release() noexcept {                                                // Releases the ownership of the managed object, get() returns nullptr after this
        return custom::exchange(_data, nullptr);
    }

    template<class Type2, _EnableConstructorReset<Type2, FalseType> = true>     // FalseType (don't allow nullptr ???)
    void reset(Type2 ptr) noexcept {                                            // Replaces the managed object and deletes old
        Pointer old = custom::exchange(_data, ptr);

        if (old)
            _deleter(old);
    }

}; // END UniquePtr[]


// build UniquePtr
template<class Ty, class... Args, EnableIf_t<!IsArray_v<Ty>, bool> = true>
UniquePtr<Ty> make_unique(Args&&... args) {
    return UniquePtr<Ty>(new Ty(custom::forward<Args>(args)...));
}

template<class Ty, EnableIf_t<IsArray_v<Ty> && Extent_v<Ty> == 0, bool> = true>
UniquePtr<Ty> make_unique(const size_t size) {
    return UniquePtr<Ty>(new RemoveExtent_t<Ty>[size]());
}

template<class Ty, class... Args, EnableIf_t<Extent_v<Ty> != 0, bool> = true>
void make_unique(Args&&...) = delete;


// Shared/Weak Ptr implementation
class /*__declspec(novtable)*/ _RefCountBase     // TODO: why __declspec(novtable) ???
{
private:
    std::atomic<long> _uses;
    std::atomic<long> _weaks;

protected:
    _RefCountBase() noexcept = default;    // non-atomic initializations

public:
    // Constructors & Operators
    
    _RefCountBase(const _RefCountBase&)               = delete;
    _RefCountBase& operator=(const _RefCountBase&)    = delete;

    virtual ~_RefCountBase() { /*Empty*/ }

public:
    // Main functions

    void _incref() noexcept {           // increment use count
        ++_uses;
    }

    void _incwref() noexcept {          // increment weak reference count
        ++_weaks;
    }

    void _decref() noexcept {           // decrement use count
        if (--_uses == 0)
        {
            _destroy();
            _decwref();
        }
    }

    void _decwref() noexcept {          // decrement weak reference count
        if (--_weaks == 0)
            _delete_this();
    }

    long _use_count() const noexcept {
        return static_cast<long>(_uses);
    }

    virtual void* _get_deleter(const TypeInfo& ti) const noexcept {    // TODO: wtf is this?
        return nullptr;
    }

private:
    // Helpers

    virtual void _destroy() noexcept {
        std::terminate();
    }

    virtual void _delete_this() noexcept {
        std::terminate();
    }
}; // END _RefCountBase

template<class Type>
class _RefCount : public _RefCountBase    // handle reference counting for pointer without deleter
{
private:
    Type* _ptr;

public:

    explicit _RefCount(Type* ptr)
        : _RefCountBase(), _ptr(ptr) { /*Empty*/ }

private:

    void _destroy() noexcept override {     // destroy managed resource
        delete _ptr;
    }

    void _delete_this() noexcept override { // destroy self
        delete this;
    }
}; // END _RefCount

template<class Type, class Deleter>
class _RefCountDeleter : public _RefCountBase     // handle reference counting for object with deleter
{
private:
    Type* _ptr;
    Deleter _deleter;

public:

    _RefCountDeleter(Type* ptr, Deleter& dt)
        : _RefCountBase(), _ptr(ptr), _deleter(dt) { /*Empty*/ }

private:
    void _destroy() noexcept override { // destroy managed resource
        _deleter(_ptr);
    }

    void _delete_this() noexcept override { // destroy self
        delete this;
    }
}; // END _RefCountDeleter

template<class Type>
class SharedPtr;

template<class Type>
class WeakPtr;

template<class Type>
class SharedWeakBase        // base class for SharedPtr and WeakPtr
{
public:
    using ElementType = RemoveExtent_t<Type>;

private:
    ElementType* _ptr       = nullptr;
    _RefCountBase* _rep     = nullptr;

    friend SharedPtr<Type>;
    friend WeakPtr<Type>;

public:
    // Constructors & Operators

    SharedWeakBase(const SharedWeakBase&)               = delete;
    SharedWeakBase& operator=(const SharedWeakBase&)    = delete;

protected:
    
    SharedWeakBase() noexcept   = default;
    ~SharedWeakBase()           = default;      // YES: each child of this class manages the destructor itself

public:
    // Main functions

    long use_count() const noexcept {
        return _rep ? _rep->_use_count() : 0;
    }

    template<class _Ty>
    bool owner_before(const SharedWeakBase<_Ty>& other) const noexcept { // compare addresses of manager objects
        return _rep < other._rep;
    }

protected:

    ElementType* _get() const noexcept {
        return _ptr;
    }

    template<class _Ty>
    void _move(SharedWeakBase<_Ty>&& other) noexcept {
        _ptr = other._ptr;
        _rep = other._rep;

        other._ptr = nullptr;
        other._rep = nullptr;
    }

    template<class _Ty>
    void _copy(const SharedPtr<_Ty>& other) noexcept {     // Only SharedPtr can copy
        other._incref();

        _ptr = other._ptr;
        _rep = other._rep;
    }

    // template<class _Ty>
    // bool _construct_from_weak(const WeakPtr<_Ty>& other) noexcept {     // TODO: check _incref here
    //     // implement shared_ptr's ctor from weak_ptr, and weak_ptr::lock()
    //     if (other._rep && other._rep->_incref())
    //     {
    //         _ptr = other._ptr;
    //         _rep = other._rep;

    //         return true;
    //     }

    //     return false;
    // }

    void _incref() const noexcept {
        if (_rep)
            _rep->_incref();
    }

    void _decref() noexcept {
        if (_rep)
            _rep->_decref();
    }

    void _incwref() const noexcept {
        if (_rep)
            _rep->_incwref();
    }

    void _decwref() noexcept {
        if (_rep)
            _rep->_decwref();
    }

    void _swap(SharedWeakBase& other) noexcept { // swap pointers
        custom::swap(_ptr, other._ptr);
        custom::swap(_rep, other._rep);
    }
}; // END SharedWeakBase


template<class Type>
class SharedPtr : public SharedWeakBase<Type>    // class for reference counted resource management
{
private:
    using Base = SharedWeakBase<Type>;

public:
    using ElementType = typename Base::ElementType;

public:
    // Constructors & Operators

    SharedPtr() noexcept = default;
    SharedPtr(std::nullptr_t) noexcept { /*Empty*/ }

    SharedPtr(const SharedPtr& other) noexcept { // construct SharedPtr object that owns same resource as other
        this->_copy(other);
    }

    SharedPtr(SharedPtr&& other) noexcept {     // construct SharedPtr object that takes resource from other
        this->_move(custom::move(other));
    }

    ~SharedPtr() {                              // release resource
        this->_decref();
    }

    SharedPtr& operator=(const SharedPtr& other) noexcept {
        SharedPtr(other).swap(*this);
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        SharedPtr(custom::move(other)).swap(*this);
        return *this;
    }

    template<EnableIf_t<!Disjunction_v<IsArray<Type>, IsVoid<Type>>, bool> = true>
    Type& operator*() const noexcept {
        return *this->_get();
    }

    template<EnableIf_t<!IsArray_v<Type>, bool> = true>
    Type* operator->() const noexcept {
        return this->_get();
    }

public:
    // Main functions

    void swap(SharedPtr& other) noexcept {
        this->_swap(other);
    }

    void reset() noexcept { // release resource and convert to empty SharedPtr object
        SharedPtr().swap(*this);
    }

}; // END SharedPtr


// template<class Type>
// class WeakPtr : public SharedWeakBase<Type>
// {
//     // TODO: implement
    
// public:
//     // Constructors & Operators
    
//     WeakPtr() noexcept { /*Empty*/ }

//     WeakPtr(const WeakPtr& other) noexcept {

//     }

//     ~WeakPtr() {
//         this->_decwref();
//     }

// }; // END WeakPtr

CUSTOM_END