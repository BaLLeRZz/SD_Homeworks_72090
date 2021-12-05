#pragma once
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
	int current_minute = -1;
	bool client_has_left{};
	bool sent_for_delivery{};
	bool back_from_delivery{};
public:
	MyStore() = default;

	void init(int workerCount, int startBanana, int startSchweppes);
	void setActionHandler(ActionHandler* handler);
	void addClients(const Client* clients, int count);
	void advanceTo(int minute);
	int getBanana() const;
	int getSchweppes() const;

	void deliver(const int employeeIndex, const int minuteOfDeliveryStart, const int product);
	void back(const int employeeIndex);
	void leave(const int clientNumber, const int leaveTime, const int bananasBought, const int schweppesBought);
	const bool bananas_delivered() const;
	const bool schweppes_delivered() const;
	const bool free_employees() const;
	void sort_clients();
	const int get_max_minute() const; // For the main function to complete the entire porccess
};
