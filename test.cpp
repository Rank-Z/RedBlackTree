#include"RedBlackTree.hpp"
#include<random>
#include<chrono>
#include<set>
#include<windows.h>
#include<iostream>
using namespace ::std::chrono;

template<typename Key>
int rbtreetest(RB_node<Key>* node)
{
	if (node == nullptr)
		return 0;
	else
	{
		int i1 = rbtreetest(node->left);
		int i2 = rbtreetest(node->right);
		if (i1 != i2)
			return -1;
		else
		{
			if (!node->isred)
				return i1 + 1;
			else
				return i1;
		}
	}
}


template<typename Key>
bool isRB_Tree(RB_node<Key>* root)
{
	if (root == nullptr)
		return false;
	if (rbtreetest(root) != -1)
		return true;
	else
		return false;
}



template<typename _Datatype>
_Datatype int_random(_Datatype _first , _Datatype _last) //range[_first,_last]
{
	long long _seed = time_point_cast<nanoseconds>(steady_clock::now()).time_since_epoch().count();//nanoseconds time from system
	::std::uniform_int_distribution<_Datatype> u(_first , _last); //int_distribution
	::std::default_random_engine e(_seed % 4294967295);//2^32-1
	return u(e);
}

template<typename _Datatype>
_Datatype real_random(_Datatype _from , _Datatype _to) //range[_from,_to)
{
	long long _seed = time_point_cast<nanoseconds>(steady_clock::now()).time_since_epoch().count();//nanoseconds time from system
	::std::uniform_real_distribution<_Datatype> u(_from , _to); //real_distribution
	::std::default_random_engine e(_seed % 4294967295);//2^32-1
	return u(e);
}


int myrand()
{
	return int_random(-1000000 , 1000000);
}



int main()
{
	RB_Tree<int,RB_node<int>> t;

	std::multiset<int> si;


	for (int i = 0; i != 30; ++i)
	{
		int k = myrand();
		si.insert(k);
		t.insert(k);
		if (!isRB_Tree(t.getroot()))
			throw 1;
		std::cout << i << std::endl;
	}

	//t.clear();


	int i = 0;
	for (auto&temp : si)
	{
		t.deletekey(temp);
		if (!isRB_Tree(t.getroot()))
			throw 1;
		std::cout << i++ << std::endl;
	}


	return 0;

}
