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

void Hierarchy::hire_helper(const string& who, const string& boss)
{
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		if (this->bosses[i] == boss && this->bosses[i + 1] != boss)
		{
			this->bosses.push_at(i + 1, boss);
			this->employees.push_at(i + 1, who);
			return;
		}
	}

	for (size_t i = 0; i < number_of_employees; i++)
	{
		if (this->employees[i] == boss)
		{
			for (size_t j = 0; j < number_of_employees; j++)
			{
				if (this->employees[i - 1] == this->bosses[j])
				{
					for (size_t k = j; k < number_of_employees; k++)
					{
						if (this->bosses[k] != this->bosses[j])
						{
							this->bosses.push_at(k, boss);
							this->employees.push_at(k, who);
							return;
						}
					}
				}
			}

			for (size_t j = i; j < number_of_employees; j++)
			{
				if (this->bosses[j] != this->bosses[i])
				{
					this->bosses.push_at(j, boss);
					this->employees.push_at(j, who);
					return;
				}
			}
		}
	}
}

void Hierarchy::fix_list()
{
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		if (this->bosses[i] != this->bosses[i + 1])
		{
			for (size_t j = i + 1; j < number_of_employees; j++)
			{
				if (this->bosses[i] == this->bosses[j])
				{
					std::swap(this->bosses[++i], this->bosses[j]);
					std::swap(this->employees[i], this->employees[j]);
				}
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

int Hierarchy::longest_chain() const
{
	int max_chain = -1;
	int count = 2;
	Vector<string> employee_bosses{};
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		if (this->bosses[i] == "Uspeshnia")
			employee_bosses.push_back(this->employees[i]);
	
	size_t index{};
	size_t size_employee_bosses = employee_bosses.get_size();
	for (size_t k = 0; k < size_employee_bosses; k++)
	{
		for (size_t i = 0; i < number_of_employees; i++)
		{
			if (this->bosses[i] == employee_bosses[k])
			{
				index = i;
				count++;
				for (size_t j = i; j < number_of_employees; j++)
				{
					if (this->employees[i] == this->bosses[j])
					{
						count++;
						i = j;
					}
				}
				if (max_chain < count)
					max_chain = count;

				count = 2;
				i = index;
			}
		}
	}

	employee_bosses.clear();
	return max_chain;
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

int Hierarchy::num_overloaded(int level) const
{
	int num = 0;
	int count = 0;
	size_t index{};
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		for (size_t j = i; j < number_of_employees; j++)
		{
			if (this->employees[i] == this->bosses[j])
			{
				index = j;
				count++;
				//std::cout << "Boss[" << j << "] : " << this->bosses[j] << std::endl;
				//for (size_t k = j; k < number_of_employees; k++)
				//{
	
				//}
			}
		}
	}

	if (num > 0)
		return num + 1;

	return num;
}

string Hierarchy::manager(const string& name) const
{
	if (name == "Uspeshnia")
		return "";

	if (!this->find(name))
		return "";

	for (size_t i = 0; i < this->employees.get_size(); i++)
		if (this->employees[i] == name)
			return this->bosses[i];

	return "";
}

int Hierarchy::num_subordinates(const string& name) const
{
	if (!this->find(name))
		return -1;

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
		return 0;

	unsigned long salary{};
	size_t count{};
	size_t number_of_employees = this->employees.get_size();
	if (who == "Uspeshnia")
	{
		for (size_t i = 0; i < number_of_employees; i++)
			if (this->bosses[i] == "Uspeshnia")
				count++;

		if (this->employees.get_size() - count < 0)
			return count * 500;

		return count * 500 + ((this->employees.get_size() - count) * 50);
	}

	Vector<string> employee_bosses{};
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
		return false;

	if (!this->find(who))
		return false;

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
			this->bosses.pop_at(i);
			this->employees.pop_at(i);
			break;
		}
	}

	this->fix_list();

	return true;
}

bool Hierarchy::hire(const string& who, const string& boss)
{
	if (!this->find(boss))
		return false;

	if (who == "Uspeshnia")
		return false;

	size_t number_of_employees = this->employees.get_size();
	if (!this->find(who))
	{
		for (size_t i = 0; i < number_of_employees; i++)
		{
			if (this->bosses[i] == boss && this->bosses[i + 1] != boss)
			{
				this->bosses.push_at(i + 1, boss);
				this->employees.push_at(i + 1, who);
				return true;
			}
		}
		this->hire_helper(who, boss);
		return true;
	}

	this->hire_helper(who, boss);
	number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		if (this->employees[i] == who && this->bosses[i] != boss)
		{
			this->bosses.pop_at(i);
			this->employees.pop_at(i);
			return true;
		}
	}

	return false;
}

void Hierarchy::incorporate()
{
	unsigned long max_salary{};
	string max_salary_employee{};
	unsigned long salary{};
	Vector<string> helper{};
	size_t helper_size{};
	int number_of_employees = this->employees.get_size();
	for (int i = number_of_employees - 1; i >= 0; i--)
	{
		if (this->bosses[i] == this->bosses[i - 1])
		{
			for (int j = i - 1; j >= 0; j--)
			{
				if (this->bosses[j] == this->bosses[i])
					helper.push_back(this->employees[j]);
				else
					break;
			}

			helper_size = helper.get_size();
			for (size_t j = 0; j < helper_size; j++)
			{
				if (j == 0)
					max_salary_employee = helper[i];

				salary = this->getSalary(helper[i]);
				if (max_salary == salary)
				{
					if (max_salary_employee > helper[i])
						max_salary_employee = helper[i];

				}
				else
				if (max_salary < salary)
				{
					max_salary = salary;
					max_salary_employee = helper[i];
				}
			}
			
			for (int j = 0; j < number_of_employees; j++)
				if (this->bosses[j] == this->bosses[i] && this->employees[j] != max_salary_employee)
					this->bosses[j] = max_salary_employee;
			
			max_salary = 0;
			max_salary_employee = "";
			helper.clear();
		}
	}
	this->fix_list();
}

int main()
{ // Boris-Kosta1\nBoris-Kosta2\nBoris-Kosta3\nBoris-Kosta4\nBoris-Kosta5\nBoris-Kosta6\nBoris-Kosta7\nBoris-Kosta8\nBoris-Kosta9\nBoris-Kosta10\nBoris-Kosta11\nBoris-Kosta12\nBoris-Kosta13\nBoris-Kosta14\nBoris-Kosta15\nBoris-Kosta16\nBoris-Kosta17\nBoris-Kosta18\nBoris-Kosta19\n
	Hierarchy a("      Uspeshnia-Gosho   \nUspeshnia -   Misho\nUspeshnia-  Slavi\nGosho-Dancho\nGosho -Pesho\nSlavi-Slav1\nSlavi-Slav2\nDancho-Boris\nDancho-Kamen\nPesho-Alex\nSlav1-Mecho\nMecho-Q12Adl\n");
	Hierarchy b(a);
	std::cout << b.print() << std::endl;
	if (b.find("Slavi1"))
		std::cout << "yes";
	else
		std::cout << "no";
	std::cout << std::endl;
	std::cout << b.num_employees() << std::endl;
	std::cout << b.manager("Mecho") << std::endl;
	std::cout << b.num_subordinates("Slavi") << std::endl;
	std::cout << b.getSalary("Uspeshnia") << std::endl;
	std::cout << b.longest_chain() << std::endl;
	b.incorporate();
	std::cout << b.print() << std::endl;
	/*if (b.hire("Gosho", "Slavi"))
		std::cout << "yes" << std::endl;
	else
		std::cout << "no" << std::endl;
	std::cout << b.print() << std::endl;*/
	//std::cout << b.num_overloaded() << std::endl;
	/*if (b.fire("Dancho"))
		std::cout << "yes" << std::endl;
	else
		std::cout << "no" << std::endl;
	std::cout << b.print();*/

	return 0;
}