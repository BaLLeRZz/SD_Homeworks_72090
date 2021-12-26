#pragma once
#include<string>
#include<fstream>
#include<string>
#include"Vector.h"

using std::string;

struct Node
{
    string value{};
    Vector<Node*> children{};
    Node() = default;
    Node(const string value) : value(value) {};
};

class Hierarchy
{
private:
    Node* root;
    size_t size = 0;
    Node* copy(const Node* other_root) const;
    void erase(Node* current_root);
    string recursive_print(const Node* node) const;
    int recursive_longest_chain(const Node* node, size_t count, size_t current_count) const;
    bool recursive_find(const Node* node, const string& name) const;
    int recursive_num_employees(const Node* node, size_t count) const;
    int recursive_num_overloaded(const Node* node, int level, size_t count, size_t num) const;
    string recursive_manager(const Node* node, const string& name) const;
    int recursive_sub_ordinates(const Node* node, const string& name) const;
    unsigned long recursive_getSalary(const Node* node, const string& who, unsigned long salary) const;
    bool recursive_fire(Node* node, const string& who);
    bool recursive_hire(Node* node, const string& who, const string& boss);
    Node* find_max_salary_employee(const Node* node);
    void recursive_incorporate(Node* node);
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