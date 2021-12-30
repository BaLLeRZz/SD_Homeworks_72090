#include "Hierarchy.h"

void Hierarchy::copy(const Hierarchy& other)
{
	this->bosses = other.bosses;
	this->employees = other.employees;
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
	std::swap(this->bosses, r.bosses);
	std::swap(this->employees, r.employees);
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

						if (data[k] == ' ' || data[k] == '\n')
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
	bool flag = false;
	for (size_t i = 0; i < relationships; i++)
	{
		for (size_t j = i + 1; j < relationships; j++)
		{
			if (this->employees[i] == this->employees[j])
			{
				std::cout << "Incorrect Input!" << std::endl;
				this->erase();
				return;
			}
		}

		for (size_t j = 0; j < relationships; j++)
		{
			if (this->bosses[i] == this->employees[j] && i < j)
			{
				std::cout << "Incorrect Input!" << std::endl;
				this->erase();
				return;
			}
		}
	}

	for (size_t i = 0; i < relationships; i++)
	{
		if (this->bosses[i] == "Uspeshnia")
			continue;

		for (size_t j = 0; j < relationships; j++)
		{
			if (this->bosses[i] == this->employees[j])
			{
				flag = true;
				break;
			}
		}

		if (!flag)
		{
			std::cout << "Incorrect Input!" << std::endl;
			this->erase();
			return;
		}
	}

	for (size_t j = 0; j < relationships; j++)
	{
		if (this->bosses[j] == this->employees[j])
		{
			std::cout << "Incorrect Input!" << std::endl;
			this->erase();
			return;
		}
	}
	this->fix_list();
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

	for (size_t i = 0; i < number_of_employees; i++)
		for (size_t j = i; j < number_of_employees; j++)
			if (this->bosses[i] == this->bosses[j] && this->employees[i] > this->employees[j])
				std::swap(this->employees[i], this->employees[j]);

	Vector<string> helper_bosses{};
	Vector<string> helper_employees{};
	for (size_t i = 0; i < number_of_employees; i++)
	{
		if (this->bosses[i] == "Uspeshnia")
		{
			helper_bosses.push_back(this->bosses[i]);
			helper_employees.push_back(this->employees[i]);
		}
	}
	
	for (size_t i = 0; i < number_of_employees; i++)
	{
		for (size_t j = 0; j < number_of_employees; j++)
		{
			if (this->employees[i] == this->bosses[j])
			{
				helper_bosses.push_back(this->bosses[j]);
				helper_employees.push_back(this->employees[j]);
			}
		}
	}

	this->bosses = helper_bosses;
	this->employees = helper_employees;
	helper_bosses.clear();
	helper_employees.clear();
}

string Hierarchy::print() const
{
	string list{};
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		list += this->bosses[i];
		list += "-";
		list += this->employees[i];
		list += "\n";
	}

	return list;
}

int Hierarchy::longest_chain() const
{
	if (this->bosses.get_size() == 0)
		return 0;

	int max_chain = 1;
	Vector<string> chains{};
	size_t index{};
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		for (size_t j = 0; j < number_of_employees; j++)
		{
			if (this->employees[i] == this->bosses[j])
			{
				chains.push_back(this->employees[j]);
				index = j;
				for (size_t k = 0; k < number_of_employees; k++)
				{
					if (this->employees[j] == this->bosses[k])
					{
						chains.push_back(this->employees[k]);
						j = k;
					}
				}

				if (max_chain < chains.get_size() + 2)
					max_chain = chains.get_size() + 2;

				chains.clear();
				j = index;
				continue;
			}

			if (max_chain < chains.get_size() + 2)
				max_chain = chains.get_size() + 2;
		}
	}

	return max_chain;
}

bool Hierarchy::find(const string& name) const
{
	if (name == "Uspeshnia" && this->bosses.get_size() > 0)
		return true;

	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		if (this->employees[i] == name)
			return true;

	return false;
}

int Hierarchy::num_employees() const
{
	if (this->bosses.get_size() > 0)
		return this->employees.get_size() + 1;

	return 0;
}

int Hierarchy::num_overloaded(int level) const
{
	bool flag = false;
	int num = 0, count = 0, index{};
	Vector<string> helper{};
	size_t helper_size{};
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		flag = true;
		index = i;
		for (size_t j = i; j < number_of_employees; j++)
		{
			if (this->employees[i] == this->bosses[j])
			{
				helper_size = helper.get_size();
				for (size_t k = 0; k < helper_size; k++)
				{
					if (helper[k] == this->employees[j])
					{
						flag = false;
						break;
					}
				}
				if (flag)
					helper.push_back(this->employees[j]);

				i = j;
			}
		}

		helper_size = helper.get_size();
		for (size_t j = 0; j < helper_size; j++)
		{
			std::cout << helper[i] << " ";
			count += this->num_subordinates(helper[j]);
		}

		if (count >= level)
			num++;

		helper.clear();
		count = 0;
		i = index;
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
		return -1;

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

	employee_bosses.clear();
	return salary + count * 50;
}

bool Hierarchy::fire(const string& who)
{
	if (who == "Uspeshnia")
		return false;

	if (!this->find(who))
		return false;

	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		if (this->employees[i] == who)
		{
			for (size_t j = i; j < number_of_employees; j++)
				if (this->bosses[j] == who)
					this->bosses[j] = this->bosses[i];

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

	this->bosses.push_back(boss);
	this->employees.push_back(who);
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		if (this->employees[i] == who && this->bosses[i] != boss)
		{
			this->bosses.pop_at(i);
			this->employees.pop_at(i);
			this->fix_list();
			return true;
		}
	}

	this->fix_list();
	return true;
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

void Hierarchy::modernize()
{
	bool flag = true;
	Vector<string> helper{};
	size_t helper_size{};
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
	{
		flag = true;
		helper_size = helper.get_size();
		for (size_t j = 0; j < helper_size; j++)
		{
			if (this->bosses[i] == helper[j])
			{
				flag = false;
				break;
			}
		}

		if (flag)
			helper.push_back(this->employees[i]);
	}

	helper_size = helper.get_size();
	for (size_t i = 0; i < helper_size; i++)
	{
		number_of_employees = this->employees.get_size();
		for (size_t j = 0; j < number_of_employees; j++)
		{
			if (helper[i] == this->employees[j])
			{
				this->fire(this->employees[j]);
				break;
			}
		}
	}
	helper.clear();
	this->fix_list();
}

Hierarchy Hierarchy::join(const Hierarchy& right) const
{
	Vector<string> helper_bosses = this->bosses;
	Vector<string> helper_employees = this->employees;
	size_t right_size = right.employees.get_size();
	for (size_t i = 0; i < right_size; i++)
	{
		if (helper_bosses[i] != right.bosses[i] && helper_employees[i] != right.employees[i])
		{
			helper_bosses.push_back(right.bosses[i]);
			helper_employees.push_back(right.employees[i]);
		}
	}

	size_t combined_hierarchy_size = helper_employees.get_size();
	for (size_t i = 0; i < combined_hierarchy_size; i++)
		for (size_t j = 0; j < combined_hierarchy_size; j++)
			if (helper_bosses[i] == helper_employees[j] && helper_employees[i] == helper_bosses[j])
				return Hierarchy("");

	for (size_t i = 0; i < combined_hierarchy_size; i++)
	{
		for (size_t j = 0; j < combined_hierarchy_size; j++)
		{

		}
	}
}
const string loz_new = "Uspeshnia-MishoPetrov\nMishoPetrov-Misho\nMishoPetrov-Slav\n";
const string lozenec =
"Uspeshnia - Gosho \n"
"Uspeshnia - Misho \n"
"Gosho     - Pesho \n"
"Gosho     - Dancho\n"
"Pesho     - Alex  \n"
"Dancho    - Boris \n"
"Dancho    - Kamen \n"
"Uspeshnia - Slavi \n"
"Slavi     - Slav1 \n"
"Slavi     - Slav2 \n"
"Slav1     - Mecho \n"
"Mecho     - Q12Adl\n";

int main()
{ // Boris-Kosta1\nBoris-Kosta2\nBoris-Kosta3\nBoris-Kosta4\nBoris-Kosta5\nBoris-Kosta6\nBoris-Kosta7\nBoris-Kosta8\nBoris-Kosta9\nBoris-Kosta10\nBoris-Kosta11\nBoris-Kosta12\nBoris-Kosta13\nBoris-Kosta14\nBoris-Kosta15\nBoris-Kosta16\nBoris-Kosta17\nBoris-Kosta18\nBoris-Kosta19\n
	Hierarchy a("      Uspeshnia-Misho   \nUspeshnia -   Gosho\nUspeshnia-  Slavi\nGosho-Pesho\nGosho -Dancho\nSlavi-Slav1\nSlavi-Slav2\nDancho-Boris\nDancho-Kamen\nPesho-Alex\nSlav1-Mecho\nMecho-Q12Adl\n");
	Hierarchy b(a);
	std::cout << b.print() << std::endl;
	std::cout << b.longest_chain() << std::endl;
	
	return 0;
}