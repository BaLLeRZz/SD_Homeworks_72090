#pragma once
#include<string>
#include<fstream>
#include"Vector.h"

using std::string;

class Hierarchy
{
private:
    Vector<string> bosses{};
    Vector<string> employees{};
    void copy(const Hierarchy& other);
    void erase();
    void get_data_from_string(const string& data);
    void fill_hierarchy(const string& data);
    bool is_correct() const;
    void fix_list();
    bool is_employed_by(const string& who, const string& boss, const Vector<string>& helper_bosses, const Vector<string>& helper_employees) const;
public:
    Hierarchy(Hierarchy&& r) noexcept; // raboti
    Hierarchy(const Hierarchy& r); // raboti
    Hierarchy(const string& data); // raboti
    ~Hierarchy() noexcept; // raboti
    void operator=(const Hierarchy&) = delete; // raboti

    string print() const; // raboti

    int longest_chain() const; // raboti
    bool find(const string& name) const; // raboti
    int num_employees() const; // raboti
    int num_overloaded(int level = 20) const; // raboti

    string manager(const string& name) const; // raboti
    int num_subordinates(const string& name) const; // raboti
    unsigned long getSalary(const string& who) const; // raboti 

    bool fire(const string& who); // raboti
    bool hire(const string& who, const string& boss); // raboti

    void incorporate(); // raboti
    void modernize(); // raboti

    Hierarchy join(const Hierarchy& right) const;
};