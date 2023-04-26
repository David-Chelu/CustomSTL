#pragma once
#include <cstddef>      // std::size_t
#include <stdexcept>    // exceptions
#include <cassert>      // assert

#define CUSTOM_BEGIN namespace custom {
#define CUSTOM_END }

#define STD_BEGIN namespace std {
#define STD_END }




CUSTOM_BEGIN

// integral constant
template<class Ty, Ty Val>
struct IntegralConstant
{
    using ValueType = Ty;
    using Type      = IntegralConstant;

    static constexpr ValueType Value = Val;

    constexpr operator ValueType() const noexcept {
        return Value;
    }

    constexpr ValueType operator()() const noexcept {
        return Value;
    }
};

// bool constant
template<bool Val>
using BoolConstant = IntegralConstant<bool, Val>;

// true / false type
using TrueType  = BoolConstant<true>;
using FalseType = BoolConstant<false>;

// identity
template<class Ty>
struct TypeIdentity { using Type = Ty; };

template<class Ty>
using TypeIdentity_t = typename TypeIdentity<Ty>::Type;

// remove const
template<class Ty>                             // remove top-level const qualifier
struct RemoveConst { using Type = Ty; };

template<class Ty>
struct RemoveConst<const Ty> { using Type = Ty; };

template<class Ty>
using RemoveConst_t = typename RemoveConst<Ty>::Type;

// remove volatile
template<class Ty>                             // remove top-level volatile qualifier
struct RemoveVolatile { using Type = Ty; };

template<class Ty>
struct RemoveVolatile<volatile Ty> { using Type = Ty; };

template<class Ty>
using RemoveVolatile_t = typename RemoveVolatile<Ty>::Type;

// remove const volatile
template<class Ty>
struct RemoveCV                                // remove top-level const and volatile qualifiers
{
    using Type = Ty;

    template<template<class> class Fn>
    using Apply = Fn<Ty>;                      // apply cv-qualifiers from the class template argument to Fn<Ty>
};

template<class Ty>
struct RemoveCV<const Ty>
{
    using Type = Ty;

    template<template<class> class Fn>
    using Apply = const Fn<Ty>;
};

template<class Ty>
struct RemoveCV<volatile Ty>
{
    using Type = Ty;

    template<template<class> class Fn>
    using Apply = volatile Fn<Ty>;
};

template<class Ty>
struct RemoveCV<const volatile Ty>
{
    using Type = Ty;

    template<template<class> class Fn>
    using Apply = const volatile Fn<Ty>;
};

template<class Ty>
using RemoveCV_t = typename RemoveCV<Ty>::Type;

// remove const volatile and ref
template<class Ty>
struct RemoveCVRef : RemoveCV<Ty> {};

template<class Ty>
struct RemoveCVRef<Ty&> : RemoveCV<Ty> {};

template<class Ty>
struct RemoveCVRef<Ty&&> : RemoveCV<Ty> {};

template<class Ty>
using RemoveCVRef_t = typename RemoveCVRef<Ty>::Type;

// remove reference
template<class Ty>
struct RemoveReference { using Type = Ty; };

template<class Ty>
struct RemoveReference<Ty&> { using Type = Ty; };

template<class Ty>
struct RemoveReference<Ty&&> { using Type = Ty; };

template<class Ty>
using RemoveReference_t = typename RemoveReference<Ty>::Type;

// remove pointer
template<class Ty>
struct RemovePointer { using Type = Ty; };

template<class Ty>
struct RemovePointer<Ty*> { using Type = Ty; };

template<class Ty>
struct RemovePointer<Ty* const> { using Type = Ty; };

template<class Ty>
struct RemovePointer<Ty* volatile> { using Type = Ty; };

template<class Ty>
struct RemovePointer<Ty* const volatile> { using Type = Ty; };

template<class Ty>
using RemovePointer_t = typename RemovePointer<Ty>::Type;

// remove extent
template<class Ty>                              // remove array extent
struct RemoveExtent { using Type = Ty; };

template<class Ty, size_t Size>
struct RemoveExtent<Ty[Size]> { using Type = Ty; };

template<class Ty>
struct RemoveExtent<Ty[]> { using Type = Ty; };

template<class Ty>
using RemoveExtent_t = typename RemoveExtent<Ty>::Type;

// is same
template<class, class>                          // determine whether arguments are the same type
constexpr bool IsSame_v = false;

template<class Ty>
constexpr bool IsSame_v<Ty, Ty> = true;

template<class Ty1, class Ty2>
struct IsSame : BoolConstant<IsSame_v<Ty1, Ty2>> {};

// is lvalue reference
template<class>                                 // determine whether type argument is an lvalue reference
constexpr bool IsLvalueReference_v = false;

template<class Ty>
constexpr bool IsLvalueReference_v<Ty&> = true;

template<class Ty>
struct IsLvalueReference : BoolConstant<IsLvalueReference_v<Ty>> {};

// is rvalue reference
template<class>                                 // determine whether type argument is an rvalue reference
constexpr bool IsRvalueReference_v = false;

template<class Ty>
constexpr bool IsRvalueReference_v<Ty&&> = true;

template<class Ty>
struct IsRvalueReference : BoolConstant<IsRvalueReference_v<Ty>> {};

// is reference
template<class>                                 // determine whether type argument is a reference
constexpr bool IsReference_v = false;

template<class Ty>
constexpr bool IsReference_v<Ty&> = true;

template<class Ty>
constexpr bool IsReference_v<Ty&&> = true;

template<class Ty>
struct IsReference : BoolConstant<IsReference_v<Ty>> {};

// is pointer
template<class>                                 // determine whether Ty is a pointer
constexpr bool IsPointer_v = false;

template<class Ty>
constexpr bool IsPointer_v<Ty*> = true;

template<class Ty>
constexpr bool IsPointer_v<Ty* const> = true;

template<class Ty>
constexpr bool IsPointer_v<Ty* volatile> = true;

template<class Ty>
constexpr bool IsPointer_v<Ty* const volatile> = true;

template<class Ty>
struct IsPointer : BoolConstant<IsPointer_v<Ty>> {};

// is void
template<class Ty>
constexpr bool IsVoid_v = IsSame_v<RemoveCV_t<Ty>, void>;

template<class Ty>
struct IsVoid : BoolConstant<IsVoid_v<Ty>> {};

template<class... Types>
using Void_t = void;

// is base of
template<class Base, class Derived>
struct IsBaseOf : BoolConstant<__is_base_of(Base, Derived)> {};

template<class Base, class Derived>
constexpr bool IsBaseOf_v = IsBaseOf<Base, Derived>::Value;

// add reference
template<class Ty, class = void>
struct _AddReference                    // add reference (non-referenceable type)
{
    using Lvalue = Ty;
    using Rvalue = Ty;
};

template<class Ty>
struct _AddReference<Ty, Void_t<Ty&>>   // (referenceable type)
{
    using Lvalue = Ty&;
    using Rvalue = Ty&&;
};

template<class Ty>
struct AddLvalueReference { using Type = typename _AddReference<Ty>::Lvalue; };

template<class Ty>
using AddLvalueReference_t = typename AddLvalueReference<Ty>::Type;

template<class Ty>
struct AddRvalueReference { using Type = typename _AddReference<Ty>::Rvalue; };

template<class Ty>
using AddRvalueReference_t = typename AddRvalueReference<Ty>::Type;

// is const
template<class>                                 // determine whether type argument is const qualified
constexpr bool IsConst_v = false;

template<class Ty>
constexpr bool IsConst_v<const Ty> = true;

template<class Ty>
struct IsConst : BoolConstant<IsConst_v<Ty>> {};

// is function
template<class Ty>                              // only function types and reference types can't be const qualified
constexpr bool IsFunction_v = !IsConst_v<const Ty> && !IsReference_v<Ty>;

template<class Ty>
struct IsFunction : BoolConstant<IsFunction_v<Ty>> {};

// is object
template<class Ty>                            // only function types and reference types can't be const qualified
constexpr bool IsObject_v = IsConst_v<const Ty> && !IsVoid_v<Ty>;

template<class Ty>
struct IsObject : BoolConstant<IsObject_v<Ty>> {};

// is array
template<class>                                 // determine whether type argument is an array
constexpr bool IsArray_v = false;

template<class Ty, size_t Size>
constexpr bool IsArray_v<Ty[Size]> = true;

template<class Ty>
constexpr bool IsArray_v<Ty[]> = true;

template<class Ty>
struct IsArray : BoolConstant<IsArray_v<Ty>> {};

// is specialization
template<class Type, template<class...> class Template>
constexpr bool IsSpecialization_v = false; // true if and only if Type is a specialization of Template

template<template<class...> class Template, class... Types>
constexpr bool IsSpecialization_v<Template<Types...>, Template> = true;

template <class Type, template<class...> class Template>
struct IsSpecialization : BoolConstant<IsSpecialization_v<Type, Template>> {};

// is member object pointer
template<class>
struct _IsMemberObjectPointer
{
    static constexpr bool Value = false;
};

template<class Ty1, class Ty2>
struct _IsMemberObjectPointer<Ty1 Ty2::*>
{
    static constexpr bool Value = !IsFunction_v<Ty1>;
    using ClassType = Ty2;
};

template<class Ty>
constexpr bool IsMemberObjectPointer_v = _IsMemberObjectPointer<RemoveCV_t<Ty>>::Value;

template<class Ty>
struct IsMemberObjectPointer : BoolConstant<IsMemberObjectPointer_v<Ty>> {};

// is member function pointer
template<class>
struct _IsMemberFunctionPointer
{
    static constexpr bool Value = false;
};

template<class Ty1, class Ty2>
struct _IsMemberFunctionPointer<Ty1 Ty2::*>
{
    static constexpr bool Value = IsFunction_v<Ty1>;
    using ClassType = Ty2;
};

template<class Ty>
constexpr bool IsMemberFunctionPointer_v = _IsMemberFunctionPointer<RemoveCV_t<Ty>>::Value;

template<class Ty>
struct IsMemberFunctionPointer : BoolConstant<IsMemberFunctionPointer_v<Ty>> {};

// is constructible
template<class Ty, class... Args>
struct IsConstructible : BoolConstant<__is_constructible(Ty, Args...)> {};

template<class Ty, class... Args>
constexpr bool IsConstructible_v = IsConstructible<Ty, Args...>::Value;

template<class Ty>
struct IsDefaultConstructible : BoolConstant<__is_constructible(Ty)> {};

template<class Ty>
constexpr bool IsDefaultConstructible_v = IsDefaultConstructible<Ty>::Value;

template<class Ty>
struct IsCopyConstructible : BoolConstant<__is_constructible(Ty, AddLvalueReference_t<const Ty>)> {};

template<class Ty>
constexpr bool IsCopyConstructible_v = IsCopyConstructible<Ty>::Value;

template<class Ty>
struct IsMoveConstructible : BoolConstant<__is_constructible(Ty, Ty)> {};

template<class Ty>
constexpr bool IsMoveConstructible_v = IsMoveConstructible<Ty>::Value;

// is assignable
template<class To, class From>
struct IsAssignable : BoolConstant<__is_assignable(To, From)> {};

template<class To, class From>
constexpr bool IsAssignable_v = IsAssignable<To, From>::Value;

template<class Ty>
struct IsCopyAssignable : BoolConstant<__is_assignable(AddLvalueReference_t<Ty>, AddLvalueReference_t<const Ty>)> {};

template<class Ty>
constexpr bool IsCopyAssignable_v = IsCopyAssignable<Ty>::Value;

template<class Ty>
struct IsMoveAssignable : BoolConstant<__is_assignable(AddLvalueReference_t<Ty>, Ty)> {};

template <class Ty>
constexpr bool IsMoveAssignable_v = IsMoveAssignable<Ty>::Value;

// is convertible
#if defined _MSC_VER
template<class From, class To>
constexpr bool IsConvertible_v = __is_convertible_to(From, To);

template<class From, class To>
struct IsConvertible : BoolConstant<IsConvertible_v<From, To>> {};
#elif defined __GNUG__
template<class From, class To>
struct _IsConvertible           // TODO: check
{
    template<class Ty>
    static auto test_returnable(int) -> decltype(void(static_cast<Ty(*)()>(nullptr)), TrueType{});

    template<class>
    static auto test_returnable(...) -> FalseType;

    template<class _From, class _To>
    static auto test_implicitly_convertible(int) -> decltype(void(std::declval<void(&)(_To)>()(std::declval<_From>())), TrueType{});

    template<class, class>
    static auto test_implicitly_convertible(...) -> FalseType;

    static constexpr bool Value = ((decltype(test_returnable<To>(0))::Value &&
                                    decltype(test_implicitly_convertible<From, To>(0))::Value) ||
                                    (IsVoid_v<From> && IsVoid_v<To>));
};

template<class From, class To>
struct IsConvertible : public BoolConstant<_IsConvertible<From, To>::Value> {};

template<class From, class To>
constexpr bool IsConvertible_v = IsConvertible<From, To>::Value;
#endif

// add pointer
template<class Ty, class = void>                // add pointer (pointer type cannot be formed)
struct AddPointer { using Type = Ty; };

template<class Ty>                              // (pointer type can be formed)
struct AddPointer<Ty, Void_t<RemoveReference_t<Ty>*>> { using Type = RemoveReference_t<Ty>*; };

template<class Ty>
using AddPointer_t = typename AddPointer<Ty>::Type;

// select
template<bool>
struct Select                                   // Select between aliases that extract either their first or second parameter
{
    template<class Ty1, class>
    using Apply = Ty1;
};

template<>
struct Select<false> 
{
    template<class, class Ty2>
    using Apply = Ty2;
};

// decay
template<class Ty>
struct Decay                                    // determines decayed version of Ty
{
    using Ty1   = RemoveReference_t<Ty>;
    using Ty2   = typename Select<IsFunction_v<Ty1>>::template Apply<AddPointer<Ty1>, RemoveCV<Ty1>>;
    using Type  = typename Select<IsArray_v<Ty1>>::template Apply<AddPointer<RemoveExtent_t<Ty1>>, Ty2>::Type;
};

template<class Ty>
using Decay_t = typename Decay<Ty>::Type;

// enable if
template<bool Test, class Ty = void>
struct EnableIf {};                             // no member "Type" when !Test

template<class Ty>                              // Type is Ty for Test
struct EnableIf<true, Ty> { using Type = Ty; };

template<bool Test, class Ty = void>
using EnableIf_t = typename EnableIf<Test, Ty>::Type;

// conditional
template<bool Test, class Ty1, class Ty2>       // Choose Ty1 if Test is true, and Ty2 otherwise
struct Conditional { using Type = Ty1; };

template<class Ty1, class Ty2>
struct Conditional<false, Ty1, Ty2> { using Type = Ty2; };

template<bool Test, class Ty1, class Ty2>
using Conditional_t = typename Conditional<Test, Ty1, Ty2>::Type;

// negation
template<class Trait>
struct Negation : BoolConstant<!static_cast<bool>(Trait::Value)> {}; // The negated result of Trait

template<class Trait>
constexpr bool Negation_v = Negation<Trait>::Value;

// conjunction
template<bool FirstValue, class First, class... Rest>
struct _Conjunction { using Type = First; };        // handle false trait or last trait

template<class True, class Next, class... Rest>     // the first trait is true, try the next one
struct _Conjunction<true, True, Next, Rest...> { using Type = typename _Conjunction<Next::Value, Next, Rest...>::Type; };

template<class... Traits>
struct Conjunction : TrueType {};                   // If Traits is empty, true type

template<class First, class... Rest>                // the first false trait in Traits, or the last trait if none are false
struct Conjunction<First, Rest...> : _Conjunction<First::Value, First, Rest...>::Type {};

template<class... Traits>
constexpr bool Conjunction_v = Conjunction<Traits...>::Value;

// Class template IntegerSequence
template<class Ty, Ty... Vals>
struct IntegerSequence
{
    using ValueType = Ty;
    static constexpr size_t size() noexcept { return sizeof...(Vals); }
};

// Alias helpers for IntegerSequence
template<size_t... Vals>
using IndexSequence = IntegerSequence<size_t, Vals...>;

template<class Ty, Ty Size>
#if defined _MSC_VER
using MakeIntegerSequence = __make_integer_seq<IntegerSequence, Ty, Size>;
#elif defined __GNUG__
using MakeIntegerSequence = IntegerSequence<Ty, __integer_pack(Size)...>;
#else
#error No "MakeIntegerSequence" implementation
#endif  // MakeIntegerSequence

template<size_t Size>
using MakeIndexSequence = MakeIntegerSequence<size_t, Size>;

template<class... Types>
using IndexSequenceFor = MakeIndexSequence<sizeof...(Types)>;
// END Alias helpers for IntegerSequence

// move
template<class Ty>
constexpr RemoveReference_t<Ty>&& move(Ty&& val) noexcept { 
    return static_cast<RemoveReference_t<Ty>&&>(val);
}

// forward
template<class Ty>
constexpr Ty&& forward(RemoveReference_t<Ty>& val) noexcept {
    return static_cast<Ty&&>(val);
}

template<class Ty>
constexpr Ty&& forward(RemoveReference_t<Ty>&& val) noexcept {
    static_assert(!IsLvalueReference_v<Ty>, "bad forward call");
    return static_cast<Ty&&>(val);
}

template<class Type>
class ReferenceWrapper;                         // TODO: implement

template<class Iterator>
class ReverseIterator                           // Adaptor for backwards iteration
{
public:
    using ValueType     = typename Iterator::ValueType;
    using Reference		= typename Iterator::Reference;
	using Pointer		= typename Iterator::Pointer;

    Iterator _Base;

public:

    explicit ReverseIterator(const Iterator& base)
        : _Base(base) { /*Empty*/ }

    ReverseIterator& operator++() {
        --_Base;
        return *this;
    }

    ReverseIterator operator++(int) {
        ReverseIterator temp = *this;
        --_Base;
        return temp;
    }

    ReverseIterator& operator--() {
        ++_Base;
        return *this;
    }

    ReverseIterator operator--(int) {
        ReverseIterator temp = *this;
        ++_Base;
        return temp;
    }

    Pointer operator->() {
        Iterator temp = _Base;
        return (--temp).operator->();
    }

    Reference operator*() {
        Iterator temp = _Base;
        return *(--temp);
    }

    bool operator==(const ReverseIterator& other) const {
        return _Base == other._Base;
    }

    bool operator!=(const ReverseIterator& other) const {
	    return !(*this == other);
    }
}; // END ReverseIterator

CUSTOM_END