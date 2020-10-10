#include <vector>
#include <string>
#include <Action.h>

#include "../include/Dish.h"
#include "../include/Customer.h"
#include "../include/Action.h"
#include "../include/Restaurant.h"
#include "../include/Table.h"



extern Restaurant* backup;

//class BaseAction
BaseAction::BaseAction():errorMsg(),status(ActionStatus::PENDING){}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete(){ status=ActionStatus::COMPLETED;}

void BaseAction::error(std::string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg=errorMsg;
    std::cout <<"Error: "+ errorMsg << std::endl;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}
std::string BaseAction::statusToString() const {
    switch (getStatus()) {
        case (ActionStatus::PENDING): {
            return " Pending";

        }
        case (ActionStatus::COMPLETED): {
            return " Completed";

        }
        default:// (ActionStatus::ERROR): {
            return " Error: " + getErrorMsg();
    }


}


void BaseAction::setStatus(ActionStatus actionStatus) {
    status=actionStatus;
}

BaseAction::~BaseAction() {

}

void BaseAction::CopyAction(ActionStatus status1, std::string ErrorMsg) {
    this->setStatus(status1);
    this->errorMsg=ErrorMsg;
}





//class OpenTable : public BaseAction

OpenTable::OpenTable(int id, std::vector<Customer*> &customersList):BaseAction(),tableId(id),customers(customersList){}

void OpenTable::act(Restaurant &restaurant) {

    if(restaurant.getTable(tableId)==nullptr||restaurant.getTable(tableId)->isOpen()) {
        std::string error_msg ="Table does not exist or is already open";
        error(error_msg);
    }
    else {
        restaurant.getTable(tableId)->openTable();

        for (int i = 0; (unsigned) i < customers.size(); ++i)
            restaurant.getTable(tableId)->addCustomer(customers[i]->clone());


        complete();

    }
}

OpenTable* OpenTable::clone() {
    return new OpenTable(*this);

}


std::string OpenTable::toString() const {
    std::string customersString;
    for (int i = 0; (unsigned)i < customers.size(); ++i) {
        customersString.append(customers[i]->toString());
        customersString.append (" ");
    }
    customersString.pop_back();
    return "open " + std::to_string(tableId) +" "+ customersString+statusToString();


}

OpenTable::OpenTable(const OpenTable &other):BaseAction(),tableId(other.tableId),customers() {
    for (int i = 0; (unsigned) i < other.customers.size(); ++i) {
        customers.push_back(other.customers[i]->clone());

    }
    this->CopyAction(other.getStatus(),other.getErrorMsg());



}

OpenTable::~OpenTable() {
    for (Customer *customer : customers) {
        delete customer;
    }
    customers.clear();
}

OpenTable::OpenTable(OpenTable &&other):BaseAction(),tableId(other.tableId),customers(other.customers) {}

Order::Order(int id):tableId(id) {}
void Order::act(Restaurant &restaurant) {
    if(restaurant.getTable(tableId)== nullptr||!restaurant.getTable(tableId)->isOpen()){
        std::string errMsg="Table does not exist or is not open";
        error(errMsg);
    }
    else{
        restaurant.getTable(tableId)->order(restaurant.getMenu());
        complete();
    }


}
std::string Order::toString() const {
    return "order "+std::to_string(tableId)+statusToString();
}

Order *Order::clone() {
    Order *order=new Order(this->tableId);
    order->CopyAction(this->getStatus(),this->getErrorMsg());

    return order;
}


//class MoveCustomer : public BaseAction {

MoveCustomer::MoveCustomer(int src, int dst, int customerId):srcTable(src),dstTable(dst),id(customerId) {}
void MoveCustomer::act(Restaurant &restaurant) {
    Table *dsttable = restaurant.getTable(dstTable);
    Table *srctable = restaurant.getTable(srcTable);

    if (dsttable == nullptr || !dsttable->isOpen() ||
        (unsigned)(dsttable->getCapacity()) == (unsigned )dsttable->getCustomers().size() || ( srctable->getCustomer(id)) ==
                                                                                             nullptr || !srctable->isOpen()) {
        error("Cannot move customer");
    } else {

        Customer *customer = srctable->getCustomer(id);

        //add customer to the new table
        dsttable->addCustomer(customer);
        std::vector<OrderPair> ordertomove = srctable->getOrders();

        //remove orderds
        for (int i = 0;(unsigned) i < ordertomove.size(); ++i) {
            if (ordertomove[i].first == id) {
                dsttable->setOders(OrderPair(ordertomove[i].first, ordertomove[i].second));
            }
        }


        srctable->removeCustomer(id);

        //incase the source table is empty,delete it
        if (srctable->getCustomers().size() == 0)
            srctable->closeTable();
        complete();
    }


}

std::string MoveCustomer::toString() const {
    return "move "+std::to_string(srcTable)+" "+std::to_string(dstTable)+" "+std::to_string(id)+statusToString();
}

MoveCustomer *MoveCustomer::clone() {
    MoveCustomer *moveCustomer= new MoveCustomer(srcTable,dstTable,id);
    moveCustomer->CopyAction(this->getStatus(),this->getErrorMsg());
    return moveCustomer;
};



//class Close : public BaseAction {
Close::Close(int id):tableId(id) {}
void Close::act(Restaurant &restaurant){
    if(restaurant.getTable(tableId)== nullptr||!restaurant.getTable(tableId)->isOpen()){
        std::string errMsg="Table does not exist or is not open";
        error(errMsg);
    }
    else{
        restaurant.getTable(tableId)->closeTable();
        std:: cout<< "Table "<<tableId<<" was closed. Bill "<<restaurant.getTable(tableId)->getBill()<<"NIS"<< std::endl;
        complete();
    }
}
std::string Close::toString() const {
    return "close "+std::to_string(tableId)+statusToString();
}

Close *Close::clone() {
    Close* close=new Close(tableId);
    close->CopyAction(this->getStatus(),this->getErrorMsg());
    return close;
}


//class CloseAll : public BaseAction {
CloseAll::CloseAll() {}
void CloseAll::act(Restaurant &restaurant) {
    for (int i = 0; (unsigned)i <(unsigned) restaurant.getNumOfTables(); ++i) {
        if(restaurant.getTable(i)->isOpen()){
            Close close=Close(i);
            close.act(restaurant);
        }

    }
    complete();
}
std::string CloseAll::toString() const {
    return "closeAll "+statusToString();
}

CloseAll *CloseAll::clone() {
    CloseAll* closeAll= new CloseAll();
    closeAll->CopyAction(this->getStatus(),this->getErrorMsg());
    return closeAll;
}


PrintMenu::PrintMenu():BaseAction() {}

void PrintMenu::act(Restaurant &restaurant){
    for (int i = 0;(unsigned) i <restaurant.getMenu().size() ; ++i) {
        Dish dish= restaurant.getMenu()[i];
        std::cout<<dish.toString()<<"NIS"<<std::endl;
    }
    complete();
}

PrintMenu* PrintMenu::clone() {
    PrintMenu *printMenu= new PrintMenu();
    printMenu->CopyAction(this->getStatus(),this->getErrorMsg());
    return printMenu;
}

std::string PrintMenu::toString() const {
    return "menu"+statusToString();
}


PrintTableStatus::PrintTableStatus(int id):tableId(id) {}

void PrintTableStatus::act(Restaurant &restaurant){

    Table* table=restaurant.getTable(tableId);
    bool open= table->isOpen();
    if(!open)
        std::cout<<"Table "<<tableId<<" status: closed"<< std::endl;
    else {
        std::cout << "Table " << tableId << " status: open" << std::endl;
        std::cout << "Customers: " << std::endl;
        std::vector<Customer*> customers =table->getCustomers();
        for (int i = 0; (unsigned)i < customers.size(); ++i) {
            {
                std::string name = customers[i]->getName();
                std::cout << customers[i]->getId() << " " << name << std::endl;
            }
        }

        std::cout << "Orders:" << std::endl;
        for (int j = 0; (unsigned) j < table->getOrders().size(); ++j) {
            Dish dish = table->getOrders()[j].second;
            int customerId = table->getOrders()[j].first;
            std::cout << dish.getName()<<" " << dish.getPrice() <<"NIS "<< customerId << std::endl;
        }
        std::cout << "Current Bill: " << table->getBill() << "NIS" << std::endl;
    }
    complete();
}
std::string PrintTableStatus::toString() const {
    return "status "+std::to_string(tableId)+statusToString();
}

PrintTableStatus *PrintTableStatus::clone() {
    PrintTableStatus *printTableStatus= new PrintTableStatus(this->tableId);
    printTableStatus->CopyAction(this->getStatus(),this->getErrorMsg());
    return  printTableStatus;
}


//class PrintActionsLog : public BaseAction {
PrintActionsLog::PrintActionsLog():BaseAction() {}
void PrintActionsLog::act(Restaurant &restaurant){
    const std::vector<BaseAction*> actions=restaurant.getActionsLog();

    for (int i = 0;(unsigned) i < actions.size(); ++i) {
        std::cout<< actions[i]->toString()<<std::endl;

    }
    complete();
}

std::string PrintActionsLog::toString() const {
    return "log"+statusToString();
}

PrintActionsLog *PrintActionsLog::clone() {
    PrintActionsLog *printActionsLog= new PrintActionsLog();
    printActionsLog->CopyAction(this->getStatus(),this->getErrorMsg());
    return printActionsLog;
}

//class BackupRestaurant : public BaseAction {
BackupRestaurant::BackupRestaurant():BaseAction() {}
void BackupRestaurant::act(Restaurant &restaurant) {
    if (backup != nullptr) {
        delete backup;
        backup = nullptr;
        backup = new Restaurant(restaurant);
        complete();
    }
    else {
        backup = new Restaurant(restaurant);
        complete();
    }
}

std::string BackupRestaurant::toString() const {
    return "backup"+statusToString();
}

BackupRestaurant *BackupRestaurant::clone() {
    BackupRestaurant *backupRestaurant= new BackupRestaurant();
    backupRestaurant->CopyAction(this->getStatus(),this->getErrorMsg());
    return backupRestaurant;
}


RestoreResturant::RestoreResturant():BaseAction() {}
void RestoreResturant::act(Restaurant &restaurant) {
    if(backup==nullptr){
        std::string error= "No backup available";
        this->error(error);
    }
    else {
        restaurant = *backup;
        complete();
    }
}


std::string RestoreResturant::toString() const {
    return "restore"+statusToString();
}

RestoreResturant* RestoreResturant::clone() {
    RestoreResturant *restoreResturant= new RestoreResturant();
    restoreResturant->CopyAction(this->getStatus(),this->getErrorMsg());
    return restoreResturant;
}