#pragma once
#include "Utility.h"


CUSTOM_BEGIN

// Pair prototype
template<class First, class Second>
class Pair;

// tag type for construction (from one arg per element)
struct TupleArgs_t { explicit TupleArgs_t() = default; };

// tag type for construction (from unpacking a tuple/pair)
struct TupleUnpack_t { explicit TupleUnpack_t() = default; };

// tuple prototype
template<class... Types>
class Tuple;

// tuple size
template<class _Tuple>
struct TupleSize;

template<class... Elements>
struct TupleSize<Tuple<Elements...>> : IntegralConstant<size_t, sizeof...(Elements)> {};

template<class _Tuple>
constexpr size_t TupleSize_v = TupleSize<_Tuple>::Value;

template<class _Tuple, class NoCV_t = RemoveCV_t<_Tuple>, class = EnableIf_t<IsSame_v<_Tuple, NoCV_t>>, size_t = TupleSize_v<_Tuple>>
using _EnableTupleSize = _Tuple;

template<class _Tuple>
struct TupleSize<const _EnableTupleSize<_Tuple>> : public TupleSize<_Tuple> {};

template<class _Tuple>
struct TupleSize<volatile _EnableTupleSize<_Tuple>> : public TupleSize<_Tuple> {};

template<class _Tuple>
struct TupleSize<const volatile _EnableTupleSize<_Tuple>> : public TupleSize<_Tuple> {};

// tuple cond explicit
// template<bool SameSize, class _Tuple, class... Args>
// constexpr bool _TupleConditionalExplicit_v = false;

// template<class... Types, class... Args>
// constexpr bool _TupleConditionalExplicit_v<true, Tuple<Types...>, Args...> = !Conjunction_v<IsConvertible<Types, Args>...>;

// template<class _Tuple, class... Args>
// constexpr bool TupleConditionalExplicit_v = _TupleConditionalExplicit_v<TupleSize_v<_Tuple> == sizeof...(Args), _Tuple, Args...>;

// tuple constructible
template<bool SameSize, class _Tuple, class... Args>
constexpr bool _TupleConstructible_v = false;

template<class... Types, class... Args>						// Each type component from tuple (Tuple<Types...>) is constructible with the corresponding argument (copy/move obj)
constexpr bool _TupleConstructible_v<true, Tuple<Types...>, Args...> = Conjunction_v<IsConstructible<Types, Args>...>;

template<class _Tuple, class... Args>						// Check tuple size with given construction arguments then above
constexpr bool TupleConstructible_v = _TupleConstructible_v<TupleSize_v<_Tuple> == sizeof...(Args), _Tuple, Args...>;

template<class _Tuple, class... Args>
struct TupleConstructible : BoolConstant<TupleConstructible_v<_Tuple, Args...>> {};

// tuple assignable
template<bool SameSize, class _Tuple, class... Args>
constexpr bool _TupleAssignable_v = false;

template<class... Types, class... Args>
constexpr bool _TupleAssignable_v<true, Tuple<Types...>, Args...> = Conjunction_v<IsAssignable<Types&, Args>...>; // note Types& instead of Types

template<class _Tuple, class... Args>
constexpr bool TupleAssignable_v = _TupleAssignable_v<TupleSize_v<_Tuple> == sizeof...(Args), _Tuple, Args...>;

template<class _Tuple, class... Args>
struct TupleAssignable : BoolConstant<TupleAssignable_v<_Tuple, Args...>> {};

// tuple convert
template<class ThisTuple, class OtherTuple, class... OtherTypes>
struct TupleConvert : TrueType {};

template<class This, class OtherTuple, class OtherType>
struct TupleConvert<Tuple<This>, OtherTuple, OtherType>
	: BoolConstant<!Disjunction_v<	IsSame<This, OtherType>,
									IsConstructible<This, OtherTuple>,
									IsConvertible<OtherTuple, This>>> {};

// tuple element
template<size_t Index, class _Tuple>
struct TupleElement;                                        // TupleElement prototype for accessing Tuple members

template<size_t Index, class This, class... Rest>			// Recursive TupleElement implementation
struct TupleElement<Index, Tuple<This, Rest...>> : public TupleElement<Index - 1, Tuple<Rest...>> {};

template<class This, class... Rest>
struct TupleElement<0, Tuple<This, Rest...>>				// Default TupleElement implementation
{
	using Type 		= This;
	using TupleType = Tuple<This, Rest...>;
};

template<size_t Index, class _Tuple>
using TupleElement_t = typename TupleElement<Index, _Tuple>::Type;

template<size_t Index, class _Tuple>
using TupleElement_tt = typename TupleElement<Index, _Tuple>::TupleType;

// tuple cat helper
template<class Type, class KxArg, class IxArg, size_t IxNext, class... Sequences>
struct TupleCat;

template<class Type, size_t... Kx, size_t... Ix, size_t IxNext>
struct TupleCat<Type, IndexSequence<Kx...>, IndexSequence<Ix...>, IxNext>
{
	using RetType	= Tuple<TupleElement_t<Kx, RemoveCVRef_t<TupleElement_t<Ix, Type>>>...>;
	using KxSeq		= IndexSequence<Kx...>;
	using IxSeq		= IndexSequence<Ix...>;
};

template<class Type, size_t... Kx, size_t... Ix, size_t IxNext, size_t... KxNext, class... Rest>
struct TupleCat<Type, IndexSequence<Kx...>, IndexSequence<Ix...>, IxNext, IndexSequence<KxNext...>, Rest...>
	: TupleCat<Type, IndexSequence<Kx..., KxNext...>, IndexSequence<Ix..., (IxNext + 0 * KxNext)...>, // repeat IxNext, ignoring the elements of KxNext
	IxNext + 1, Rest...> {};


template<>
class Tuple<>												// Default Tuple implementation
{
public:
	// Constructors

	template<class Tag, EnableIf_t<IsSame_v<Tag, TupleArgs_t>, bool> = true>
	Tuple(Tag) { /*Empty*/ }

	Tuple()				= default;
	Tuple(const Tuple&) = default;
	Tuple(Tuple&&)		= default;
	~Tuple()			= default;

public:
	// Operators

	Tuple& operator=(const Tuple&) = default;
};


template<class This, class... Rest>
class Tuple<This, Rest...> : public Tuple<Rest...>			// Recursive Tuple implementation
{
public:
	using ThisType	= This;
	using Base		= Tuple<Rest...>;

	ThisType First;											// Data stored to this iteration

public:
	// Construction Helpers

	// (H1) Helper for (1), (H2)
	template<class Tag, class _This, class... _Rest, EnableIf_t<IsSame_v<Tag, TupleArgs_t>, bool> = true>
	Tuple(Tag, _This&& first, _Rest&&...rest)
		: Base(TupleArgs_t{}, custom::forward<_Rest>(rest)...), First(custom::forward<_This>(first)) { /*Empty*/ }
	
	// (H2) Helper for (H3)
	template<class Tag, class _Tuple, size_t... Indices, EnableIf_t<IsSame_v<Tag, TupleUnpack_t>, bool> = true>
	Tuple(Tag, _Tuple&& other, IndexSequence<Indices...>);		// defined after custom::get()

	// (H3) Helper for (2), (3)
	template<class Tag, class _Tuple, EnableIf_t<IsSame_v<Tag, TupleUnpack_t>, bool> = true>
	Tuple(Tag, _Tuple&& other)
		: Tuple(TupleUnpack_t{}, custom::forward<_Tuple>(other),
				MakeIndexSequence<TupleSize_v<RemoveReference_t<_Tuple>>>{}) { /*Empty*/ }	// Uses (H2)

public:
	// Constructors

	// (0) Default constructor
	template<class _This = This,
	EnableIf_t<Conjunction_v<IsDefaultConstructible<_This>, IsDefaultConstructible<Rest>...>, bool> = true>
	Tuple()
		: Base(), First() { /*Empty*/ }

	// (1) Copy/Move obj constructor
	template<class _This = This, class... _Rest,
	EnableIf_t<TupleConstructible_v<Tuple, _This, _Rest...>, bool> = true>
	Tuple(_This&& first, _Rest&&... rest)
		: Tuple(TupleArgs_t{}, custom::forward<_This>(first), custom::forward<_Rest>(rest)...) { /*Empty*/ }	// Uses (H1)

	// (2) Copy convertible constructor
	template<class... OtherTypes,
	EnableIf_t<Conjunction_v<	TupleConstructible<Tuple, const OtherTypes&...>,
								TupleConvert<Tuple, const Tuple<OtherTypes...>&, OtherTypes...>>, bool> = true>
	Tuple(const Tuple<OtherTypes...>& other)
		: Tuple(TupleUnpack_t{}, other) { /*Empty*/ }	// Uses (H3)

	// (3) Move convertible constructor
	template<class... OtherTypes,
	EnableIf_t<Conjunction_v<	TupleConstructible<Tuple, OtherTypes...>,
								TupleConvert<Tuple, Tuple<OtherTypes...>, OtherTypes...>>, bool> = true>
	Tuple(Tuple<OtherTypes...>&& other)
		: Tuple(TupleUnpack_t{}, custom::move(other)) { /*Empty*/ }		// Uses (H3)

	// TODO: check
	// // (4) Pair copy constructor
	// template<class First, class Second,
    // EnableIf_t<TupleConstructible_v<Tuple, const First&, const Second&>, bool> = true>
    // Tuple(const Pair<First, Second>& other)
    //     : Tuple(TupleUnpack_t{}, other) { /*Empty*/ }	// Uses (2)

	// // (5) Pair move constructor
    // template<class First, class Second,
	// EnableIf_t<TupleConstructible_v<Tuple, First, Second>, bool> = true>
	// Tuple(Pair<First, Second>&& other)
    //     : Tuple(TupleUnpack_t{}, custom::move(other)) { /*Empty*/ }		// Uses (3)

	Tuple(const Tuple&) = default;
	Tuple(Tuple&&)		= default;
    ~Tuple()            = default;

public:
	// Operators

	// TODO: complete
};

template<class... Types>
Tuple(Types...) -> Tuple<Types...>;

template<class First, class Second>
Tuple(Pair<First, Second>) -> Tuple<First, Second>;


// get
template<int Index, class... Types>
TupleElement_t<Index, Tuple<Types...>>& get(Tuple<Types...>& tuple) {					// Function to get Tuple member from reference
	using TupleType = TupleElement_tt<Index, Tuple<Types...>>;

	return static_cast<TupleType&>(tuple).First;
}

template<int Index, class... Types>
const TupleElement_t<Index, Tuple<Types...>>& get(const Tuple<Types...>& tuple) {		// Same but const
	using TupleType = TupleElement_tt<Index, Tuple<Types...>>;

	return static_cast<const TupleType&>(tuple).First;
}

template<int Index, class... Types>
TupleElement_t<Index, Tuple<Types...>>&& get(Tuple<Types...>&& tuple) {					// Function to get Tuple member from rval
	using Type		= TupleElement_t<Index, Tuple<Types...>>;
	using TupleType = TupleElement_tt<Index, Tuple<Types...>>;

	return static_cast<Type&&>(static_cast<TupleType&>(tuple).First);
}

template<int Index, class... Types>
const TupleElement_t<Index, Tuple<Types...>>&& get(const Tuple<Types...>&& tuple) {		// Same but const
	using Type		= TupleElement_t<Index, Tuple<Types...>>;
	using TupleType = TupleElement_tt<Index, Tuple<Types...>>;

	return static_cast<const Type&&>(static_cast<const TupleType&>(tuple).First);
}

// (H2) Constructor helper defined here because of custom::get()
template<class This, class... Rest>
template<class Tag, class _Tuple, size_t... Indices, EnableIf_t<IsSame_v<Tag, TupleUnpack_t>, bool>>
Tuple<This, Rest...>::Tuple(Tag, _Tuple&& other, IndexSequence<Indices...>)
	: Tuple(TupleArgs_t{}, custom::get<Indices>(custom::forward<_Tuple>(other))...) { /*Empty*/ }	// Uses (H1)

// forward as tuple
template<class... Types>
Tuple<Types&&...> forward_as_tuple(Types&&... args) {									// Forward arguments in a tuple
	return Tuple<Types&&...>(custom::forward<Types>(args)...);
}

// apply
template<class Callable, class _Tuple, size_t... Indices>
decltype(auto) _apply_impl(Callable&& func, _Tuple&& tuple, IndexSequence<Indices...>) noexcept {
    return custom::invoke(custom::forward<Callable>(func), custom::get<Indices>(custom::forward<_Tuple>(tuple))...);
}

template<class Callable, class _Tuple>
decltype(auto) apply(Callable&& func, _Tuple&& tuple) noexcept {						// Invoke Callable obj with args as Tuple
    return _apply_impl(custom::forward<Callable>(func), custom::forward<_Tuple>(tuple), MakeIndexSequence<TupleSize_v<RemoveReference_t<_Tuple>>>{});
}

// make tuple / tie
template<class... Args>
Tuple<UnRefWrap_t<Args>...> make_tuple(Args&&... args) {								// Create Tuple with args
	return Tuple<UnRefWrap_t<Args>...>(custom::forward<Args>(args)...);
}

template<class... Args>
Tuple<Args&...> tie(Args&... args) noexcept {											// Create Tuple with references of args
	return Tuple<Args&...>(args...);
}

// make from tuple
template<class Type, class _Tuple, size_t... Indices>
Type _make_from_tuple_impl(_Tuple&& tuple, IndexSequence<Indices...>) {
	static_assert(IsConstructible_v<Type, decltype(custom::get<Indices>(custom::forward<_Tuple>(tuple)))...>, "The target type must be constructible using the arguments in tuple.");
	return Type(custom::get<Indices>(custom::forward<_Tuple>(tuple))...);
}

template<class Type, class _Tuple>
Type make_from_tuple(_Tuple&& tuple) {													// construct Type from the elements of tuple
	return _make_from_tuple_impl<Type>(custom::forward<_Tuple>(tuple), MakeIndexSequence<TupleSize_v<RemoveReference_t<_Tuple>>>{});
}

// tuple cat
template<class RetType, size_t... Kx, size_t... Ix, class _Tuple>
RetType _tuple_cat_impl(IndexSequence<Kx...>, IndexSequence<Ix...>, _Tuple tuple) {		// tuple as copy
	return RetType(custom::get<Kx>(custom::get<Ix>(custom::move(tuple)))...);
}

template<class... Tuples>																// concatenate tuples
typename TupleCat<Tuple<Tuples&&...>, IndexSequence<>, IndexSequence<>, 0,
					MakeIndexSequence<TupleSize_v<RemoveCVRef_t<Tuples>>>...>::RetType tuple_cat(Tuples&&... tuples) {
	
	using Cat		= TupleCat<Tuple<Tuples&&...>, IndexSequence<>, IndexSequence<>, 0,
								MakeIndexSequence<TupleSize_v<RemoveCVRef_t<Tuples>>>...>;
	using RetType	= typename Cat::RetType;
	using KxSeq		= typename Cat::KxSeq;
	using IxSeq		= typename Cat::IxSeq;

	return _tuple_cat_impl<RetType>(KxSeq{}, IxSeq{}, custom::forward_as_tuple(custom::forward<Tuples>(tuples)...));
}

CUSTOM_END