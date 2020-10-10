#include <vector>
#include "../include/Customer.h"
#include "../include/Dish.h"
#include "../include/Table.h"
#include <iostream>

typedef std::pair<int, Dish> OrderPair;

    Table::Table(int t_capacity):capacity(t_capacity),open(false),customersList(),orderList(),cutomers2remove(){

    }

    Table::Table(const Table &other):capacity(other.capacity),open(other.open),customersList(),orderList(),cutomers2remove(){

        for (int j = 0; (unsigned) j< other.orderList.size(); ++j) {
            OrderPair orderPair=OrderPair(other.orderList[j].first,other.orderList[j].second);
            orderList.push_back(orderPair);
        }

        for (int i = 0; (unsigned )i <other.customersList.size(); ++i) {
            customersList.push_back(other.customersList[i]->clone());
        }

        for (int k = 0; k < (int) other.cutomers2remove.size(); ++k) {
            cutomers2remove.push_back(other.cutomers2remove[k]->clone());

        }

    }




Table::Table(Table &&other):capacity(other.capacity),open(other.open),customersList(),orderList(),cutomers2remove() {
    for(Customer* customer:other.customersList)
        customersList.push_back(customer);
    for(OrderPair pair: other.orderList)
        orderList.push_back(pair);
    for (int i = 0; i <(int)other.cutomers2remove.size() ; ++i)
        cutomers2remove.push_back(cutomers2remove[i]);

    other.customersList.clear();
    other.orderList.clear();
    other.cutomers2remove.clear();
}


    Table::~Table() {
        for (int i = 0; (unsigned) i < customersList.size(); ++i) {
            if (customersList[i] != nullptr) {
                delete (customersList[i]);
                customersList[i] = nullptr;
            }
        }
        customersList.clear();

        for (int i = 0; (unsigned) i < cutomers2remove.size(); ++i) {
            if (cutomers2remove[i] != nullptr) {
                delete (cutomers2remove[i]);
                cutomers2remove[i] = nullptr;
            }
        }
        cutomers2remove.clear();
    }


        Table& Table::operator=(const Table &other) {
        if(this==&other) return *this;
        capacity=other.capacity;
        open=other.open;

        orderList.clear();
        for (int j = 0; (unsigned)j < other.orderList.size(); ++j) {
            OrderPair orderPair=OrderPair(other.orderList[j].first,other.orderList[j].second);
            orderList.push_back(orderPair);
        }

        std::vector<Customer*> temp=*new std::vector<Customer*>();
        for (int i = 0; (unsigned)i < customersList.size(); ++i) {//delete old data
            temp.push_back(other.customersList[i]);
            delete(customersList[i]);
        }
        customersList.clear();
        for (int j = 0; (unsigned)j <temp.size() ; ++j) {
            addCustomer(temp[j]);
        }
            customersList.clear();

        for (int i = 0; (unsigned) i < cutomers2remove.size(); ++i) {
                if (cutomers2remove[i] != nullptr) {
                    delete (cutomers2remove[i]);
                    cutomers2remove[i] = nullptr;
                }
            }
            cutomers2remove.clear();
        for (int k = 0; k < (int) other.cutomers2remove.size(); ++k) {
            cutomers2remove.push_back(other.cutomers2remove[k]);
            }

        return *this;
    }


Table& Table::operator=(Table &&other) {
    for (int i = 0;(unsigned) i <customersList.size() ; ++i) {
        delete customersList[i];
    }  for (int i = 0;(unsigned) i <cutomers2remove.size() ; ++i) {
        delete cutomers2remove[i];
    }
    customersList.clear();
    orderList.clear();

    capacity=other.capacity;
    open=other.open;
    for(Customer* customer:other.customersList)
        customersList.push_back(customer);
    for(OrderPair pair: other.orderList)
        orderList.push_back(pair);
    for (int j = 0; j <(int) other.cutomers2remove.size(); ++j) {
        cutomers2remove.push_back(other.cutomers2remove[j]);
    }
    other.customersList.clear();
    other.orderList.clear();
    other.cutomers2remove.clear();
    return *this;
}



int Table::getCapacity() const {
        return this->capacity;
    }


    void  Table::addCustomer(Customer* customer){
        if((signed)customersList.size()<capacity)
            customersList.push_back(customer);


    }

    void Table::removeCustomer(int id) {
        //delete from customerlist
        for (int i = 0; (unsigned) i < customersList.size(); i++) {
            if(customersList[i]->getId()==id)
                customersList.erase(customersList.begin() + i);
        }
                //delete from orderList
                std::vector<OrderPair> orderListCopy;
                for (int j = 0; (unsigned) j < orderList.size(); ++j) {
                    if (orderList[j].first != id) {
                        orderListCopy.push_back(orderList[j]);
                    }
                }

                    orderList.clear();
                    for (int j = 0; (unsigned) j < orderListCopy.size(); ++j)
                        orderList.push_back(orderListCopy[j]);

                }





    Customer*  Table::getCustomer(int id) {
            for (int i = 0; (unsigned)i < customersList.size(); ++i) {
                if(customersList[i]->getId()==id)
                    return customersList[i];
            }

       return nullptr;
    }


    std::vector<Customer*>&  Table::getCustomers() {
            return  customersList;
    }
    std::vector<OrderPair>&  Table::getOrders() {
        return orderList;
    }

    void  Table::order(const std::vector<Dish> &menu){
        for (int i=0; (unsigned) i<customersList.size();i++) {
            std::vector<int> DishId = customersList[i]->order(menu);

            for (int j = 0; (unsigned)j < DishId.size(); j++) {

                OrderPair  pair =OrderPair ((*customersList[i]).getId(),menu[DishId[j]]);
                orderList.push_back(pair);
            }
        }

    }
    void Table::openTable(){
       orderList.clear();

        for (int i = 0; i < (int)customersList.size(); ++i) {
            cutomers2remove.push_back(customersList[i]);
        }
        customersList.clear();
        open=true;
    }
    void Table::closeTable(){
        open=false;




    }

    int Table::getBill() {
        int bill=0;
        for (int i = 0; (unsigned) i < orderList.size(); ++i) {
            bill+=orderList[i].second.getPrice();
        }
        return bill;
    }
    bool Table::isOpen(){
        return open==true;
    }


    void Table::setOders(OrderPair pair) {
        this->orderList.push_back(pair);}
