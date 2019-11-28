#include <iostream>
#include <cstdlib>
#include <vector>
#include <limits>
#include <set>
#include <ctime>
#include <cassert>

// Based on: https://github.com/cpdomina/SplayTree
class Splay
{
public:
	static const std::size_t empty;

	struct Node
	{
		int         value;
		std::size_t child[2];
		Node(int value)	: value(value)
		{
			child[0] = empty;
			child[1] = empty;
		}
		
		std::size_t& operator[](bool index)
		{
			return child[index];
		}
	};

private:
	std::size_t       root;
	const std::size_t aux;
	std::vector<Node> data;

	void print(std::size_t node, int depth)
	{
		for (int i(0); i < depth; ++i)
			std::cout << '\t';
		if (node == empty)
		{
			std::cout << "NULL\n";
		}
		else
		{
			std::cout << node << ": " << data[node].value << '\n';
			print(data[node][0], depth + 1);
			print(data[node][1], depth + 1);
		}
	}
	
	void splay(int value)
	{
		assert(root != empty);
		std::size_t node(root);
		std::size_t leftRight[2] = {aux, aux};
		data[aux][0] = data[aux][1] = empty;
		
		while (true)
		{
			if (data[node].value == value)
				break;
			bool cmp(data[node].value < value);
			
			// rotate
			std::size_t child(data[node][cmp]);
			if (child == empty)
				break;
			if (data[child].value != value)
			{
				bool cmpChild(data[child].value < value);
				if (cmp == cmpChild)
				{
					data[node][cmp] = data[child][!cmp];
					data[child][!cmp] = node;
						
					node = child;
					child = data[node][cmp];
					if (child == empty)
						break;
				}
			}
			
			// link
			data[leftRight[!cmp]][cmp] = node;
			leftRight[!cmp] = node;
			node = child;
		}
		
		//assemble
		data[leftRight[0]][1] = data[node][0];
		data[leftRight[1]][0] = data[node][1];
		data[node][0] = data[aux][1];
		data[node][1] = data[aux][0];
		root = node;
	}

public:
	Splay(std::size_t reserve = 0) : root(empty), aux(0)
	{
		data.reserve(reserve + 1);
		data.push_back(Node(0));
	}

	const Node& operator[](std::size_t index)
	{
		return data[index];
	}

	void print()
	{
		print(root, 0);
	}
	
	std::size_t find(int value)
	{
		if (root != empty)
		{
			splay(value);
			if (data[root].value != value)
				return empty;
		}
		return root;
	}

	void insert(int value)
	{
		if (root == empty)
		{
			data.push_back(value);
		}
		else
		{
			splay(value);
			if (data[root].value == value)
				return;
			data.push_back(value);
			
			if (value < data[root].value)
			{
				data[data.size() - 1][0] = data[root][0];
				data[data.size() - 1][1] = root;
				data[root][0] = empty;
			}
			else
			{
				data[data.size() - 1][0] = root;
				data[data.size() - 1][1] = data[root][1];
				data[root][1] = empty;
			}
		}
		root = data.size() - 1;
	}

	void erase(int value)
	{
		if (root == empty)
			return;
		splay(value);
		if (data[root].value != value)
			return;

		if (data[root][0] == empty)
		{
			root = data[root][1];
		}
		else
		{
			std::size_t rootRight(data[root][1]);
			root = data[root][0];
			splay(value);
			data[root][1] = rootRight;
		}
	}
};

const std::size_t Splay::empty(std::numeric_limits<std::size_t>::max());

bool getRandBool()
{
	return static_cast<bool>(rand() & 1);
}

int getRand()
{
	int a(rand());
	int b(rand() % 53);
	int c(rand() << 3);
	int sign(getRandBool() ? 1 : -1);
	return ((a * b) ^ c) * sign;
}

int main()
{
	std::set<int> control;
	Splay test(100000000);

	srand(time(0));
	int task(1000);
	int taskSize(100000);

	double beg(std::clock());
	for (int t(0); t < task; ++t)
	{
		std::cout << t << '\n';
		for (int i(0); i < taskSize; ++i)
		{
			int n0(getRand());
			if (getRandBool())
			{
				//control.insert(n0);
				test.insert(n0);
			}
			else
			{
				//control.erase(n0);
				test.erase(n0);
			}

			int n1(getRand());
			volatile std::size_t tmp(test.find(n1));
			/*if (control.count(n1) != (test.find(n1) != Splay::empty))
			{
				std::cout << "Task Failed: " << i << " lookup=" << n1 << ' ' << control.count(n1) << ' ' << test.find(n1) << std::endl;
				test.print();
				goto fail;
			}*/
		}
	}
	fail:
	double end(std::clock());

	std::cout << (end - beg) / CLOCKS_PER_SEC << std::endl;

	return 0;
}

