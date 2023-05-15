// tuple standard header (core)

// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once
#ifndef _TUPLE_
#define _TUPLE_
#include <yvals_core.h>
#if _STL_COMPILER_PREPROCESSOR
#ifdef __cpp_lib_concepts
#include <compare>
#endif // __cpp_lib_concepts
#include <__msvc_iter_core.hpp>
#include <type_traits>

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_STD_BEGIN
template <bool _Same, class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_conditional_explicit_v0 = false;

template <class... _Dests, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_conditional_explicit_v0<true, tuple<_Dests...>, _Srcs...> =
    !conjunction_v<is_convertible<_Srcs, _Dests>...>;

template <class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_conditional_explicit_v =
    _Tuple_conditional_explicit_v0<tuple_size_v<_Dest> == sizeof...(_Srcs), _Dest, _Srcs...>;

template <bool _Same, class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_constructible_v0 = false;

template <class... _Dests, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_constructible_v0<true, tuple<_Dests...>, _Srcs...> =
    conjunction_v<is_constructible<_Dests, _Srcs>...>;

template <class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_constructible_v =
    _Tuple_constructible_v0<tuple_size_v<_Dest> == sizeof...(_Srcs), _Dest, _Srcs...>;

template <class _Dest, class... _Srcs>
struct _Tuple_constructible_val : bool_constant<_Tuple_constructible_v<_Dest, _Srcs...>> {};

template <bool _Same, class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_nothrow_constructible_v0 = false;

template <class... _Dests, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_nothrow_constructible_v0<true, tuple<_Dests...>, _Srcs...> =
    conjunction_v<is_nothrow_constructible<_Dests, _Srcs>...>;

template <class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_nothrow_constructible_v =
    _Tuple_nothrow_constructible_v0<tuple_size_v<_Dest> == sizeof...(_Srcs), _Dest, _Srcs...>;

template <bool _Same, class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_assignable_v0 = false;

template <class... _Dests, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_assignable_v0<true, tuple<_Dests...>, _Srcs...> =
    conjunction_v<is_assignable<_Dests&, _Srcs>...>; // note _Dests& instead of _Dests

#if _HAS_CXX23
template <class... _Dests, class... _Srcs>
inline constexpr bool _Tuple_assignable_v0<true, const tuple<_Dests...>, _Srcs...> =
    conjunction_v<is_assignable<const _Dests&, _Srcs>...>;
#endif // _HAS_CXX23

template <class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_assignable_v =
    _Tuple_assignable_v0<tuple_size_v<_Dest> == sizeof...(_Srcs), _Dest, _Srcs...>;

template <class _Dest, class... _Srcs>
struct _Tuple_assignable_val : bool_constant<_Tuple_assignable_v<_Dest, _Srcs...>> {};

template <bool _Same, class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_nothrow_assignable_v0 = false;

template <class... _Dests, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_nothrow_assignable_v0<true, tuple<_Dests...>, _Srcs...> =
    conjunction_v<is_nothrow_assignable<_Dests&, _Srcs>...>; // note _Dests& instead of _Dests

#if _HAS_CXX23
template <class... _Dests, class... _Srcs>
inline constexpr bool _Tuple_nothrow_assignable_v0<true, const tuple<_Dests...>, _Srcs...> =
    conjunction_v<is_nothrow_assignable<const _Dests&, _Srcs>...>;
#endif // _HAS_CXX23

template <class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_nothrow_assignable_v =
    _Tuple_nothrow_assignable_v0<tuple_size_v<_Dest> == sizeof...(_Srcs), _Dest, _Srcs...>;

// Constrain tuple's converting constructors
template <class _Myself, class _OtherTuple, class... _Other>
struct _Tuple_convert_val : true_type {};

template <class _This, class _OtherTuple, class _Uty>
struct _Tuple_convert_val<tuple<_This>, _OtherTuple, _Uty>
    : bool_constant<!disjunction_v<is_same<_This, _Uty>, is_constructible<_This, _OtherTuple>,
          is_convertible<_OtherTuple, _This>>> {};

// Constrain tuple's perfect forwarding constructor (LWG-3121)
template <class _Myself, class _This2, class... _Rest2>
struct _Tuple_perfect_val : true_type {};

template <class _Myself, class _This2>
struct _Tuple_perfect_val<_Myself, _This2> : bool_constant<!is_same_v<_Myself, _Remove_cvref_t<_This2>>> {};

template <class _Ty0, class _Ty1, class _Uty0, class _Uty1>
struct _Tuple_perfect_val<tuple<_Ty0, _Ty1>, _Uty0, _Uty1>
    : bool_constant<disjunction_v<negation<is_same<_Remove_cvref_t<_Uty0>, allocator_arg_t>>,
          is_same<_Remove_cvref_t<_Ty0>, allocator_arg_t>>> {};

template <class _Ty0, class _Ty1, class _Ty2, class _Uty0, class _Uty1, class _Uty2>
struct _Tuple_perfect_val<tuple<_Ty0, _Ty1, _Ty2>, _Uty0, _Uty1, _Uty2>
    : bool_constant<disjunction_v<negation<is_same<_Remove_cvref_t<_Uty0>, allocator_arg_t>>,
          is_same<_Remove_cvref_t<_Ty0>, allocator_arg_t>>> {};

struct _Ignore { // struct that ignores assignments
    template <class _Ty>
    constexpr const _Ignore& operator=(const _Ty&) const noexcept /* strengthened */ {
        // do nothing
        return *this;
    }
};

_EXPORT_STD _INLINE_VAR constexpr _Ignore ignore{};

// Note: To improve throughput, this file uses extra _STD qualification for names that appear in the
// arguments of enable_if_t. Specifically, we qualify names which appear anywhere in the STL as members of
// some class - including injected-class-names! - that we know are not members of the class being defined.
// This avoids pointless class-member lookup for those names in this context.

template <class _Ty>
struct _Tuple_val { // stores each value in a tuple
    constexpr _Tuple_val() : _Val() {}

    template <class _Other>
    constexpr _Tuple_val(_Other&& _Arg) : _Val(_STD forward<_Other>(_Arg)) {}

    template <class _Alloc, class... _Other, enable_if_t<!uses_allocator_v<_Ty, _Alloc>, int> = 0>
    constexpr _Tuple_val(const _Alloc&, allocator_arg_t, _Other&&... _Arg) : _Val(_STD forward<_Other>(_Arg)...) {}

    template <class _Alloc, class... _Other,
        enable_if_t<conjunction_v<_STD uses_allocator<_Ty, _Alloc>,
                        _STD is_constructible<_Ty, _STD allocator_arg_t, const _Alloc&, _Other...>>,
            int> = 0>
    constexpr _Tuple_val(const _Alloc& _Al, allocator_arg_t, _Other&&... _Arg)
        : _Val(allocator_arg, _Al, _STD forward<_Other>(_Arg)...) {}

    template <class _Alloc, class... _Other,
        enable_if_t<conjunction_v<_STD uses_allocator<_Ty, _Alloc>,
                        _STD negation<_STD is_constructible<_Ty, _STD allocator_arg_t, const _Alloc&, _Other...>>>,
            int> = 0>
    constexpr _Tuple_val(const _Alloc& _Al, allocator_arg_t, _Other&&... _Arg)
        : _Val(_STD forward<_Other>(_Arg)..., _Al) {}

    _Ty _Val;
};

struct _Exact_args_t {
    explicit _Exact_args_t() = default;
}; // tag type to disambiguate construction (from one arg per element)

struct _Unpack_tuple_t {
    explicit _Unpack_tuple_t() = default;
}; // tag type to disambiguate construction (from unpacking a tuple/pair)

struct _Alloc_exact_args_t {
    explicit _Alloc_exact_args_t() = default;
}; // tag type to disambiguate construction (from an allocator and one arg per element)

struct _Alloc_unpack_tuple_t {
    explicit _Alloc_unpack_tuple_t() = default;
}; // tag type to disambiguate construction (from an allocator and unpacking a tuple/pair)

_EXPORT_STD template <class... _Types>
class tuple;

template <>
class tuple<> { // empty tuple
public:
    constexpr tuple() noexcept = default; /* strengthened */

    constexpr tuple(const tuple&) noexcept /* strengthened */ {} // TRANSITION, ABI: should be defaulted

    template <class _Alloc>
    _CONSTEXPR20 tuple(allocator_arg_t, const _Alloc&) noexcept /* strengthened */ {}

    template <class _Alloc>
    _CONSTEXPR20 tuple(allocator_arg_t, const _Alloc&, const tuple&) noexcept /* strengthened */ {}

    template <class _Tag, enable_if_t<is_same_v<_Tag, _STD _Exact_args_t>, int> = 0>
    constexpr tuple(_Tag) noexcept /* strengthened */ {}

    template <class _Tag, class _Alloc, enable_if_t<is_same_v<_Tag, _STD _Alloc_exact_args_t>, int> = 0>
    constexpr tuple(_Tag, const _Alloc&) noexcept /* strengthened */ {}

    constexpr tuple& operator=(const tuple&) = default;
#if _HAS_CXX23
    constexpr const tuple& operator=(const tuple&) const noexcept /* strengthened */ {
        return *this;
    }
#endif // _HAS_CXX23

    _CONSTEXPR20 void swap(tuple&) noexcept {}
#if _HAS_CXX23
    constexpr void swap(const tuple&) const noexcept {}
#endif // _HAS_CXX23

    constexpr bool _Equals(const tuple&) const noexcept {
        return true;
    }

#ifdef __cpp_lib_concepts
    _NODISCARD constexpr strong_ordering _Three_way_compare(const tuple&) const noexcept {
        return strong_ordering::equal;
    }
#else // ^^^ defined(__cpp_lib_concepts) / !defined(__cpp_lib_concepts) vvv
    _NODISCARD constexpr bool _Less(const tuple&) const noexcept {
        return false;
    }
#endif // ^^^ !defined(__cpp_lib_concepts) ^^^
};

template <class _This, class... _Rest>
class tuple<_This, _Rest...> : private tuple<_Rest...> { // recursive tuple definition
public:
    using _This_type = _This;
    using _Mybase    = tuple<_Rest...>;

    template <class _Tag, class _This2, class... _Rest2, enable_if_t<is_same_v<_Tag, _STD _Exact_args_t>, int> = 0>
    constexpr tuple(_Tag, _This2&& _This_arg, _Rest2&&... _Rest_arg)
        : _Mybase(_Exact_args_t{}, _STD forward<_Rest2>(_Rest_arg)...), _Myfirst(_STD forward<_This2>(_This_arg)) {}

    template <class _Tag, class _Tpl, size_t... _Indices, enable_if_t<is_same_v<_Tag, _STD _Unpack_tuple_t>, int> = 0>
    constexpr tuple(_Tag, _Tpl&& _Right, index_sequence<_Indices...>);

    template <class _Tag, class _Tpl, enable_if_t<is_same_v<_Tag, _STD _Unpack_tuple_t>, int> = 0>
    constexpr tuple(_Tag, _Tpl&& _Right)
        : tuple(_Unpack_tuple_t{}, _STD forward<_Tpl>(_Right),
            make_index_sequence<tuple_size_v<remove_reference_t<_Tpl>>>{}) {}

    template <class _Tag, class _Alloc, class _This2, class... _Rest2,
        enable_if_t<is_same_v<_Tag, _STD _Alloc_exact_args_t>, int> = 0>
    constexpr tuple(_Tag, const _Alloc& _Al, _This2&& _This_arg, _Rest2&&... _Rest_arg)
        : _Mybase(_Alloc_exact_args_t{}, _Al, _STD forward<_Rest2>(_Rest_arg)...),
          _Myfirst(_Al, allocator_arg, _STD forward<_This2>(_This_arg)) {}

    template <class _Tag, class _Alloc, class _Tpl, size_t... _Indices,
        enable_if_t<is_same_v<_Tag, _STD _Alloc_unpack_tuple_t>, int> = 0>
    constexpr tuple(_Tag, const _Alloc& _Al, _Tpl&& _Right, index_sequence<_Indices...>);

    template <class _Tag, class _Alloc, class _Tpl, enable_if_t<is_same_v<_Tag, _STD _Alloc_unpack_tuple_t>, int> = 0>
    constexpr tuple(_Tag, const _Alloc& _Al, _Tpl&& _Right)
        : tuple(_Alloc_unpack_tuple_t{}, _Al, _STD forward<_Tpl>(_Right),
            make_index_sequence<tuple_size_v<remove_reference_t<_Tpl>>>{}) {}

    template <class _This2 = _This,
        enable_if_t<conjunction_v<_STD is_default_constructible<_This2>, _STD is_default_constructible<_Rest>...>,
            int>           = 0>
    constexpr explicit(
        !conjunction_v<_Is_implicitly_default_constructible<_This2>, _Is_implicitly_default_constructible<_Rest>...>)
        tuple() noexcept(conjunction_v<is_nothrow_default_constructible<_This2>,
            is_nothrow_default_constructible<_Rest>...>) // strengthened
        : _Mybase(), _Myfirst() {}

    template <class _This2 = _This, enable_if_t<_Tuple_constructible_v<tuple, const _This2&, const _Rest&...>, int> = 0>
    constexpr explicit(_Tuple_conditional_explicit_v<tuple, const _This2&, const _Rest&...>) tuple(
        const _This& _This_arg, const _Rest&... _Rest_arg) noexcept(conjunction_v<is_nothrow_copy_constructible<_This2>,
        is_nothrow_copy_constructible<_Rest>...>) // strengthened
        : tuple(_Exact_args_t{}, _This_arg, _Rest_arg...) {}

    template <class _This2, class... _Rest2,
        enable_if_t<conjunction_v<_STD _Tuple_perfect_val<tuple, _This2, _Rest2...>,
                        _STD _Tuple_constructible_val<tuple, _This2, _Rest2...>>,
            int> = 0>
    constexpr explicit(_Tuple_conditional_explicit_v<tuple, _This2, _Rest2...>) tuple(_This2&& _This_arg,
        _Rest2&&... _Rest_arg) noexcept(_Tuple_nothrow_constructible_v<tuple, _This2, _Rest2...>) // strengthened
        : tuple(_Exact_args_t{}, _STD forward<_This2>(_This_arg), _STD forward<_Rest2>(_Rest_arg)...) {}

    tuple(const tuple&) = default;
    tuple(tuple&&)      = default;

#if _HAS_CXX23
    template <class... _Other, enable_if_t<conjunction_v<_STD _Tuple_constructible_val<tuple, _Other&...>,
                                               _STD _Tuple_convert_val<tuple, tuple<_Other...>&, _Other...>>,
                                   int> = 0>
    constexpr explicit(_Tuple_conditional_explicit_v<tuple, _Other&...>)
        tuple(tuple<_Other...>& _Right) noexcept(_Tuple_nothrow_constructible_v<tuple, _Other&...>) // strengthened
        : tuple(_Unpack_tuple_t{}, _Right) {}
#endif // _HAS_CXX23

    template <class... _Other, enable_if_t<conjunction_v<_STD _Tuple_constructible_val<tuple, const _Other&...>,
                                               _STD _Tuple_convert_val<tuple, const tuple<_Other...>&, _Other...>>,
                                   int> = 0>
    constexpr explicit(_Tuple_conditional_explicit_v<tuple, const _Other&...>)
        tuple(const tuple<_Other...>& _Right) noexcept(
            _Tuple_nothrow_constructible_v<tuple, const _Other&...>) // strengthened
        : tuple(_Unpack_tuple_t{}, _Right) {}

    template <class... _Other, enable_if_t<conjunction_v<_STD _Tuple_constructible_val<tuple, _Other...>,
                                               _STD _Tuple_convert_val<tuple, tuple<_Other...>, _Other...>>,
                                   int> = 0>
    constexpr explicit(_Tuple_conditional_explicit_v<tuple, _Other...>)
        tuple(tuple<_Other...>&& _Right) noexcept(_Tuple_nothrow_constructible_v<tuple, _Other...>) // strengthened
        : tuple(_Unpack_tuple_t{}, _STD move(_Right)) {}

#if _HAS_CXX23
    template <class... _Other, enable_if_t<conjunction_v<_STD _Tuple_constructible_val<tuple, const _Other...>,
                                               _STD _Tuple_convert_val<tuple, const tuple<_Other...>, _Other...>>,
                                   int> = 0>
    constexpr explicit(_Tuple_conditional_explicit_v<tuple, const _Other...>)
        tuple(const tuple<_Other...>&& _Right) noexcept(
            _Tuple_nothrow_constructible_v<tuple, const _Other...>) // strengthened
        : tuple(_Unpack_tuple_t{}, _STD move(_Right)) {}

    template <class _First, class _Second, enable_if_t<_Tuple_constructible_v<tuple, _First&, _Second&>, int> = 0>
    constexpr explicit(_Tuple_conditional_explicit_v<tuple, _First&, _Second&>)
        tuple(pair<_First, _Second>& _Right) noexcept(
            _Tuple_nothrow_constructible_v<tuple, _First&, _Second&>) // strengthened
        : tuple(_Unpack_tuple_t{}, _Right) {}
#endif // _HAS_CXX23

    template <class _First, class _Second,
        enable_if_t<_Tuple_constructible_v<tuple, const _First&, const _Second&>, int> = 0>
    constexpr explicit(_Tuple_conditional_explicit_v<tuple, const _First&, const _Second&>)
        tuple(const pair<_First, _Second>& _Right) noexcept(
            _Tuple_nothrow_constructible_v<tuple, const _First&, const _Second&>) // strengthened
        : tuple(_Unpack_tuple_t{}, _Right) {}

    template <class _First, class _Second, enable_if_t<_Tuple_constructible_v<tuple, _First, _Second>, int> = 0>
    constexpr explicit(_Tuple_conditional_explicit_v<tuple, _First, _Second>) tuple(
        pair<_First, _Second>&& _Right) noexcept(_Tuple_nothrow_constructible_v<tuple, _First, _Second>) // strengthened
        : tuple(_Unpack_tuple_t{}, _STD move(_Right)) {}

#if _HAS_CXX23
    template <class _First, class _Second,
        enable_if_t<_Tuple_constructible_v<tuple, const _First, const _Second>, int> = 0>
    constexpr explicit(_Tuple_conditional_explicit_v<tuple, const _First, const _Second>)
        tuple(const pair<_First, _Second>&& _Right) noexcept(
            _Tuple_nothrow_constructible_v<tuple, const _First, const _Second>) // strengthened
        : tuple(_Unpack_tuple_t{}, _STD move(_Right)) {}
#endif // _HAS_CXX23

    template <class _Alloc, class _This2 = _This,
        enable_if_t<conjunction_v<_STD is_default_constructible<_This2>, _STD is_default_constructible<_Rest>...>,
            int> = 0>
    _CONSTEXPR20 explicit(
        !conjunction_v<_Is_implicitly_default_constructible<_This2>, _Is_implicitly_default_constructible<_Rest>...>)
        tuple(allocator_arg_t, const _Alloc& _Al)
        : _Mybase(allocator_arg, _Al), _Myfirst(_Al, allocator_arg) {}

    template <class _Alloc, class _This2 = _This,
        enable_if_t<_Tuple_constructible_v<tuple, const _This2&, const _Rest&...>, int> = 0>
    _CONSTEXPR20 explicit(_Tuple_conditional_explicit_v<tuple, const _This2&, const _Rest&...>)
        tuple(allocator_arg_t, const _Alloc& _Al, const _This& _This_arg, const _Rest&... _Rest_arg)
        : tuple(_Alloc_exact_args_t{}, _Al, _This_arg, _Rest_arg...) {}

    template <class _Alloc, class _This2, class... _Rest2,
        enable_if_t<conjunction_v<_STD _Tuple_perfect_val<tuple, _This2, _Rest2...>,
                        _STD _Tuple_constructible_val<tuple, _This2, _Rest2...>>,
            int> = 0>
    _CONSTEXPR20 explicit(_Tuple_conditional_explicit_v<tuple, _This2, _Rest2...>)
        tuple(allocator_arg_t, const _Alloc& _Al, _This2&& _This_arg, _Rest2&&... _Rest_arg)
        : tuple(_Alloc_exact_args_t{}, _Al, _STD forward<_This2>(_This_arg), _STD forward<_Rest2>(_Rest_arg)...) {}

    template <class _Alloc, class _This2 = _This,
        enable_if_t<_Tuple_constructible_v<tuple, const _This2&, const _Rest&...>, int> = 0>
    _CONSTEXPR20 tuple(allocator_arg_t, const _Alloc& _Al, const tuple& _Right)
        : tuple(_Alloc_unpack_tuple_t{}, _Al, _Right) {}

    template <class _Alloc, class _This2 = _This, enable_if_t<_Tuple_constructible_v<tuple, _This2, _Rest...>, int> = 0>
    _CONSTEXPR20 tuple(allocator_arg_t, const _Alloc& _Al, tuple&& _Right)
        : tuple(_Alloc_unpack_tuple_t{}, _Al, _STD move(_Right)) {}

#if _HAS_CXX23
    template <class _Alloc, class... _Other,
        enable_if_t<conjunction_v<_STD _Tuple_constructible_val<tuple, _Other&...>,
                        _STD _Tuple_convert_val<tuple, tuple<_Other...>&, _Other...>>,
            int> = 0>
    constexpr explicit(_Tuple_conditional_explicit_v<tuple, _Other&...>)
        tuple(allocator_arg_t, const _Alloc& _Al, tuple<_Other...>& _Right)
        : tuple(_Alloc_unpack_tuple_t{}, _Al, _Right) {}
#endif // _HAS_CXX23

    template <class _Alloc, class... _Other,
        enable_if_t<conjunction_v<_STD _Tuple_constructible_val<tuple, const _Other&...>,
                        _STD _Tuple_convert_val<tuple, const tuple<_Other...>&, _Other...>>,
            int> = 0>
    _CONSTEXPR20 explicit(_Tuple_conditional_explicit_v<tuple, const _Other&...>)
        tuple(allocator_arg_t, const _Alloc& _Al, const tuple<_Other...>& _Right)
        : tuple(_Alloc_unpack_tuple_t{}, _Al, _Right) {}

    template <class _Alloc, class... _Other,
        enable_if_t<conjunction_v<_STD _Tuple_constructible_val<tuple, _Other...>,
                        _STD _Tuple_convert_val<tuple, tuple<_Other...>, _Other...>>,
            int> = 0>
    _CONSTEXPR20 explicit(_Tuple_conditional_explicit_v<tuple, _Other...>)
        tuple(allocator_arg_t, const _Alloc& _Al, tuple<_Other...>&& _Right)
        : tuple(_Alloc_unpack_tuple_t{}, _Al, _STD move(_Right)) {}

#if _HAS_CXX23
    template <class _Alloc, class... _Other,
        enable_if_t<conjunction_v<_STD _Tuple_constructible_val<tuple, const _Other...>,
                        _STD _Tuple_convert_val<tuple, const tuple<_Other...>, _Other...>>,
            int> = 0>
    constexpr explicit(_Tuple_conditional_explicit_v<tuple, const _Other...>)
        tuple(allocator_arg_t, const _Alloc& _Al, const tuple<_Other...>&& _Right)
        : tuple(_Alloc_unpack_tuple_t{}, _Al, _STD move(_Right)) {}

    template <class _Alloc, class _First, class _Second,
        enable_if_t<_Tuple_constructible_v<tuple, _First&, _Second&>, int> = 0>
    constexpr explicit(_Tuple_conditional_explicit_v<tuple, _First&, _Second&>)
        tuple(allocator_arg_t, const _Alloc& _Al, pair<_First, _Second>& _Right)
        : tuple(_Alloc_unpack_tuple_t{}, _Al, _Right) {}
#endif // _HAS_CXX23

    template <class _Alloc, class _First, class _Second,
        enable_if_t<_Tuple_constructible_v<tuple, const _First&, const _Second&>, int> = 0>
    _CONSTEXPR20 explicit(_Tuple_conditional_explicit_v<tuple, const _First&, const _Second&>)
        tuple(allocator_arg_t, const _Alloc& _Al, const pair<_First, _Second>& _Right)
        : tuple(_Alloc_unpack_tuple_t{}, _Al, _Right) {}

    template <class _Alloc, class _First, class _Second,
        enable_if_t<_Tuple_constructible_v<tuple, _First, _Second>, int> = 0>
    _CONSTEXPR20 explicit(_Tuple_conditional_explicit_v<tuple, _First, _Second>)
        tuple(allocator_arg_t, const _Alloc& _Al, pair<_First, _Second>&& _Right)
        : tuple(_Alloc_unpack_tuple_t{}, _Al, _STD move(_Right)) {}

#if _HAS_CXX23
    template <class _Alloc, class _First, class _Second,
        enable_if_t<_Tuple_constructible_v<tuple, const _First, const _Second>, int> = 0>
    constexpr explicit(_Tuple_conditional_explicit_v<tuple, const _First, const _Second>)
        tuple(allocator_arg_t, const _Alloc& _Al, const pair<_First, _Second>&& _Right)
        : tuple(_Alloc_unpack_tuple_t{}, _Al, _STD move(_Right)) {}
#endif // _HAS_CXX23

    tuple& operator=(const volatile tuple&) = delete;

    template <class _Myself = tuple, class _This2 = _This,
        enable_if_t<conjunction_v<_STD _Is_copy_assignable_no_precondition_check<_This2>,
                        _STD _Is_copy_assignable_no_precondition_check<_Rest>...>,
            int> = 0>
    _CONSTEXPR20 tuple& operator=(_Identity_t<const _Myself&> _Right) noexcept(
        conjunction_v<is_nothrow_copy_assignable<_This2>, is_nothrow_copy_assignable<_Rest>...>) /* strengthened */ {
        _Myfirst._Val = _Right._Myfirst._Val;
        _Get_rest()   = _Right._Get_rest();
        return *this;
    }

#if _HAS_CXX23
    template <class _Myself = tuple, class _This2 = _This,
        enable_if_t<conjunction_v<_STD _Is_copy_assignable_no_precondition_check<const _This2>,
                        _STD _Is_copy_assignable_no_precondition_check<const _Rest>...>,
            int> = 0>
    constexpr const tuple& operator=(_Identity_t<const _Myself&> _Right) const
        noexcept(conjunction_v<is_nothrow_copy_assignable<const _This2>,
            is_nothrow_copy_assignable<const _Rest>...>) /* strengthened */ {
        _Myfirst._Val = _Right._Myfirst._Val;
        _Get_rest()   = _Right._Get_rest();
        return *this;
    }
#endif // _HAS_CXX23

    template <class _Myself = tuple, class _This2 = _This,
        enable_if_t<conjunction_v<_STD _Is_move_assignable_no_precondition_check<_This2>,
                        _STD _Is_move_assignable_no_precondition_check<_Rest>...>,
            int> = 0>
    _CONSTEXPR20 tuple& operator=(_Identity_t<_Myself&&> _Right) noexcept(
        conjunction_v<is_nothrow_move_assignable<_This2>, is_nothrow_move_assignable<_Rest>...>) {
        _Myfirst._Val = _STD forward<_This>(_Right._Myfirst._Val);
        _Get_rest()   = _STD forward<_Mybase>(_Right._Get_rest());
        return *this;
    }

#if _HAS_CXX23
    template <class _Myself = tuple, class _This2 = _This,
        enable_if_t<conjunction_v<_STD _Is_assignable_no_precondition_check<const _This2&, _This2>,
                        _STD _Is_assignable_no_precondition_check<const _Rest&, _Rest>...>,
            int> = 0>
    constexpr const tuple& operator=(_Identity_t<_Myself&&> _Right) const
        noexcept(conjunction_v<is_nothrow_assignable<const _This2&, _This2>,
            is_nothrow_assignable<const _Rest&, _Rest>...>) /* strengthened */ {
        _Myfirst._Val = _STD forward<_This>(_Right._Myfirst._Val);
        _Get_rest()   = _STD forward<_Mybase>(_Right._Get_rest());
        return *this;
    }
#endif // _HAS_CXX23

    template <class... _Other, enable_if_t<conjunction_v<_STD negation<_STD is_same<tuple, _STD tuple<_Other...>>>,
                                               _STD _Tuple_assignable_val<tuple, const _Other&...>>,
                                   int> = 0>
    _CONSTEXPR20 tuple& operator=(const tuple<_Other...>& _Right) noexcept(
        _Tuple_nothrow_assignable_v<tuple, const _Other&...>) /* strengthened */ {
        _Myfirst._Val = _Right._Myfirst._Val;
        _Get_rest()   = _Right._Get_rest();
        return *this;
    }

#if _HAS_CXX23
    template <class... _Other, enable_if_t<conjunction_v<_STD negation<_STD is_same<tuple, _STD tuple<_Other...>>>,
                                               _STD _Tuple_assignable_val<const tuple, const _Other&...>>,
                                   int> = 0>
    constexpr const tuple& operator=(const tuple<_Other...>& _Right) const
        noexcept(_Tuple_nothrow_assignable_v<const tuple, const _Other&...>) /* strengthened */ {
        _Myfirst._Val = _Right._Myfirst._Val;
        _Get_rest()   = _Right._Get_rest();
        return *this;
    }
#endif // _HAS_CXX23

    template <class... _Other, enable_if_t<conjunction_v<_STD negation<_STD is_same<tuple, _STD tuple<_Other...>>>,
                                               _STD _Tuple_assignable_val<tuple, _Other...>>,
                                   int> = 0>
    _CONSTEXPR20 tuple& operator=(tuple<_Other...>&& _Right) noexcept(
        _Tuple_nothrow_assignable_v<tuple, _Other...>) /* strengthened */ {
        _Myfirst._Val = _STD forward<typename tuple<_Other...>::_This_type>(_Right._Myfirst._Val);
        _Get_rest()   = _STD forward<typename tuple<_Other...>::_Mybase>(_Right._Get_rest());
        return *this;
    }

#if _HAS_CXX23
    template <class... _Other, enable_if_t<conjunction_v<_STD negation<_STD is_same<tuple, _STD tuple<_Other...>>>,
                                               _STD _Tuple_assignable_val<const tuple, _Other...>>,
                                   int> = 0>
    constexpr const tuple& operator=(tuple<_Other...>&& _Right) const
        noexcept(_Tuple_nothrow_assignable_v<const tuple, _Other...>) /* strengthened */ {
        _Myfirst._Val = _STD forward<typename tuple<_Other...>::_This_type>(_Right._Myfirst._Val);
        _Get_rest()   = _STD forward<typename tuple<_Other...>::_Mybase>(_Right._Get_rest());
        return *this;
    }
#endif // _HAS_CXX23

    template <class _First, class _Second,
        enable_if_t<_Tuple_assignable_v<tuple, const _First&, const _Second&>, int> = 0>
    _CONSTEXPR20 tuple& operator=(const pair<_First, _Second>& _Right) noexcept(
        _Tuple_nothrow_assignable_v<tuple, const _First&, const _Second&>) /* strengthened */ {
        _Myfirst._Val             = _Right.first;
        _Get_rest()._Myfirst._Val = _Right.second;
        return *this;
    }

#if _HAS_CXX23
    template <class _First, class _Second,
        enable_if_t<_Tuple_assignable_v<const tuple, const _First&, const _Second&>, int> = 0>
    constexpr const tuple& operator=(const pair<_First, _Second>& _Right) const
        noexcept(_Tuple_nothrow_assignable_v<const tuple, const _First&, const _Second&>) /* strengthened */ {
        _Myfirst._Val             = _Right.first;
        _Get_rest()._Myfirst._Val = _Right.second;
        return *this;
    }
#endif // _HAS_CXX23

    template <class _First, class _Second, enable_if_t<_Tuple_assignable_v<tuple, _First, _Second>, int> = 0>
    _CONSTEXPR20 tuple& operator=(pair<_First, _Second>&& _Right) noexcept(
        _Tuple_nothrow_assignable_v<tuple, _First, _Second>) /* strengthened */ {
        _Myfirst._Val             = _STD forward<_First>(_Right.first);
        _Get_rest()._Myfirst._Val = _STD forward<_Second>(_Right.second);
        return *this;
    }

#if _HAS_CXX23
    template <class _First, class _Second, enable_if_t<_Tuple_assignable_v<const tuple, _First, _Second>, int> = 0>
    constexpr const tuple& operator=(pair<_First, _Second>&& _Right) const
        noexcept(_Tuple_nothrow_assignable_v<const tuple, _First, _Second>) /* strengthened */ {
        _Myfirst._Val             = _STD forward<_First>(_Right.first);
        _Get_rest()._Myfirst._Val = _STD forward<_Second>(_Right.second);
        return *this;
    }
#endif // _HAS_CXX23

    _CONSTEXPR20 void swap(tuple& _Right) noexcept(
        conjunction_v<_Is_nothrow_swappable<_This>, _Is_nothrow_swappable<_Rest>...>) {
        _Swap_adl(_Myfirst._Val, _Right._Myfirst._Val);
        _Mybase::swap(_Right._Get_rest());
    }

#if _HAS_CXX23
    template <int = 0> // see GH-3013
    constexpr void swap(const tuple& _Right) const
        noexcept(conjunction_v<is_nothrow_swappable<const _This>, is_nothrow_swappable<const _Rest>...>) {
        _Swap_adl(_Myfirst._Val, _Right._Myfirst._Val);
        _Mybase::swap(_Right._Get_rest());
    }
#endif // _HAS_CXX23

    constexpr _Mybase& _Get_rest() noexcept { // get reference to rest of elements
        return *this;
    }

    constexpr const _Mybase& _Get_rest() const noexcept { // get const reference to rest of elements
        return *this;
    }

    template <class... _Other>
    constexpr bool _Equals(const tuple<_Other...>& _Right) const {
        return _Myfirst._Val == _Right._Myfirst._Val && _Mybase::_Equals(_Right._Get_rest());
    }

#ifdef __cpp_lib_concepts
    template <class _First, class... _Other>
    _NODISCARD constexpr common_comparison_category_t<_Synth_three_way_result<_This, _First>,
        _Synth_three_way_result<_Rest, _Other>...>
        _Three_way_compare(const tuple<_First, _Other...>& _Right) const {
        if (auto _Result = _Synth_three_way{}(_Myfirst._Val, _Right._Myfirst._Val); _Result != 0) {
            return _Result;
        }
        return _Mybase::_Three_way_compare(_Right._Get_rest());
    }
#else // ^^^ defined(__cpp_lib_concepts) / !defined(__cpp_lib_concepts) vvv
    template <class... _Other>
    _NODISCARD constexpr bool _Less(const tuple<_Other...>& _Right) const {
        return _Myfirst._Val < _Right._Myfirst._Val
            || (!(_Right._Myfirst._Val < _Myfirst._Val) && _Mybase::_Less(_Right._Get_rest()));
    }
#endif // ^^^ !defined(__cpp_lib_concepts) ^^^

    template <size_t _Index, class... _Types>
    friend constexpr tuple_element_t<_Index, tuple<_Types...>>& get(tuple<_Types...>& _Tuple) noexcept;

    template <size_t _Index, class... _Types>
    friend constexpr const tuple_element_t<_Index, tuple<_Types...>>& get(const tuple<_Types...>& _Tuple) noexcept;

    template <size_t _Index, class... _Types>
    friend constexpr tuple_element_t<_Index, tuple<_Types...>>&& get(tuple<_Types...>&& _Tuple) noexcept;

    template <size_t _Index, class... _Types>
    friend constexpr const tuple_element_t<_Index, tuple<_Types...>>&& get(const tuple<_Types...>&& _Tuple) noexcept;

    template <size_t _Index, class... _Types>
    friend constexpr auto&& _Tuple_get(tuple<_Types...>&& _Tuple) noexcept;

    template <class _Ty, class... _Types>
    friend constexpr _Ty& get(tuple<_Types...>& _Tuple) noexcept;

    template <class _Ty, class... _Types>
    friend constexpr const _Ty& get(const tuple<_Types...>& _Tuple) noexcept;

    template <class _Ty, class... _Types>
    friend constexpr _Ty&& get(tuple<_Types...>&& _Tuple) noexcept;

    template <class _Ty, class... _Types>
    friend constexpr const _Ty&& get(const tuple<_Types...>&& _Tuple) noexcept;

    _Tuple_val<_This> _Myfirst; // the stored element
};

#if _HAS_CXX17
template <class... _Types>
tuple(_Types...) -> tuple<_Types...>;

template <class _Ty1, class _Ty2>
tuple(pair<_Ty1, _Ty2>) -> tuple<_Ty1, _Ty2>;

template <class _Alloc, class... _Types>
tuple(allocator_arg_t, _Alloc, _Types...) -> tuple<_Types...>;

template <class _Alloc, class _Ty1, class _Ty2>
tuple(allocator_arg_t, _Alloc, pair<_Ty1, _Ty2>) -> tuple<_Ty1, _Ty2>;

template <class _Alloc, class... _Types>
tuple(allocator_arg_t, _Alloc, tuple<_Types...>) -> tuple<_Types...>;
#endif // _HAS_CXX17

_EXPORT_STD template <class... _Types1, class... _Types2>
_NODISCARD constexpr bool operator==(const tuple<_Types1...>& _Left, const tuple<_Types2...>& _Right) {
    static_assert(sizeof...(_Types1) == sizeof...(_Types2), "cannot compare tuples of different sizes");
    return _Left._Equals(_Right);
}

#ifdef __cpp_lib_concepts
_EXPORT_STD template <class... _Types1, class... _Types2>
_NODISCARD constexpr common_comparison_category_t<_Synth_three_way_result<_Types1, _Types2>...> operator<=>(
    const tuple<_Types1...>& _Left, const tuple<_Types2...>& _Right) {
    static_assert(sizeof...(_Types1) == sizeof...(_Types2), "cannot compare tuples of different sizes");
    return _Left._Three_way_compare(_Right);
}
#else // ^^^ defined(__cpp_lib_concepts) / !defined(__cpp_lib_concepts) vvv
#if !_HAS_CXX20
template <class... _Types1, class... _Types2>
_NODISCARD constexpr bool operator!=(const tuple<_Types1...>& _Left, const tuple<_Types2...>& _Right) {
    return !(_Left == _Right);
}
#endif // !_HAS_CXX20

template <class... _Types1, class... _Types2>
_NODISCARD constexpr bool operator<(const tuple<_Types1...>& _Left, const tuple<_Types2...>& _Right) {
    static_assert(sizeof...(_Types1) == sizeof...(_Types2), "cannot compare tuples of different sizes");
    return _Left._Less(_Right);
}

template <class... _Types1, class... _Types2>
_NODISCARD constexpr bool operator>=(const tuple<_Types1...>& _Left, const tuple<_Types2...>& _Right) {
    return !(_Left < _Right);
}

template <class... _Types1, class... _Types2>
_NODISCARD constexpr bool operator>(const tuple<_Types1...>& _Left, const tuple<_Types2...>& _Right) {
    return _Right < _Left;
}

template <class... _Types1, class... _Types2>
_NODISCARD constexpr bool operator<=(const tuple<_Types1...>& _Left, const tuple<_Types2...>& _Right) {
    return !(_Right < _Left);
}
#endif // ^^^ !defined(__cpp_lib_concepts) ^^^

_EXPORT_STD template <class... _Types, enable_if_t<conjunction_v<_Is_swappable<_Types>...>, int> = 0>
_CONSTEXPR20 void swap(tuple<_Types...>& _Left, tuple<_Types...>& _Right) noexcept(noexcept(_Left.swap(_Right))) {
    _Left.swap(_Right);
}

#if _HAS_CXX23
_EXPORT_STD template <class... _Types, enable_if_t<conjunction_v<is_swappable<const _Types>...>, int> = 0>
constexpr void swap(const tuple<_Types...>& _Left, const tuple<_Types...>& _Right) noexcept(
    noexcept(_Left.swap(_Right))) {
    _Left.swap(_Right);
}
#endif // _HAS_CXX23

template <class _Ty, class _Tuple>
struct _Tuple_element {}; // backstop _Tuple_element definition

template <class _This, class... _Rest>
struct _Tuple_element<_This, tuple<_This, _Rest...>> { // select first element
    static_assert(!_Is_any_of_v<_This, _Rest...>, "duplicate type T in get<T>(tuple)");
    using _Ttype = tuple<_This, _Rest...>;
};

template <class _Ty, class _This, class... _Rest>
struct _Tuple_element<_Ty, tuple<_This, _Rest...>> { // recursive _Tuple_element definition
    using _Ttype = typename _Tuple_element<_Ty, tuple<_Rest...>>::_Ttype;
};

_EXPORT_STD template <size_t _Index, class... _Types>
_NODISCARD constexpr tuple_element_t<_Index, tuple<_Types...>>& get(tuple<_Types...>& _Tuple) noexcept {
    using _Ttype = typename tuple_element<_Index, tuple<_Types...>>::_Ttype;
    return static_cast<_Ttype&>(_Tuple)._Myfirst._Val;
}

_EXPORT_STD template <size_t _Index, class... _Types>
_NODISCARD constexpr const tuple_element_t<_Index, tuple<_Types...>>& get(const tuple<_Types...>& _Tuple) noexcept {
    using _Ttype = typename tuple_element<_Index, tuple<_Types...>>::_Ttype;
    return static_cast<const _Ttype&>(_Tuple)._Myfirst._Val;
}

_EXPORT_STD template <size_t _Index, class... _Types>
_NODISCARD constexpr tuple_element_t<_Index, tuple<_Types...>>&& get(tuple<_Types...>&& _Tuple) noexcept {
    using _Ty    = tuple_element_t<_Index, tuple<_Types...>>;
    using _Ttype = typename tuple_element<_Index, tuple<_Types...>>::_Ttype;
    return static_cast<_Ty&&>(static_cast<_Ttype&>(_Tuple)._Myfirst._Val);
}

_EXPORT_STD template <size_t _Index, class... _Types>
_NODISCARD constexpr const tuple_element_t<_Index, tuple<_Types...>>&& get(const tuple<_Types...>&& _Tuple) noexcept {
    using _Ty    = tuple_element_t<_Index, tuple<_Types...>>;
    using _Ttype = typename tuple_element<_Index, tuple<_Types...>>::_Ttype;
    return static_cast<const _Ty&&>(static_cast<const _Ttype&>(_Tuple)._Myfirst._Val);
}

_EXPORT_STD /* TRANSITION, VSO-1538698 */ template <size_t _Index, class... _Types>
_NODISCARD constexpr auto&& _Tuple_get(tuple<_Types...>&& _Tuple) noexcept {
    // used by pair's piecewise constructor
    using _Ty    = tuple_element_t<_Index, tuple<_Types...>>;
    using _Ttype = typename tuple_element<_Index, tuple<_Types...>>::_Ttype;
    return static_cast<_Ty&&>(static_cast<_Ttype&>(_Tuple)._Myfirst._Val);
}

_EXPORT_STD template <class _Ty, class... _Types>
_NODISCARD constexpr _Ty& get(tuple<_Types...>& _Tuple) noexcept {
    using _Ttype = typename _Tuple_element<_Ty, tuple<_Types...>>::_Ttype;
    return static_cast<_Ttype&>(_Tuple)._Myfirst._Val;
}

_EXPORT_STD template <class _Ty, class... _Types>
_NODISCARD constexpr const _Ty& get(const tuple<_Types...>& _Tuple) noexcept {
    using _Ttype = typename _Tuple_element<_Ty, tuple<_Types...>>::_Ttype;
    return static_cast<const _Ttype&>(_Tuple)._Myfirst._Val;
}

_EXPORT_STD template <class _Ty, class... _Types>
_NODISCARD constexpr _Ty&& get(tuple<_Types...>&& _Tuple) noexcept {
    using _Ttype = typename _Tuple_element<_Ty, tuple<_Types...>>::_Ttype;
    return static_cast<_Ty&&>(static_cast<_Ttype&>(_Tuple)._Myfirst._Val);
}

_EXPORT_STD template <class _Ty, class... _Types>
_NODISCARD constexpr const _Ty&& get(const tuple<_Types...>&& _Tuple) noexcept {
    using _Ttype = typename _Tuple_element<_Ty, tuple<_Types...>>::_Ttype;
    return static_cast<const _Ty&&>(static_cast<const _Ttype&>(_Tuple)._Myfirst._Val);
}

template <class _This, class... _Rest>
template <class _Tag, class _Tpl, size_t... _Indices, enable_if_t<is_same_v<_Tag, _STD _Unpack_tuple_t>, int>>
constexpr tuple<_This, _Rest...>::tuple(_Tag, _Tpl&& _Right, index_sequence<_Indices...>)
    : tuple(_Exact_args_t{}, _STD get<_Indices>(_STD forward<_Tpl>(_Right))...) {}

template <class _This, class... _Rest>
template <class _Tag, class _Alloc, class _Tpl, size_t... _Indices,
    enable_if_t<is_same_v<_Tag, _STD _Alloc_unpack_tuple_t>, int>>
constexpr tuple<_This, _Rest...>::tuple(_Tag, const _Alloc& _Al, _Tpl&& _Right, index_sequence<_Indices...>)
    : tuple(_Alloc_exact_args_t{}, _Al, _STD get<_Indices>(_STD forward<_Tpl>(_Right))...) {}

_EXPORT_STD template <class... _Types>
_NODISCARD constexpr tuple<_Unrefwrap_t<_Types>...> make_tuple(_Types&&... _Args) { // make tuple from elements
    using _Ttype = tuple<_Unrefwrap_t<_Types>...>;
    return _Ttype(_STD forward<_Types>(_Args)...);
}

_EXPORT_STD template <class... _Types>
_NODISCARD constexpr tuple<_Types&...> tie(_Types&... _Args) noexcept { // make tuple from elements
    using _Ttype = tuple<_Types&...>;
    return _Ttype(_Args...);
}

_EXPORT_STD template <class... _Types>
_NODISCARD constexpr tuple<_Types&&...> forward_as_tuple(_Types&&... _Args) noexcept { // forward arguments in a tuple
    return tuple<_Types&&...>(_STD forward<_Types>(_Args)...);
}

_EXPORT_STD template <class _Ty, size_t _Size>
class array;

_EXPORT_STD template <size_t _Idx, class _Ty, size_t _Size>
_NODISCARD constexpr _Ty& get(array<_Ty, _Size>& _Arr) noexcept;

_EXPORT_STD template <size_t _Idx, class _Ty, size_t _Size>
_NODISCARD constexpr const _Ty& get(const array<_Ty, _Size>& _Arr) noexcept;

_EXPORT_STD template <size_t _Idx, class _Ty, size_t _Size>
_NODISCARD constexpr _Ty&& get(array<_Ty, _Size>&& _Arr) noexcept;

_EXPORT_STD template <size_t _Idx, class _Ty, size_t _Size>
_NODISCARD constexpr const _Ty&& get(const array<_Ty, _Size>&& _Arr) noexcept;

template <class _Ty, class _Kx_arg, class _Ix_arg, size_t _Ix_next, class... _Sequences>
struct _Tuple_cat2;

template <class _Ty, size_t... _Kx, size_t... _Ix, size_t _Ix_next>
struct _Tuple_cat2<_Ty, index_sequence<_Kx...>, index_sequence<_Ix...>, _Ix_next> {
    using _Ret    = tuple<tuple_element_t<_Kx, _Remove_cvref_t<tuple_element_t<_Ix, _Ty>>>...>;
    using _Kx_seq = index_sequence<_Kx...>;
    using _Ix_seq = index_sequence<_Ix...>;
};

template <class _Ty, size_t... _Kx, size_t... _Ix, size_t _Ix_next, size_t... _Kx_next, class... _Rest>
struct _Tuple_cat2<_Ty, index_sequence<_Kx...>, index_sequence<_Ix...>, _Ix_next, index_sequence<_Kx_next...>, _Rest...>
    : _Tuple_cat2<_Ty, index_sequence<_Kx..., _Kx_next...>,
          index_sequence<_Ix..., (_Ix_next + 0 * _Kx_next)...>, // repeat _Ix_next, ignoring the elements of _Kx_next
          _Ix_next + 1, _Rest...> {};

template <class... _Tuples>
using _Tuple_cat1 = _Tuple_cat2<tuple<_Tuples&&...>, index_sequence<>, index_sequence<>, 0,
    make_index_sequence<tuple_size_v<_Remove_cvref_t<_Tuples>>>...>;

template <class _Ret, size_t... _Kx, size_t... _Ix, class _Ty>
constexpr _Ret _Tuple_cat(index_sequence<_Kx...>, index_sequence<_Ix...>, _Ty _Arg) {
    return _Ret{_STD get<_Kx>(_STD get<_Ix>(_STD move(_Arg)))...};
}

_EXPORT_STD template <class... _Tuples>
_NODISCARD constexpr typename _Tuple_cat1<_Tuples...>::_Ret tuple_cat(_Tuples&&... _Tpls) { // concatenate tuples
    using _Cat1   = _Tuple_cat1<_Tuples...>;
    using _Ret    = typename _Cat1::_Ret;
    using _Kx_seq = typename _Cat1::_Kx_seq;
    using _Ix_seq = typename _Cat1::_Ix_seq;
    return _Tuple_cat<_Ret>(_Kx_seq{}, _Ix_seq{}, _STD forward_as_tuple(_STD forward<_Tuples>(_Tpls)...));
}

#if _HAS_CXX17
template <class _Callable, class _Tuple, size_t... _Indices>
constexpr decltype(auto) _Apply_impl(_Callable&& _Obj, _Tuple&& _Tpl, index_sequence<_Indices...>) noexcept(
    noexcept(_STD invoke(_STD forward<_Callable>(_Obj), _STD get<_Indices>(_STD forward<_Tuple>(_Tpl))...))) {
    return _STD invoke(_STD forward<_Callable>(_Obj), _STD get<_Indices>(_STD forward<_Tuple>(_Tpl))...);
}

_EXPORT_STD template <class _Callable, class _Tuple>
constexpr decltype(auto) apply(_Callable&& _Obj, _Tuple&& _Tpl) noexcept(
    noexcept(_Apply_impl(_STD forward<_Callable>(_Obj), _STD forward<_Tuple>(_Tpl),
        make_index_sequence<tuple_size_v<remove_reference_t<_Tuple>>>{}))) {
    return _Apply_impl(_STD forward<_Callable>(_Obj), _STD forward<_Tuple>(_Tpl),
        make_index_sequence<tuple_size_v<remove_reference_t<_Tuple>>>{});
}

template <class _Ty, class _Tuple, size_t... _Indices>
constexpr _Ty _Make_from_tuple_impl(_Tuple&& _Tpl, index_sequence<_Indices...>) {
    // construct _Ty from the elements of _Tpl
    static_assert(is_constructible_v<_Ty, decltype(_STD get<_Indices>(_STD forward<_Tuple>(_Tpl)))...>,
        "the target type must be constructible from the fields of the argument tuple (N4892 [tuple.apply]/2).");
    return _Ty(_STD get<_Indices>(_STD forward<_Tuple>(_Tpl))...);
}

_EXPORT_STD template <class _Ty, class _Tuple>
_NODISCARD constexpr _Ty make_from_tuple(_Tuple&& _Tpl) { // construct _Ty from the elements of _Tpl
    return _Make_from_tuple_impl<_Ty>(
        _STD forward<_Tuple>(_Tpl), make_index_sequence<tuple_size_v<remove_reference_t<_Tuple>>>{});
}
#endif // _HAS_CXX17

template <class... _Types, class _Alloc>
struct uses_allocator<tuple<_Types...>, _Alloc> : true_type {}; // true_type if container allocator enabled

#ifdef __cpp_lib_concepts
template <class... _TTypes, class... _UTypes, template <class> class _TQual, template <class> class _UQual>
    requires requires { typename tuple<common_reference_t<_TQual<_TTypes>, _UQual<_UTypes>>...>; }
struct basic_common_reference<tuple<_TTypes...>, tuple<_UTypes...>, _TQual, _UQual> {
    using type = tuple<common_reference_t<_TQual<_TTypes>, _UQual<_UTypes>>...>;
};

template <class... _TTypes, class... _UTypes>
    requires requires { typename tuple<common_type_t<_TTypes, _UTypes>...>; }
struct common_type<tuple<_TTypes...>, tuple<_UTypes...>> {
    using type = tuple<common_type_t<_TTypes, _UTypes>...>;
};
#endif // __cpp_lib_concepts

#if _HAS_TR1_NAMESPACE
namespace _DEPRECATE_TR1_NAMESPACE tr1 {
    using _STD get;
    using _STD ignore;
    using _STD make_tuple;
    using _STD ref;
    using _STD tie;
    using _STD tuple;
} // namespace _DEPRECATE_TR1_NAMESPACE tr1
#endif // _HAS_TR1_NAMESPACE

_STD_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)
#endif // _STL_COMPILER_PREPROCESSOR
#endif // _TUPLE_
