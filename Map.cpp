#include <iostream>
#include <algorithm>
using namespace std;
template <typename Key, typename Value>
class ConstIterator;
template <typename Key, typename Value>
class Iterator;
template <typename Key,typename Value>
class Map
{
	friend class Iterator<Key, Value>;
	friend class ConstIterator<Key, Value>;
private:
	
	struct Node
	{
		Node* left;
		Node* right;
		Node* parent;
		Key key;
		Value value;
		int height;
		Node(const Key& key, const Value& value)
		{
			this->height = 1;
			this->key = key;
			this->value = value;
			this->left = nullptr;
			this->right = nullptr;
			this->parent = nullptr;
		}
	};
	Node* root;

	int h(Node* node)
	{
		return node ? node->height : 0;
	}
	int balance(Node* node)
	{
		return h(node->left) - h(node->right);
	}
	void update(Node* node)
	{
		node->height = 1 + std::max(h(node->left), h(node->right));
	}
	Node* clone(Node* node,Node* parent = nullptr)
	{
		Node* newNode = new Node(node->key, node->value);
		newNode->height = node->height;
		newNode->parent = parent;
		newNode->left = clone(node->left, newNode);
		newNode->right = clone(node->right, newNode);
		return newNode;
	}
	Node* rotateRight(Node* node)
	{
		Node* x = node->left;
		Node* T2 = x->right;
		x->right = node;
		node->left = T2;
		
		x->parent = node->parent;
		node->parent = x;
		if (T2)
		{
			T2->parent = node;
		}
		if (x->parent)
		{
			if (x->parent->left == node)
			{
				x->parent->left = x;
			}
			else
			{
				x->parent->right = x;
			}
		}
		

		update(node);
		update(x);

		return x;
	}
	Node* rotateLeft(Node* node)
	{
		Node* x = node->right;
		Node* T2 = x->left;
		x->left = node;
		node->right = T2;
		x->parent = node->parent;
		node->parent = x;

		if (T2)
		{
			T2->parent = node;
		}
		if (x->parent)
		{
			if (x->parent->left == node)
			{
				x->parent->left = x;
			}
			else
			{
				x->parent->right = x;
			}

		}
		update(node);
		update(x);

		return x;
	}
	Node* fix(Node* node)
	{
		update(node);
		int sub = balance(node);


		if (sub > 1 && balance(node->left) >= 0)
		{
			return rotateRight(node);
		}
		if (sub > 1 && balance(node->left) < 0)
		{
			node->left = rotateLeft(node->left);
			return rotateRight(node);
		}
		if (sub < -1 && balance(node->right) < 0)
		{
			return rotateLeft(node);
		}
		if (sub < -1 && balance(node->right) >= 0)
		{
			node->right = rotateRight(node->right);
			return rotateLeft(node);
		}
		return  node;
	}
	Node* getMin(Node* node) const
	{
		while (node->left != nullptr)
		{
			node = node->left;
		}
		return node;
	}
	Node* insert(Node* node, const Key& key, const Value& value,Node*& inserted)
	{
		if (!node)
		{
			node = new Node(key, value);
			inserted = node;
			return node;

		}
		if (key < node->key)
		{
			node->left = insert(node->left,key,value,inserted);
			node->left->parent = node;
		}
		else if (key > node->key)
		{
			node->right = insert(node->right,key,value,inserted);
			node->right->parent = node;
		}
		else
		{
			node->value = value;
			inserted = node;
			return node;
		}
		return fix(node);
	}

	Node* erase(Node* node, const Key& key, Node*& next)
	{
		if (!node)
			return nullptr;

		if (key < node->key)
		{
			node->left = erase(node->left, key, next);
			if (node->left)
				node->left->parent = node;
		}
		else if (key > node->key)
		{
			node->right = erase(node->right, key, next);
			if (node->right)
				node->right->parent = node;
		}
		else
		{
			next = getMin(node->right);
			if (!node->left || !node->right)
			{
				Node* child = node->left ? node->left : node->right;

				if (child)
					child->parent = node->parent;

				delete node;
				return child;
			}

			Node* succ = getMin(node->right);

			node->key = succ->key;
			node->value = succ->value;

			node->right = erase(node->right, succ->key, next);

			if (node->right)
				node->right->parent = node;
		}

		return fix(node);
	}
	Node* find(Node* node, const Key& key,Node*& next)
	{
		if (!node) return nullptr;
		if (key < node->key)
		{
			return find(node->left,key,next);
		}
		else if (key > node->key)
		{
			return find(node->right,key,next);
		}
		else
		{
			next = node;
			return node;
		}
	}
	void clear(Node* node)
	{
		if (!node) return;
		clear(node->left);
		clear(node->right);
		delete node;
	}
	void print(Node* node)
	{
		if (!node) return;
		print(node->left);
		std::cout << node->key << ":" << node->value << "\n";
		print(node->right);
	}
public:
	Value& operator[](const Key& key)
	{
		Node* inserted = nullptr;

		root = insert(root, key, Value(), inserted);
		return inserted->value;
	}
	Map(const Map& other)
	{
		root = clone(other.root);
	}
	Map& operator=(const Map& other)
	{
		if (this == &other)
		{
			return *this;
		}
		clear();
		root = clone(other.root);
		return *this;
	}
	Map(Map&& other)
	{
		root = other.root;
		other.root = nullptr;
	}
	Map& operator=(Map&& other)
	{
		if (this == &other)
		{
			return *this;
		}
		clear();
		root = other.root;
		other.root = nullptr;
		return *this;
	}
	Map()
	{
		this->root = nullptr;
	}
	Iterator<Key,Value> insert(const Key& key, const Value& value)
	{
		Node* temp = nullptr;
		root = insert(root, key, value,temp);
		return Iterator<Key,Value>(temp);
		
	}
	Iterator<Key,Value> erase(const Key& key)
	{
		Node* temp = nullptr;
		root = erase(root, key, temp);
		return Iterator<Key,Value>(temp);
	}
	void clear()
	{
		clear(root);
		root = nullptr;
	}
	Iterator<Key,Value> begin()
	{
		return Iterator<Key,Value>(getMin(root));
	}
	Iterator<Key,Value> end()
	{
		return Iterator<Key,Value>(nullptr);
	}
	const ConstIterator<Key,Value> begin() const 
	{
		return ConstIterator<Key,Value>(getMin(root));
	}
	const ConstIterator<Key,Value> end() const
	{
		return ConstIterator<Key, Value>(nullptr);
	}
	void print()
	{
		print(root);
	}
	Iterator<Key,Value> find(const Key& key)
	{
		Node* temp = nullptr;
		find(root,key, temp);
		return Iterator<Key,Value>(temp);
	}
	~Map()
	{
		clear();
	}
};
template <typename Key, typename Value>
class Iterator
{
private:
	using Node = typename Map<Key, Value>::Node;
	Node* node = nullptr;
	Node* next(Node* n)
	{
		if (n->right)
		{
			n = n->right;
			while (n && n->left)
			{
				n = n->left;
			}
			return n;
		}
		else
		{
			Node* p = n->parent;
			while (p && n == p->right)
			{
				n = p;
				p = p->parent;
			}
			return p;
		}
	}
	Node* prev(Node* n)
	{
		if (n->left)
		{
			n = n->left;
			while (n && n->right)
			{
				n = n->right;
			}
			return n;
		}
		else
		{
			Node* p = n->parent;
			while (p&& n == p->right)
			{
				n = p;
				p = p->parent;
			}
			return p;
		}
	}
public:
	Iterator(Node* node)
	{
		this->node = node;
	}
	std::pair<Key&,Value&> operator*()
	{
		return  { node->key,node->value };
	}
	Iterator& operator++()
	{
		node = next(node);
		return *this;
	}
	Iterator& operator--()
	{
		node = prev(node);
		return *this;
	}
	std::pair<Key&,Value&> operator->()
	{
		return { &node->key,&node->value };
	}
	bool operator!=(const Iterator& other)
	{
		return node != other.node;
	}
};
template <typename Key, typename Value>
class ConstIterator
{
private:
	using Node = typename Map<Key, Value>::Node;
	Node* node;
	Node* next(Node* n)
	{
		if (n->right)
		{
			n = n->right;
			while (n && n->left)
			{
				n = n->left;
			}
			return n;
		}
		else
		{
			Node* p = n->parent;
			while (p && n == p->right)
			{
				n = p;
				p = p->parent;
			}
			return p;
		}
	}
	Node* prev(Node* n)
	{
		if (n->left)
		{
			n = n->left;
			while (n && n->right)
			{
				n = n->right;
			}
			return n;
		}
		else
		{
			Node* p = n->parent;
			while (p&& n == p->right)
			{
				n = p;
				p = p->parent;
			}
			return p;
		}
	}
public:
	ConstIterator(Node* node)
	{
		this->node = node;
	}
	const std::pair<Key&, Value&> operator*()
	{
		return { node->key,node->value };
	}
	ConstIterator& operator++()
	{
		node = next(node);
		return *this;
	}
	ConstIterator& operator--()
	{
		node = prev(node);
		return *this;
	}
	std::pair<Key&,Value&>* operator->()
	{
		return { &node->key,&node->value };
	}
	bool operator != (const ConstIterator& other)
	{
		return node != other.node;
	}
};
int main()
{

		Map<int, std::string> mp;

		// тест вставки
		mp.insert(10, "ten");
		mp.insert(20, "twenty");
		mp.insert(5, "five");
		mp.insert(15, "fifteen");
		mp.insert(25, "twenty five");
		mp.insert(3, "three");
		mp.insert(7, "seven");

		std::cout << "Initial tree (in-order):\n";
		mp.print();

		std::cout << "\nOperator[] test:\n";
		mp[10] = "TEN_UPDATED";
		mp[100] = "hundred (auto insert)";

		std::cout << "After operator[] changes:\n";
		mp.print();

		std::cout << "\nIterator traversal:\n";
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			auto [k, v] = *it;
			std::cout << k << " -> " << v << "\n";
		}

		std::cout << "\nConst iterator traversal:\n";
		const Map<int, std::string>& cmp = mp;
		for (auto it = cmp.begin(); it != cmp.end(); ++it)
		{
			auto [k, v] = *it;
			std::cout << k << " -> " << v << "\n";
		}

		std::cout << "\nErase test:\n";
		mp.erase(10);
		mp.erase(5);
		mp.erase(100);

		mp.print();

		std::cout << "\nFinal iteration after erase:\n";
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			std::cout << (*it).first << " -> " << (*it).second << "\n";
		}

		mp.clear();
		std::cout << "\nAfter clear (should be empty):\n";
		mp.print();

		return 0;
	
}

