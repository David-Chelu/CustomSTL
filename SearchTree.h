#pragma once
#include "Common.h"
#include "List.h"
#include "Pair.h"

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
    using MappedType    = typename Traits::MappedType;			// Type of Mapped Value
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
		_elems = std::move(other._elems);
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
			_elems = std::move(other._elems);
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
		Node* newNode = new Node(std::forward<Args>(args)...);
		const KeyType& newKey = Traits::extract_key(newNode->Value);

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
		TreeNode* promotedNode = subroot->Right;
		subroot->Right = promotedNode->Left;			// subroot adopt left child of promoted

		if(promotedNode->Left != nullptr)
			promotedNode->Left->Parent = subroot;		// subroot-right-left parent set

		promotedNode->Parent = subroot->Parent;			// promoted takes subroot parent

		if (subroot == _head)							// special case when tree root is chosen for rotation
		{
			_head = promotedNode;
			promotedNode->Parent = nullptr;
		}
		else if (subroot == subroot->Parent->Left)		// parent links his new promoted child
			subroot->Parent->Left = promotedNode;
		else
			subroot->Parent->Right = promotedNode;

		promotedNode->Left = subroot;					// promoted takes subroot as left child
		subroot->Parent = promotedNode;					// subroot has promoted as new parent
	}

	void _rotate_right(TreeNode* subroot) {				// promotes subroot left
		TreeNode* promotedNode = subroot->Left;
		subroot->Left = promotedNode->Right;			// subroot adopt right child of promoted

		if (promotedNode->Right != nullptr)
			promotedNode->Right->Parent = subroot;		// subroot-left-right parent set

		promotedNode->Parent = subroot->Parent;			// promoted takes subroot parent

		if (subroot == _head)							// special case when tree root is chosen for rotation
		{
			_head = promotedNode;
			promotedNode->Parent = nullptr;
		}
		else if (subroot == subroot->Parent->Left)
			subroot->Parent->Left = promotedNode;		// parent links his new promoted child
		else
			subroot->Parent->Right = promotedNode;

		promotedNode->Right = subroot;					// promoted takes subroot as right child
		subroot->Parent = promotedNode;					// subroot has promoted as new parent
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
			
			if(_less(Traits::extract_key(newNode->Value->Value), Traits::extract_key(futureParent->Value->Value)))	// TODO: value is pointer here!!!
			{
				_workspaceNode = _workspaceNode->Left;
				if(_workspaceNode == nullptr)
				{
					futureParent->Left = newNode;
					break;
				}
			}
			else
			{
				_workspaceNode = _workspaceNode->Right;
				if(_workspaceNode == nullptr)
				{
					futureParent->Right = newNode;
					break;
				}
			}
		}

		newNode->Parent = futureParent;
		newNode->Color = Red;
	}

	void _fix_inserted(TreeNode* newNode) {
		
		while(newNode->Parent->Color == Red)
		{
			if(newNode->Parent == newNode->Parent->Parent->Left)
			{
				_workspaceNode = newNode->Parent->Parent->Right;
				if(_workspaceNode->Color == Red)						// case1
				{
					newNode->Parent->Color = Black;
					_workspaceNode->Color = Black;
					newNode->Parent->Parent->Color = Red;
					newNode = newNode->Parent->Parent;
				}
				else 
				{
					if (newNode == newNode->Parent->Right)				// case2
					{
						newNode = newNode->Parent;
						_rotate_left(newNode);
					}

					newNode->Parent->Color = Black;						// case3
					newNode->Parent->Parent->Color = Red;
					_rotate_right(newNode->Parent->Parent);
				}
			}
			else
			{
				_workspaceNode = newNode->Parent->Parent->Left;
				if(_workspaceNode->Color == Red)
				{
					newNode->Parent->Color = Black;
					_workspaceNode->Color = Black;
					newNode->Parent->Parent->Color = Red;
					newNode = newNode->Parent->Parent;
				}
				else 
				{
					if (newNode == newNode->Parent->Left)
					{
						newNode = newNode->Parent;
						_rotate_right(newNode);
					}

					newNode->Parent->Color = Black;
					newNode->Parent->Parent->Color = Red;
					_rotate_left(newNode->Parent->Parent);
				}
			}
		}

		_head->Color = Black;
	}

	TreeNode* _find_in_tree(const KeyType& key) const {
		TreeNode* found = nullptr;

		for(_workspaceNode = _head; _workspaceNode!= nullptr; )
		{
			if(key == Traits::extract_key(_workspaceNode->Value))
				found = _workspaceNode;
			else if(_less(key, Traits::extract_key(_workspaceNode->Value)))
			{
				_workspaceNode = _workspaceNode->Left;
			}
			else
			{
				_workspaceNode = _workspaceNode->Right;
			}
		}

		return found;
	}
};
// SearchTree Template =====================================================
// END

CUSTOM_END