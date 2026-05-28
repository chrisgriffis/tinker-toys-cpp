#include <stdio.h>
#include <string>
#include <vector>
#include <stack>

using std::string;
using std::vector;
 
// IMPORTANT: DO NOT MODIFY THIS CLASS
class Employee {
private:
    const int id;
    const string name;
    vector<Employee*> reports;
 
public:
    Employee(int id, string name) : id(id), name(name) {
    }
 
    int getId() {
        return id;
    }
 
    vector<Employee*>& getReports() {
        return reports;
    }
 
    void addReport(Employee* employee) {
        reports.push_back(employee);
    }
};
 
/*
 * Read the attached PDF for more explanation about the problem
 * Note: Don't modify the signature of this function
 * @param ceo
 * @param firstEmployee
 *
 * @param secondEmployee
 *
 * @return common manager for both the employee that is closest to them.
 */

//Space complexity S(n) = O(lg N) for stack depth
//Time complexity T(n) = O(|V|+|E|) for edge based search graph. 
//performs two DFS searches but # of traversals is fixed and does not change time complexity
static bool GetManagementChain(std::stack<Employee*>& managers, Employee*& employee);
static bool IsASubordinate(Employee* const& manager, Employee* const& employee);

static Employee* closestCommonManager(Employee* ceo, Employee* firstEmployee, Employee* secondEmployee) {
    // Implement me
    std::stack<Employee*> managementChain;
    managementChain.push(ceo);
    GetManagementChain(managementChain,firstEmployee);
    while (!managementChain.empty())
    {
        Employee* boss = managementChain.top();
        if (IsASubordinate(boss,secondEmployee))
        {
            return boss;
        }
        managementChain.pop();
    }
    return NULL;
}

static bool GetManagementChain(std::stack<Employee*>& managers, Employee*& employee)
{
    using namespace std;
    if (!managers.empty())
    {
		for(auto directReport : managers.top()->getReports())
        {
            if (employee == directReport) return true;
            managers.push(directReport);
            if(GetManagementChain(managers,employee)) return true;
            managers.pop();
        }
    }
    return false;
}

static bool IsASubordinate(Employee* const& manager, Employee* const& employee)
{
    if (manager != NULL)
        for(auto directReport : manager->getReports())
			if ((employee == directReport) || IsASubordinate(directReport,employee))
				return true;
    return false;
}

int main()
{
    Employee chairman(1,"chairman");
        Employee a(1,"a");
            Employee b(1,"b");
            Employee c(1,"c");
                Employee d(1,"d");
                Employee e(1,"e");
            Employee f(1,"f");
                Employee g(1,"g");
                Employee h(1,"h");
                    Employee i(1,"i");
                    Employee j(1,"j");

    chairman.addReport(&a);

    a.addReport(&b);
    a.addReport(&c);
    a.addReport(&f);

    c.addReport(&d);
    c.addReport(&e);

    f.addReport(&g);
    f.addReport(&h);

    h.addReport(&i);
    h.addReport(&j);


    bool ShouldBeTrue1 = (&a == closestCommonManager(&chairman,&b,&e));
    bool ShouldBeTrue2 = (&c == closestCommonManager(&chairman,&d,&e));
    bool ShouldBeTrue3 = (&f == closestCommonManager(&chairman,&i,&g));
    bool ShouldBeTrue4 = (&a == closestCommonManager(&chairman,&j,&e));
    bool ShouldBeTrue5 = (&f == closestCommonManager(&chairman,&j,&h));
    bool ShouldBeTrue5b = (&h != closestCommonManager(&chairman,&j,&h));
    bool ShouldBeTrue6 = (&a == closestCommonManager(&chairman,&c,&h));

    bool ShouldBeTrue7 = (NULL == closestCommonManager(&chairman,&a,&chairman));
    bool ShouldAlsoBeTrueButIsnt7 = (NULL == closestCommonManager(&chairman,&chairman,&a));

    return 0;
}