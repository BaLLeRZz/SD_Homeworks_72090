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
    Hierarchy(Hierarchy&& r) noexcept;
    Hierarchy(const Hierarchy& r);
    Hierarchy(const string& data);
    ~Hierarchy() noexcept;
    void operator=(const Hierarchy&) = delete;

    string print() const;

    int longest_chain() const;
    bool find(const string& name) const;
    int num_employees() const;
    int num_overloaded(int level = 20) const;

    string manager(const string& name) const;
    int num_subordinates(const string& name) const;
    unsigned long getSalary(const string& who) const; 

    bool fire(const string& who);
    bool hire(const string& who, const string& boss);

    void incorporate();
    void modernize();

    Hierarchy join(const Hierarchy& right) const;
};