#include "Hierarchy.h"

void Hierarchy::copy(const Hierarchy& other)
{
	this->bosses = other.bosses;
	this->employees = other.employees;
	this->size = other.size;
}

void Hierarchy::erase()
{
	this->bosses.clear();
	this->employees.clear();
}

Hierarchy::Hierarchy(Hierarchy&& r) noexcept
{
	this->bosses.clear();
	this->employees.clear();
	this->size = 0;
	std::swap(this->bosses, r.bosses);
	std::swap(this->employees, r.employees);
	std::swap(this->size, r.size);
}

Hierarchy::Hierarchy(const Hierarchy& r)
{
	this->copy(r);
}

Hierarchy::Hierarchy(const string& data)
{
	this->fill_hierarchy(data);
}

Hierarchy::~Hierarchy() noexcept
{
	this->erase();
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
			this->erase();
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
						this->erase();
						return;
					}

					for (size_t k = j + 1; k < length; k++)
					{
						if (data[k] == '-')
						{
							this->erase();
							return;
						}

						if (data[k] == ' ')
							continue;

						employee += data[k];
						if (k + 1 > length || data[k + 1] == '-')
						{
							this->erase();
							return;
						}

						if (k + 1 == length)
						{
							flag = true;
							i = k;
							j = length - 1;
							this->bosses.push_back(boss);
							this->employees.push_back(employee);
							boss.clear();
							employee.clear();
							break;
						}

						if (data[k + 1] == ' ' || data[k + 1] == '\n')
						{
							flag = true;
							i = k;
							j = length - 1;
							this->bosses.push_back(boss);
							this->employees.push_back(employee);
							boss.clear();
							employee.clear();
							break;
						}
					}
				}
			}
			if (!flag)
			{
				this->erase();
				return;
			}
		}
	}
}

void Hierarchy::fill_hierarchy(const string& data)
{
	this->get_data_from_string(data);
	if (this->bosses.get_size() != this->employees.get_size() || this->bosses[0] != "Uspeshnia")
	{
		std::cout << "Incorrect Input!" << std::endl;
		this->erase();
		return;
	}
	size_t relationships = this->bosses.get_size();
	this->size = this->bosses.get_size() + 1;
	for (size_t i = 0; i < relationships; i++)
	{
		if (this->bosses[i] != this->bosses[i + 1])
		{
			for (size_t j = i + 1; j < relationships; j++)
			{
				if (this->bosses[i] == this->bosses[j])
				{
					std::cout << "Incorrect Input!" << std::endl;
					this->erase();
					return;
				}
			}
		}

		for (size_t j = i + 1; j < relationships; j++)
		{
			if (this->employees[i] == this->employees[j])
			{
				std::cout << "Incorrect Input!" << std::endl;
				this->erase();
				return;
			}
		}
	}
}

string Hierarchy::print() const
{
	string list{};
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		list += this->bosses[i];
		list += " - ";
		list += this->employees[i];
		list += "\n";
	}

	return list;
}

bool Hierarchy::find(const string& name) const
{
	if (name == "Uspeshnia")
		return true;

	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		if (this->employees[i] == name)
			return true;

	return false;
}

int Hierarchy::num_employees() const
{
	return this->employees.get_size();
}

string Hierarchy::manager(const string& name) const
{
	if (name == "Uspeshnia")
		return "Uspeshnia has no managers.";

	if (!this->find(name))
		return name + " was not found in this hierarchy.";

	for (size_t i = 0; i < this->employees.get_size(); i++)
		if (this->employees[i] == name)
			return this->bosses[i];

	return "";
}

int Hierarchy::num_subordinates(const string& name) const
{
	if (!this->find(name))
	{
		std::cout << name + " was not found in this hierarchy." << std::endl;
		return -1;
	}

	size_t count{};
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		if (this->bosses[i] == name)
			count++;

	return count;
}

unsigned long Hierarchy::getSalary(const string& who) const
{
	if (!this->find(who))
	{
		std::cout << who + " was not found in this hierarchy." << std::endl;
		return -1;
	}

	unsigned long salary{};
	//if (who == "Uspeshnia")
		//salary += 50;

	size_t count{};
	Vector<string> employee_bosses{};
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		if (this->bosses[i] == who)
		{
			salary += 500;
			for (size_t j = 0; j < number_of_employees; j++)
			{
				if (this->employees[i] == this->bosses[j])
				{
					count++;
					employee_bosses.push_back(this->employees[j]);
				}
			}
		}
	}

	size_t size_employee_bosses = employee_bosses.get_size();
	for (size_t i = 0; i < size_employee_bosses; i++)
		for (size_t j = 0; j < number_of_employees; j++)
			if (employee_bosses[i] == this->bosses[j])
				salary += 50;

	return salary + count * 50;
}

bool Hierarchy::fire(const string& who)
{
	if (who == "Uspeshnia")
	{
		std::cout << who + " cannot be fired!" << std::endl;
		return false;
	}

	if (!this->find(who))
	{
		std::cout << who + " was not found in this hierarchy." << std::endl;
		return false;
	}

	size_t index{};
	bool has_employees = false; 
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		if (this->employees[i] == who)
		{
			for (size_t j = i; j < number_of_employees; j++)
			{
				if (this->bosses[j] == who)
				{
					this->bosses[j] = this->bosses[i];
					has_employees = true;
				}
			}
			index = i;
			this->bosses.pop_at(i);
			this->employees.pop_at(i);
			return true;
		}
	}

	return false;
}

bool Hierarchy::hire(const string& who, const string& boss)
{
	if (!this->find(boss))
	{
		std::cout << boss + " was not found in this hierarchy." << std::endl;
		return false;
	}

	size_t number_of_employees = this->employees.get_size();
	if (!this->find(who))
	{
		for (size_t i = 0; i < number_of_employees; i++)
		{
			if (this->bosses[i] == boss && this->bosses[i + 1] != boss)
			{
				this->bosses.push_at(i + 1, boss);
				this->employees.push_at(i + 1, who);
			}
		}
		return true;
	}


	return false;
}

int main()
{
	Hierarchy a("      Uspeshnia-Gosho   \nUspeshnia -   Misho\nUspeshnia-  Slavi\nGosho-Dancho\nGosho -Pesho\nSlavi-Slav1\nSlavi-Slav2\nDancho-Boris\nDancho-Kamen\nPesho-Alex\nSlav1-Mecho\nMecho-Q12Adl\n");
	std::cout << a.print() << std::endl;
	if (a.find("Slavi1"))
		std::cout << "yes";
	else
		std::cout << "no";
	std::cout << std::endl;
	std::cout << a.num_employees() << std::endl;
	std::cout << a.manager("Mecho") << std::endl;
	std::cout << a.num_subordinates("Slavi") << std::endl;
	std::cout << a.getSalary("Uspeshnia") << std::endl;
	if (a.fire("Slav1"))
		std::cout << "yes" << std::endl;
	else
		std::cout << "no" << std::endl;
	std::cout << a.print();

	return 0;
}