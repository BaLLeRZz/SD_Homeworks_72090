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
    Node* root = new Node("Uspeshnia");
    Vector<Node*> bosses{};
    Vector<Node*> employees{};
    size_t size{};
    Node* copy(const Node* other_root) const;
    void erase(Node* current_root);
    int recursive_longest_chain(Node* node, size_t count, size_t current_count) const;
    int recursive_num_overloaded(const Node* node, int level, size_t count, size_t num) const;
    Node* find_employee(const string& name) const;
    bool recursive_hire(Node* node, const string& who, const string& boss);
    Node* find_max_salary_employee(const Node* node);
    void recursive_incorporate(Node* node);
    void recursive_modernize(Node* node, size_t level);
    void get_data_from_string(const string& data);
    void make_relationships(Node* node);
    void fill_hierarchy(const string& data);
public:
    Hierarchy(Hierarchy&& r) noexcept; // raboti
    Hierarchy(const Hierarchy& r);
    Hierarchy(const string& data); // raboti
    ~Hierarchy() noexcept; // raboti
    void operator=(const Hierarchy&) = delete; // raboti

    string print() const; // raboti

    int longest_chain() const;
    bool find(const string& name) const; // raboti
    int num_employees() const; // raboti
    int num_overloaded(int level = 20) const;

    string manager(const string& name) const; // raboti
    int num_subordinates(const string& name) const; // raboti
    unsigned long getSalary(const string& who) const; // raboti

    bool fire(const string& who); // raboti
    bool hire(const string& who, const string& boss);

    void incorporate();
    void modernize();

    //Hierarchy join(const Hierarchy& right) const;
};