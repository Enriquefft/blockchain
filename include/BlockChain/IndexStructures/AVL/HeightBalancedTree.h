#pragma once

#ifndef HEIGHT_BALANCED_TREE_H
#define HEIGHT_BALANCED_TREE_H

//	File:       HeightBalancedTree.h
//	Author:     Dimiter Georgiev (dimitertg@yahoo.com)
//	Date:       Fri Dec 2, 2005
//              Fri Dec 14, 2005, Chage: see FindComparable
//
//  Licence:    This file is in the public domain.
//              You may do whatever you want with it.

#include <string>
#include <stdexcept>
#include <locale>

extern size_t cc;
extern size_t dc;

template<typename DataType> struct NullDeleter
{
	inline static void DeleteData(const DataType &)
	{
		// do nothing.
	}
};

template<typename DataType> struct DefaultComparator
{
	inline static int Compare(const DataType & a, const DataType & b)
	{
		if (a < b)
		{
			return -1;
		}
		if (b < a)
		{
			return 1;
		}
		return 0;
	}
};

template <typename T> T ToUpper(T c)
{
	static const std::locale CurrentLocale;
	static const std::ctype<T> & CurrentFacet =
		std::use_facet< std::ctype<T> > (CurrentLocale);
	return CurrentFacet.toupper(c);
}

template<typename StringType> struct StringComparator
{
	static int Compare(const StringType & a, const StringType & b)
	{
		if (&a == &b)
		{
			return 0;
		}
		const size_t size = min(a.size(), b.size());
		for (size_t i = 0; i < size; ++i)
		{
			int aUpper = ToUpper(a[i]);
			int bUpper = ToUpper(b[i]);
			if (aUpper != bUpper)
			{
				return aUpper - bUpper;
			}
		}
		if (a.size() < b.size())
		{
			return -1;
		}
		if (a.size() > b.size())
		{
			return 1;
		}
		for (size_t i = 0; i < size; ++i)
		{
			int aChar = a[i];
			int bChar = b[i];
			if (aChar != bChar)
			{
				return aChar - bChar;
			}
		}
		return 0;
	}
};

template <
	typename DataType,
	typename Comparator = DefaultComparator<DataType>,
	typename DataDeleter = NullDeleter<DataType>
>
class HeightBalancedTree
{
	class Node
	{
		Node * m_left;
		Node * m_right;
		int m_height;
		size_t m_size;
		DataType m_data;
	public:
		Node(const DataType & data):
			m_left(NULL),
			m_right(NULL),
			m_height(1),
			m_size(1),
			m_data(data)
		{
			++cc;
		}

		~Node()
		{
			++dc;
		}

		void FixStatistics()
		{
			this->m_height = max(GetHeight(m_left), GetHeight(m_right)) + 1;
			this->m_size = GetSize(m_left) + GetSize(m_right) + 1;
		}

		int GetHeight() const
		{
			return m_height;
		}

		size_t GetSize() const
		{
			return m_size;
		}

		Node * GetLeft() const
		{
			return m_left;
		}

		Node * GetRight() const
		{
			return m_right;
		}

		void SetLeft(Node * left)
		{
			this->m_left = left;
			FixStatistics();
		}

		void SetRight(Node * right)
		{
			this->m_right = right;
			FixStatistics();
		}

		DataType & GetData()
		{
			return m_data;
		}

		static int GetHeight(const Node * node)
		{
			if (node == NULL)
			{
				return 0;
			}
			return node->m_height;
		}

		static size_t GetSize(const Node * node)
		{
			if (node == NULL)
			{
				return 0;
			}
			return node->m_size;
		}

		static Node * GetLeft(const Node * node)
		{
			if (node == NULL)
			{
				return NULL;
			}
			return node->m_left;
		}

		static Node * GetRight(const Node * node)
		{
			if (node == NULL)
			{
				return NULL;
			}
			return node->m_right;
		}
	};
	// don't define these two:
	HeightBalancedTree(const HeightBalancedTree & );
	HeightBalancedTree& operator=(const HeightBalancedTree & );
public:

	HeightBalancedTree():
		m_head(NULL)
	{
	}

	~HeightBalancedTree()
	{
		DelTree(m_head);
	}

	size_t Size() const
	{
		return Node::GetSize(m_head);
	}

	int Height() const
	{
		return Node::GetHeight(m_head);
	}

	void InsertComparable(const DataType & obj)
	{
		m_head = InsertComparable(m_head, obj);
	}

	static Node * InsertComparable(
		Node * node,
		const DataType & obj
	)
	{
		if (node == NULL)
		{
			Node * newNode = new Node(obj);
			return newNode;
		}
		int cmp = Comparator::Compare(obj, node->GetData());
		if (cmp <= 0)
		{
			node->SetLeft(InsertComparable(node->GetLeft(), obj));
			return BringBalance(node);
		}
		else
		{
			node->SetRight(InsertComparable(node->GetRight(), obj));
			return BringBalance(node);
		}
	}

	void InsertByIndex(size_t index, const DataType & obj)
	{
		m_head = InsertByIndex(m_head, index, obj);
	}

	static Node * InsertByIndex(
		Node * node,
		size_t index,
		const DataType & obj
	) {
		if (node == NULL)
		{
			Node * newNode = new Node(obj);
			return newNode;
		}
		size_t leftSize = Node::GetSize(node->GetLeft());
		if (index <= leftSize)
		{
			node->SetLeft(InsertByIndex(node->GetLeft(), index, obj));
			return BringBalance(node);
		}
		else
		{
			node->SetRight(InsertByIndex(node->GetRight(), index - leftSize - 1, obj));
			return BringBalance(node);
		}
	}

	// Bug fixed on December 14, 2005:
	// The returned index was wrong.
	// This function returns the index of the object
	// or Size() if the object is not present.
	size_t FindComparable(const DataType & obj) const
	{
		Node * node = m_head;
		size_t index = Node::GetSize(Node::GetLeft(node));
		while (node != NULL)
		{
			int cmp = Comparator::Compare(obj, node->GetData());
			if (cmp == 0)
			{
				return index;
			}
			if (cmp < 0)
			{
				node = node->GetLeft();
				index = (index - Node::GetSize(Node::GetRight(node))) - 1;
			}
			else
			{
				node = node->GetRight();
				index = (index + Node::GetSize(Node::GetLeft(node))) + 1;
			}
		}
		return Size();
	}

	DataType & FindByIndex(size_t index)
	{
		Node * node = m_head;
		while (node != NULL)
		{
			size_t leftSize = Node::GetSize(node->GetLeft());
			if (index == leftSize)
			{
				return node->GetData();
			}
			if (index < leftSize)
			{
				node = node->GetLeft();
			}
			else
			{
				node = node->GetRight();
				index = index - leftSize - 1;
			}
		}
		throw std::out_of_range("HeightBalancedTree<T> index out of range");
	}

	void RemoveComparable(const DataType & obj)
	{
		m_head = RemoveComparable(m_head, obj);
	}

	static Node * RemoveComparable(
		Node * node,
		const DataType & obj
	) {
		if (node == NULL)
		{
			return NULL;
		}
		int cmp = Comparator::Compare(obj, node->GetData());
		if (cmp == 0)
		{
			Node * left = node->GetLeft();
			Node * right = node->GetRight();
			if (right == NULL)
			{
				DelNode(node);
				return left;
			}
			if (left == NULL)
			{
				DelNode(node);
				return right;
			}
			Node * child = left;
			Node * parent = node;
			while (child->GetRight() != NULL)
			{
				parent = child;
				child = child->GetRight();
			}
			if (left == child)
			{
				node->SetLeft(left->GetLeft());
				node->SetRight(NULL);
				left->SetRight(right);
				left->SetLeft(node);
			}
			else
			{
				node->SetLeft(child->GetLeft());
				node->SetRight(NULL);
				parent->SetRight(node);
				child->SetLeft(left);
				child->SetRight(right);
			}
			node = child;
			--cmp;
		}
		if (cmp < 0)
		{
			node->SetLeft(RemoveComparable(node->GetLeft(), obj));
			return BringBalance(node);
		}
		else
		{
			node->SetRight(RemoveComparable(node->GetRight(), obj));
			return BringBalance(node);
		}
	}

	bool RemoveByIndex(size_t index)
	{
		bool result = index < Size();
		m_head = RemoveByIndex(m_head, index);
		return result;
	}

	static Node * RemoveByIndex(Node * node, size_t index)
	{
		if (node == NULL)
		{
			return NULL;
		}
		size_t leftSize = Node::GetSize(node->GetLeft());
		if (index == leftSize)
		{
			Node * left = node->GetLeft();
			Node * right = node->GetRight();
			if (right == NULL)
			{
				DelNode(node);
				return left;
			}
			if (left == NULL)
			{
				DelNode(node);
				return right;
			}
			Node * child = left;
			Node * parent = node;
			while (child->GetRight() != NULL)
			{
				parent = child;
				child = child->GetRight();
			}
			if (left == child)
			{
				node->SetLeft(left->GetLeft());
				node->SetRight(NULL);
				left->SetRight(right);
				left->SetLeft(node);
			}
			else
			{
				node->SetLeft(child->GetLeft());
				node->SetRight(NULL);
				parent->SetRight(node);
				child->SetLeft(left);
				child->SetRight(right);
			}
			node = child;
			--index;
		}
		if (index < leftSize)
		{
			node->SetLeft(RemoveByIndex(node->GetLeft(), index));
			return BringBalance(node);
		}
		else
		{
			node->SetRight(RemoveByIndex(node->GetRight(), index - leftSize - 1));
			return BringBalance(node);
		}
	}

	static Node * BringBalance(Node * node)
	{
		if (node == NULL)
		{
			return node;
		}
		int leftHeight = Node::GetHeight(node->GetLeft());
		int rightHeight = Node::GetHeight(node->GetRight());
		if (abs(leftHeight - rightHeight) < 2)
		{
			return node;
		}
		bool rotateRight = leftHeight > rightHeight;
		if (rotateRight)
		{
			Node * left = node->GetLeft();
			Node * leftChild = left->GetLeft();
			Node * rightChild = left->GetRight();
			int leftChildHeight = Node::GetHeight(leftChild);
			int rightChildHeight = Node::GetHeight(rightChild);
			if (leftChildHeight > rightChildHeight)
			{
				node->SetLeft(rightChild);
				left->SetRight(node);
				node = left;
			}
			else
			{
				node->SetLeft(rightChild->GetRight());
				left->SetRight(rightChild->GetLeft());
				rightChild->SetLeft(left);
				rightChild->SetRight(node);
				node = rightChild;
			}
		}
		else
		{
			Node * right = node->GetRight();
			Node * leftChild = right->GetLeft();
			Node * rightChild = right->GetRight();
			int leftChildHeight = Node::GetHeight(leftChild);
			int rightChildHeight = Node::GetHeight(rightChild);
			if (leftChildHeight < rightChildHeight)
			{
				node->SetRight(leftChild);
				right->SetLeft(node);
				node = right;
			}
			else
			{
				node->SetRight(leftChild->GetLeft());
				right->SetLeft(leftChild->GetRight());
				leftChild->SetRight(right);
				leftChild->SetLeft(node);
				node = leftChild;
			}
		}
		return node;
	}

	DataType & operator[](size_t index)
	{
		return FindByIndex(index);
	}

private:
	Node * m_head;

	static void DelTree(Node * node)
	{
		if (node == NULL)
		{
			return;
		}
		Node * left = node->GetLeft();
		Node * right = node->GetRight();
		DelNode(node);
		DelTree(left);
		DelTree(right);
	}

	static void DelNode(Node * node)
	{
		DataDeleter::DeleteData(node->GetData());
		delete node;
	}

};

#endif
