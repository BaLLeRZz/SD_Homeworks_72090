#include "Shop.h"

void Shop::add_employee(const Employee& employee)
{
	this->employees.push_back(employee);
}

void Shop::add_client(Client& client)
{
	this->clients.push_back(client);
}

const bool Shop::are_bananas_being_delivered() const
{
	size_t employee_count = this->employees.get_size();
	for (size_t i = 0; i < employee_count; i++)
		if (this->employees[i].get_product_delivering() == 1)
			return true;

	return false;
}

const bool Shop::are_schweppes_being_delivered() const
{
	size_t employee_count = this->employees.get_size();
	for (size_t i = 0; i < employee_count; i++)
		if (this->employees[i].get_product_delivering() == 2)
			return true;

	return false;
}

void Shop::execute_proccess()
{
	size_t number_of_clients = this->clients.get_size();
	size_t clients_left = number_of_clients;
	size_t client_arrival;
	size_t client_want_bananas;
	size_t client_want_schweppes;
	size_t client_max_wait;
	size_t client_number = 0;
	bool should_leave;
	size_t leave_time;
	size_t number_of_employees = this->employees.get_size();
	char employee_status;
	size_t employee_back;
	size_t employee_delivery_product;
	bool should_come_back;
	while (number_of_clients > 0)
	{
		clients_left = number_of_clients;
		for (size_t i = 0; i < clients_left; i++)
		{
			client_arrival = this->clients[i].get_minute_of_arrival();
			client_want_bananas = this->clients[i].get_want_bananas();
			client_want_schweppes = this->clients[i].get_want_schweppes();
			client_max_wait = this->clients[i].get_max_wait();
			leave_time = client_arrival;
			if (this->bananas < client_want_bananas || this->schweppes < client_want_schweppes)
			{
				leave_time += client_max_wait;
				for (size_t j = 0; j < number_of_employees; j++)
				{
					if (this->employees[j].get_status() == 'F')
					{
						if ((int)(this->bananas - client_want_bananas) <= (int)(this->schweppes - client_want_schweppes))
						{
							if (!this->are_bananas_being_delivered() || (this->are_bananas_being_delivered() && this->are_schweppes_being_delivered()))
								this->employees[j].deliver(client_arrival, 1);
							else
							if (!this->are_schweppes_being_delivered() && client_want_schweppes > 0)
								this->employees[j].deliver(client_arrival, 2);
						}
						else
						{
							if (!this->are_schweppes_being_delivered() || (this->are_schweppes_being_delivered() && this->are_bananas_being_delivered()))
								this->employees[j].deliver(client_arrival, 2);
							else
							if (!this->are_bananas_being_delivered() && client_want_bananas > 0)
								this->employees[j].deliver(client_arrival, 1);
						}
					}

					employee_status = this->employees[j].get_status();
					employee_back = this->employees[j].get_minute_back();
					should_come_back = true;
					for (size_t k = 0; k < number_of_clients; k++)
					{
						if (employee_status == 'F' || employee_back > this->clients[k].get_minute_of_arrival() + this->clients[k].get_max_wait())
						{
							should_come_back = false;
							break;
						}
					}

					if (should_come_back)
					{
					    employee_delivery_product = this->employees[j].get_product_delivering();
						this->employees[j].back();
						if (employee_delivery_product == 1)
							this->bananas += 100;

						if (employee_delivery_product == 2)
							this->schweppes += 100;

						leave_time = employee_back;
					}
					break;
				}
			}
			else
			{
				this->clients[i].leave(client_number++, client_arrival, client_want_bananas, client_want_schweppes);
				this->bananas -= client_want_bananas;
				this->schweppes -= client_want_schweppes;
				this->clients.pop_at(i--);
				number_of_clients--;
			}

			should_leave = true;
			for (size_t j = 0; j < number_of_employees; j++)
			{
				if (this->employees[j].get_status() == 'B' && this->employees[j].get_minute_back() >= client_arrival && this->employees[j].get_minute_back() <= client_arrival + client_max_wait)
				{
					should_leave = false;
					break;
				}
			}

			if (should_leave)
			{
				if (this->bananas >= client_want_bananas && this->schweppes < client_want_schweppes)
				{
					this->clients[i].leave(client_number++, leave_time, client_want_bananas, this->schweppes);
					this->bananas -= client_want_bananas;
					this->schweppes = 0;
					this->clients.pop_at(i--);
					number_of_clients--;
					continue;
				}

				if (this->schweppes >= client_want_schweppes && this->bananas < client_want_bananas)
				{
					this->clients[i].leave(client_number++, leave_time, this->bananas, client_want_schweppes);
					this->bananas = 0;
					this->schweppes -= client_want_schweppes;
					this->clients.pop_at(i--);
					number_of_clients--;
					continue;
				}

				if (this->bananas >= client_want_bananas && this->schweppes >= client_want_schweppes)
				{
					this->clients[i].leave(client_number++, leave_time, client_want_bananas, client_want_schweppes);
					this->bananas -= client_want_bananas;
					this->schweppes -= client_want_schweppes;
					this->clients.pop_at(i--);
					number_of_clients--;
					continue;
				}
						
				this->clients[i].leave(client_number++, leave_time, this->bananas, this->schweppes);
				this->bananas = this->schweppes = 0;
				this->clients.pop_at(i--);
				number_of_clients--;	
			}
		}
	}
}