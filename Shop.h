#pragma once
#include"Client.h"
#include"Employee.h"
#include"DLList.h"
class Shop
{
private:
	DLList<Employee> employees{};
	DLList<Client> clients{};
	size_t bananas{};
	size_t schweppes{};
public:
	Shop() = default;

	void add_employee(const Employee& employee);
	void add_client(Client& client);
	const bool are_bananas_being_delivered() const;
	const bool are_schweppes_being_delivered() const;
	void execute_proccess();
};