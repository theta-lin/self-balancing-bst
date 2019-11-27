#include <iostream>
#include <cstdlib>
#include <vector>
#include <limits>
#include <set>
#include <ctime>

class LLRB
{
public:
	static const bool        black;
	static const bool        red;
	static const std::size_t empty;

	struct Node
	{
		int         value : 31;
		bool        color : 1;
		std::size_t left;
		std::size_t right;
		Node(int value) : value(value), color(red), left(empty), right(empty)
		{
		}
	};

private:
	std::size_t       root;
	std::vector<Node> data;

	void print(std::size_t node, int depth)
	{
		for (int i(0); i < depth; ++i)
			std::cout << '\t';
		if (node == empty)
		{
			std::cout << "NULL(B)\n";
		}
		else
		{
			std::cout << node << '(' << (data[node].color ? 'R' : 'B') << ')' << ": " << data[node].value << '\n';
			print(data[node].left, depth + 1);
			print(data[node].right, depth + 1);
		}
	}

	bool isRed(std::size_t node)
	{
		return (node != empty) && data[node].color;
	}

	std::size_t rotateLeft(std::size_t node)
	{
		std::size_t rightChild(data[node].right);
		data[node].right = data[rightChild].left;
		data[rightChild].left = node;
		data[rightChild].color = data[node].color;
		data[node].color = red;
		return rightChild;
	}

	std::size_t rotateRight(std::size_t node)
	{
		std::size_t leftChild(data[node].left);
		data[node].left = data[leftChild].right;
		data[leftChild].right = node;
		data[leftChild].color = data[node].color;
		data[node].color = red;
		return leftChild;
	}

	void flipColor(std::size_t node)
	{
		data[node].color = !data[node].color;
		if (data[node].left != empty)
			data[data[node].left].color = !data[data[node].left].color;
		if (data[node].right != empty)
			data[data[node].right].color = !data[data[node].right].color;
	}

	std::size_t fixUp(std::size_t node)
	{
		if (isRed(data[node].right) && !isRed(data[node].left))
			node = rotateLeft(node);
		if (isRed(data[node].left) && isRed(data[data[node].left].left))
			node = rotateRight(node);
		if (isRed(data[node].left) && isRed(data[node].right))
			flipColor(node);
		return node;
	}

	std::size_t find(std::size_t node, int value)
	{
		if (node == empty || value == data[node].value)
			return node;
		else if (value < data[node].value)
			return find(data[node].left, value);
		else
			return find(data[node].right, value);
	}

	std::size_t insert(std::size_t node, int value)
	{
		if (node == empty)
		{
			data.push_back(Node(value));
			return data.size() - 1;
		}

		if (value < data[node].value)
		{
			std::size_t tmp(insert(data[node].left, value));
			data[node].left = tmp;
		}
		else if (value > data[node].value)
		{
			std::size_t tmp(insert(data[node].right, value));
			data[node].right = tmp;
		}
		return fixUp(node);
	}
	
	std::size_t moveRedLeft(std::size_t node)
	{
		flipColor(node);
		if (isRed(data[node].right) && isRed(data[data[node].right].left))
		{
			data[node].right = rotateRight(data[node].right);
			node = rotateLeft(node);
			flipColor(node);
		}
		return node;
	}
	
	std::size_t moveRedRight(std::size_t node)
	{
		flipColor(node);
		if (isRed(data[data[node].left].left))
		{
			node = rotateRight(node);
			flipColor(node);
		}
		return node;
	}
	
	std::size_t moveMin(std::size_t node, std::size_t dst)
	{
		if (data[node].left == empty)
		{
			data[dst].value = data[node].value;
			return empty;
		}
		else
		{
			if (!isRed(data[node].left) && !isRed(data[data[node].left].left))
				node = moveRedLeft(node);
			data[node].left = moveMin(data[node].left, dst);
			return fixUp(node);
		}
	}
	
	std::size_t erase(std::size_t node, int value)
	{
		if (node == empty)
			return empty;

		if (value < data[node].value)
		{
			if (   data[node].left != empty
			    && !isRed(data[node].left) && !isRed(data[data[node].left].left))
				node = moveRedLeft(node);
			data[node].left = erase(data[node].left, value);
		}
		else
		{
			if (isRed(data[node].left))
				node = rotateRight(node);
			if (value == data[node].value && data[node].right == empty)
				return empty;
				
			if (   data[node].right != empty
			    && !isRed(data[node].right) && !isRed(data[data[node].right].left))
			    node = moveRedRight(node);
			if (value == data[node].value)
				data[node].right = moveMin(data[node].right, node);
			else
				data[node].right = erase(data[node].right, value);
		}
		
		return fixUp(node);
	}

public:
	LLRB(std::size_t reserve = 1) : root(empty)
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
	
	std::size_t find(int value)
	{
		return find(root, value);
	}

	void insert(int value)
	{
		root = insert(root, value);
		data[root].color = black;
	}
	
	void erase(int value)
	{
		root = erase(root, value);
		data[root].color = black;
	}
};

const bool        LLRB::black(false);
const bool        LLRB::red(true);
const std::size_t LLRB::empty(std::numeric_limits<std::size_t>::max());

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
	LLRB test(100000000);

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
				control.insert(n0);
				test.insert(n0);
			}
			else
			{
				control.erase(n0);
				test.erase(n0);
			}
			
			int n1(getRand());
			if (control.count(n1) != (test.find(n1) != LLRB::empty))
			{
				std::cout << "Task Failed: " << i << " lookup=" << n1 << ' ' << control.count(n1) << ' ' << test.find(n1) << std::endl;
				test.print();
				goto fail;
			}
		}
	}
	fail:
	double end(std::clock());
	
	std::cout << (end - beg) / CLOCKS_PER_SEC << std::endl;
	return 0;
}

