#pragma once
#include<iostream>

class Employee
{
private:
	char status = 'F'; // Free
	size_t minute_back{};
	size_t product_delivering{}; // 1 = bananas, 2 = schweppes
public:
	Employee() = default;

	const char get_status() const
	{
		return this->status;
	}

	const size_t get_minute_back() const
	{
		return this->minute_back;
	}

	const size_t get_product_delivering() const
	{
		return this->product_delivering;
	}

	void deliver(const size_t& minute_of_delivery_start, size_t product)
	{
		this->status = 'B'; // Busy
		this->minute_back = minute_of_delivery_start + 60;
		std::cout << "W ";
		std::cout << minute_of_delivery_start << " ";
		this->product_delivering = product;
		if (product == 1)
			std::cout << "bananas" << std::endl;

		if (product == 2)
			std::cout << "schweppes" << std::endl;
	}

	void back()
	{
		this->status = 'F';
		std::cout << "D ";
		std::cout << this->minute_back << " ";

		if (this->product_delivering == 1)
			std::cout << "bananas" << std::endl;

		if (this->product_delivering == 2)
			std::cout << "schweppes" << std::endl;
	}
};