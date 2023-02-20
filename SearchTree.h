#pragma once
#include "List.h"
#include "Pair.h"
#include "Utility.h"

#include <iostream>
#include <cmath>		// std::ceil
#include <functional>	// std::less
#include <queue>

CUSTOM_BEGIN

// Headings =================================================================================

template<class Traits>
class SearchTree			// SearchTree Template
{
protected:
    using KeyType       = typename Traits::KeyType;				// Type of Key
    using MappedType    = typename Traits::MappedType;			// Type of Mapped _Value
    using KeyCompare    = typename Traits::KeyCompare;			// Comparison struct

    using IterList      = List<typename Traits::ValueType>;		// List of ValueType used for iterating
    using Node          = typename IterList::Node;				// Node component from List
	using TreeNode 		= TreeNode<Node*>;						// Node component from Tree

public:
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

	SearchTree();
	SearchTree(const SearchTree& other);
	SearchTree(SearchTree&& other) noexcept;
	virtual ~SearchTree();

protected:
	// Operators

	SearchTree& operator=(const SearchTree& other);
	SearchTree& operator=(SearchTree&& other) noexcept;

	bool operator==(const SearchTree& other) const;						// Contains the same elems, but not the same tree
	bool operator!=(const SearchTree& other) const;

public:
    // Main functions

    template<class... Args>
	Iterator emplace(Args&&... args);									// Constructs Node first with any given arguments
	Iterator erase(const KeyType& key);
	Iterator erase(const Iterator& iterator);
	Iterator find(const KeyType& key) const;
	
	void clear();
	size_t size() const;
	bool empty() const;

	void printDetails();

public:
	// Iterator functions

	Iterator begin();
	const Iterator begin() const;

	Iterator end();
	const Iterator end() const;

protected:
	// Others

	template<class _KeyType, class... Args>
	Iterator _try_emplace(_KeyType&& key, Args&&... args);			// Force construction with known key and given arguments for object

	const MappedType& _at(const KeyType& key) const;				// Access _Value at key with check
	MappedType& _at(const KeyType& key);

private:
	// Helpers

	void _rotate_left(TreeNode* subroot);				// promotes subroot right
	void _rotate_right(TreeNode* subroot);				// promotes subroot left

	void _insert(TreeNode* newNode);
	void _insert_raw(TreeNode* newNode);
	void _fix_inserted(TreeNode* newNode);

	TreeNode* _find_in_tree(const KeyType& key) const;

	bool _has_parent(TreeNode* node);
	bool _has_grandparent(TreeNode* node);
}; // END SearchTree Template


// Definitions =================================================================================

// SearchTree Template
template<class Traits>
SearchTree<Traits>::SearchTree() {
	// TODO: complete
}

template<class Traits>
SearchTree<Traits>::SearchTree(const SearchTree& other) {
	_elems = other._elems;
	// TODO: complete
}

template<class Traits>
SearchTree<Traits>::SearchTree(SearchTree&& other) noexcept {
	_elems = custom::move(other._elems);
	// TODO: complete
}

template<class Traits>
SearchTree<Traits>::~SearchTree() {
	clear();
}

template<class Traits>
SearchTree<Traits>& SearchTree<Traits>::operator=(const SearchTree& other) {
	if (_elems._head != other._elems._head)
	{
		_elems = other._elems;
		// TODO: complete

	}

	return *this;
}

template<class Traits>
SearchTree<Traits>& SearchTree<Traits>::operator=(SearchTree&& other) noexcept {
	if (_elems._head != other._elems._head)
	{
		_elems = custom::move(other._elems);
		// TODO: complete

	}

	return *this;
}

template<class Traits>
bool SearchTree<Traits>::operator==(const SearchTree& other) const {
	if (size() != other.size())
		return false;

	// TODO: complete

	return true;
}

template<class Traits>
bool SearchTree<Traits>::operator!=(const SearchTree& other) const {
	return !(*this == other);
}

template<class Traits>
template<class... Args>
typename SearchTree<Traits>::Iterator SearchTree<Traits>::emplace(Args&&... args) {
	Node* newNode = new Node(custom::forward<Args>(args)...);
	const KeyType& newKey = Traits::extract_key(newNode->_Value);

	_elems._insert_node_before(_elems._head, newNode);
	TreeNode* newTreeNode = new TreeNode(newNode);
	_insert(newTreeNode);

	return Iterator(newNode, _elems._update_iteration_data());
}

template<class Traits>
typename SearchTree<Traits>::Iterator SearchTree<Traits>::find(const KeyType& key) const {
	// TODO: complete

	//return Iterator(*it, _elems.update_iteration_data());
}

template<class Traits>
void SearchTree<Traits>::clear() {
	_elems.clear();													// Delete all Node* with values
	// TODO: complete

}

template<class Traits>
size_t SearchTree<Traits>::size() const {
	return _elems.size();
}

template<class Traits>
bool SearchTree<Traits>::empty() const {
	return _elems.empty();
}

template<class Traits>
void SearchTree<Traits>::printDetails() {
	if(_head == nullptr)
		return;
	
	std::queue<TreeNode*> q;
	q.push(_head);

	while(!q.empty())
	{
		int count = q.size();

		while(count > 0)
		{
			TreeNode* node = q.front();
			std::cout << Traits::extract_key(node->_Value->_Value) << (int)node->_Color << ' ';
			q.pop();
			if(node->_Left != nullptr)
				q.push(node->_Left);
			if(node->_Right != nullptr)
				q.push(node->_Right);
			--count;
		}
		std::cout << '\n';
	}
}

template<class Traits>
typename SearchTree<Traits>::Iterator SearchTree<Traits>::begin() {
	return _elems.begin();
}

template<class Traits>
const typename SearchTree<Traits>::Iterator SearchTree<Traits>::begin() const {
	return _elems.begin();
}

template<class Traits>
typename SearchTree<Traits>::Iterator SearchTree<Traits>::end() {
	return _elems.end();
}

template<class Traits>
const typename SearchTree<Traits>::Iterator SearchTree<Traits>::end() const {
	return _elems.end();
}

template<class Traits>
void SearchTree<Traits>::_rotate_left(TreeNode* subroot) {
	TreeNode* promotedNode = subroot->_Right;
	subroot->_Right = promotedNode->_Left;			// subroot adopt left child of promoted

	if (promotedNode->_Left != nullptr)
		promotedNode->_Left->_Parent = subroot;		// subroot-right-left parent set

	promotedNode->_Parent = subroot->_Parent;		// promoted takes subroot parent

	if (subroot == _head)							// special case when tree root is chosen for rotation
	{
		_head = promotedNode;
		promotedNode->_Parent = nullptr;
	}
	else if (subroot == subroot->_Parent->_Left)	// parent links his new promoted child
		subroot->_Parent->_Left = promotedNode;
	else
		subroot->_Parent->_Right = promotedNode;

	promotedNode->_Left = subroot;					// promoted takes subroot as left child
	subroot->_Parent = promotedNode;				// subroot has promoted as new parent
}

template<class Traits>
void SearchTree<Traits>::_rotate_right(TreeNode* subroot) {
	TreeNode* promotedNode = subroot->_Left;
	subroot->_Left = promotedNode->_Right;			// subroot adopt right child of promoted

	if (promotedNode->_Right != nullptr)
		promotedNode->_Right->_Parent = subroot;	// subroot-left-right parent set

	promotedNode->_Parent = subroot->_Parent;		// promoted takes subroot parent

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
	subroot->_Parent = promotedNode;				// subroot has promoted as new parent
}

template<class Traits>
void SearchTree<Traits>::_insert(TreeNode* newNode) {

	if (_head == nullptr)
		_head = newNode;
	else
	{
		_insert_raw(newNode);
		_fix_inserted(newNode);
	}
}

template<class Traits>
void SearchTree<Traits>::_insert_raw(TreeNode* newNode) {	// TODO: ok here
	TreeNode* futureParent = nullptr;

	for (_workspaceNode = _head; _workspaceNode != nullptr;)					// find parent for newly created node
	{
		futureParent = _workspaceNode;

		if (_less(Traits::extract_key(newNode->_Value->_Value), Traits::extract_key(futureParent->_Value->_Value)))
		{
			_workspaceNode = _workspaceNode->_Left;
			if (_workspaceNode == nullptr)
				futureParent->_Left = newNode;
		}
		else
		{
			_workspaceNode = _workspaceNode->_Right;
			if (_workspaceNode == nullptr)
				futureParent->_Right = newNode;
		}
	}

	newNode->_Parent = futureParent;
	newNode->_Color = Red;
}

template<class Traits>
void SearchTree<Traits>::_fix_inserted(TreeNode* newNode) {		// TODO: check
	TreeNode* uncle = nullptr;
	TreeNode* _workspaceNode = newNode;							// initialize violation with newly inserted node

	while (_has_grandparent(_workspaceNode) && _workspaceNode->_Parent->_Color == Red)
	{
		if (_workspaceNode->_Parent == _workspaceNode->_Parent->_Parent->_Left)
		{
			uncle = _workspaceNode->_Parent->_Parent->_Right;
			if(uncle == nullptr || uncle->_Color == Black)		// uncle black
			{
				if (_workspaceNode == _workspaceNode->_Parent->_Right)		// case 2 = uncle black (triangle)
				{
					_workspaceNode = _workspaceNode->_Parent;
					_rotate_left(_workspaceNode);
				}	

				_workspaceNode->_Parent->_Color = Black;
				_workspaceNode->_Parent->_Parent->_Color = Red;

				//if (_workspaceNode == _workspaceNode->_Parent->_Left)		// case 3 = uncle black (line)
				if (_has_grandparent(_workspaceNode))
					_rotate_right(_workspaceNode->_Parent->_Parent);
			}
			else															// case 1 = uncle red
			{
				_workspaceNode->_Parent->_Color = Black;
				uncle->_Color = Black;
				_workspaceNode->_Parent->_Parent->_Color = Red;
				_workspaceNode = _workspaceNode->_Parent->_Parent;
			}
		}
		else
		{
			uncle = _workspaceNode->_Parent->_Parent->_Left;
			if(uncle == nullptr || uncle->_Color == Black)
			{
				if (_workspaceNode == _workspaceNode->_Parent->_Left)
				{
					_workspaceNode = _workspaceNode->_Parent;
					_rotate_right(_workspaceNode->_Parent);
				}

				_workspaceNode->_Parent->_Color = Black;
				_workspaceNode->_Parent->_Parent->_Color = Red;

				//if (_workspaceNode == _workspaceNode->_Parent->_Right)
				if (_has_grandparent(_workspaceNode))
					_rotate_left(_workspaceNode->_Parent->_Parent);
			}
			else
			{
				_workspaceNode->_Parent->_Color = Black;
				uncle->_Color = Black;
				_workspaceNode->_Parent->_Parent->_Color = Red;
				_workspaceNode = _workspaceNode->_Parent->_Parent;
			}
		}
	}

	_head->_Color = Black;
}

template<class Traits>
typename SearchTree<Traits>::TreeNode* SearchTree<Traits>::_find_in_tree(const KeyType& key) const {
	TreeNode* found = nullptr;

	for (_workspaceNode = _head; _workspaceNode != nullptr; )
	{
		if (key == Traits::extract_key(_workspaceNode->_Value))
			found = _workspaceNode;
		else if (_less(key, Traits::extract_key(_workspaceNode->_Value)))
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

template<class Traits>
bool SearchTree<Traits>::_has_parent(TreeNode* node) {
	return node->_Parent != nullptr;
}

template<class Traits>
bool SearchTree<Traits>::_has_grandparent(TreeNode* node) {
	return (_has_parent(node) && node->_Parent->_Parent != nullptr);
}
// END SearchTree Template

CUSTOM_END