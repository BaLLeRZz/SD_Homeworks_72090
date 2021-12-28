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
	this->fill_hierarchy(data);
}

Hierarchy::~Hierarchy() noexcept
{
	this->erase(this->root);
	this->root = nullptr;
}

int Hierarchy::recursive_longest_chain(Node* node, size_t count, size_t current_count) const
{
	if (!node)
		return 0;

	//if (current_count < count)
		//current_count = count;

	Node& cpy = *node;
	std::cout << cpy.value << std::endl;
	cpy.value = "patkan";
	std::cout << node->value << std::endl;

	//std::cout << node->value << " - " << count << std::endl;
	//size_t number_of_employees = node->children.get_size();
	//for (size_t i = 0; i < number_of_employees; i++)
	//{
	//    //std::cout << "Next Iteration:" << std::endl;
	//	this->recursive_longest_chain(node->children[i], count += 1, current_count);
	//	count -= 1; 
	//}

	return current_count;
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
			//node->children.pop_at(i);
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

Node* Hierarchy::find_employee(const string& name) const
{
	if ("Uspeshnia" == name)
		return this->root;

	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		if (this->employees[i]->value == name)
			return this->employees[i];
	
	return nullptr;
}

bool Hierarchy::recursive_hire(Node* node, const string& who, const string& boss)
{
	if (!node)
		return false;

	if (node->value == boss)
	{
		if (!this->find(who))
		{
			Node* new_employee = new Node(who);
			node->children.push_back(new_employee);
		}
		else
		{
			Node* cpy = new Node(who);
			//cpy = this->copy(this->find_employee(this->root, who));
			//this->erase(this->find_employee(this->root, who)); // bez pop
			node->children.push_back(cpy);
		}
		return true;
	}

	size_t number_of_employees = node->children.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		this->recursive_hire(node->children[i], who, boss);

	return false;
}

Node* Hierarchy::find_max_salary_employee(const Node* node)
{
	if (!node || node->children.get_size() == 0)
		return nullptr;

	Node* max_salary_employee = new Node(node->children[0]->value);
	size_t number_of_employees = node->children.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		for (size_t j = i; j < number_of_employees; j++)
		{
			if (this->getSalary(node->children[i]->value) < this->getSalary(node->children[j]->value))
				max_salary_employee = this->copy(node->children[j]);

			else if (this->getSalary(node->children[i]->value) == this->getSalary(node->children[j]->value))
			{
				size_t length_i = node->children[i]->value.size();
				size_t length_j = node->children[j]->value.size();
				bool flag = false;
				if (length_i < length_j)
				{
					for (size_t k = 0; k < length_i; k++)
					{
						if (node->children[i]->value[k] < node->children[j]->value[k])
						{
							max_salary_employee = this->copy(node->children[i]);
							flag = true;
							break;
						}

						if (node->children[i]->value[k] > node->children[j]->value[k])
						{
							max_salary_employee = this->copy(node->children[j]);
							flag = true;
							break;
						}
					}

					if (!flag)
						max_salary_employee = this->copy(node->children[i]);
				}
				else if (length_i > length_j)
				{
					for (size_t k = 0; k < length_j; k++)
					{
						if (node->children[j]->value[k] < node->children[i]->value[k])
						{
							max_salary_employee = this->copy(node->children[j]);
							flag = true;
							break;
						}

						if (node->children[j]->value[k] > node->children[i]->value[k])
						{
							max_salary_employee = this->copy(node->children[i]);
							flag = true;
							break;
						}
					}

					if (!flag)
						max_salary_employee = this->copy(node->children[j]);
				}
				else
				{
					for (size_t k = 0; k < length_i; k++)
					{
						if (node->children[i]->value[k] < node->children[j]->value[k])
						{
							max_salary_employee = this->copy(node->children[i]);
							break;
						}

						if (node->children[i]->value[k] > node->children[j]->value[k])
						{
							max_salary_employee = this->copy(node->children[j]);
							break;
						}
					}
				}
			}
		}
	}

	return max_salary_employee;
}

void Hierarchy::recursive_incorporate(Node* node)
{
	if (!node)
		return;

	size_t number_of_employees = node->children.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		this->recursive_incorporate(node->children[i]);
	
	if (number_of_employees >= 2)
	{
		Node* temp = new Node(node->children[0]->value);
		Node* max_salary_employee = new Node(node->children[0]->value);
		max_salary_employee = this->copy(this->find_max_salary_employee(node));
		for (size_t i = 0; i < number_of_employees; i++)
		{
			if (node->children[i]->value == max_salary_employee->value)
			{
				if (i != 0)
				{
					temp = this->copy(node->children[0]);
					node->children[0] = this->copy(node->children[i]);
					node->children[i] = this->copy(temp);
				}
				break;
			}
		}

		for (size_t i = 1; i < number_of_employees; i++)
			node->children[0]->children.push_back(node->children[i]);
		
		Node* cpy = new Node(node->children[0]->value);
		cpy = this->copy(node->children[0]);
		node->children.clear();
		node->children.push_back(cpy);
	}
}

void Hierarchy::recursive_modernize(Node* node, size_t level)
{
	if (!node)
		return;

	size_t number_of_employees = node->children.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		if (level % 2 == 0)
		{
			if (node->children[i]->children.get_size() > 0)
			{
				size_t child_employees = node->children[i]->children.get_size();
				for (size_t j = 0; j < child_employees; j++)
					node->children.push_at(j, node->children[i]->children[j]);

				this->erase(node->children[i]);
			}
		}
		else
			this->recursive_modernize(node->children[i], level++);
	}
}

void Hierarchy::get_data_from_string(const string& data)
{
	size_t length = data.size();
	string boss{}, employee{};
	bool flag = false;
	for (size_t i = 0; i < length; i++)
	{
		flag = false;
		if (data[i] == '-')
		{
			this->erase(this->root);
			this->root = nullptr;
			return;
		}

		if (data[i] == ' ' || data[i] == '\n')
			continue;

		boss += data[i];
		if (i + 1 < length && (data[i + 1] == ' ' || data[i + 1] == '-'))
		{
			for (size_t j = i + 1; j < length; j++)
			{
				if (data[j] == ' ')
					continue;

				if (data[j] != ' ' && data[j] != '-')
					break;

				if (data[j] == '-')
				{
					if (j + 1 >= length)
					{
						this->erase(this->root);
						this->root = nullptr;
						return;
					}

					for (size_t k = j + 1; k < length; k++)
					{
						if (data[k] == '-')
						{
							this->erase(this->root);
							this->root = nullptr;
							return;
						}

						if (data[k] == ' ')
							continue;

						employee += data[k];
						if (k + 1 > length || data[k + 1] == '-')
						{
							this->erase(this->root);
							this->root = nullptr;
							return;
						}

						if (k + 1 == length)
						{
							flag = true;
							i = k;
							j = length - 1;
							Node* temp1 = new Node(boss);
							Node* temp2 = new Node(employee);
							this->bosses.push_back(temp1);
							this->employees.push_back(temp2);
							boss.clear();
							employee.clear();
							break;
						}

						if (data[k + 1] == ' ' || data[k + 1] == '\n')
						{
							flag = true;
							i = k;
							j = length - 1;
							Node* temp1 = new Node(boss);
							Node* temp2 = new Node(employee);
							this->bosses.push_back(temp1);
							this->employees.push_back(temp2);
							boss.clear();
							employee.clear();
							break;
						}
					}
				}
			}
			if (!flag)
			{
				this->erase(this->root);
				this->root = nullptr;
				return;
			}
		}
	}
}

void Hierarchy::make_relationships(Node* node)
{
	if (!node)
	{
		std::cout << "Incorrect Input!" << std::endl;
		this->erase(this->root);
		this->root = nullptr;
		return;
	}

	size_t relationships = this->bosses.get_size();
	for (size_t i = 0; i < relationships; i++)
	{
		if (node->value == this->bosses[i]->value)
		{
			Node* temp = new Node(this->employees[i]->value);
			node->children.push_back(temp);
		}
	}

	size_t number_of_employees = node->children.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		this->make_relationships(node->children[i]);
}

void Hierarchy::fill_hierarchy(const string& data)
{
	if (!this->root)
	{
		std::cout << "Incorrect Input!" << std::endl;
		this->erase(this->root);
		this->root = nullptr;
		return;
	}

	this->get_data_from_string(data);
	if (this->bosses.get_size() != this->employees.get_size() || this->bosses[0]->value != "Uspeshnia")
	{
		std::cout << "Incorrect Input!" << std::endl;
		this->erase(this->root);
		this->root = nullptr;
		return;
	}
	size_t relationships = this->bosses.get_size();
	this->size = this->bosses.get_size() + 1;
	for (size_t i = 0; i < relationships; i++)
	{
		if (this->bosses[i]->value != this->bosses[i + 1]->value)
		{
			for (size_t j = i + 1; j < relationships; j++)
			{
				if (this->bosses[i]->value == this->bosses[j]->value)
				{
					std::cout << "Incorrect Input!" << std::endl;
					this->erase(this->root);
					this->root = nullptr;
					return;
				}
			}
		}

		for (size_t j = i + 1; j < relationships; j++)
		{
			if (this->employees[i]->value == this->employees[j]->value)
			{
				std::cout << "Incorrect Input!" << std::endl;
				this->erase(this->root);
				this->root = nullptr;
				return;
			}
		}
	}

	this->make_relationships(this->root);
}

string Hierarchy::print() const
{
	string list{};
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		list += this->bosses[i]->value;
		list += " - ";
		list += this->employees[i]->value;
		list += "\n";
	}

	return list;
}

int Hierarchy::longest_chain() const
{
	return this->recursive_longest_chain(this->root, 1, 1);
}

bool Hierarchy::find(const string& name) const
{
	if (name == "Uspeshnia")
		return true;

	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		if (this->employees[i]->value == name)
			return true;

	return false;
}

int Hierarchy::num_employees() const
{
	return this->employees.get_size();
}

int Hierarchy::num_overloaded(int level) const
{
	return this->recursive_num_overloaded(this->root, level, 0, 0);
}

string Hierarchy::manager(const string& name) const
{
	if (name == "Uspeshnia")
		return "";

	for (size_t i = 0; i < this->employees.get_size(); i++)
		if (this->employees[i]->value == name)
			return this->bosses[i]->value;

	return "";
}

int Hierarchy::num_subordinates(const string& name) const
{
	size_t count{};
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		if (this->bosses[i]->value == name)
			count++;

	return count;
}

unsigned long Hierarchy::getSalary(const string& who) const
{
	unsigned long salary{};
	size_t count{};
	Vector<Node*> employee_bosses{};
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		if (this->bosses[i]->value == who)
		{
			salary += 500;
			for (size_t j = 0; j < number_of_employees; j++)
			{
				if (this->employees[i]->value == this->bosses[j]->value)
				{
					count++;
					employee_bosses.push_back(this->employees[j]);
					std::cout << this->employees[j]->value << ", ";
				}
			}
		}
	}

	std::cout << std::endl;
	size_t size_employee_bosses = employee_bosses.get_size();
	for (size_t i = 0; i < size_employee_bosses; i++)
		std::cout << employee_bosses[i]->value << std::endl;

	for (size_t i = 0; i < size_employee_bosses; i++)
	{
		for (size_t j = 0; j < number_of_employees; j++)
		{
			if (employee_bosses[i]->value == this->bosses[j]->value)
			{
				std::cout << employee_bosses[i]->value << " - " << this->bosses[j]->value << std::endl;
				salary += 50;
			}
		}
	}
	return salary + count * 50;
}

bool Hierarchy::fire(const string& who)
{
	return this->recursive_fire(this->root, who);
}

bool Hierarchy::hire(const string& who, const string& boss)
{
	return this->recursive_hire(this->root, who, boss);
}

void Hierarchy::incorporate()
{
	this->recursive_incorporate(this->root);
}

void Hierarchy::modernize()
{
	this->recursive_modernize(this->root, 0);
}

int main()
{
	Hierarchy a("      Uspeshnia-Gosho   \nUspeshnia -   Misho\nUspeshnia-  Slavi\nGosho-Dancho\nGosho -Pesho\nSlavi-Slav1\nSlavi-Slav2\nDancho-Boris\nDancho-Kamen\nPesho-Alex\nSlav1-Mecho\nMecho-Q12Adl\n");
	std::cout << a.print();
	std::cout << std::endl;
	if (a.find("Slavi1"))
		std::cout << "yes";
	else
		std::cout << "no";
	std::cout << std::endl;
	std::cout << a.num_employees() << std::endl;
	std::cout << a.manager("Mecho") << std::endl;
	std::cout << a.num_subordinates("Slavi") << std::endl;
	std::cout << a.getSalary("Uspeshnia") << std::endl;
	//std::cout << a.longest_chain() << std::endl;
	return 0;
}