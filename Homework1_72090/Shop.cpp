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
	size_t number_of_employees = this->employees.get_size();
	if (number_of_clients == 0 || number_of_employees == 0)
	{
		std::cout << "You don't have any clients or employees!" << std::endl;
		return;
	}

	size_t client_count = 0;
	while (number_of_clients > 0)
	{
		size_t clients_left = number_of_clients;
		for (size_t k = 0; k < clients_left; k++)
		{
	        size_t leave_time = this->clients[k].get_minute_of_arrival();
			if (this->bananas < this->clients[k].get_want_bananas() || this->schweppes < this->clients[k].get_want_schweppes())
			{
				leave_time += this->clients[k].get_max_wait();
				for (size_t i = 0; i < number_of_employees; i++)
				{
					if (this->employees[i].get_status() == 'F')
					{
						if (this->bananas - this->clients[k].get_want_bananas() >= this->schweppes - this->clients[k].get_want_schweppes())
						{
							if (!this->are_bananas_being_delivered() || (this->are_bananas_being_delivered() && this->are_schweppes_being_delivered()))
								this->employees[i].deliver(this->clients[k].get_minute_of_arrival(), 1);
							else
								if (!this->are_schweppes_being_delivered() && this->clients[k].get_want_schweppes() > 0)
									this->employees[i].deliver(this->clients[k].get_minute_of_arrival(), 2);
						}
						else
						{
							if (!this->are_schweppes_being_delivered() || (this->are_schweppes_being_delivered() && this->are_bananas_being_delivered()))
								this->employees[i].deliver(this->clients[k].get_minute_of_arrival(), 2);
							else
								if (!this->are_bananas_being_delivered() && this->clients[k].get_want_bananas() > 0)
									this->employees[i].deliver(this->clients[k].get_minute_of_arrival(), 1);
						}
					}

					bool should_come_back = false;
					for (size_t j = 0; j < number_of_clients; j++)
					{
						if (this->employees[i].get_status() == 'F' || this->employees[i].get_minute_back() > this->clients[j].get_minute_of_arrival() + this->clients[j].get_max_wait() && this->employees[i].get_minute_back() > this->clients[j].get_minute_of_arrival())
							break;

						if (j == number_of_clients - 1)
							should_come_back = true;
					}

					if (should_come_back)
					{
						this->employees[i].back();
						if (this->employees[i].get_product_delivering() == 1)
							this->bananas += 100;

						if (this->employees[i].get_product_delivering() == 2)
							this->schweppes += 100;
						
						leave_time = this->clients[k].get_minute_of_arrival() - (this->clients[k].get_minute_of_arrival() + this->clients[k].get_max_wait() - (this->employees[i].get_minute_back() + this->clients[k].get_max_wait()));
					}
					break;
				}
			}
			else
			{
				this->clients[k].leave(client_count, this->clients[k].get_minute_of_arrival(), this->clients[k].get_want_bananas(), this->clients[k].get_want_schweppes());
				this->bananas -= this->clients[k].get_want_bananas();
				this->schweppes -= this->clients[k].get_want_schweppes();
				this->clients.pop_at(k);
				k--;
				client_count++;
				number_of_clients--;
			}
			bool leave = true;
			size_t client_arrival = this->clients[k].get_minute_of_arrival();
			size_t client_max_wait = this->clients[k].get_max_wait();
			size_t client_want_bananas = this->clients[k].get_want_bananas();
			size_t client_want_schweppes = this->clients[k].get_want_schweppes();
			for (size_t i = 0; i < number_of_employees; i++)
			{
				if (this->employees[i].get_status() == 'B' && this->employees[i].get_minute_back() >= client_arrival && this->employees[i].get_minute_back() <= client_arrival + client_max_wait)
				{
					leave = false;
					break;
				}
			}

			if (leave)
			{
				if (this->bananas >= client_want_bananas && this->schweppes < client_want_schweppes)
				{
					this->clients[k].leave(client_count, leave_time, client_want_bananas, this->schweppes);
					this->bananas -= client_want_bananas;
					this->schweppes = 0;
					this->clients.pop_at(k);
					k--;
					client_count++;
					number_of_clients--;
				}
				else
					if (this->schweppes >= client_want_schweppes && this->bananas < client_want_bananas)
					{
						this->clients[k].leave(client_count, leave_time, this->bananas, client_want_schweppes);
						this->bananas = 0;
						this->schweppes -= client_want_schweppes;
						this->clients.pop_at(k);
						k--;
						client_count++;
						number_of_clients--;
					}
					else
						if (this->bananas >= client_want_bananas && this->schweppes >= client_want_schweppes)
						{
							this->clients[k].leave(client_count, leave_time, client_want_bananas, client_want_schweppes);
							this->bananas -= client_want_bananas;
							this->schweppes -= client_want_schweppes;
							this->clients.pop_at(k);
							k--;
							client_count++;
							number_of_clients--;
						}
						else
						{
							this->clients[k].leave(client_count, leave_time, this->bananas, this->schweppes);
							this->bananas = this->schweppes = 0;
							this->clients.pop_at(k);
							k--;
							client_count++;
							number_of_clients--;
						}
			}
		}
	}
}