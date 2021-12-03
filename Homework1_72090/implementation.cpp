#include"interface.h"
#include"DLList.h"

struct Employee
{
	char status = 'F'; // F = Free, B = Busy
	int minute_back{};
	int product_delivering{}; // 1 = banana, 2 = schweppes
};

class MyStore : public Store
{
private:
	ActionHandler* handler{};
	DLList<Employee> employees{};
	DLList<Client> clients{};
	DLList<size_t> client_numbers{};
	int bananas{};
	int schweppess{};
	int current_minute{};
public:
	MyStore() = default;

	void init(int workerCount, int startBanana, int startSchweppes);
	void setActionHandler(ActionHandler* handler);
	void addClients(const Client* clients, int count);
	void advanceTo(int minute);
	int getBanana() const;
	int getSchweppes() const;

	void deliver(const int employeeIndex, const int minuteOfDeliveryStart, const int product) const;
	void back(const int employeeIndex);
	void leave(const int clientNumber, const int leaveTime, const int bananasBought, const int schweppesBought) const;
	const bool bananas_delivered() const;
	const bool schweppes_delivered() const;
	const bool free_employees() const;
	void sort_clients();
	void execute_proccess();
};

void MyStore::init(int workerCount, int startBanana, int startSchweppes)
{
	this->bananas = startBanana;
	this->schweppess = startSchweppes;
	Employee employee;
	while (workerCount > 0)
	{
		this->employees.push_back(employee);
		workerCount--;
	}
}

void MyStore::setActionHandler(ActionHandler* handler)
{
	this->handler = handler;
}

void MyStore::addClients(const Client* clients, int count)
{
	for (int i = 0; i < count; i++)
		this->clients.push_back(clients[i]);
}

void MyStore::advanceTo(int minute) // nqkakva struktura ili stek, koqto da pazi vseki event pootdelno
{
	if (minute <= this->current_minute)
		return;

	this->current_minute = minute;
}

int MyStore::getBanana() const
{
	return this->bananas;
}

int MyStore::getSchweppes() const
{
	return this->schweppess;
}

void MyStore::deliver(const int employeeIndex, const int minuteOfDeliveryStart, const int product) const
{
	this->employees[employeeIndex].status = 'B';
	this->employees[employeeIndex].minute_back = minuteOfDeliveryStart + 60;
	this->employees[employeeIndex].product_delivering = product;
	std::cout << "W " << minuteOfDeliveryStart << " ";
	if (product == 1)
		std::cout << "banana" << std::endl;
	else
		std::cout << "schweppes" << std::endl;
}

void MyStore::back(const int employeeIndex)
{
	this->employees[employeeIndex].status = 'F';
	std::cout << "D " << this->employees[employeeIndex].minute_back << " ";
	if (this->employees[employeeIndex].product_delivering == 1)
	{
		std::cout << "banana" << std::endl;
		this->bananas += 100;
	}
	else
	{
		std::cout << "schweppes" << std::endl;
		this->schweppess += 100;
	}
}

void MyStore::leave(const int clientNumber, const int leaveTime, const int bananasBought, const int schweppesBought) const
{
	std::cout << clientNumber << " ";
	std::cout << leaveTime << " ";
	std::cout << bananasBought << " ";
	std::cout << schweppesBought << std::endl;
}

const bool MyStore::bananas_delivered() const
{
	size_t employee_count = this->employees.get_size();
	for (size_t i = 0; i < employee_count; i++)
		if (this->employees[i].product_delivering == 1)
			return true;

	return false;
}

const bool MyStore::schweppes_delivered() const
{
	size_t employee_count = this->employees.get_size();
	for (size_t i = 0; i < employee_count; i++)
		if (this->employees[i].product_delivering == 2)
			return true;

	return false;
}

const bool MyStore::free_employees() const
{
	size_t number_of_employees = this->employees.get_size();
	for (size_t i = 0; i < number_of_employees; i++)
		if (this->employees[i].status == 'F')
			return true;

	return false;
}

void MyStore::sort_clients()
{
	int number_of_clients = this->clients.get_size();
	Client temp;
	for (size_t i = 0; i < number_of_clients; i++)
	{
		for (size_t j = i; j < number_of_clients; j++)
		{
			if (this->clients[i].arriveMinute > this->clients[j].arriveMinute)
			{
				temp = this->clients[i];
				this->clients[i] = this->clients[j];
				this->clients[j] = temp;
			}
		}
	}

	for (size_t i = 0; i < number_of_clients; i++)
		this->client_numbers.push_back(i);
}

void MyStore::execute_proccess()
{
	this->sort_clients();
	int number_of_clients = this->clients.get_size();
	int clients_left = number_of_clients;
	int client_arrival;
	int client_want_bananas;
	int client_want_schweppes;
	int client_max_wait;
	bool should_leave;
	int leave_time;
	int number_of_employees = this->employees.get_size();
	char employee_status;
	int employee_off;
	int employee_back;
	int employee_delivery_product;
	bool should_come_back = false;
	int first_to_come_back = 0;
	bool sent_for_bananas;
	bool sent_for_schweppes;
	while (number_of_clients > 0)
	{
		clients_left = number_of_clients;
		for (size_t i = 0; i < clients_left; i++)
		{
			sent_for_bananas = sent_for_schweppes = false;
			client_arrival = this->clients[i].arriveMinute;
			client_want_bananas = this->clients[i].banana;
			client_want_schweppes = this->clients[i].schweppes;
			client_max_wait = this->clients[i].maxWaitTime;
			leave_time = client_arrival;
			if (should_come_back || leave_time < first_to_come_back)
			{
				leave_time = first_to_come_back;
				should_come_back = false;
			}

			if (this->bananas < client_want_bananas || this->schweppess < client_want_schweppes)
			{
				leave_time = client_arrival + client_max_wait;
				for (size_t j = 0; j < number_of_employees; j++)
				{
					if (sent_for_bananas && sent_for_schweppes)
						break;

					if (sent_for_bananas && client_want_schweppes == 0)
						break;

					if (sent_for_schweppes && client_want_bananas == 0)
						break;

					employee_back = this->employees[j].minute_back;
					if (this->employees[j].status == 'F')
					{
						if (employee_back >= client_arrival && employee_back <= client_arrival + client_max_wait)
							employee_off = employee_back;
						else
							employee_off = client_arrival;

						if (this->bananas - client_want_bananas <= this->schweppess - client_want_schweppes)
						{
							if (!this->bananas_delivered() || (this->bananas_delivered() && this->bananas + 100 < client_want_bananas))
							{
								this->deliver(j, employee_off, 1);
							    sent_for_bananas = true;
							}
							else
							if (this->bananas_delivered() && client_want_schweppes > 0)
							{
								if (!this->schweppes_delivered() || (this->schweppes_delivered() && this->schweppess + 100 < client_want_schweppes))
								{
									this->deliver(j, employee_off, 2);
									sent_for_schweppes = true;
									sent_for_bananas = true;
								}
							}
						}
						else
						{
							if (!this->schweppes_delivered() || (this->schweppes_delivered() && this->schweppess + 100 < client_want_schweppes))
							{
								this->deliver(j, employee_off, 2);
							    sent_for_schweppes = true;
							}

							else
							if (this->schweppes_delivered() && client_want_bananas > 0)
							{
								if (!this->bananas_delivered() || (this->bananas_delivered() && this->bananas + 100 < client_want_bananas))
								{
									this->deliver(j, employee_off, 1);
									sent_for_bananas = true;
									sent_for_schweppes = true;
								}
							}
						}
					}
					
					employee_status = this->employees[j].status;
					employee_back = this->employees[j].minute_back;
					employee_delivery_product = this->employees[j].product_delivering;
					should_come_back = true;

					for (size_t k = 0; k < clients_left; k++)
					{
						if (employee_status == 'F' || employee_back > this->clients[k].arriveMinute + this->clients[k].maxWaitTime)
						{
							should_come_back = false;
							break;
						}
						
						if (employee_back > this->clients[k].arriveMinute)
						{
							if (this->clients[k].banana > 0 && employee_delivery_product == 1 && this->clients[k].schweppes > 0 && !this->schweppes_delivered() && this->free_employees())
							{
								should_come_back = false;
								break;
							}

							if (this->clients[k].schweppes > 0 && employee_delivery_product == 2 && this->clients[k].banana > 0 && !this->bananas_delivered() && this->free_employees())
							{
								should_come_back = false;
								break;
							}
						}
					}

					if (should_come_back)
					{
						this->back(j);
						first_to_come_back = employee_back;
						break;
					}
				}
			}
			else
			{
				this->leave(this->client_numbers[i], leave_time, client_want_bananas, client_want_schweppes);
				this->bananas -= client_want_bananas;
				this->schweppess -= client_want_schweppes;
				this->clients.pop_at(i);
				this->client_numbers.pop_at(i);
				number_of_clients--;
				break;
			}

			if (should_come_back)
				break;
			
			should_leave = true;
			for (size_t j = 0; j < number_of_employees; j++)
			{
				if (this->employees[j].status == 'B' && this->employees[j].minute_back >= client_arrival && this->employees[j].minute_back <= client_arrival + client_max_wait)
				{
					should_leave = false;
					break;
				}
			}

			if (should_leave)
			{
				if (this->bananas >= client_want_bananas && this->schweppess < client_want_schweppes)
				{
					this->leave(this->client_numbers[i], leave_time, client_want_bananas, this->schweppess);
					this->bananas -= client_want_bananas;
					this->schweppess = 0;
					this->clients.pop_at(i);
					this->client_numbers.pop_at(i);
					number_of_clients--;
					break;
				}

				if (this->schweppess >= client_want_schweppes && this->bananas < client_want_bananas)
				{
					this->leave(this->client_numbers[i], leave_time, this->bananas, client_want_schweppes);
					this->bananas = 0;
					this->schweppess -= client_want_schweppes;
					this->clients.pop_at(i);
					this->client_numbers.pop_at(i);
					number_of_clients--;
					break;
				}

				if (this->bananas >= client_want_bananas && this->schweppess >= client_want_schweppes)
				{
					this->leave(this->client_numbers[i], leave_time, client_want_bananas, client_want_schweppes);
					this->bananas -= client_want_bananas;
					this->schweppess -= client_want_schweppes;
					this->clients.pop_at(i);
					this->client_numbers.pop_at(i);
					number_of_clients--;
					break;
				}

				this->leave(this->client_numbers[i], leave_time, this->bananas, this->schweppess);
				this->bananas = this->schweppess = 0;
				this->clients.pop_at(i);
				this->client_numbers.pop_at(i);
				number_of_clients--;
				break;
			}
		}

		if (number_of_clients == 0)
		{
			for (size_t i = 0; i < number_of_employees; i++)
			{
				if (this->employees[i].status == 'B')
				{
					this->back(i);
				}
			}
		}
	}
}

Store* createStore()
{
	return new MyStore();
}

int main()
{
	int employees, clients;
	int arrival_time, want_bananas, want_schweppes, max_wait;
	std::cin >> employees >> clients;
	if (employees <= 0 || clients <= 0)
	{
		std::cout << "You do not have any clients or employees!" << std::endl;
		return 0;
	}

	Client* clientsList = new Client[clients];
	for (int i = 0; i < clients; i++)
	{
		std::cin >> arrival_time >> want_bananas >> want_schweppes >> max_wait;
		clientsList[i] = Client{ arrival_time, want_bananas, want_schweppes, max_wait };
	}
	MyStore store;
	store.init(employees, 0, 0);
	store.addClients(clientsList, clients);
	std::cout << std::endl;
	store.execute_proccess();
	delete[] clientsList;
	return 0;
}