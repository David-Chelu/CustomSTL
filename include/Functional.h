#pragma once
#include "Utility.h"

CUSTOM_BEGIN


enum class InvokeFlag
{
    Functor,    // non-member function

    PMFObject,  // member function from object
    PMFRefwrap, // member function from reference wrapper (not implemented)
    PMFPointer, // member function from pointer object

    PMDObject,
    PMDRefwrap, // (not implemented)
    PMDPointer
};

// All invoke implmentations
struct _InvokerFunctor
{
    static constexpr InvokeFlag Flag = InvokeFlag::Functor;

    template<class Callable, class... Args>
    static constexpr auto invoke_impl(Callable&& func, Args&&... _Args) noexcept
    -> decltype(static_cast<Callable&&>(func)(static_cast<Args&&>(_Args)...)) {
        return static_cast<Callable&&>(func)(static_cast<Args&&>(_Args)...);
    }
};

struct _InvokerPMFObject
{
    static constexpr InvokeFlag Flag = InvokeFlag::PMFObject;

    template <class Decayed, class Type, class... Args>
    static constexpr auto invoke_impl(Decayed pmf, Type&& arg1, Args&&... args) noexcept
    -> decltype((static_cast<Type&&>(arg1).*pmf)(static_cast<Args&&>(args)...)) {
        return (static_cast<Type&&>(arg1).*pmf)(static_cast<Args&&>(args)...);
    }
};

struct _InvokerPMFRefwrap
{
    static constexpr InvokeFlag Flag = InvokeFlag::PMFRefwrap;

    // template <class Decayed, class _Refwrap, class... Args>
    // static constexpr auto invoke_impl(Decayed pmf, _Refwrap _Rw, Args&&... args) noexcept(
    //     noexcept((_Rw.get().*pmf)(static_cast<Args&&>(args)...)))
    //     -> decltype((_Rw.get().*pmf)(static_cast<Args&&>(args)...)) {
    //     return (_Rw.get().*pmf)(static_cast<Args&&>(args)...);
    // }
};

struct _InvokerPMFPointer
{
    static constexpr InvokeFlag Flag = InvokeFlag::PMFPointer;

    template <class Decayed, class Type, class... Args>
    static constexpr auto invoke_impl(Decayed pmf, Type&& arg1, Args&&... args) noexcept
    -> decltype(((*static_cast<Type&&>(arg1)).*pmf)(static_cast<Args&&>(args)...)) {
        return ((*static_cast<Type&&>(arg1)).*pmf)(static_cast<Args&&>(args)...);
    }
};

struct _InvokerPMDObject
{
    static constexpr InvokeFlag Flag = InvokeFlag::PMDObject;

    template <class Decayed, class Type>
    static constexpr auto invoke_impl(Decayed pmd, Type&& arg1) noexcept
    -> decltype(static_cast<Type&&>(arg1).*pmd) {
        return static_cast<Type&&>(arg1).*pmd;
    }
};

struct _InvokerPMDRefwrap
{
    static constexpr InvokeFlag Flag = InvokeFlag::PMDRefwrap;

    // template <class Decayed, class _Refwrap>
    // static constexpr auto invoke_impl(Decayed pmd, _Refwrap _Rw) noexcept -> decltype(_Rw.get().*pmd) {
    //     return _Rw.get().*pmd;
    // }
};

struct _InvokerPMDPointer
{
    static constexpr InvokeFlag Flag = InvokeFlag::PMDPointer;

    template <class Decayed, class Type>
    static constexpr auto invoke_impl(Decayed pmd, Type&& arg1) noexcept
    -> decltype((*static_cast<Type&&>(arg1)).*pmd) {
        return (*static_cast<Type&&>(arg1)).*pmd;
    }
};

// Invoker implementation choice
template <class Callable, class Type, class NoCVRef_t = RemoveCVRef_t<Callable>,
bool IsPMF = IsMemberFunctionPointer_v<NoCVRef_t>,
bool IsPMD = IsMemberObjectPointer_v<NoCVRef_t>>
struct _Invoker;

template <class Callable, class Type, class NoCVRef_t>
struct _Invoker<Callable, Type, NoCVRef_t, true, false>
: Conditional_t<IsBaseOf_v<typename _IsMemberFunctionPointer<NoCVRef_t>::ClassType, RemoveReference_t<Type>>,
_InvokerPMFObject,
/*Conditional_t<_Is_specialization_v<_Remove_cvref_t<Type>, reference_wrapper>, _Invoker_pmf_refwrap,*/
_InvokerPMFPointer> {}; // pointer to member function

template <class _Callable, class Type, class NoCVRef_t>
struct _Invoker<_Callable, Type, NoCVRef_t, false, true>
: Conditional_t<IsBaseOf_v<typename _IsMemberObjectPointer<NoCVRef_t>::ClassType, RemoveReference_t<Type>>,
_InvokerPMDObject,
/*Conditional_t<_Is_specialization_v<_Remove_cvref_t<Type>, reference_wrapper>, _Invoker_pmd_refwrap,*/
_InvokerPMDPointer> {}; // pointer to member data

// Choose and call Invoke implementation
template<class Callable, class Type, class... Args>
auto invoke(Callable&& func, Type&& arg1, Args&&... args) noexcept
-> decltype(_Invoker<Callable, Type>::invoke_impl(static_cast<Callable&&>(func), static_cast<Type&&>(arg1), static_cast<Args&&>(args)...)) {

    switch (_Invoker<Callable, Type>::Flag)
    {
        case InvokeFlag::Functor:
            return static_cast<Callable&&>(func)(static_cast<Type&&>(arg1), static_cast<Args&&>(args)...);

        case InvokeFlag::PMFObject:
            return;

        case InvokeFlag::PMFRefwrap:
            return; // not implemented

        case InvokeFlag::PMFPointer:
            return;

        case InvokeFlag::PMDObject:
            return;

        case InvokeFlag::PMDRefwrap:
            return; // not implemented

        case InvokeFlag::PMDPointer:
            return;
    }
}


// TODO: refactor
template<class Signature>
class Function;

template<class RetType, class... ArgTypes>
class Function<RetType(ArgTypes...)>        // Wrapper template to function pointer
{
private:
    using Invoker           = RetType(*)(const Function&, ArgTypes&&...);
    using Copiator          = void*(*)(const Function&);
    using Destructor        = void(*)(const Function&);

private:
    void* _functor          = nullptr;
    Invoker _invoker        = nullptr;
    Copiator _copiator      = nullptr;
    Destructor _destructor  = nullptr;

private:
    // Core functions

    template<class Functor>
    static RetType _invoke_impl(const Function& target, ArgTypes&&... args) {
        return (*reinterpret_cast<Functor*>(target._functor))(custom::forward<ArgTypes>(args)...);
    }

    template<class Functor>
    static void* _copy_impl(const Function& target) {
        return reinterpret_cast<void*>(new Functor(*reinterpret_cast<Functor*>(target._functor)));
    }

    template<class Functor>
    static void _destruct_impl(const Function& target) {
        delete reinterpret_cast<Functor*>(target._functor);
    }

public:
    // Constructors

    Function() = default;

    template<class Functor, EnableIf_t<!IsSame<Decay_t<Functor>, Function>::Value, bool> = true>
    Function(Functor&& f) {
        using FunctorType = Decay_t<Functor>;

        _invoker    = reinterpret_cast<Invoker>(_invoke_impl<FunctorType>);
        _copiator   = reinterpret_cast<Copiator>(_copy_impl<FunctorType>);
        _destructor = reinterpret_cast<Destructor>(_destruct_impl<FunctorType>);
        _functor    = reinterpret_cast<void*>(new FunctorType(custom::forward<Functor>(f)));
    }

    Function(const Function& other) {
        _copy(other);
    }

    Function(Function&& other) noexcept {
        _move(custom::move(other));
    }

    ~Function() {
        _clear();
    }
    
public:
    // Operators

    RetType operator()(ArgTypes&&... args) const {
        if (_functor == nullptr) 
            throw std::runtime_error("call an empty function object");
        
        return _invoker(*this, custom::forward<ArgTypes>(args)...);
    }

    template<class Functor, EnableIf_t<!IsSame<Decay_t<Functor>, Function>::Value, bool> = true>
    Function& operator=(Functor&& func) {
        using FunctorType = Decay_t<Functor>;

        _clear();
        _invoker    = reinterpret_cast<Invoker>(_invoke_impl<FunctorType>);
        _copiator   = reinterpret_cast<Copiator>(_copy_impl<FunctorType>);
        _destructor = reinterpret_cast<Destructor>(_destruct_impl<FunctorType>);
        _functor    = reinterpret_cast<void*>(new FunctorType(custom::forward<Functor>(func)));

        return *this;
    }

    Function& operator=(const Function& other) {
        if (_functor != other._functor)
        {
            _clear();
            _copy(other);
        }

        return *this;
    }

    Function& operator=(Function&& other) {
        if (_functor != other._functor)
        {
            _clear();
            _move(custom::move(other));
        }

        return *this;
    }

    explicit operator bool() const noexcept {
        if (_functor != nullptr)
            return true;

        return false;
    }

private:
    // Helpers

    void _clear() {
        if (_functor != nullptr)
        {
            _destructor(*this);
            _functor = nullptr;
        }
    }

    void _copy(const Function& other) {
        _invoker        = other._invoker;
        _copiator       = other._copiator;
        _destructor     = other._destructor;
        _functor        = other._copiator(other);
    }

    void _move(Function&& other) {
        _invoker        = other._invoker;
        _copiator       = other._copiator;
        _destructor     = other._destructor;
        _functor        = other._functor;

        other._functor  = nullptr;
    }
}; // END Function template

CUSTOM_END