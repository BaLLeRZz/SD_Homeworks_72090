#include"Shop.h"
int main()
{
	size_t employees, clients;
	std::cin >> employees >> clients;
	Shop shop;
	size_t moment_of_arrival, want_schweppes, want_bananas, max_wait;
	Employee employee;
	for (size_t i = 0; i < employees; i++)
	{
		shop.add_employee(employee);
	}
	for (size_t i = 0; i < clients; i++)
	{
		std::cin >> moment_of_arrival >> want_schweppes >> want_bananas >> max_wait;
		Client client(moment_of_arrival, want_schweppes, want_bananas, max_wait);
		shop.add_client(client);
	}
	std::cout << std::endl;
	shop.execute_proccess();
	return 0;
}