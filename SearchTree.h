#pragma once
#include "List.h"
#include "Pair.h"
#include "Utility.h"

#include <iostream>
#include <cmath>		// std::ceil
#include <functional>	// std::less

CUSTOM_BEGIN

// SearchTree Template =====================================================
template<class Traits>
class SearchTree
{
protected:
    using KeyType       = typename Traits::KeyType;				// Type of Key
    using MappedType    = typename Traits::MappedType;			// Type of Mapped _Value
    using KeyCompare    = typename Traits::KeyCompare;			// Comparison struct

    using IterList      = List<typename Traits::ValueType>;		// List of ValueType used for iterating
    using Node          = typename IterList::Node;				// Node component from List
	using TreeNode 		= TreeNode<Node*>;						// Node component from Tree

	enum Colors 
	{
		Red,
		Black
	};

public:
    using ValueType     = typename Traits::ValueType;			// Type of values stored in container
    using Iterator      = typename List<ValueType>::Iterator;	// Iterator for this container (identical to List iterator)

protected:
    KeyCompare _less;											// Used for comparison
    IterList _elems;											// Used to iterate through container

	TreeNode* _head 					= nullptr;				// Head of the Tree
	mutable TreeNode* _workspaceNode 	= nullptr;				// Auxiliary Node for work

protected:
	// Constructors

	SearchTree() {
		// TODO: complete
	
	}

	SearchTree(const SearchTree& other) {
		_elems = other._elems;
		// TODO: complete

	}

	SearchTree(SearchTree&& other) noexcept {
		_elems = custom::move(other._elems);
		// TODO: complete

	}

	virtual ~SearchTree() {
		clear();
	}

protected:
	// Operators

	SearchTree& operator=(const SearchTree& other) {
		if (_elems._head != other._elems._head)
		{
			_elems = other._elems;
			// TODO: complete

		}

		return *this;
	}

	SearchTree& operator=(SearchTree&& other) noexcept {
		if (_elems._head != other._elems._head)
		{
			_elems = custom::move(other._elems);
			// TODO: complete

		}

		return *this;
	}

	bool operator==(const SearchTree& other) const {						// Contains the same elems, but not the same hashtable
		if (size() != other.size())
			return false;

		// TODO: complete

		return true;
	}

public:
    // Main functions

    template<class... Args>
	Iterator emplace(Args&&... args) {									// Constructs Node first with any given arguments
		Node* newNode = new Node(custom::forward<Args>(args)...);
		const KeyType& newKey = Traits::extract_key(newNode->_Value);

		_elems._insert_node_before(_elems._head, newNode);
		_insert(new TreeNode(newNode));

		return Iterator(newNode, _elems._update_iteration_data());
	}

	Iterator find(const KeyType& key) const {
		// TODO: complete

		//return Iterator(*it, _elems.update_iteration_data());
	}

	void clear() {
		_elems.clear();													// Delete all Node* with values
		// TODO: complete

	}

	bool empty() const {
		return _elems.empty();
	}

	size_t size() const {
		return _elems.size();
	}

public:
	// Iterator functions

	Iterator begin() {
		return _elems.begin();
	}

	const Iterator begin() const {
		return _elems.begin();
	}

	Iterator end() {
		return _elems.end();
	}

	const Iterator end() const {
		return _elems.end();
	}

private:
	// Helpers

	void _rotate_left(TreeNode* subroot) {				// promotes subroot right
		TreeNode* promotedNode = subroot->_Right;
		subroot->_Right = promotedNode->_Left;			// subroot adopt left child of promoted

		if(promotedNode->_Left != nullptr)
			promotedNode->_Left->_Parent = subroot;		// subroot-right-left parent set

		promotedNode->_Parent = subroot->_Parent;			// promoted takes subroot parent

		if (subroot == _head)							// special case when tree root is chosen for rotation
		{
			_head = promotedNode;
			promotedNode->_Parent = nullptr;
		}
		else if (subroot == subroot->_Parent->_Left)		// parent links his new promoted child
			subroot->_Parent->_Left = promotedNode;
		else
			subroot->_Parent->_Right = promotedNode;

		promotedNode->_Left = subroot;					// promoted takes subroot as left child
		subroot->_Parent = promotedNode;					// subroot has promoted as new parent
	}

	void _rotate_right(TreeNode* subroot) {				// promotes subroot left
		TreeNode* promotedNode = subroot->_Left;
		subroot->_Left = promotedNode->_Right;			// subroot adopt right child of promoted

		if (promotedNode->_Right != nullptr)
			promotedNode->_Right->_Parent = subroot;		// subroot-left-right parent set

		promotedNode->_Parent = subroot->_Parent;			// promoted takes subroot parent

		if (subroot == _head)							// special case when tree root is chosen for rotation
		{
			_head = promotedNode;
			promotedNode->_Parent = nullptr;
		}
		else if (subroot == subroot->_Parent->_Left)
			subroot->_Parent->_Left = promotedNode;		// parent links his new promoted child
		else
			subroot->_Parent->_Right = promotedNode;

		promotedNode->_Right = subroot;					// promoted takes subroot as right child
		subroot->_Parent = promotedNode;					// subroot has promoted as new parent
	}

	void _insert(TreeNode* newNode) {

		if(_head == nullptr)
			_head = newNode;
		else
		{
			_insert_raw(newNode);
			_fix_inserted(newNode);
		}
	}

	void _insert_raw(TreeNode* newNode) {
		TreeNode* futureParent 	= nullptr;

		for(_workspaceNode = _head;;)					// find parent for newly created node
		{
			futureParent = _workspaceNode;
			
			if(_less(Traits::extract_key(newNode->_Value->_Value), Traits::extract_key(futureParent->_Value->_Value)))	// TODO: value is pointer here!!!
			{
				_workspaceNode = _workspaceNode->_Left;
				if(_workspaceNode == nullptr)
				{
					futureParent->_Left = newNode;
					break;
				}
			}
			else
			{
				_workspaceNode = _workspaceNode->_Right;
				if(_workspaceNode == nullptr)
				{
					futureParent->_Right = newNode;
					break;
				}
			}
		}

		newNode->_Parent = futureParent;
		newNode->_Color = Red;
	}

	void _fix_inserted(TreeNode* newNode) {
		
		while(newNode->_Parent->_Color == Red)
		{
			if(newNode->_Parent == newNode->_Parent->_Parent->_Left)
			{
				_workspaceNode = newNode->_Parent->_Parent->_Right;
				if(_workspaceNode->_Color == Red)						// case1
				{
					newNode->_Parent->_Color = Black;
					_workspaceNode->_Color = Black;
					newNode->_Parent->_Parent->_Color = Red;
					newNode = newNode->_Parent->_Parent;
				}
				else 
				{
					if (newNode == newNode->_Parent->_Right)				// case2
					{
						newNode = newNode->_Parent;
						_rotate_left(newNode);
					}

					newNode->_Parent->_Color = Black;						// case3
					newNode->_Parent->_Parent->_Color = Red;
					_rotate_right(newNode->_Parent->_Parent);
				}
			}
			else
			{
				_workspaceNode = newNode->_Parent->_Parent->_Left;
				if(_workspaceNode->_Color == Red)
				{
					newNode->_Parent->_Color = Black;
					_workspaceNode->_Color = Black;
					newNode->_Parent->_Parent->_Color = Red;
					newNode = newNode->_Parent->_Parent;
				}
				else 
				{
					if (newNode == newNode->_Parent->_Left)
					{
						newNode = newNode->_Parent;
						_rotate_right(newNode);
					}

					newNode->_Parent->_Color = Black;
					newNode->_Parent->_Parent->_Color = Red;
					_rotate_left(newNode->_Parent->_Parent);
				}
			}
		}

		_head->_Color = Black;
	}

	TreeNode* _find_in_tree(const KeyType& key) const {
		TreeNode* found = nullptr;

		for(_workspaceNode = _head; _workspaceNode!= nullptr; )
		{
			if(key == Traits::extract_key(_workspaceNode->_Value))
				found = _workspaceNode;
			else if(_less(key, Traits::extract_key(_workspaceNode->_Value)))
			{
				_workspaceNode = _workspaceNode->_Left;
			}
			else
			{
				_workspaceNode = _workspaceNode->_Right;
			}
		}

		return found;
	}
};
// SearchTree Template =====================================================
// END

CUSTOM_END