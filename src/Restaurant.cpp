#include <vector>
#include <string>
#include "../include/Dish.h"
#include "../include/Customer.h"
#include "../include/Action.h"
#include "../include/Restaurant.h"
#include "../include/Table.h"
#include <fstream>
#include <sstream>
#include <Restaurant.h>

//0504840490



Restaurant::Restaurant():open(false),tables(),menu(),actionsLog(),numOfCustomers(0){}


Restaurant::~Restaurant() {

    for (int i = 0; (unsigned)i < actionsLog.size(); ++i) {
        delete (actionsLog[i]);
        actionsLog[i] = nullptr;
    }
actionsLog.clear();

    for (int k = 0;(unsigned) k < tables.size(); ++k) {
        delete (tables[k]);
        tables[k] = nullptr;
    }

    tables.clear();

    }


    Restaurant::Restaurant(const Restaurant &other):open(other.open),tables(),menu(),actionsLog(),numOfCustomers(other.numOfCustomers) {

      //Copy tables
        for (int k = 0;(unsigned) k <(unsigned) other.getNumOfTables(); ++k) {
           Table *table= other.tables[k];
           tables.push_back(new Table(*table));
        }

        //copy Dishes
        for (int j = 0;(unsigned) j < other.menu.size(); ++j) {
            menu.push_back(other.menu[j]);
        }
        //Copy actionlog
        for (unsigned int i = 0; i < other.actionsLog.size(); ++i) {
                actionsLog.push_back(other.actionsLog[i]->clone());
        }

}

Restaurant::Restaurant(Restaurant &&other):open(other.open),tables(),menu(),actionsLog(),numOfCustomers(other.numOfCustomers) {
    for (Table* table: other.tables) {
        tables.push_back(table);
    }
    for(BaseAction* action:other.actionsLog){
        actionsLog.push_back(action);
    }
    for (int i = 0; i <(int)other.menu.size(); ++i) {
        menu.push_back(menu[i]);
    }
    other.tables.clear();
    other.menu.clear();
    other.actionsLog.clear();
}


Restaurant& Restaurant::operator=(const Restaurant &other) {
    if (this == &other) return *this;
    open = other.open;

    //delete old actions
    for (int j = 0;(unsigned) j < actionsLog.size(); ++j) {
        delete actionsLog[j];
    }
    actionsLog.clear();
    //add new actions
    for (int i = 0; (unsigned) i < other.actionsLog.size(); ++i) {
        actionsLog.push_back(other.actionsLog[i]->clone());
    }

    //delete old tables
    for (int j = 0; (unsigned)j < tables.size(); ++j) {
        delete tables[j];
    }
    tables.clear();

    for (int k = 0; k <(int) other.getNumOfTables(); ++k) {
        const Table *table = other.tables[k];
        tables.push_back(new Table(*table));

        //update the dishes of the menu
        menu.clear();

        for (int j = 0;(unsigned) j < other.menu.size(); ++j) {
            menu.push_back(other.menu[j]);
        }
    }
    return *this;
}


Restaurant& Restaurant::operator=(Restaurant &&other) {
    for (int i = 0; (unsigned) i <tables.size() ; ++i) {
        delete tables[i];
    }
    tables.clear();
    for (int j = 0; (unsigned) j <actionsLog.size() ; ++j) {
        delete actionsLog[j];
    }
    actionsLog.clear();
    menu.clear();

    open=other.open;
    numOfCustomers=other.numOfCustomers;
    for (Table* table: other.tables) {
        tables.push_back(table);
    }
    for(BaseAction* action:other.actionsLog){
        actionsLog.push_back(action);
    }
    for (int k = 0; k < (int)other.menu.size(); ++k)
        menu.push_back(menu[k]);
    other.tables.clear();
    other.menu.clear();
    other.actionsLog.clear();
    return *this;
}

Restaurant::Restaurant(const std::string &configFilePath):open(false),tables(),menu(),actionsLog(),numOfCustomers(0){
    std::string file;
    std::fstream my_stream;
    my_stream.open(configFilePath);
    int i = 1;
    int id_of_dishes = 0;


   if (my_stream.is_open()) {
     while (!my_stream.eof()) {
         while (std::getline(my_stream, file)) {
                if(file.find('\r')!=std::string::npos)
                    file.erase(file.find('\r'));
                if (file==""||(file.at(0)=='#'))
                    continue;
                switch (i) {
                    case (1): {//number of tables
                        i++;
                        break;
                    }
                    case (2): { // list of capacities seperated by ','
                        std::istringstream tables_capacity(file);
                        std::string table_capacity;
                        while (std::getline(tables_capacity, table_capacity, ',')) {
                            tables.push_back(new Table(std::stoi(table_capacity, nullptr, 10)));
                        }
                        i++;
                        break;
                    }

                    case (3): {//list of dishes - will be the menu
                        std::istringstream dish_information(file);
                        std::string data, name_of_dish;
                        int price = 0, j = 1;
                        DishType type;


                        while (std::getline(dish_information, data, ',')) {
                            switch (j) {
                                case (1): {
                                    name_of_dish = data;
                                    j = 2;
                                    break;

                                }
                                case (2): {
                                    if (data == "VEG")
                                        type = DishType::VEG;
                                    else if (data == "SPC")
                                        type = DishType::SPC;
                                    else if (data == "BVG")
                                        type = DishType::BVG;
                                    else if (data == "ALC")
                                        type = DishType::ALC;

                                    j++;
                                    break;

                                }

                                case (3): {
                                    price = std::stoi(data, nullptr, 10);
                                    j=0;
                                    break;
                                }
                                default: {
                                }
                            }
                        }
                        menu.push_back(Dish(id_of_dishes, name_of_dish, price, type));
                        id_of_dishes++;

                    }
                    default: {
                    }
                }
            }

            my_stream.close();
        }
    }
}

void Restaurant::start() {
    open=true;
    std::cout << "Restaurant is now open!" << std::endl;
    std::string input;
    int j = 1, id;

    while (open) {
      //  std::vector<Customer> Delte
         std::getline(std::cin, input) ;
            if (input.find("open") != std::string::npos) {

                std::string parts;
                std::istringstream input_stream(input);
                std::vector<Customer *> customersList;

                //Get input from user and separted it one by one
                while (std::getline(input_stream, parts, ' ')) {
                    // j==1 - reading the opcode --> continue
                    if (j == 1) {
                        j = 2;
                        continue;
                    }
                        //j==2 - reading the table id
                    else if (j == 2) {
                        id = std::stoi(parts, nullptr, 10);
                        j++;
                        continue;
                    }
                        //j==3 - list of all customers- every call is one customer
                    else {
                        // separte <name>,<id>
                        int index = parts.find_first_of(",");
                        std::string name = parts.substr(0, index);
                        std::string type_string = parts.substr(index + 1);
                        if (type_string == "veg") {
                            customersList.push_back(new VegetarianCustomer(name, numOfCustomers));
                            numOfCustomers++;

                        } else if (type_string == "spc") {
                            customersList.push_back(new SpicyCustomer(name, numOfCustomers));
                            numOfCustomers++;
                        } else if (type_string == "chp") {
                            customersList.push_back(new CheapCustomer(name, numOfCustomers));
                            numOfCustomers++;
                        } else if (type_string == "alc") {
                            customersList.push_back(new AlchoholicCustomer(name, numOfCustomers));
                            numOfCustomers++;
                        }


                    }

                }   j = 1;

                    OpenTable *openTable = new OpenTable(id, customersList);
                    openTable->act(*this);
                    actionsLog.push_back(openTable);
            }

            else if (input.find("order") != std::string::npos) {
                std::string parts;
                std::istringstream id(input);

                std::getline(id, parts,' ');
                std::getline(id, parts,' ');
                int orderId=std::stoi(parts, nullptr, 10);
                Order *order = new Order(orderId);
                order->act(*this);
                actionsLog.push_back(order);


            }
            else if (input.find("move") != std::string::npos) {
                std::string parts;
                std::istringstream input_stream(input);
                int originTable, destTable, customerId;
                int i = 0;
                while (std::getline(input_stream, parts, ' ')) {
                    if (i==0){
                        i=1;
                        continue;
                    }
                    if (i == 1) {
                        originTable = std::stoi(parts, nullptr, 10);
                        i++;
                    } else if (i == 2) {
                        destTable = std::stoi(parts, nullptr, 10);
                        i++;
                    } else if (i == 3)
                        customerId = std::stoi(parts, nullptr, 10);
                }
                MoveCustomer *moveCustomer = new MoveCustomer(originTable, destTable, customerId);
                moveCustomer->act(*this);
                actionsLog.push_back(moveCustomer);
            }
            else if (input.find("close") != std::string::npos&&input.find("all")==std::string::npos) {
                std::string id;
                std::istringstream input_stream(input);
                int j=1;
                while(j!=3){
                    std::getline(input_stream, id,' ');
                    if (j==1){
                        j++;
                        continue;
                    }
                    if(j==2){
                        j=3;
                        break;
                    }
                }
                Close *close = new Close(std::stoi(id, nullptr, 10));
                close->act(*this);
                actionsLog.push_back(close);
            }


            else if (input.find("closeall") != std::string::npos) {
                open = false;
                CloseAll *closeAll = new CloseAll();
                closeAll->act(*this);
                actionsLog.push_back(closeAll);




                }


            else if (input.find("status") != std::string::npos) {
                std::string id;
                std::istringstream input_stream(input);
                std::getline(input_stream, id, ' ');
                std::getline(input_stream, id,' ');

                PrintTableStatus *printTableStatus=new PrintTableStatus(std::stoi(id, nullptr, 10));
                printTableStatus->act(*this);
                actionsLog.push_back(printTableStatus);
            }

            else if (input.find("menu") != std::string::npos) {
                PrintMenu *printMenu = new PrintMenu();
                printMenu->act(*this);
                actionsLog.push_back(printMenu);

            }

            else if (input.find("log") != std::string::npos) {
                PrintActionsLog *printActionsLog=new PrintActionsLog();
                printActionsLog->act(*this);
                actionsLog.push_back(printActionsLog);
            }

            else if (input.find("backup") != std::string::npos) {
                BackupRestaurant *backupRestaurant=new BackupRestaurant();
                backupRestaurant->act(*this);
                actionsLog.push_back(backupRestaurant);

            }

            else if (input.find("restore") != std::string::npos) {
            RestoreResturant *restoreResturant=new RestoreResturant();
            restoreResturant->act(*this);
            actionsLog.push_back(restoreResturant);
            }
    }

}


    int Restaurant::getNumOfTables() const {
    return static_cast<int>(tables.size());
    }



Table* Restaurant::getTable(int ind) {

    if (ind>=(int)tables.size())
        return nullptr;
    return tables[ind];
}


const std::vector<BaseAction *> & Restaurant::getActionsLog() const {return actionsLog;}// Return a reference to the history of actions


std::vector<Dish> & Restaurant::getMenu() {
    return menu;
}

int Restaurant::getnumOfCustomers() {
    return this->numOfCustomers;
}

