#pragma once
#include<iostream>

enum ResourceType
{
	banana,
	schweppes,
};

struct ActionHandler
{
	virtual void onWorkerSend(int minute, ResourceType resource) = 0;
	virtual void onWorkerBack(int minute, ResourceType resource) = 0;
	virtual void onClientDepart(int index, int minute, int banana, int schweppes) = 0;
};

struct Client
{
	int arriveMinute{};
	int banana{};
	int schweppes{};
	int maxWaitTime{};

	Client() = default;
	Client& operator=(const Client& other)
	{
		if (this != &other)
		{
			this->arriveMinute = other.arriveMinute;
			this->banana = other.banana;
			this->schweppes = other.schweppes;
			this->maxWaitTime = other.maxWaitTime;
		}
		return *this;
	}
};

struct Store
{
	virtual void init(int workerCount, int startBanana, int startSchweppes) = 0;
	virtual void setActionHandler(ActionHandler* handler) = 0;
	virtual void addClients(const Client* clients, int count) = 0;
	virtual void advanceTo(int minute) = 0;
	virtual int getBanana() const = 0;
	virtual int getSchweppes() const = 0;
	virtual ~Store() {}
};

Store* createStore();