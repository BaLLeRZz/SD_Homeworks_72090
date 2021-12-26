#include "Hierarchy.h"

Node* Hierarchy::copy(const Node* other_root) const
{
	if (!other_root)
		return nullptr;

	Node* cpy = new Node(other_root->value);
	size_t number_of_employees = other_root->children.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		cpy->children.push_back(this->copy(other_root->children[i]));

	return cpy;
}

void Hierarchy::erase(Node* current_root)
{
	if (!current_root)
		return;

	size_t number_of_employees = current_root->children.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		this->erase(current_root->children[i]);

	delete current_root;
}

Hierarchy::Hierarchy(Hierarchy&& r) noexcept
{
	this->root = nullptr;
	this->size = 0;
	std::swap(this->root, r.root);
	std::swap(this->size, r.size);
}

Hierarchy::Hierarchy(const Hierarchy& r)
{
	this->root = this->copy(r.root);
	this->size = r.size;
}

Hierarchy::Hierarchy(const string& data)
{
	this->root->value = data;
	this->size = 1;
}

Hierarchy::~Hierarchy() noexcept
{
	this->erase(this->root);
	this->root = nullptr;
}

string Hierarchy::recursive_print(const Node* node) const
{
	if (!node)
		return "";

	size_t number_of_employees = node->children.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		this->recursive_print(node->children[i]);

	return node->value;
}

int Hierarchy::recursive_longest_chain(const Node* node, size_t count, size_t current_count) const
{
	if (!node)
		return 0;

	if (current_count < count)
		current_count = count;

	size_t number_of_employees = node->children.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		this->recursive_longest_chain(node->children[i], count++, current_count);

	return current_count;
}

bool Hierarchy::recursive_find(const Node* node, const string& name) const
{
	if (!node)
		return false;

	size_t number_of_employees = node->children.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		this->recursive_find(node->children[i], name);

	if (node->value == name)
		return true;
	
	return false;
}

int Hierarchy::recursive_num_employees(const Node* node, size_t count) const
{
	if (!node)
		return 0;

	size_t number_of_employees = node->children.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		this->recursive_num_employees(node->children[i], count++);

	return count;
}

int Hierarchy::recursive_num_overloaded(const Node* node, int level, size_t count, size_t num) const
{
	if (!node)
		return 0;

	if (count > level)
		num++;

	size_t current_count = 0;
	size_t number_of_employees = node->children.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		this->recursive_num_overloaded(node->children[i], level, current_count++, num);

	return num;
}

string Hierarchy::recursive_manager(const Node* node, const string& name) const
{
	if (!node)
		return "";

	size_t number_of_employees = node->children.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		if (node->children[i]->value == name)
			return node->value;

	for (size_t i = 0; i < number_of_employees; i++)
		this->recursive_manager(node->children[i], name);
}

int Hierarchy::recursive_sub_ordinates(const Node* node, const string& name) const
{
	if (!node)
		return 0;

	if (node->value == name)
		return node->children.get_size();

	size_t number_of_employees = node->children.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		this->recursive_sub_ordinates(node->children[i], name);

}

unsigned long Hierarchy::recursive_getSalary(const Node* node, const string& who, unsigned long salary) const
{
	if (!node)
		return 0;

	if (node->value == who)
	{
		size_t number_of_employees = node->children.get_size();
		for (size_t i = 0; i < number_of_employees; i++)
			this->recursive_getSalary(node->children[i], who, salary += 50);

		return 500 * node->children.get_size() + salary;
	}
	else
	{
		size_t number_of_employees = node->children.get_size();
		for (size_t i = 0; i < number_of_employees; i++)
			this->recursive_getSalary(node->children[i], who, salary);
	}

	return salary;
}

bool Hierarchy::recursive_fire(Node* node, const string& who)
{
	if (!node)
		return false;

	size_t number_of_employees = node->children.get_size();
	for(size_t i = 0; i < number_of_employees; i++)
	{ 
		if (node->children[i]->value == who)
		{
			Node* cpy = new Node(node->children[i]->value);
			cpy = this->copy(node->children[i]);
			this->erase(node->children[i]);
			node->children.pop_at(i);
			size_t number_of_employees_cpy = cpy->children.get_size();
			for (size_t j = 0; j < number_of_employees_cpy; j++)
				node->children.push_at(i++, cpy->children[j]);
			
			return true;
		}
	}

	for (size_t i = 0; i < number_of_employees; i++)
		this->recursive_fire(node->children[i], who);

	return false;
}

bool Hierarchy::recursive_hire(Node* node, const string& who, const string& boss)
{
	if (!node)
		return false;

	if (node->value == boss)
	{
		Node* new_employee = new Node(who);
		node->children.push_back(new_employee);
		return true;
	}

	size_t number_of_employees = node->children.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		this->recursive_hire(node->children[i], who, boss);

	return false;
}

string Hierarchy::print() const
{
	return this->recursive_print(this->root);
}

int Hierarchy::longest_chain() const
{
	return this->recursive_longest_chain(this->root, 1, 1);
}

bool Hierarchy::find(const string& name) const
{
	return this->recursive_find(this->root, name);
}

int Hierarchy::num_employees() const
{
	return this->recursive_num_employees(this->root, 0);
}

int Hierarchy::num_overloaded(int level) const
{
	return this->recursive_num_overloaded(this->root, level, 0, 0);
}

string Hierarchy::manager(const string& name) const
{
	return this->recursive_manager(this->root, name);
}

int Hierarchy::num_subordinates(const string& name) const
{
	return this->recursive_sub_ordinates(this->root, name);
}

unsigned long Hierarchy::getSalary(const string& who) const
{
	return this->recursive_getSalary(this->root, who, 0);
}

bool Hierarchy::fire(const string& who)
{
	return this->recursive_fire(this->root, who);
}

bool Hierarchy::hire(const string& who, const string& boss)
{
	return this->recursive_hire(this->root, who, boss);
}
