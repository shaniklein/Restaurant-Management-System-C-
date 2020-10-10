#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    virtual ~Customer();
    virtual Customer* clone()=0;
    std::string getName() const;
    int getId() const;
private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    VegetarianCustomer* clone();
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    CheapCustomer* clone();
    void setOrdered(bool _ordered);

private:
    bool ordered;
};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    SpicyCustomer* clone();
    void setOrdered(bool _ordered);



private:
    bool ordered;
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    void setAlcohol(int orderedTime,int CheapestAlcoholPrice,int nextCheapest,int CheapestAlcoholId );
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    AlchoholicCustomer* clone();
    AlchoholicCustomer(const AlchoholicCustomer &other);


private:
    int orderTime;
    int CheapestAlcoholPrice;
    int nextCheapest;
    int CheapestAlcoholId;
    int nextCheapestId;


};


#endif
