#include <iostream>
#include <cstdlib>
#include <vector>
#include <limits>
#include <set>
#include <ctime>

class Splay
{
public:
	static const std::size_t empty;

	struct Node
	{
		int         value;
		std::size_t parent;
		std::size_t child[2];
		Node(int value, std::size_t parent)	: value(value), parent(parent)
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
	std::vector<Node> data;

	void print(std::size_t node, int depth)
	{
		if (depth > 6)
			return;
		
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

	void rotate(std::size_t node)
	{
		std::size_t parent(data[node].parent);
		bool side(node == data[parent][1]);
		data[parent][side] = data[node][!side];
		data[node][!side] = parent;
		data[node].parent = data[parent].parent;
		data[parent].parent = node;
	}
	
	void splay(std::size_t node)
	{
		std::size_t parent;
		std::size_t grandparent;
		while (parent = data[node].parent, parent != empty)
		{
			grandparent = data[parent].parent;
			if (grandparent != empty)
			{
				bool side(node == data[parent][1]);
				bool sideParent(parent == data[grandparent][1]);
				if (side == sideParent)
					rotate(parent);
				else
					rotate(node);
			}
			rotate(node);
		}
		root = node;
	}

public:
	Splay(std::size_t reserve = 1) : root(empty)
	{
		data.reserve(reserve);
	}

	const Node& operator[](std::size_t index)
	{
		return data[index];
	}

	void print()
	{
		print(root, 0);
	}
	
	std::size_t find(int value, bool getPred = false)
	{
		//std::cout << "find" << '\n';
		std::size_t node(root), pred(empty);
		int count(0);
		while (node != empty && data[node].value != value)
		{
			pred = node;
			if (value < data[node].value)
				node = data[node][0];
			else
				node = data[node][1];
			//std::cout << node << ' ' << data[node][0] << ' ' << data[node][1] << '\n';
			++count;
			if (count > 10)
			{
				std::cout << "FAILED\n";
				break;
			}
		}
		return getPred ? pred : node;
	}

	void insert(int value)
	{
		//std::cout << "insert" << '\n';
		std::size_t node(find(value, true));
		if (node == empty || data[node].value != value)
		{
			data.push_back(Node(value, node));
			if (node != empty)
			{
				if (value < data[node].value)
					data[node][0] = data.size() - 1;
				else
					data[node][1] = data.size() - 1;
			}
			splay(data.size() - 1);
		}
	}

	void erase(int value)
	{
		//std::cout << "erase" << '\n';
		std::size_t node(find(value));
		if (node != empty)
		{
			splay(node);
			if (data[root][0] == empty)
			{
				if (data[root][1] != empty)
					data[data[root][1]].parent = empty;
				root = data[root][1];
			}
			else
			{
				data[data[root][0]].parent = empty;
				std::size_t max(data[root][0]);
				while (data[max][1] != empty)
					max = data[max][1];
				splay(max);
				
				if (data[root][1] != empty)
					data[data[root][1]].parent = data[root][0];
				data[data[root][0]][1] = data[root][1];
				root = data[root][0];
			}
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
	int task(1);
	int taskSize(10);

	double beg(std::clock());
	for (int t(0); t < task; ++t)
	{
		std::cout << t << '\n';
		for (int i(0); i < taskSize; ++i)
		{
			int n0(getRand());
			if (getRandBool())
			{
				control.insert(n0);
				test.insert(n0);
			}
			else
			{
				control.erase(n0);
				test.erase(n0);
			}

			int n1(getRand());
			if (control.count(n1) != (test.find(n1) != Splay::empty))
			{
				std::cout << "Task Failed: " << i << " lookup=" << n1 << ' ' << control.count(n1) << ' ' << test.find(n1) << std::endl;
				test.print();
				goto fail;
			}
		}
	}
	fail:
	double end(std::clock());
	
	test.print();

	std::cout << (end - beg) / CLOCKS_PER_SEC << std::endl;

	return 0;
}

