#pragma once
#include<iostream>

class Client
{
private:
	size_t minute_of_arrival{};
	size_t want_schweppes{};
	size_t want_bananas{};
	size_t max_wait{};
	bool has_left = false;
public:
	Client() = default;
	Client(const size_t& minute_of_arrival,
		    const size_t& want_bananas,
		    const size_t& want_schweppes,
		    const size_t& max_wait) : minute_of_arrival(minute_of_arrival),
		                              want_bananas(want_bananas),
		                              want_schweppes(want_schweppes),
		                              max_wait(max_wait) {};

	const size_t& get_minute_of_arrival() const
	{
		return this->minute_of_arrival;
	}

	const size_t& get_want_bananas() const
	{
		return this->want_bananas;
	}

	const size_t& get_want_schweppes() const
	{
		return this->want_schweppes;
	}


	const size_t& get_max_wait() const
	{
		return this->max_wait;
	}

	void leave(const size_t& client_number,
		       const size_t& leave_time,
		       const size_t& bought_bananas,
		       const size_t& bought_schweppes)
	{
		this->has_left = true;
		std::cout << client_number << " ";
		std::cout << leave_time << " ";
		std::cout << bought_bananas << " ";
		std::cout << bought_schweppes << std::endl;
	}
};