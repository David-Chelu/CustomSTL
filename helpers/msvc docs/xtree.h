// xtree internal header

// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once
#ifndef _XTREE_
#define _XTREE_
#include <yvals_core.h>
#if _STL_COMPILER_PREPROCESSOR
#include <xmemory>

#if _HAS_CXX17
#include <xnode_handle.h>
#endif // _HAS_CXX17

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_STD_BEGIN
template <class _Mytree, class _Base = _Iterator_base0>
class _Tree_unchecked_const_iterator : public _Base {
public:
    using iterator_category = bidirectional_iterator_tag;

    using _Nodeptr        = typename _Mytree::_Nodeptr;
    using value_type      = typename _Mytree::value_type;
    using difference_type = typename _Mytree::difference_type;
    using pointer         = typename _Mytree::const_pointer;
    using reference       = const value_type&;

    _Tree_unchecked_const_iterator() noexcept : _Ptr() {}

    _Tree_unchecked_const_iterator(_Nodeptr _Pnode, const _Mytree* _Plist) noexcept : _Ptr(_Pnode) {
        this->_Adopt(_Plist);
    }

    _NODISCARD reference operator*() const noexcept {
        return _Ptr->_Myval;
    }

    _NODISCARD pointer operator->() const noexcept {
        return pointer_traits<pointer>::pointer_to(**this);
    }

    _Tree_unchecked_const_iterator& operator++() noexcept {
        if (_Ptr->_Right->_Isnil) { // climb looking for right subtree
            _Nodeptr _Pnode;
            while (!(_Pnode = _Ptr->_Parent)->_Isnil && _Ptr == _Pnode->_Right) {
                _Ptr = _Pnode; // ==> parent while right subtree
            }

            _Ptr = _Pnode; // ==> parent (head if end())
        } else {
            _Ptr = _Mytree::_Min(_Ptr->_Right); // ==> smallest of right subtree
        }

        return *this;
    }

    _Tree_unchecked_const_iterator operator++(int) noexcept {
        _Tree_unchecked_const_iterator _Tmp = *this;
        ++*this;
        return _Tmp;
    }

    _Tree_unchecked_const_iterator& operator--() noexcept {
        if (_Ptr->_Isnil) {
            _Ptr = _Ptr->_Right; // end() ==> rightmost
        } else if (_Ptr->_Left->_Isnil) { // climb looking for left subtree
            _Nodeptr _Pnode;
            while (!(_Pnode = _Ptr->_Parent)->_Isnil && _Ptr == _Pnode->_Left) {
                _Ptr = _Pnode; // ==> parent while left subtree
            }

            if (!_Ptr->_Isnil) { // decrement non-begin()
                _Ptr = _Pnode; // ==> parent if not head
            }
        } else {
            _Ptr = _Mytree::_Max(_Ptr->_Left); // ==> largest of left subtree
        }

        return *this;
    }

    _Tree_unchecked_const_iterator operator--(int) noexcept {
        _Tree_unchecked_const_iterator _Tmp = *this;
        --*this;
        return _Tmp;
    }

    _NODISCARD bool operator==(const _Tree_unchecked_const_iterator& _Right) const noexcept {
        return _Ptr == _Right._Ptr;
    }

#if !_HAS_CXX20
    _NODISCARD bool operator!=(const _Tree_unchecked_const_iterator& _Right) const noexcept {
        return !(*this == _Right);
    }
#endif // !_HAS_CXX20

    _NODISCARD bool operator==(_Default_sentinel) const noexcept {
        return !!_Ptr->_Isnil; // TRANSITION, avoid warning C4800:
                               // "Implicit conversion from 'char' to bool. Possible information loss" (/Wall)
    }

#if !_HAS_CXX20
    _NODISCARD bool operator!=(_Default_sentinel) const noexcept {
        return !_Ptr->_Isnil;
    }
#endif // !_HAS_CXX20

    _Nodeptr _Ptr; // pointer to node
};

template <class _Mytree>
class _Tree_unchecked_iterator : public _Tree_unchecked_const_iterator<_Mytree> {
public:
    using _Mybase           = _Tree_unchecked_const_iterator<_Mytree>;
    using iterator_category = bidirectional_iterator_tag;

    using _Nodeptr        = typename _Mytree::_Nodeptr;
    using value_type      = typename _Mytree::value_type;
    using difference_type = typename _Mytree::difference_type;
    using pointer         = typename _Mytree::pointer;
    using reference       = value_type&;

    using _Mybase::_Mybase;

    _NODISCARD reference operator*() const noexcept {
        return const_cast<reference>(_Mybase::operator*());
    }

    _NODISCARD pointer operator->() const noexcept {
        return pointer_traits<pointer>::pointer_to(**this);
    }

    _Tree_unchecked_iterator& operator++() noexcept {
        _Mybase::operator++();
        return *this;
    }

    _Tree_unchecked_iterator operator++(int) noexcept {
        _Tree_unchecked_iterator _Tmp = *this;
        _Mybase::operator++();
        return _Tmp;
    }

    _Tree_unchecked_iterator& operator--() noexcept {
        _Mybase::operator--();
        return *this;
    }

    _Tree_unchecked_iterator operator--(int) noexcept {
        _Tree_unchecked_iterator _Tmp = *this;
        _Mybase::operator--();
        return _Tmp;
    }
};

template <class _Mytree>
class _Tree_const_iterator : public _Tree_unchecked_const_iterator<_Mytree, _Iterator_base> {
public:
    using _Mybase           = _Tree_unchecked_const_iterator<_Mytree, _Iterator_base>;
    using iterator_category = bidirectional_iterator_tag;

    using _Nodeptr        = typename _Mytree::_Nodeptr;
    using value_type      = typename _Mytree::value_type;
    using difference_type = typename _Mytree::difference_type;
    using pointer         = typename _Mytree::const_pointer;
    using reference       = const value_type&;

    using _Mybase::_Mybase;

    _NODISCARD reference operator*() const noexcept {
#if _ITERATOR_DEBUG_LEVEL == 2
        const auto _Mycont = static_cast<const _Mytree*>(this->_Getcont());
        _STL_ASSERT(_Mycont, "cannot dereference value-initialized map/set iterator");
        _STL_VERIFY(this->_Ptr != _Mycont->_Myhead, "cannot dereference end map/set iterator");
#endif // _ITERATOR_DEBUG_LEVEL == 2

        return this->_Ptr->_Myval;
    }

    _NODISCARD pointer operator->() const noexcept {
        return pointer_traits<pointer>::pointer_to(**this);
    }

    _Tree_const_iterator& operator++() noexcept {
#if _ITERATOR_DEBUG_LEVEL == 2
        _STL_VERIFY(this->_Getcont(), "cannot increment value-initialized map/set iterator");
        _STL_VERIFY(!this->_Ptr->_Isnil, "cannot increment end map/set iterator");
#endif // _ITERATOR_DEBUG_LEVEL == 2

        _Mybase::operator++();
        return *this;
    }

    _Tree_const_iterator operator++(int) noexcept {
        _Tree_const_iterator _Tmp = *this;
        ++*this;
        return _Tmp;
    }

    _Tree_const_iterator& operator--() noexcept {
#if _ITERATOR_DEBUG_LEVEL == 2
        _STL_ASSERT(this->_Getcont(), "cannot decrement value-initialized map/set iterator");
        _Nodeptr _Ptrsav = this->_Ptr;
        _Mybase::operator--();
        _STL_VERIFY(_Ptrsav != this->_Ptr, "cannot decrement begin map/set iterator");
#else // ^^^ _ITERATOR_DEBUG_LEVEL == 2 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 2 vvv
        _Mybase::operator--();
#endif // _ITERATOR_DEBUG_LEVEL == 2

        return *this;
    }

    _Tree_const_iterator operator--(int) noexcept {
        _Tree_const_iterator _Tmp = *this;
        --*this;
        return _Tmp;
    }

    _NODISCARD bool operator==(const _Tree_const_iterator& _Right) const noexcept {
#if _ITERATOR_DEBUG_LEVEL == 2
        _STL_VERIFY(this->_Getcont() == _Right._Getcont(), "map/set iterators incompatible");
#endif // _ITERATOR_DEBUG_LEVEL == 2

        return this->_Ptr == _Right._Ptr;
    }

#if !_HAS_CXX20
    _NODISCARD bool operator!=(const _Tree_const_iterator& _Right) const noexcept {
        return !(*this == _Right);
    }
#endif // !_HAS_CXX20

#if _ITERATOR_DEBUG_LEVEL == 2
    friend void _Verify_range(const _Tree_const_iterator& _First, const _Tree_const_iterator& _Last) noexcept {
        _STL_VERIFY(_First._Getcont() == _Last._Getcont(), "map/set iterators in range are from different containers");
    }
#endif // _ITERATOR_DEBUG_LEVEL == 2

    using _Prevent_inheriting_unwrap = _Tree_const_iterator;

    _NODISCARD _Tree_unchecked_const_iterator<_Mytree> _Unwrapped() const noexcept {
        return _Tree_unchecked_const_iterator<_Mytree>(this->_Ptr, static_cast<const _Mytree*>(this->_Getcont()));
    }

    void _Seek_to(const _Tree_unchecked_const_iterator<_Mytree> _It) noexcept {
        this->_Ptr = _It._Ptr;
    }
};

template <class _Mytree>
class _Tree_iterator : public _Tree_const_iterator<_Mytree> {
public:
    using _Mybase           = _Tree_const_iterator<_Mytree>;
    using iterator_category = bidirectional_iterator_tag;

    using _Nodeptr        = typename _Mytree::_Nodeptr;
    using value_type      = typename _Mytree::value_type;
    using difference_type = typename _Mytree::difference_type;

    using pointer   = typename _Mytree::pointer;
    using reference = value_type&;

    using _Mybase::_Mybase;

    _NODISCARD reference operator*() const noexcept {
        return const_cast<reference>(_Mybase::operator*());
    }

    _NODISCARD pointer operator->() const noexcept {
        return pointer_traits<pointer>::pointer_to(**this);
    }

    _Tree_iterator& operator++() noexcept {
        _Mybase::operator++();
        return *this;
    }

    _Tree_iterator operator++(int) noexcept {
        _Tree_iterator _Tmp = *this;
        _Mybase::operator++();
        return _Tmp;
    }

    _Tree_iterator& operator--() noexcept {
        _Mybase::operator--();
        return *this;
    }

    _Tree_iterator operator--(int) noexcept {
        _Tree_iterator _Tmp = *this;
        _Mybase::operator--();
        return _Tmp;
    }

    using _Prevent_inheriting_unwrap = _Tree_iterator;

    _NODISCARD _Tree_unchecked_iterator<_Mytree> _Unwrapped() const noexcept {
        return _Tree_unchecked_iterator<_Mytree>(this->_Ptr, static_cast<const _Mytree*>(this->_Getcont()));
    }
};

template <class _Value_type, class _Size_type, class _Difference_type, class _Pointer, class _Const_pointer,
    class _Reference, class _Const_reference, class _Nodeptr_type>
struct _Tree_iter_types {
    using value_type      = _Value_type;
    using size_type       = _Size_type;
    using difference_type = _Difference_type;
    using pointer         = _Pointer;
    using const_pointer   = _Const_pointer;
    using _Nodeptr        = _Nodeptr_type;
};

template <class _Value_type, class _Voidptr>
struct _Tree_node {
    using _Nodeptr   = _Rebind_pointer_t<_Voidptr, _Tree_node>;
    using value_type = _Value_type;
    _Nodeptr _Left; // left subtree, or smallest element if head
    _Nodeptr _Parent; // parent, or root of tree if head
    _Nodeptr _Right; // right subtree, or largest element if head
    char _Color; // _Red or _Black, _Black if head
    char _Isnil; // true only if head (also nil) node; TRANSITION, should be bool
    value_type _Myval = // the stored value, unused if head
        _Returns_exactly<value_type>(); // fake a viable constructor to workaround GH-2749

    enum _Redbl { // colors for link to parent
        _Red,
        _Black
    };

    _Tree_node()                             = default;
    _Tree_node(const _Tree_node&)            = delete;
    _Tree_node& operator=(const _Tree_node&) = delete;

    template <class _Alloc>
    static _Nodeptr _Buyheadnode(_Alloc& _Al) {
        static_assert(is_same_v<typename _Alloc::value_type, _Tree_node>, "Bad _Buyheadnode call");
        const auto _Pnode = _Al.allocate(1);
        _Construct_in_place(_Pnode->_Left, _Pnode);
        _Construct_in_place(_Pnode->_Parent, _Pnode);
        _Construct_in_place(_Pnode->_Right, _Pnode);
        _Pnode->_Color = _Black;
        _Pnode->_Isnil = true;
        return _Pnode;
    }

    template <class _Alloc, class... _Valty>
    static _Nodeptr _Buynode(_Alloc& _Al, _Nodeptr _Myhead, _Valty&&... _Val) {
        // allocate a node with defaults and set links and value
        static_assert(is_same_v<typename _Alloc::value_type, _Tree_node>, "Bad _Buynode call");
        _Alloc_construct_ptr<_Alloc> _Newnode(_Al);
        _Newnode._Allocate();
        allocator_traits<_Alloc>::construct(_Al, _STD addressof(_Newnode._Ptr->_Myval), _STD forward<_Valty>(_Val)...);
        _Construct_in_place(_Newnode._Ptr->_Left, _Myhead);
        _Construct_in_place(_Newnode._Ptr->_Parent, _Myhead);
        _Construct_in_place(_Newnode._Ptr->_Right, _Myhead);
        _Newnode._Ptr->_Color = _Red;
        _Newnode._Ptr->_Isnil = false;
        return _Newnode._Release();
    }

    template <class _Alloc>
    static void _Freenode0(_Alloc& _Al, _Nodeptr _Ptr) noexcept {
        static_assert(is_same_v<typename _Alloc::value_type, _Tree_node>, "Bad _Freenode0 call");
        _Destroy_in_place(_Ptr->_Left);
        _Destroy_in_place(_Ptr->_Parent);
        _Destroy_in_place(_Ptr->_Right);
        allocator_traits<_Alloc>::deallocate(_Al, _Ptr, 1);
    }

    template <class _Alloc>
    static void _Freenode(_Alloc& _Al, _Nodeptr _Ptr) noexcept {
        static_assert(is_same_v<typename _Alloc::value_type, _Tree_node>, "Bad _Freenode call");
        allocator_traits<_Alloc>::destroy(_Al, _STD addressof(_Ptr->_Myval));
        _Freenode0(_Al, _Ptr);
    }
};

template <class _Ty>
struct _Tree_simple_types : _Simple_types<_Ty> {
    using _Node    = _Tree_node<_Ty, void*>;
    using _Nodeptr = _Node*;
};

enum class _Tree_child {
    _Right, // perf note: compare with _Right rather than _Left where possible for comparison with zero
    _Left,
    _Unused // indicates that tree child should never be used for insertion
};

template <class _Nodeptr>
struct _Tree_id {
    _Nodeptr _Parent; // the leaf node under which a new node should be inserted
    _Tree_child _Child;
};

template <class _Nodeptr>
struct _Tree_find_result {
    _Tree_id<_Nodeptr> _Location;
    _Nodeptr _Bound;
};

template <class _Nodeptr>
struct _Tree_find_hint_result {
    _Tree_id<_Nodeptr> _Location;
    bool _Duplicate;
};

[[noreturn]] inline void _Throw_tree_length_error() {
    _Xlength_error("map/set too long");
}

template <class _Val_types>
class _Tree_val : public _Container_base {
public:
    using _Nodeptr = typename _Val_types::_Nodeptr;

    using value_type      = typename _Val_types::value_type;
    using size_type       = typename _Val_types::size_type;
    using difference_type = typename _Val_types::difference_type;
    using pointer         = typename _Val_types::pointer;
    using const_pointer   = typename _Val_types::const_pointer;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using _Unchecked_const_iterator = _Tree_unchecked_const_iterator<_Tree_val>;
    using const_iterator            = _Tree_const_iterator<_Tree_val>;

    _Tree_val() noexcept : _Myhead(), _Mysize(0) {}

    enum _Redbl { // colors for link to parent
        _Red,
        _Black
    };

    static _Nodeptr _Max(_Nodeptr _Pnode) noexcept { // return rightmost node in subtree at _Pnode
        while (!_Pnode->_Right->_Isnil) {
            _Pnode = _Pnode->_Right;
        }

        return _Pnode;
    }

    static _Nodeptr _Min(_Nodeptr _Pnode) noexcept { // return leftmost node in subtree at _Pnode
        while (!_Pnode->_Left->_Isnil) {
            _Pnode = _Pnode->_Left;
        }

        return _Pnode;
    }

    void _Lrotate(_Nodeptr _Wherenode) noexcept { // promote right node to root of subtree
        _Nodeptr _Pnode    = _Wherenode->_Right;
        _Wherenode->_Right = _Pnode->_Left;

        if (!_Pnode->_Left->_Isnil) {
            _Pnode->_Left->_Parent = _Wherenode;
        }

        _Pnode->_Parent = _Wherenode->_Parent;

        if (_Wherenode == _Myhead->_Parent) {
            _Myhead->_Parent = _Pnode;
        } else if (_Wherenode == _Wherenode->_Parent->_Left) {
            _Wherenode->_Parent->_Left = _Pnode;
        } else {
            _Wherenode->_Parent->_Right = _Pnode;
        }

        _Pnode->_Left       = _Wherenode;
        _Wherenode->_Parent = _Pnode;
    }

    void _Rrotate(_Nodeptr _Wherenode) noexcept { // promote left node to root of subtree
        _Nodeptr _Pnode   = _Wherenode->_Left;
        _Wherenode->_Left = _Pnode->_Right;

        if (!_Pnode->_Right->_Isnil) {
            _Pnode->_Right->_Parent = _Wherenode;
        }

        _Pnode->_Parent = _Wherenode->_Parent;

        if (_Wherenode == _Myhead->_Parent) {
            _Myhead->_Parent = _Pnode;
        } else if (_Wherenode == _Wherenode->_Parent->_Right) {
            _Wherenode->_Parent->_Right = _Pnode;
        } else {
            _Wherenode->_Parent->_Left = _Pnode;
        }

        _Pnode->_Right      = _Wherenode;
        _Wherenode->_Parent = _Pnode;
    }

    _Nodeptr _Extract(_Unchecked_const_iterator _Where) noexcept {
        _Nodeptr _Erasednode = _Where._Ptr; // node to erase
        ++_Where; // save successor iterator for return

        _Nodeptr _Fixnode; // the node to recolor as needed
        _Nodeptr _Fixnodeparent; // parent of _Fixnode (which may be nil)
        _Nodeptr _Pnode = _Erasednode;

        if (_Pnode->_Left->_Isnil) {
            _Fixnode = _Pnode->_Right; // stitch up right subtree
        } else if (_Pnode->_Right->_Isnil) {
            _Fixnode = _Pnode->_Left; // stitch up left subtree
        } else { // two subtrees, must lift successor node to replace erased
            _Pnode   = _Where._Ptr; // _Pnode is successor node
            _Fixnode = _Pnode->_Right; // _Fixnode is only subtree
        }

        if (_Pnode == _Erasednode) { // at most one subtree, relink it
            _Fixnodeparent = _Erasednode->_Parent;
            if (!_Fixnode->_Isnil) {
                _Fixnode->_Parent = _Fixnodeparent; // link up
            }

            if (_Myhead->_Parent == _Erasednode) {
                _Myhead->_Parent = _Fixnode; // link down from root
            } else if (_Fixnodeparent->_Left == _Erasednode) {
                _Fixnodeparent->_Left = _Fixnode; // link down to left
            } else {
                _Fixnodeparent->_Right = _Fixnode; // link down to right
            }

            if (_Myhead->_Left == _Erasednode) {
                _Myhead->_Left = _Fixnode->_Isnil ? _Fixnodeparent // smallest is parent of erased node
                                                  : _Min(_Fixnode); // smallest in relinked subtree
            }

            if (_Myhead->_Right == _Erasednode) {
                _Myhead->_Right = _Fixnode->_Isnil ? _Fixnodeparent // largest is parent of erased node
                                                   : _Max(_Fixnode); // largest in relinked subtree
            }
        } else { // erased has two subtrees, _Pnode is successor to erased
            _Erasednode->_Left->_Parent = _Pnode; // link left up
            _Pnode->_Left               = _Erasednode->_Left; // link successor down

            if (_Pnode == _Erasednode->_Right) {
                _Fixnodeparent = _Pnode; // successor is next to erased
            } else { // successor further down, link in place of erased
                _Fixnodeparent = _Pnode->_Parent; // parent is successor's
                if (!_Fixnode->_Isnil) {
                    _Fixnode->_Parent = _Fixnodeparent; // link fix up
                }

                _Fixnodeparent->_Left        = _Fixnode; // link fix down
                _Pnode->_Right               = _Erasednode->_Right; // link next down
                _Erasednode->_Right->_Parent = _Pnode; // right up
            }

            if (_Myhead->_Parent == _Erasednode) {
                _Myhead->_Parent = _Pnode; // link down from root
            } else if (_Erasednode->_Parent->_Left == _Erasednode) {
                _Erasednode->_Parent->_Left = _Pnode; // link down to left
            } else {
                _Erasednode->_Parent->_Right = _Pnode; // link down to right
            }

            _Pnode->_Parent = _Erasednode->_Parent; // link successor up
            _STD swap(_Pnode->_Color, _Erasednode->_Color); // recolor it
        }

        if (_Erasednode->_Color == _Black) { // erasing black link, must recolor/rebalance tree
            for (; _Fixnode != _Myhead->_Parent && _Fixnode->_Color == _Black; _Fixnodeparent = _Fixnode->_Parent) {
                if (_Fixnode == _Fixnodeparent->_Left) { // fixup left subtree
                    _Pnode = _Fixnodeparent->_Right;
                    if (_Pnode->_Color == _Red) { // rotate red up from right subtree
                        _Pnode->_Color         = _Black;
                        _Fixnodeparent->_Color = _Red;
                        _Lrotate(_Fixnodeparent);
                        _Pnode = _Fixnodeparent->_Right;
                    }

                    if (_Pnode->_Isnil) {
                        _Fixnode = _Fixnodeparent; // shouldn't happen
                    } else if (_Pnode->_Left->_Color == _Black
                               && _Pnode->_Right->_Color == _Black) { // redden right subtree with black children
                        _Pnode->_Color = _Red;
                        _Fixnode       = _Fixnodeparent;
                    } else { // must rearrange right subtree
                        if (_Pnode->_Right->_Color == _Black) { // rotate red up from left sub-subtree
                            _Pnode->_Left->_Color = _Black;
                            _Pnode->_Color        = _Red;
                            _Rrotate(_Pnode);
                            _Pnode = _Fixnodeparent->_Right;
                        }

                        _Pnode->_Color         = _Fixnodeparent->_Color;
                        _Fixnodeparent->_Color = _Black;
                        _Pnode->_Right->_Color = _Black;
                        _Lrotate(_Fixnodeparent);
                        break; // tree now recolored/rebalanced
                    }
                } else { // fixup right subtree
                    _Pnode = _Fixnodeparent->_Left;
                    if (_Pnode->_Color == _Red) { // rotate red up from left subtree
                        _Pnode->_Color         = _Black;
                        _Fixnodeparent->_Color = _Red;
                        _Rrotate(_Fixnodeparent);
                        _Pnode = _Fixnodeparent->_Left;
                    }

                    if (_Pnode->_Isnil) {
                        _Fixnode = _Fixnodeparent; // shouldn't happen
                    } else if (_Pnode->_Right->_Color == _Black
                               && _Pnode->_Left->_Color == _Black) { // redden left subtree with black children
                        _Pnode->_Color = _Red;
                        _Fixnode       = _Fixnodeparent;
                    } else { // must rearrange left subtree
                        if (_Pnode->_Left->_Color == _Black) { // rotate red up from right sub-subtree
                            _Pnode->_Right->_Color = _Black;
                            _Pnode->_Color         = _Red;
                            _Lrotate(_Pnode);
                            _Pnode = _Fixnodeparent->_Left;
                        }

                        _Pnode->_Color         = _Fixnodeparent->_Color;
                        _Fixnodeparent->_Color = _Black;
                        _Pnode->_Left->_Color  = _Black;
                        _Rrotate(_Fixnodeparent);
                        break; // tree now recolored/rebalanced
                    }
                }
            }

            _Fixnode->_Color = _Black; // stopping node is black
        }

        if (0 < _Mysize) {
            --_Mysize;
        }

        return _Erasednode;
    }

    _Nodeptr _Insert_node(const _Tree_id<_Nodeptr> _Loc, const _Nodeptr _Newnode) noexcept {
        ++_Mysize;
        const auto _Head  = _Myhead;
        _Newnode->_Parent = _Loc._Parent;
        if (_Loc._Parent == _Head) { // first node in tree, just set head values
            _Head->_Left     = _Newnode;
            _Head->_Parent   = _Newnode;
            _Head->_Right    = _Newnode;
            _Newnode->_Color = _Black; // the root is black
            return _Newnode;
        }

        _STL_INTERNAL_CHECK(_Loc._Child != _Tree_child::_Unused);
        if (_Loc._Child == _Tree_child::_Right) { // add to right of _Loc._Parent
            _STL_INTERNAL_CHECK(_Loc._Parent->_Right->_Isnil);
            _Loc._Parent->_Right = _Newnode;
            if (_Loc._Parent == _Head->_Right) { // remember rightmost node
                _Head->_Right = _Newnode;
            }
        } else { // add to left of _Loc._Parent
            _STL_INTERNAL_CHECK(_Loc._Parent->_Left->_Isnil);
            _Loc._Parent->_Left = _Newnode;
            if (_Loc._Parent == _Head->_Left) { // remember leftmost node
                _Head->_Left = _Newnode;
            }
        }

        for (_Nodeptr _Pnode = _Newnode; _Pnode->_Parent->_Color == _Red;) {
            if (_Pnode->_Parent == _Pnode->_Parent->_Parent->_Left) { // fixup red-red in left subtree
                const auto _Parent_sibling = _Pnode->_Parent->_Parent->_Right;
                if (_Parent_sibling->_Color == _Red) { // parent's sibling has two red children, blacken both
                    _Pnode->_Parent->_Color          = _Black;
                    _Parent_sibling->_Color          = _Black;
                    _Pnode->_Parent->_Parent->_Color = _Red;
                    _Pnode                           = _Pnode->_Parent->_Parent;
                } else { // parent's sibling has red and black children
                    if (_Pnode == _Pnode->_Parent->_Right) { // rotate right child to left
                        _Pnode = _Pnode->_Parent;
                        _Lrotate(_Pnode);
                    }

                    _Pnode->_Parent->_Color          = _Black; // propagate red up
                    _Pnode->_Parent->_Parent->_Color = _Red;
                    _Rrotate(_Pnode->_Parent->_Parent);
                }
            } else { // fixup red-red in right subtree
                const auto _Parent_sibling = _Pnode->_Parent->_Parent->_Left;
                if (_Parent_sibling->_Color == _Red) { // parent's sibling has two red children, blacken both
                    _Pnode->_Parent->_Color          = _Black;
                    _Parent_sibling->_Color          = _Black;
                    _Pnode->_Parent->_Parent->_Color = _Red;
                    _Pnode                           = _Pnode->_Parent->_Parent;
                } else { // parent's sibling has red and black children
                    if (_Pnode == _Pnode->_Parent->_Left) { // rotate left child to right
                        _Pnode = _Pnode->_Parent;
                        _Rrotate(_Pnode);
                    }

                    _Pnode->_Parent->_Color          = _Black; // propagate red up
                    _Pnode->_Parent->_Parent->_Color = _Red;
                    _Lrotate(_Pnode->_Parent->_Parent);
                }
            }
        }

        _Head->_Parent->_Color = _Black; // root is always black
        return _Newnode;
    }

    void _Orphan_ptr(const _Nodeptr _Ptr) noexcept {
#if _ITERATOR_DEBUG_LEVEL == 2
        _Lockit _Lock(_LOCK_DEBUG);
        _Iterator_base12** _Pnext = &this->_Myproxy->_Myfirstiter;
        while (*_Pnext) {
            const auto _Pnextptr = static_cast<const_iterator&>(**_Pnext)._Ptr;
            if (_Pnextptr == _Myhead || (_Ptr != nullptr && _Pnextptr != _Ptr)) {
                _Pnext = &(*_Pnext)->_Mynextiter;
            } else { // orphan the iterator
                (*_Pnext)->_Myproxy = nullptr;
                *_Pnext             = (*_Pnext)->_Mynextiter;
            }
        }
#else // ^^^ _ITERATOR_DEBUG_LEVEL == 2 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 2 vvv
        (void) _Ptr;
#endif // _ITERATOR_DEBUG_LEVEL == 2
    }

    template <class _Alnode>
    void _Erase_tree_and_orphan(_Alnode& _Al, _Nodeptr _Rootnode) noexcept {
        while (!_Rootnode->_Isnil) { // free subtrees, then node
            _Erase_tree_and_orphan(_Al, _Rootnode->_Right);
            auto _To_delete = _STD exchange(_Rootnode, _Rootnode->_Left);
            _Orphan_ptr(_To_delete);
            _Alnode::value_type::_Freenode(_Al, _To_delete);
        }
    }

    template <class _Alnode>
    void _Erase_tree(_Alnode& _Al, _Nodeptr _Rootnode) noexcept {
        while (!_Rootnode->_Isnil) { // free subtrees, then node
            _Erase_tree(_Al, _Rootnode->_Right);
            _Alnode::value_type::_Freenode(_Al, _STD exchange(_Rootnode, _Rootnode->_Left));
        }
    }

    template <class _Alnode>
    void _Erase_head(_Alnode& _Al) noexcept {
        this->_Orphan_all();
        _Erase_tree(_Al, _Myhead->_Parent);
        _Alnode::value_type::_Freenode0(_Al, _Myhead);
    }

    _Nodeptr _Myhead; // pointer to head node
    size_type _Mysize; // number of elements
};

template <class _Alnode, class _Scary_val>
struct _Tree_head_scoped_ptr { // temporary storage for allocated node pointers to ensure exception safety
    _Alnode& _Al;
    _Scary_val* _Mycont;

    _Tree_head_scoped_ptr(_Alnode& _Al_, _Scary_val& _Mycont_) : _Al(_Al_), _Mycont(_STD addressof(_Mycont_)) {
        _Mycont->_Myhead = _Alnode::value_type::_Buyheadnode(_Al);
    }

    void _Release() noexcept {
        _Mycont = nullptr;
    }

    ~_Tree_head_scoped_ptr() {
        if (_Mycont) {
            _Mycont->_Erase_head(_Al);
        }
    }
};

template <class _Alnode>
struct _Tree_temp_node_alloc : _Alloc_construct_ptr<_Alnode> {
    // EH helper for _Tree_temp_node
    explicit _Tree_temp_node_alloc(_Alnode& _Al_) : _Alloc_construct_ptr<_Alnode>(_Al_) {
        _Alloc_construct_ptr<_Alnode>::_Allocate();
    }

    _Tree_temp_node_alloc(const _Tree_temp_node_alloc&)            = delete;
    _Tree_temp_node_alloc& operator=(const _Tree_temp_node_alloc&) = delete;
};

template <class _Alnode>
struct _Tree_temp_node : _Tree_temp_node_alloc<_Alnode> {
    // temporarily stores a constructed tree node
    using _Alnode_traits = allocator_traits<_Alnode>;
    using _Nodeptr       = typename _Alnode_traits::pointer;

    enum _Redbl { // colors for link to parent
        _Red,
        _Black
    };

    template <class... _Valtys>
    explicit _Tree_temp_node(_Alnode& _Al_, _Nodeptr _Myhead, _Valtys&&... _Vals)
        : _Tree_temp_node_alloc<_Alnode>(_Al_) {
        _Alnode_traits::construct(this->_Al, _STD addressof(this->_Ptr->_Myval), _STD forward<_Valtys>(_Vals)...);
        _Construct_in_place(this->_Ptr->_Left, _Myhead);
        _Construct_in_place(this->_Ptr->_Parent, _Myhead);
        _Construct_in_place(this->_Ptr->_Right, _Myhead);
        this->_Ptr->_Color = _Red;
        this->_Ptr->_Isnil = false;
    }

    _Tree_temp_node(const _Tree_temp_node&)            = delete;
    _Tree_temp_node& operator=(const _Tree_temp_node&) = delete;

    ~_Tree_temp_node() {
        if (this->_Ptr) {
            _Destroy_in_place(this->_Ptr->_Left);
            _Destroy_in_place(this->_Ptr->_Parent);
            _Destroy_in_place(this->_Ptr->_Right);
            _Alnode_traits::destroy(this->_Al, _STD addressof(this->_Ptr->_Myval));
        }
    }
};

template <class _Traits>
class _Tree { // ordered red-black tree for map/multimap/set/multiset
public:
    using key_type       = typename _Traits::key_type;
    using value_type     = typename _Traits::value_type;
    using allocator_type = typename _Traits::allocator_type;

protected:
    using _Alty          = _Rebind_alloc_t<allocator_type, value_type>;
    using _Alty_traits   = allocator_traits<_Alty>;
    using _Node          = _Tree_node<value_type, typename _Alty_traits::void_pointer>;
    using _Alnode        = _Rebind_alloc_t<allocator_type, _Node>;
    using _Alnode_traits = allocator_traits<_Alnode>;
    using _Nodeptr       = typename _Alnode_traits::pointer;

    using _Scary_val = _Tree_val<conditional_t<_Is_simple_alloc_v<_Alnode>, _Tree_simple_types<value_type>,
        _Tree_iter_types<value_type, typename _Alty_traits::size_type, typename _Alty_traits::difference_type,
            typename _Alty_traits::pointer, typename _Alty_traits::const_pointer, value_type&, const value_type&,
            _Nodeptr>>>;

    static constexpr bool _Multi  = _Traits::_Multi;
    static constexpr bool _Is_set = is_same_v<key_type, value_type>;

    enum _Redbl { // colors for link to parent
        _Red,
        _Black
    };

public:
    using value_compare = typename _Traits::value_compare;

    using key_compare = typename _Traits::key_compare;

    using size_type       = typename _Alty_traits::size_type;
    using difference_type = typename _Alty_traits::difference_type;
    using pointer         = typename _Alty_traits::pointer;
    using const_pointer   = typename _Alty_traits::const_pointer;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator       = conditional_t<_Is_set, _Tree_const_iterator<_Scary_val>, _Tree_iterator<_Scary_val>>;
    using const_iterator = _Tree_const_iterator<_Scary_val>;
    using _Unchecked_iterator =
        conditional_t<_Is_set, _Tree_unchecked_const_iterator<_Scary_val>, _Tree_unchecked_iterator<_Scary_val>>;
    using _Unchecked_const_iterator = _Tree_unchecked_const_iterator<_Scary_val>;

    using reverse_iterator       = _STD reverse_iterator<iterator>;
    using const_reverse_iterator = _STD reverse_iterator<const_iterator>;

    enum class _Strategy : bool {
        _Copy,
        _Move,
    };

    _Tree(const key_compare& _Parg) : _Mypair(_One_then_variadic_args_t{}, _Parg, _Zero_then_variadic_args_t{}) {
        _Alloc_sentinel_and_proxy();
    }

    _Tree(const key_compare& _Parg, const allocator_type& _Al)
        : _Mypair(_One_then_variadic_args_t{}, _Parg, _One_then_variadic_args_t{}, _Al) {
        _Alloc_sentinel_and_proxy();
    }

    template <class _Any_alloc>
    _Tree(const _Tree& _Right, _Any_alloc&& _Al)
        : _Mypair(_One_then_variadic_args_t{}, _Right.key_comp(), _One_then_variadic_args_t{},
            _STD forward<_Any_alloc>(_Al)) {
        auto&& _Alproxy   = _GET_PROXY_ALLOCATOR(_Alnode, _Getal());
        const auto _Scary = _Get_scary();
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, *_Scary);
        _Tree_head_scoped_ptr<_Alnode, _Scary_val> _Sentinel(_Getal(), *_Scary);
        _Copy<_Strategy::_Copy>(_Right);
        _Sentinel._Release();
        _Proxy._Release();
    }

    _Tree(_Tree&& _Right)
        : _Mypair(_One_then_variadic_args_t{}, _Right.key_comp(), // intentionally copy comparator, see LWG-2227
            _One_then_variadic_args_t{}, _STD move(_Right._Getal())) {
        _Alloc_sentinel_and_proxy();
        _Swap_val_excluding_comp(_Right);
    }

private:
    void _Different_allocator_move_construct(_Tree&& _Right) {
        // TRANSITION, VSO-761321 (inline into only caller when that is fixed)
        auto&& _Alproxy   = _GET_PROXY_ALLOCATOR(_Alnode, _Getal());
        const auto _Scary = _Get_scary();
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, *_Scary);
        _Tree_head_scoped_ptr<_Alnode, _Scary_val> _Sentinel(_Getal(), *_Scary);
        _Copy<_Strategy::_Move>(_Right);
        _Sentinel._Release();
        _Proxy._Release();
    }

public:
    _Tree(_Tree&& _Right, const allocator_type& _Al)
        : _Mypair(_One_then_variadic_args_t{}, _Right.key_comp(), // intentionally copy comparator, see LWG-2227
            _One_then_variadic_args_t{}, _Al) {
        if constexpr (!_Alnode_traits::is_always_equal::value) {
            if (_Getal() != _Right._Getal()) {
                _Different_allocator_move_construct(_STD move(_Right));
                return;
            }
        }

        _Alloc_sentinel_and_proxy();
        _Swap_val_excluding_comp(_Right);
    }

    _Tree& operator=(_Tree&& _Right) noexcept(
        _Choose_pocma_v<_Alnode> == _Pocma_values::_Equal_allocators && is_nothrow_move_assignable_v<key_compare>) {
        if (this == _STD addressof(_Right)) {
            return *this;
        }

        auto& _Al                 = _Getal();
        auto& _Right_al           = _Right._Getal();
        constexpr auto _Pocma_val = _Choose_pocma_v<_Alnode>;
        if constexpr (_Pocma_val == _Pocma_values::_Propagate_allocators) {
            if (_Al != _Right_al) {
                clear();
                _Getcomp()            = _Right._Getcomp(); // intentionally copy comparator, see LWG-2227
                auto&& _Alproxy       = _GET_PROXY_ALLOCATOR(_Alnode, _Al);
                auto&& _Right_alproxy = _GET_PROXY_ALLOCATOR(_Alnode, _Right_al);
                _Container_proxy_ptr<_Alty> _Proxy(_Right_alproxy, _Leave_proxy_unbound{});
                const auto _Scary       = _Get_scary();
                const auto _Right_scary = _Right._Get_scary();
                const auto _Newhead     = _STD exchange(_Right_scary->_Myhead, _Node::_Buyheadnode(_Right_al));
                const auto _Newsize     = _STD exchange(_Right_scary->_Mysize, size_type{0});
                _Scary->_Erase_head(_Al);
                _Pocma(_Al, _Right_al);
                _Scary->_Myhead = _Newhead;
                _Scary->_Mysize = _Newsize;
                _Proxy._Bind(_Alproxy, _Scary);
                _Scary->_Swap_proxy_and_iterators(*_Right_scary);
                return *this;
            }
        } else if constexpr (_Pocma_val == _Pocma_values::_No_propagate_allocators) {
            if (_Al != _Right_al) {
                clear();
                _Getcomp() = _Right._Getcomp(); // intentionally copy comparator, see LWG-2227
                _Copy<_Strategy::_Move>(_Right);
                return *this;
            }
        }

        clear();
        _Getcomp() = _Right._Getcomp(); // intentionally copy comparator, see LWG-2227
        _Pocma(_Al, _Right_al);
        _Swap_val_excluding_comp(_Right);

        return *this;
    }

private:
    void _Swap_val_excluding_comp(_Tree& _Right) { // swap contents (except comparator) with _Right, equal allocators
        const auto _Scary       = _Get_scary();
        const auto _Right_scary = _Right._Get_scary();
        _Scary->_Swap_proxy_and_iterators(*_Right_scary);
        _Swap_adl(_Scary->_Myhead, _Right_scary->_Myhead);
        _STD swap(_Scary->_Mysize, _Right_scary->_Mysize);
    }

protected:
    template <class... _Valtys>
    pair<_Nodeptr, bool> _Emplace(_Valtys&&... _Vals) {
        using _In_place_key_extractor = typename _Traits::template _In_place_key_extractor<_Remove_cvref_t<_Valtys>...>;
        const auto _Scary             = _Get_scary();
        _Tree_find_result<_Nodeptr> _Loc;
        _Nodeptr _Inserted;
        if constexpr (!_Multi && _In_place_key_extractor::_Extractable) {
            const auto& _Keyval = _In_place_key_extractor::_Extract(_Vals...);
            _Loc                = _Find_lower_bound(_Keyval);
            if (_Lower_bound_duplicate(_Loc._Bound, _Keyval)) {
                return {_Loc._Bound, false};
            }

            _Check_grow_by_1();
            _Inserted = _Tree_temp_node<_Alnode>(_Getal(), _Scary->_Myhead, _STD forward<_Valtys>(_Vals)...)._Release();
            // nothrow hereafter
        } else {
            _Tree_temp_node<_Alnode> _Newnode(_Getal(), _Scary->_Myhead, _STD forward<_Valtys>(_Vals)...);
            const auto& _Keyval = _Traits::_Kfn(_Newnode._Ptr->_Myval);
            if constexpr (_Multi) { // duplicate check if unique
                _Loc = _Find_upper_bound(_Keyval);
            } else {
                _Loc = _Find_lower_bound(_Keyval);
                if (_Lower_bound_duplicate(_Loc._Bound, _Keyval)) {
                    return {_Loc._Bound, false};
                }
            }

            _Check_grow_by_1();
            // nothrow hereafter
            _Inserted = _Newnode._Release();
        }

        return {_Scary->_Insert_node(_Loc._Location, _Inserted), true};
    }

public:
    template <class... _Valtys>
    pair<iterator, bool> emplace(_Valtys&&... _Vals) {
        const auto _Result = _Emplace(_STD forward<_Valtys>(_Vals)...);
        return {iterator(_Result.first, _Get_scary()), _Result.second};
    }

protected:
    template <class... _Valtys>
    _Nodeptr _Emplace_hint(const _Nodeptr _Hint, _Valtys&&... _Vals) {
        using _In_place_key_extractor = typename _Traits::template _In_place_key_extractor<_Remove_cvref_t<_Valtys>...>;
        const auto _Scary             = _Get_scary();
        _Tree_find_hint_result<_Nodeptr> _Loc;
        _Nodeptr _Inserted;
        if constexpr (!_Multi && _In_place_key_extractor::_Extractable) {
            _Loc = _Find_hint(_Hint, _In_place_key_extractor::_Extract(_Vals...));
            if (_Loc._Duplicate) {
                return _Loc._Location._Parent;
            }

            _Check_grow_by_1();
            _Inserted = _Tree_temp_node<_Alnode>(_Getal(), _Scary->_Myhead, _STD forward<_Valtys>(_Vals)...)._Release();
            // nothrow hereafter
        } else {
            _Tree_temp_node<_Alnode> _Newnode(_Getal(), _Scary->_Myhead, _STD forward<_Valtys>(_Vals)...);
            _Loc = _Find_hint(_Hint, _Traits::_Kfn(_Newnode._Ptr->_Myval));
            if constexpr (!_Multi) {
                if (_Loc._Duplicate) {
                    return _Loc._Location._Parent;
                }
            }

            _Check_grow_by_1();
            // nothrow hereafter
            _Inserted = _Newnode._Release();
        }

        return _Scary->_Insert_node(_Loc._Location, _Inserted);
    }

public:
    template <class... _Valtys>
    iterator emplace_hint(const_iterator _Where, _Valtys&&... _Vals) { // insert value_type(_Val...) at _Where
        const auto _Scary = _Get_scary();
#if _ITERATOR_DEBUG_LEVEL == 2
        _STL_VERIFY(_Where._Getcont() == _Scary, "map/set insert iterator outside range");
#endif // _ITERATOR_DEBUG_LEVEL == 2
        return iterator(_Emplace_hint(_Where._Ptr, _STD forward<_Valtys>(_Vals)...), _Scary);
    }

    ~_Tree() noexcept {
        const auto _Scary = _Get_scary();
        _Scary->_Erase_head(_Getal());
#if _ITERATOR_DEBUG_LEVEL != 0 // TRANSITION, ABI
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alnode, _Getal());
        _Delete_plain_internal(_Alproxy, _Scary->_Myproxy);
#endif // _ITERATOR_DEBUG_LEVEL != 0
    }

public:
    _Tree& operator=(const _Tree& _Right) {
        if (this == _STD addressof(_Right)) {
            return *this;
        }

        auto& _Al       = _Getal();
        auto& _Right_al = _Right._Getal();
        if constexpr (_Choose_pocca_v<_Alnode>) {
            if (_Al != _Right_al) {
                clear();
                const auto _Scary = _Get_scary();
                _Scary->_Orphan_all();
                auto&& _Alproxy       = _GET_PROXY_ALLOCATOR(_Alnode, _Al);
                auto&& _Right_alproxy = _GET_PROXY_ALLOCATOR(_Alnode, _Right_al);
                _Container_proxy_ptr<_Alty> _Proxy(_Right_alproxy, _Leave_proxy_unbound{});
                auto _Right_al_non_const = _Right_al;
                auto _Newhead            = _Node::_Buyheadnode(_Right_al_non_const);
                _Node::_Freenode0(_Al, _Scary->_Myhead);
                _Pocca(_Al, _Right_al);
                _Scary->_Myhead = _Newhead;
                _Proxy._Bind(_Alproxy, _Scary);
                _Getcomp() = _Right._Getcomp();
                _Copy<_Strategy::_Copy>(_Right);
                return *this;
            }
        }

        clear();
        _Getcomp() = _Right._Getcomp();
        _Pocca(_Al, _Right_al);
        _Copy<_Strategy::_Copy>(_Right);

        return *this;
    }

    _NODISCARD iterator begin() noexcept {
        const auto _Scary = _Get_scary();
        return iterator(_Scary->_Myhead->_Left, _Scary);
    }

    _NODISCARD const_iterator begin() const noexcept {
        const auto _Scary = _Get_scary();
        return const_iterator(_Scary->_Myhead->_Left, _Scary);
    }

    _NODISCARD iterator end() noexcept {
        const auto _Scary = _Get_scary();
        return iterator(_Scary->_Myhead, _Scary);
    }

    _NODISCARD const_iterator end() const noexcept {
        const auto _Scary = _Get_scary();
        return const_iterator(_Scary->_Myhead, _Scary);
    }

    _Unchecked_iterator _Unchecked_begin() noexcept {
        return _Unchecked_iterator(_Get_scary()->_Myhead->_Left, nullptr);
    }

    _Unchecked_const_iterator _Unchecked_begin() const noexcept {
        return _Unchecked_const_iterator(_Get_scary()->_Myhead->_Left, nullptr);
    }

    _Default_sentinel _Unchecked_end() const noexcept {
        return {};
    }

    _Unchecked_const_iterator _Unchecked_end_iter() const noexcept {
        return _Unchecked_const_iterator(_Get_scary()->_Myhead, nullptr);
    }

    _NODISCARD reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    _NODISCARD const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    _NODISCARD reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    _NODISCARD const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    _NODISCARD const_iterator cbegin() const noexcept {
        return begin();
    }

    _NODISCARD const_iterator cend() const noexcept {
        return end();
    }

    _NODISCARD const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }

    _NODISCARD const_reverse_iterator crend() const noexcept {
        return rend();
    }

    _NODISCARD size_type size() const noexcept {
        return _Get_scary()->_Mysize;
    }

    _NODISCARD size_type max_size() const noexcept {
        return (_STD min)(
            static_cast<size_type>((numeric_limits<difference_type>::max)()), _Alnode_traits::max_size(_Getal()));
    }

    _NODISCARD_EMPTY_MEMBER bool empty() const noexcept {
        return _Get_scary()->_Mysize == 0;
    }

    _NODISCARD allocator_type get_allocator() const noexcept {
        return static_cast<allocator_type>(_Getal());
    }

    _NODISCARD key_compare key_comp() const {
        return _Getcomp();
    }

    _NODISCARD value_compare value_comp() const {
        return value_compare(key_comp());
    }

    template <bool _Multi2 = _Multi, enable_if_t<!_Multi2, int> = 0>
    pair<iterator, bool> insert(const value_type& _Val) {
        const auto _Result = _Emplace(_Val);
        return {iterator(_Result.first, _Get_scary()), _Result.second};
    }

    template <bool _Multi2 = _Multi, enable_if_t<_Multi2, int> = 0>
    iterator insert(const value_type& _Val) {
        return iterator(_Emplace(_Val).first, _Get_scary());
    }

    template <bool _Multi2 = _Multi, enable_if_t<!_Multi2, int> = 0>
    pair<iterator, bool> insert(value_type&& _Val) {
        const auto _Result = _Emplace(_STD move(_Val));
        return {iterator(_Result.first, _Get_scary()), _Result.second};
    }

    template <bool _Multi2 = _Multi, enable_if_t<_Multi2, int> = 0>
    iterator insert(value_type&& _Val) {
        return iterator(_Emplace(_STD move(_Val)).first, _Get_scary());
    }

    iterator insert(const_iterator _Where, const value_type& _Val) {
        const auto _Scary = _Get_scary();
#if _ITERATOR_DEBUG_LEVEL == 2
        _STL_VERIFY(_Where._Getcont() == _Scary, "map/set insert iterator outside range");
#endif // _ITERATOR_DEBUG_LEVEL == 2
        return iterator(_Emplace_hint(_Where._Ptr, _Val), _Scary);
    }

    iterator insert(const_iterator _Where, value_type&& _Val) {
        const auto _Scary = _Get_scary();
#if _ITERATOR_DEBUG_LEVEL == 2
        _STL_VERIFY(_Where._Getcont() == _Scary, "map/set insert iterator outside range");
#endif // _ITERATOR_DEBUG_LEVEL == 2
        return iterator(_Emplace_hint(_Where._Ptr, _STD move(_Val)), _Scary);
    }

protected:
    template <class _Iter, class _Sent>
    void _Insert_range_unchecked(_Iter _First, const _Sent _Last) {
        const auto _Myhead = _Get_scary()->_Myhead;
        for (; _First != _Last; ++_First) {
            _Emplace_hint(_Myhead, *_First);
        }
    }

public:
    template <class _Iter>
    void insert(_Iter _First, _Iter _Last) {
        _Adl_verify_range(_First, _Last);
        _Insert_range_unchecked(_Get_unwrapped(_First), _Get_unwrapped(_Last));
    }

#if _HAS_CXX23 && defined(__cpp_lib_concepts) // TRANSITION, GH-395
    template <_Container_compatible_range<value_type> _Rng>
    void insert_range(_Rng&& _Range) {
        _Insert_range_unchecked(_RANGES _Ubegin(_Range), _RANGES _Uend(_Range));
    }
#endif // _HAS_CXX23 && defined(__cpp_lib_concepts)

    void insert(initializer_list<value_type> _Ilist) {
        _Insert_range_unchecked(_Ilist.begin(), _Ilist.end());
    }

private:
    _Nodeptr _Erase_unchecked(_Unchecked_const_iterator _Where) noexcept {
        const auto _Scary                    = _Get_scary();
        _Unchecked_const_iterator _Successor = _Where;
        ++_Successor; // save successor iterator for return
        _Nodeptr _Erasednode = _Scary->_Extract(_Where); // node to erase
        _Scary->_Orphan_ptr(_Erasednode);
        _Node::_Freenode(_Getal(), _Erasednode); // delete erased node
        return _Successor._Ptr; // return successor nodeptr
    }

    _Nodeptr _Erase_unchecked(_Unchecked_const_iterator _First, _Unchecked_const_iterator _Last) noexcept {
        const auto _Begin = _Unchecked_begin();
        if (_First == _Begin && _Last._Ptr->_Isnil) {
            // erase all
            clear();
            return _Last._Ptr;
        }

        // partial erase, one at a time
        while (_First != _Last) {
            _Erase_unchecked(_First++);
        }

        return _Last._Ptr;
    }

    size_type _Erase(const pair<_Nodeptr, _Nodeptr> _Where) noexcept {
        const _Unchecked_const_iterator _First(_Where.first, nullptr);
        const _Unchecked_const_iterator _Last(_Where.second, nullptr);
        const auto _Num = static_cast<size_type>(_STD distance(_First, _Last));
        _Erase_unchecked(_First, _Last);
        return _Num;
    }

public:
    template <class _Iter = iterator, enable_if_t<!is_same_v<_Iter, const_iterator>, int> = 0>
    iterator erase(iterator _Where) noexcept /* strengthened */ {
        const auto _Scary = _Get_scary();
#if _ITERATOR_DEBUG_LEVEL == 2
        _STL_VERIFY(_Where._Getcont() == _Scary, "map/set erase iterator from incorrect container");
        _STL_VERIFY(!_Where._Ptr->_Isnil, "cannot erase map/set end() iterator");
#endif // _ITERATOR_DEBUG_LEVEL == 2
        return iterator(_Erase_unchecked(_Where._Unwrapped()), _Scary);
    }

    iterator erase(const_iterator _Where) noexcept /* strengthened */ {
        const auto _Scary = _Get_scary();
#if _ITERATOR_DEBUG_LEVEL == 2
        _STL_VERIFY(_Where._Getcont() == _Scary, "map/set erase iterator from incorrect container");
        _STL_VERIFY(!_Where._Ptr->_Isnil, "cannot erase map/set end() iterator");
#endif // _ITERATOR_DEBUG_LEVEL == 2
        return iterator(_Erase_unchecked(_Where._Unwrapped()), _Scary);
    }

    iterator erase(const_iterator _First, const_iterator _Last) noexcept /* strengthened */ {
        return iterator(_Erase_unchecked(_First._Unwrapped(), _Last._Unwrapped()), _Get_scary());
    }

    size_type erase(const key_type& _Keyval) noexcept(noexcept(_Eqrange(_Keyval))) /* strengthened */ {
        return _Erase(_Eqrange(_Keyval));
    }

#if _HAS_CXX23
    template <class _Kx, class _Mycomp = key_compare, class = typename _Mycomp::is_transparent,
        enable_if_t<!disjunction_v<is_convertible<_Kx, const_iterator>, is_convertible<_Kx, iterator>>, int> = 0>
    size_type erase(_Kx&& _Keyval) noexcept(noexcept(_Eqrange(_Keyval))) /* strengthened */ {
        return _Erase(_Eqrange(_Keyval));
    }
#endif // _HAS_CXX23

    void clear() noexcept {
        const auto _Scary = _Get_scary();
        _Scary->_Orphan_ptr(nullptr);
        auto _Head = _Scary->_Myhead;
        _Scary->_Erase_tree(_Getal(), _Head->_Parent);
        _Head->_Parent  = _Head;
        _Head->_Left    = _Head;
        _Head->_Right   = _Head;
        _Scary->_Mysize = 0;
    }

private:
    template <class _Other>
    _NODISCARD _Nodeptr _Find(const _Other& _Keyval) const {
        const _Tree_find_result<_Nodeptr> _Loc = _Find_lower_bound(_Keyval);
        if (_Lower_bound_duplicate(_Loc._Bound, _Keyval)) {
            return _Loc._Bound;
        }

        return _Get_scary()->_Myhead;
    }

public:
    _NODISCARD iterator find(const key_type& _Keyval) {
        return iterator(_Find(_Keyval), _Get_scary());
    }

    _NODISCARD const_iterator find(const key_type& _Keyval) const {
        return const_iterator(_Find(_Keyval), _Get_scary());
    }

    template <class _Other, class _Mycomp = key_compare, class = typename _Mycomp::is_transparent>
    _NODISCARD iterator find(const _Other& _Keyval) {
        return iterator(_Find(_Keyval), _Get_scary());
    }

    template <class _Other, class _Mycomp = key_compare, class = typename _Mycomp::is_transparent>
    _NODISCARD const_iterator find(const _Other& _Keyval) const {
        return const_iterator(_Find(_Keyval), _Get_scary());
    }

#if _HAS_CXX20
    _NODISCARD bool contains(const key_type& _Keyval) const {
        return _Lower_bound_duplicate(_Find_lower_bound(_Keyval)._Bound, _Keyval);
    }

    template <class _Other, class _Mycomp = key_compare, class = typename _Mycomp::is_transparent>
    _NODISCARD bool contains(const _Other& _Keyval) const {
        return _Lower_bound_duplicate(_Find_lower_bound(_Keyval)._Bound, _Keyval);
    }
#endif // _HAS_CXX20

    _NODISCARD size_type count(const key_type& _Keyval) const {
        if constexpr (_Multi) {
            const auto _Ans = _Eqrange(_Keyval);
            return static_cast<size_type>(_STD distance(
                _Unchecked_const_iterator(_Ans.first, nullptr), _Unchecked_const_iterator(_Ans.second, nullptr)));
        } else {
            return _Lower_bound_duplicate(_Find_lower_bound(_Keyval)._Bound, _Keyval);
        }
    }

    template <class _Other, class _Mycomp = key_compare, class = typename _Mycomp::is_transparent>
    _NODISCARD size_type count(const _Other& _Keyval) const {
        const auto _Ans = _Eqrange(_Keyval);
        return static_cast<size_type>(_STD distance(
            _Unchecked_const_iterator(_Ans.first, nullptr), _Unchecked_const_iterator(_Ans.second, nullptr)));
    }

    _NODISCARD iterator lower_bound(const key_type& _Keyval) {
        return iterator(_Find_lower_bound(_Keyval)._Bound, _Get_scary());
    }

    _NODISCARD const_iterator lower_bound(const key_type& _Keyval) const {
        return const_iterator(_Find_lower_bound(_Keyval)._Bound, _Get_scary());
    }

    template <class _Other, class _Mycomp = key_compare, class = typename _Mycomp::is_transparent>
    _NODISCARD iterator lower_bound(const _Other& _Keyval) {
        return iterator(_Find_lower_bound(_Keyval)._Bound, _Get_scary());
    }

    template <class _Other, class _Mycomp = key_compare, class = typename _Mycomp::is_transparent>
    _NODISCARD const_iterator lower_bound(const _Other& _Keyval) const {
        return const_iterator(_Find_lower_bound(_Keyval)._Bound, _Get_scary());
    }

    _NODISCARD iterator upper_bound(const key_type& _Keyval) {
        return iterator(_Find_upper_bound(_Keyval)._Bound, _Get_scary());
    }

    _NODISCARD const_iterator upper_bound(const key_type& _Keyval) const {
        return const_iterator(_Find_upper_bound(_Keyval)._Bound, _Get_scary());
    }

    template <class _Other, class _Mycomp = key_compare, class = typename _Mycomp::is_transparent>
    _NODISCARD iterator upper_bound(const _Other& _Keyval) {
        return iterator(_Find_upper_bound(_Keyval)._Bound, _Get_scary());
    }

    template <class _Other, class _Mycomp = key_compare, class = typename _Mycomp::is_transparent>
    _NODISCARD const_iterator upper_bound(const _Other& _Keyval) const {
        return const_iterator(_Find_upper_bound(_Keyval)._Bound, _Get_scary());
    }

    _NODISCARD pair<iterator, iterator> equal_range(const key_type& _Keyval) {
        const auto _Result = _Eqrange(_Keyval);
        const auto _Scary  = _Get_scary();
        return {iterator(_Result.first, _Scary), iterator(_Result.second, _Scary)};
    }

    _NODISCARD pair<const_iterator, const_iterator> equal_range(const key_type& _Keyval) const {
        const auto _Result = _Eqrange(_Keyval);
        const auto _Scary  = _Get_scary();
        return {const_iterator(_Result.first, _Scary), const_iterator(_Result.second, _Scary)};
    }

    template <class _Other, class _Mycomp = key_compare, class = typename _Mycomp::is_transparent>
    _NODISCARD pair<iterator, iterator> equal_range(const _Other& _Keyval) {
        const auto _Result = _Eqrange(_Keyval);
        const auto _Scary  = _Get_scary();
        return {iterator(_Result.first, _Scary), iterator(_Result.second, _Scary)};
    }

    template <class _Other, class _Mycomp = key_compare, class = typename _Mycomp::is_transparent>
    _NODISCARD pair<const_iterator, const_iterator> equal_range(const _Other& _Keyval) const {
        const auto _Result = _Eqrange(_Keyval);
        const auto _Scary  = _Get_scary();
        return {const_iterator(_Result.first, _Scary), const_iterator(_Result.second, _Scary)};
    }

    void swap(_Tree& _Right) noexcept(_Is_nothrow_swappable<key_compare>::value) /* strengthened */ {
        if (this != _STD addressof(_Right)) {
            _Pocs(_Getal(), _Right._Getal());
            _Swap_val_excluding_comp(_Right);
            _Swap_adl(_Getcomp(), _Right._Getcomp());
        }
    }

protected:
    template <class _Keyty>
    _Tree_find_hint_result<_Nodeptr> _Find_hint(const _Nodeptr _Hint, const _Keyty& _Keyval) const {
        const auto& _Comp = _Getcomp();
        const auto _Head  = _Get_scary()->_Myhead;
        if constexpr (_Multi) { // insert even if duplicate
            if (_Hint->_Isnil) {
                // insert at end if greater than or equal to last element
                if (_Head->_Parent->_Isnil || !_DEBUG_LT_PRED(_Comp, _Keyval, _Traits::_Kfn(_Head->_Right->_Myval))) {
                    return {{_Head->_Right, _Tree_child::_Right}, false};
                }

                // _Hint is end(); it must be closer to the end of equivalents
                return {_Find_upper_bound(_Keyval)._Location, false};
            }

            if (_Hint == _Head->_Left) {
                // insert at beginning if less than or equal to first element
                if (!_DEBUG_LT_PRED(_Comp, _Traits::_Kfn(_Hint->_Myval), _Keyval)) {
                    return {{_Hint, _Tree_child::_Left}, false};
                }

                // _Hint is begin(); it must be closer to the beginning of equivalents
                return {_Find_lower_bound(_Keyval)._Location, false};
            }

            if (!_DEBUG_LT_PRED(_Comp, _Traits::_Kfn(_Hint->_Myval), _Keyval)) {
                // _Val <= *_Hint
                const auto _Prev = (--(_Unchecked_const_iterator(_Hint, nullptr)))._Ptr;
                if (!_DEBUG_LT_PRED(_Comp, _Keyval, _Traits::_Kfn(_Prev->_Myval))) {
                    // _Val <= *_Hint and _Val >= *prev(_Hint); insert here
                    if (_Prev->_Right->_Isnil) {
                        return {{_Prev, _Tree_child::_Right}, false};
                    } else {
                        return {{_Hint, _Tree_child::_Left}, false};
                    }
                }

                // _Val goes before *_Hint; _Hint must be closer to the end of equivalents
                return {_Find_upper_bound(_Keyval)._Location, false};
            }

            // _Val goes after *_Hint; _Hint must be closer to the beginning of equivalents
            return {_Find_lower_bound(_Keyval)._Location, false};
        } else { // insert only if unique
            if (_Hint->_Isnil) { // insert at end if after last element
                // insert at end if greater than last element
                if (_Head->_Parent->_Isnil || _DEBUG_LT_PRED(_Comp, _Traits::_Kfn(_Head->_Right->_Myval), _Keyval)) {
                    return {{_Head->_Right, _Tree_child::_Right}, false};
                }
            } else if (_Hint == _Head->_Left) {
                // insert at beginning if less than first element
                if (_DEBUG_LT_PRED(_Comp, _Keyval, _Traits::_Kfn(_Hint->_Myval))) {
                    return {{_Hint, _Tree_child::_Left}, false};
                }
            } else if (_DEBUG_LT_PRED(_Comp, _Keyval, _Traits::_Kfn(_Hint->_Myval))) {
                // _Val < *_Hint
                const auto _Prev = (--(_Unchecked_const_iterator(_Hint, nullptr)))._Ptr;
                if (_DEBUG_LT_PRED(_Comp, _Traits::_Kfn(_Prev->_Myval), _Keyval)) {
                    // _Val < *_Hint and _Val > *prev(_Hint); insert here
                    if (_Prev->_Right->_Isnil) {
                        return {{_Prev, _Tree_child::_Right}, false};
                    } else {
                        return {{_Hint, _Tree_child::_Left}, false};
                    }
                }
            } else if (_DEBUG_LT_PRED(_Comp, _Traits::_Kfn(_Hint->_Myval), _Keyval)) {
                // _Val > *_Hint
                const auto _Next = (++(_Unchecked_const_iterator(_Hint, nullptr)))._Ptr;
                if (_Next->_Isnil || _DEBUG_LT_PRED(_Comp, _Keyval, _Traits::_Kfn(_Next->_Myval))) {
                    // _Val > *_Hint and _Val < *next(_Hint); insert here
                    if (_Hint->_Right->_Isnil) {
                        return {{_Hint, _Tree_child::_Right}, false};
                    }

                    return {{_Next, _Tree_child::_Left}, false};
                }
            } else {
                // _Val is equivalent to *_Hint; don't insert
                return {{_Hint, _Tree_child::_Right}, true};
            }

            const auto _Loc = _Find_lower_bound(_Keyval);
            if (_Lower_bound_duplicate(_Loc._Bound, _Keyval)) {
                return {{_Loc._Bound, _Tree_child::_Unused}, true};
            }

            return {_Loc._Location, false};
        }
    }

    template <class _Keyty>
    _Tree_find_result<_Nodeptr> _Find_upper_bound(const _Keyty& _Keyval) const {
        const auto _Scary = _Get_scary();
        _Tree_find_result<_Nodeptr> _Result{{_Scary->_Myhead->_Parent, _Tree_child::_Right}, _Scary->_Myhead};
        _Nodeptr _Trynode = _Result._Location._Parent;
        while (!_Trynode->_Isnil) {
            _Result._Location._Parent = _Trynode;
            if (_DEBUG_LT_PRED(_Getcomp(), _Keyval, _Traits::_Kfn(_Trynode->_Myval))) {
                _Result._Location._Child = _Tree_child::_Left;
                _Result._Bound           = _Trynode;
                _Trynode                 = _Trynode->_Left;
            } else {
                _Result._Location._Child = _Tree_child::_Right;
                _Trynode                 = _Trynode->_Right;
            }
        }

        return _Result;
    }

    template <class _Keyty>
    bool _Lower_bound_duplicate(const _Nodeptr _Bound, const _Keyty& _Keyval) const {
        return !_Bound->_Isnil && !_DEBUG_LT_PRED(_Getcomp(), _Keyval, _Traits::_Kfn(_Bound->_Myval));
    }

    template <class _Keyty>
    _Tree_find_result<_Nodeptr> _Find_lower_bound(const _Keyty& _Keyval) const {
        const auto _Scary = _Get_scary();
        _Tree_find_result<_Nodeptr> _Result{{_Scary->_Myhead->_Parent, _Tree_child::_Right}, _Scary->_Myhead};
        _Nodeptr _Trynode = _Result._Location._Parent;
        while (!_Trynode->_Isnil) {
            _Result._Location._Parent = _Trynode;
            if (_DEBUG_LT_PRED(_Getcomp(), _Traits::_Kfn(_Trynode->_Myval), _Keyval)) {
                _Result._Location._Child = _Tree_child::_Right;
                _Trynode                 = _Trynode->_Right;
            } else {
                _Result._Location._Child = _Tree_child::_Left;
                _Result._Bound           = _Trynode;
                _Trynode                 = _Trynode->_Left;
            }
        }

        return _Result;
    }

    void _Check_grow_by_1() {
        if (max_size() == _Get_scary()->_Mysize) {
            _Throw_tree_length_error();
        }
    }

    template <_Strategy _Strat>
    void _Copy(const _Tree& _Right) { // copy or move entire tree from _Right
        const auto _Scary        = _Get_scary();
        const auto _Right_scary  = _Right._Get_scary();
        _Scary->_Myhead->_Parent = _Copy_nodes<_Strat>(_Right_scary->_Myhead->_Parent, _Scary->_Myhead);
        _Scary->_Mysize          = _Right_scary->_Mysize;
        if (!_Scary->_Myhead->_Parent->_Isnil) { // nonempty tree, look for new smallest and largest
            _Scary->_Myhead->_Left  = _Scary_val::_Min(_Scary->_Myhead->_Parent);
            _Scary->_Myhead->_Right = _Scary_val::_Max(_Scary->_Myhead->_Parent);
        } else { // empty tree, just tidy head pointers
            _Scary->_Myhead->_Left  = _Scary->_Myhead;
            _Scary->_Myhead->_Right = _Scary->_Myhead;
        }
    }

    template <_Strategy _Strat, class _Ty>
    _Nodeptr _Copy_or_move(_Ty& _Val) {
        if constexpr (_Strat == _Strategy::_Copy) {
            return _Buynode(_Val);
        } else {
            if constexpr (_Is_set) {
                return _Buynode(_STD move(_Val));
            } else {
                return _Buynode(_STD move(const_cast<key_type&>(_Val.first)), _STD move(_Val.second));
            }
        }
    }

    template <_Strategy _Strat>
    _Nodeptr _Copy_nodes(_Nodeptr _Rootnode, _Nodeptr _Wherenode) {
        // copy entire subtree, recursively
        const auto _Scary = _Get_scary();
        _Nodeptr _Newroot = _Scary->_Myhead; // point at nil node

        if (!_Rootnode->_Isnil) { // copy or move a node, then any subtrees
            _Nodeptr _Pnode = _Copy_or_move<_Strat>(_Rootnode->_Myval);
            _Pnode->_Parent = _Wherenode;
            _Pnode->_Color  = _Rootnode->_Color;
            if (_Newroot->_Isnil) {
                _Newroot = _Pnode; // memorize new root
            }

            _TRY_BEGIN
            _Pnode->_Left  = _Copy_nodes<_Strat>(_Rootnode->_Left, _Pnode);
            _Pnode->_Right = _Copy_nodes<_Strat>(_Rootnode->_Right, _Pnode);
            _CATCH_ALL
            _Scary->_Erase_tree_and_orphan(_Getal(), _Newroot); // subtree copy failed, bail out
            _RERAISE;
            _CATCH_END
        }

        return _Newroot; // return newly constructed tree
    }

    template <class _Other>
    pair<_Nodeptr, _Nodeptr> _Eqrange(const _Other& _Keyval) const
        noexcept(_Nothrow_compare<key_compare, key_type, _Other>&& _Nothrow_compare<key_compare, _Other, key_type>) {
        // find range of nodes equivalent to _Keyval
        const auto _Scary = _Get_scary();
        const auto& _Comp = _Getcomp();
        _Nodeptr _Pnode   = _Scary->_Myhead->_Parent;
        _Nodeptr _Lonode  = _Scary->_Myhead; // end() if search fails
        _Nodeptr _Hinode  = _Scary->_Myhead; // end() if search fails

        while (!_Pnode->_Isnil) {
            const auto& _Nodekey = _Traits::_Kfn(_Pnode->_Myval);
            if (_DEBUG_LT_PRED(_Comp, _Nodekey, _Keyval)) {
                _Pnode = _Pnode->_Right; // descend right subtree
            } else { // _Pnode not less than _Keyval, remember it
                if (_Hinode->_Isnil && _DEBUG_LT_PRED(_Comp, _Keyval, _Nodekey)) {
                    _Hinode = _Pnode; // _Pnode greater, remember it
                }

                _Lonode = _Pnode;
                _Pnode  = _Pnode->_Left; // descend left subtree
            }
        }

        _Pnode = _Hinode->_Isnil ? _Scary->_Myhead->_Parent : _Hinode->_Left; // continue scan for upper bound
        while (!_Pnode->_Isnil) {
            if (_DEBUG_LT_PRED(_Getcomp(), _Keyval, _Traits::_Kfn(_Pnode->_Myval))) {
                // _Pnode greater than _Keyval, remember it
                _Hinode = _Pnode;
                _Pnode  = _Pnode->_Left; // descend left subtree
            } else {
                _Pnode = _Pnode->_Right; // descend right subtree
            }
        }

        return {_Lonode, _Hinode};
    }

#if _HAS_CXX17
public:
    using node_type = typename _Traits::node_type;

    node_type extract(const const_iterator _Where) {
        const auto _Scary = _Get_scary();
#if _ITERATOR_DEBUG_LEVEL == 2
        _STL_VERIFY(_Where._Getcont() == _Scary && !_Where._Ptr->_Isnil, "map/set erase iterator outside range");
#endif // _ITERATOR_DEBUG_LEVEL == 2

        const auto _Ptr = _Scary->_Extract(_Where._Unwrapped());
        _Scary->_Orphan_ptr(_Ptr);
        return node_type::_Make(_Ptr, _Getal());
    }

    node_type extract(const key_type& _Keyval) {
        const const_iterator _Where = find(_Keyval);
        if (_Where == end()) {
            return node_type{};
        }

        return extract(_Where);
    }

#if _HAS_CXX23
    template <class _Kx, class _Mycomp = key_compare, class = typename _Mycomp::is_transparent,
        enable_if_t<!disjunction_v<is_convertible<_Kx, const_iterator>, is_convertible<_Kx, iterator>>, int> = 0>
    node_type extract(_Kx&& _Keyval) {
        const const_iterator _Where = find(_Keyval);
        if (_Where == end()) {
            return node_type{};
        }

        return extract(_Where);
    }
#endif // _HAS_CXX23

    auto insert(node_type&& _Handle) {
        if (_Handle.empty()) {
            if constexpr (_Multi) {
                return end();
            } else {
                return _Insert_return_type<iterator, node_type>{end(), false, {}};
            }
        }

        _Check_node_allocator(_Handle);
        const auto _Scary        = _Get_scary();
        const auto _Attempt_node = _Handle._Getptr();
        const auto& _Keyval      = _Traits::_Kfn(_Attempt_node->_Myval);
        _Tree_find_result<_Nodeptr> _Loc;
        if constexpr (_Multi) {
            _Loc = _Find_upper_bound(_Keyval);
        } else {
            _Loc = _Find_lower_bound(_Keyval);
            if (_Lower_bound_duplicate(_Loc._Bound, _Keyval)) {
                return _Insert_return_type<iterator, node_type>{
                    iterator(_Loc._Bound, _Scary), false, _STD move(_Handle)};
            }
        }

        _Check_grow_by_1();

        // nothrow hereafter

        _Attempt_node->_Left = _Scary->_Myhead;
        // _Attempt_node->_Parent handled in _Insert_node
        _Attempt_node->_Right = _Scary->_Myhead;
        _Attempt_node->_Color = _Red;

        const auto _Inserted = _Scary->_Insert_node(_Loc._Location, _Handle._Release());
        if constexpr (_Multi) {
            return iterator(_Inserted, _Scary);
        } else {
            return _Insert_return_type<iterator, node_type>{iterator(_Inserted, _Scary), true, _STD move(_Handle)};
        }
    }

    iterator insert(const const_iterator _Hint, node_type&& _Handle) {
        const auto _Scary = _Get_scary();
#if _ITERATOR_DEBUG_LEVEL == 2
        _STL_VERIFY(_Hint._Getcont() == _Scary, "map/set insert iterator outside range");
#endif // _ITERATOR_DEBUG_LEVEL == 2
        if (_Handle.empty()) {
            return end();
        }

        _Check_node_allocator(_Handle);
        const auto _Attempt_node = _Handle._Getptr();
        const auto& _Keyval      = _Traits::_Kfn(_Attempt_node->_Myval);
        const auto _Loc          = _Find_hint(_Hint._Ptr, _Keyval);
        if (_Loc._Duplicate) {
            return iterator(_Loc._Location._Parent, _Scary);
        }

        _Check_grow_by_1();

        _Attempt_node->_Left = _Scary->_Myhead;
        // _Attempt_node->_Parent handled in _Insert_node
        _Attempt_node->_Right = _Scary->_Myhead;
        _Attempt_node->_Color = _Red;

        return iterator(_Scary->_Insert_node(_Loc._Location, _Handle._Release()), _Scary);
    }

    template <class>
    friend class _Tree;

    template <class _Other_traits>
    void merge(_Tree<_Other_traits>& _That) {
        static_assert(is_same_v<_Nodeptr, typename _Tree<_Other_traits>::_Nodeptr>,
            "merge() requires an argument with a compatible node type.");

        static_assert(is_same_v<allocator_type, typename _Tree<_Other_traits>::allocator_type>,
            "merge() requires an argument with the same allocator type.");

        if constexpr (is_same_v<_Tree, _Tree<_Other_traits>>) {
            if (this == _STD addressof(_That)) {
                return;
            }
        }

#if _ITERATOR_DEBUG_LEVEL == 2
        if constexpr (!_Alnode_traits::is_always_equal::value) {
            _STL_VERIFY(_Getal() == _That._Getal(), "allocator incompatible for merge");
        }
#endif // _ITERATOR_DEBUG_LEVEL == 2

        const auto _Scary      = _Get_scary();
        const auto _Head       = _Scary->_Myhead;
        const auto _That_scary = _That._Get_scary();
        auto _First            = _That._Unchecked_begin();
        while (!_First._Ptr->_Isnil) {
            const auto _Attempt_node = _First._Ptr;
            ++_First;
            _Tree_find_result<_Nodeptr> _Loc;
            const auto& _Keyval = _Traits::_Kfn(_Attempt_node->_Myval);
            if constexpr (_Multi) {
                _Loc = _Find_upper_bound(_Keyval);
            } else {
                _Loc = _Find_lower_bound(_Keyval);
                if (_Lower_bound_duplicate(_Loc._Bound, _Keyval)) {
                    continue;
                }
            }

            _Check_grow_by_1();

            // nothrow hereafter for this iteration
            const auto _Extracted = _That_scary->_Extract(_Unchecked_const_iterator(_Attempt_node, nullptr));
            _Extracted->_Left     = _Head;
            // _Extracted->_Parent handled in _Insert_node
            _Extracted->_Right = _Head;
            _Extracted->_Color = _Red;

            const auto _Inserted = _Scary->_Insert_node(_Loc._Location, _Extracted);
            _Reparent_ptr(_Inserted, _That);
        }
    }

    template <class _Other_traits>
    void merge(_Tree<_Other_traits>&& _That) {
        static_assert(is_same_v<_Nodeptr, typename _Tree<_Other_traits>::_Nodeptr>,
            "merge() requires an argument with a compatible node type.");

        static_assert(is_same_v<allocator_type, typename _Tree<_Other_traits>::allocator_type>,
            "merge() requires an argument with the same allocator type.");

        merge(_That);
    }

protected:
    template <class _Other_traits>
    void _Reparent_ptr(const _Nodeptr _Ptr, _Tree<_Other_traits>& _Old_parent) {
        (void) _Ptr;
        (void) _Old_parent;
#if _ITERATOR_DEBUG_LEVEL == 2
        _Lockit _Lock(_LOCK_DEBUG);
        const auto _Old_parent_scary = _Old_parent._Get_scary();
        _Iterator_base12** _Pnext    = &_Old_parent_scary->_Myproxy->_Myfirstiter;
        _STL_VERIFY(_Pnext, "source container corrupted");
        if (_Ptr == nullptr || _Ptr == _Old_parent_scary->_Myhead) {
            return;
        }

        const auto _My_saved_proxy               = _Get_scary()->_Myproxy;
        _Iterator_base12** const _My_saved_first = &_My_saved_proxy->_Myfirstiter;

        while (*_Pnext) {
            _Iterator_base12** const _Next = &(*_Pnext)->_Mynextiter;
            const auto _Iter               = static_cast<const_iterator*>(*_Pnext);
            if (_Iter->_Ptr == _Ptr) { // reparent the iterator
                *_Pnext            = *_Next;
                _Iter->_Myproxy    = _My_saved_proxy;
                _Iter->_Mynextiter = *_My_saved_first;
                *_My_saved_first   = _Iter;
            } else { // skip the iterator
                _Pnext = _Next;
            }
        }
#endif // _ITERATOR_DEBUG_LEVEL == 2
    }

    void _Check_node_allocator(node_type& _Handle) const {
        (void) _Handle;
#if _ITERATOR_DEBUG_LEVEL == 2
        _STL_VERIFY(get_allocator() == _Handle._Getal(), "node handle allocator incompatible for insert");
#endif // _ITERATOR_DEBUG_LEVEL == 2
    }
#endif // _HAS_CXX17

    void _Alloc_sentinel_and_proxy() {
        const auto _Scary = _Get_scary();
        auto&& _Alproxy   = _GET_PROXY_ALLOCATOR(_Alnode, _Getal());
        _Container_proxy_ptr<_Alnode> _Proxy(_Alproxy, *_Scary);
        _Scary->_Myhead = _Node::_Buyheadnode(_Getal());
        _Proxy._Release();
    }

    template <class... _Valty>
    _Nodeptr _Buynode(_Valty&&... _Val) {
        return _Node::_Buynode(_Getal(), _Get_scary()->_Myhead, _STD forward<_Valty>(_Val)...);
    }

    key_compare& _Getcomp() noexcept {
        return _Mypair._Get_first();
    }

    const key_compare& _Getcomp() const noexcept {
        return _Mypair._Get_first();
    }

    _Alnode& _Getal() noexcept {
        return _Mypair._Myval2._Get_first();
    }

    const _Alnode& _Getal() const noexcept {
        return _Mypair._Myval2._Get_first();
    }

    _Scary_val* _Get_scary() noexcept {
        return _STD addressof(_Mypair._Myval2._Myval2);
    }

    const _Scary_val* _Get_scary() const noexcept {
        return _STD addressof(_Mypair._Myval2._Myval2);
    }

private:
    _Compressed_pair<key_compare, _Compressed_pair<_Alnode, _Scary_val>> _Mypair;
};
_STD_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)
#endif // _STL_COMPILER_PREPROCESSOR
#endif // _XTREE_
