// AVL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//	File:		AVL.cpp
//	Author:		Dimiter Georgiev (dimitertg@yahoo.com)
//	Date:		Fri Dec 2, 2005

#include <iostream>
#include <ostream>
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include <time.h>

#include "HeightBalancedTree.h"
#include "HighResElapsedTimer.h"

using namespace std;

#ifdef UNICODE
	std::wostream & tcout = wcout;
	typedef HeightBalancedTree<
		std::basic_string<wchar_t>,
		StringComparator<
			std::basic_string<wchar_t>
		>
	> TStringTree;
#else
	std::ostream & tcout = cout;
	typedef HeightBalancedTree<
		std::string,
		StringComparator<std::string>
	> TStringTree;
#endif

typedef HeightBalancedTree<size_t> SizeTTree;

double MathRandom()
{
	return double(rand()) / double(RAND_MAX);
}

template <typename T> T MathRound(double value)
{
	return static_cast<T>(value + 0.5);
}

size_t cc = 0;
size_t dc = 0;

void test()
{
	{
	try
	{
		const size_t n = 100000;
		size_t data[n];
		size_t indices[n];
		for (size_t i = 0; i < n; ++i)
		{
			data[i] = i;
			indices[i] = MathRound<size_t>(MathRandom() * i);
		}
		indices[0] = 0;
		{
			vector<size_t> vec;
			{
				HighResElapsedTimer timer("Adding to std::vector");
				for (size_t i = 0; i < n; ++i)
				{
					vec.insert(vec.begin() + indices[i], data[i]);
				}
			}
			system("pause");
			{
				SizeTTree tree;
				{
					HighResElapsedTimer timer("Adding to SizeTTree, by index");
					for (size_t i = 0; i < n; ++i)
					{
						tree.InsertByIndex(indices[i], data[i]);
					}
				}
				for (size_t i = 0; i < n; ++i)
				{
					if (vec[i] != tree[i])
					{
						throw std::exception();
					}
				}
				{
					for (size_t i = 0; i < n; ++i)
					{
						tree.RemoveByIndex(0);
					}
					tcout << "size = " << tree.Size() << endl;
				}
			}
		}
		{
			SizeTTree tree;
			{
				HighResElapsedTimer timer("Adding to SizeTTree, comparable");
				for (size_t i = 0; i < n; ++i)
				{
					if (tree.FindComparable(data[indices[i]]) == tree.Size())
					{
						tree.InsertComparable(data[indices[i]]);
					}
				}
			}
			cout << "Added: " << tree.Size() << " unique elements." << endl;
			system("pause");
			for (size_t i = 0; i < min(size_t(3), tree.Size()); ++i)
			{
				tcout << (tree[tree.Size() - i - 1]) << endl;
			}
			for (size_t i = 0; i < tree.Size(); ++i)
			{
				if (tree.FindComparable(tree[i]) != i)
				{
					cout << "tree[" << i << "] = " << tree[i] << endl;
					cout << "tree.FindComparable(tree[" << i << "]) = " << tree.FindComparable(tree[i]) << endl;
					throw std::exception();
				}
			}
			for (size_t i = 0; i < n; ++i)
			{
				tree.RemoveComparable(data[indices[i]]);
			}
			tcout << "size = " << (tree.Size()) << endl;
		}
		{
			TStringTree tree;
			tree.InsertComparable(TEXT("ABE"));
			tree.InsertComparable(TEXT("aBdE"));
			tree.InsertComparable(TEXT("AbCd"));
			tree.InsertComparable(TEXT("aBc"));
			tree.InsertComparable(TEXT("AbD"));
			tree.InsertComparable(TEXT("aBe"));
			for (size_t i = 0; i < tree.Size(); ++i)
			{
				tcout << "tree[" << i << "] = " << tree[i] << endl;
				tcout << "tree.FindComparable(" << tree[i] << ") = " << tree.FindComparable(tree[i]) << endl;
			}
		}
	}
	catch (exception & exc)
	{
		tcout << exc.what() << endl;
	}
	}
	tcout << "Finished, check memory usage now!" << endl;
	system("pause");
}

struct SmallString
{
	TCHAR buf[100];
	SmallString()
	{
		buf[0] = 0;
	}
	SmallString(const SmallString&)
	{
		buf[0] = 0;
	}
	SmallString& operator=(const SmallString&);
};

int _tmain(int /*argc*/, _TCHAR* /*argv[]*/)
{
	srand((unsigned)(time(0)));
	{
		HeightBalancedTree<SmallString> tree;
		tree.InsertByIndex(0, SmallString());
		tree.InsertByIndex(0, SmallString());
		tree.InsertByIndex(0, SmallString());
	}
	test();
	cout << "cc = " << (cc) << endl;
	cout << "dc = " << (dc) << endl;
	return 0;
}
