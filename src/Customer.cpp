#include <vector>
#include <string>
#include "../include/Dish.h"
#include "../include/Customer.h"
#include <iostream>
#include <Customer.h>

//Class customer
    Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id){ }


    std::string Customer::getName() const{
        return  name;
    }

    int   Customer::getId() const{
        return  id;
    }

Customer::~Customer() {

}




VegetarianCustomer::VegetarianCustomer(std::string name, int id):Customer(name,id){}

    std::vector<int> VegetarianCustomer::order(const std::vector <Dish> &menu) {
        std::vector<int> dishes = std::vector<int>();
        bool orderedFood = false;
        bool oderedDrik = false;
        int maxPrice = 0;
        int maxDish = 0;


        for (int i = 0; (unsigned)i < menu.size(); ++i) {
            if (orderedFood && oderedDrik) {
                return dishes;
            }
            switch (menu[i].getType()) {
                case (DishType::VEG): {
                    if (!orderedFood) {
                        dishes.push_back(menu[i].getId());
                    //    std::cout << getName() + " ordered " + menu[i].getName() << std::endl;
                        orderedFood = true;
                    }
                    break;
                }

                case (DishType::BVG): {
                        if (menu[i].getPrice() > maxPrice) {
                            maxPrice = menu[i].getPrice();
                            maxDish = menu[i].getId();
                        }

                    break;
                }

                default:
                    break;
            }


        }

        if (maxPrice!=0) {
            dishes.push_back(menu[maxDish].getId());
        }

            if(dishes.size()!=2)
                dishes.clear();

            for (int j = 0; (unsigned )j < dishes.size(); ++j) {
                    std::cout << getName() + " ordered " + menu[dishes[j]].getName() << std::endl;
                }

        return dishes;
    }




    std::string VegetarianCustomer::toString() const {
    return this->getName()+",veg";
}

VegetarianCustomer* VegetarianCustomer::clone(){
    return new VegetarianCustomer(this->getName(),this->getId());
}

    CheapCustomer::CheapCustomer(std::string name, int id) :Customer(name,id),ordered(false){}
    std::vector<int> CheapCustomer::order(const std::vector <Dish> &menu) {
        std::vector<int> dishes=std::vector<int>();

        if (ordered) {
            return dishes;
        }
            if(menu.size()>0) {
                int cheapestPrice = menu[0].getPrice();
                int cheapestDishId = 0;


                for (int i = 0; (unsigned) i < menu.size(); ++i) {
                    if (menu[i].getPrice() < cheapestPrice) {
                        cheapestDishId = i;
                        cheapestPrice = menu[i].getPrice();
                    }
                }
                ordered = true;
                dishes.push_back(menu[cheapestDishId].getId());
                std::cout << getName() + " ordered " + menu[cheapestDishId].getName() << std::endl;
            }
        return dishes;

    }
std::string CheapCustomer::toString() const {
        return this->getName()+",chp";
}


CheapCustomer* CheapCustomer::clone(){
    CheapCustomer *customer =new CheapCustomer(this->getName(),this->getId());
    customer->setOrdered(this->ordered);
    return customer;
}

void CheapCustomer::setOrdered(bool _ordered) {
    ordered=_ordered;
}


std::vector<int> SpicyCustomer::order(const std::vector <Dish> &menu) {



        std::vector<int> dishes=std::vector<int>();
        int SpcMostExpensivePrice=0;
        int SpcMostExpensiveDish=-1;
        int nonAlcoholicCheapestPrice=INT32_MAX;
        int nonAlcoholicCheapestDish;

    if(menu.size()>0) {
        if (!ordered) {
            for (int i = 0; (unsigned) i < menu.size(); ++i) {
                if (menu[i].getType() == DishType::SPC && menu[i].getPrice() > SpcMostExpensivePrice) {
                    SpcMostExpensivePrice = menu[i].getPrice();
                    SpcMostExpensiveDish = i;
                }
            }
            if (SpcMostExpensivePrice != 0) {
                dishes.push_back(menu[SpcMostExpensiveDish].getId());
                std::cout << getName() + " ordered " + menu[SpcMostExpensiveDish].getName() << std::endl;

                ordered = true;
            }
            return dishes;
        }

        //if already ordered
        for (int j = 0; (unsigned) j < menu.size(); ++j) {
            if (menu[j].getType() == DishType::BVG && menu[j].getPrice() < nonAlcoholicCheapestPrice) {
                nonAlcoholicCheapestPrice = menu[j].getPrice();
                nonAlcoholicCheapestDish = j;
            }
        }
        if (!(nonAlcoholicCheapestPrice == INT32_MAX)) {
            dishes.push_back(menu[nonAlcoholicCheapestDish].getId());
            std::cout << getName() + " ordered " << menu[nonAlcoholicCheapestDish].getName() << std::endl;
        }
    }
        return dishes;
    }

    std::string SpicyCustomer::toString() const {return this->getName()+",spc";}

    SpicyCustomer* SpicyCustomer::clone() {
    SpicyCustomer *spicyCustomer= new SpicyCustomer(this->getName(),this->getId());
    spicyCustomer->setOrdered(this->ordered);
        return spicyCustomer;
}



SpicyCustomer::SpicyCustomer(std::string name, int id) : Customer(name, id),ordered(false) {

}

void SpicyCustomer::setOrdered(bool _ordered) {
    ordered=_ordered;

}

AlchoholicCustomer::AlchoholicCustomer(std::string name, int id)
:Customer(name,id)
,orderTime(0),
CheapestAlcoholPrice(INT32_MAX),nextCheapest(INT32_MAX),CheapestAlcoholId(-1),nextCheapestId(-1)//,alcoholDished()
{}



AlchoholicCustomer::AlchoholicCustomer(const AlchoholicCustomer &other):
        Customer(other.getName(),other.getId()),orderTime(other.orderTime),CheapestAlcoholPrice(other.CheapestAlcoholPrice),nextCheapest(other.nextCheapest),CheapestAlcoholId(other.CheapestAlcoholId)//,//alcoholDished()
         ,nextCheapestId(other.nextCheapestId){  }



std::vector<int> AlchoholicCustomer::order(const std::vector <Dish> &menu) {
    std::vector<int> output;
    std::vector<Dish> alcoholDished;
    if(menu.size()>0) {

        //Getting all alcohols
        for (int i = 0; (unsigned) i < menu.size(); ++i) {
            if (menu[i].getType() == DishType::ALC)
                alcoholDished.push_back(menu[i]);
        }
        if (orderTime == 0) {

            //find Cheapst Dish
            for (int j = 0; (unsigned) j < alcoholDished.size(); ++j) {
                if (alcoholDished[j].getPrice() < CheapestAlcoholPrice) {
                    CheapestAlcoholPrice = alcoholDished[j].getPrice();
                    CheapestAlcoholId = alcoholDished[j].getId();
                }
            }
            output.push_back(CheapestAlcoholId);
            orderTime++;
            std::cout << getName() << " ordered " << menu[CheapestAlcoholId].getName() << std::endl;
            nextCheapestId=CheapestAlcoholId;
            return output;
        } else {
            if (orderTime > (int) alcoholDished.size() - 1)
                return output;


            int samePrice = -1;
        nextCheapest=INT32_MAX;
        int lastId=INT32_MAX;
       // int difPrice;
        //int Price=-INT32_MAX;
            int difId=INT32_MAX;
            for (int j = 0; (unsigned) j < alcoholDished.size(); ++j) {
                if (alcoholDished[j].getId() != CheapestAlcoholId &&
                    alcoholDished[j].getPrice() >= CheapestAlcoholPrice && alcoholDished[j].getPrice() < nextCheapest) {
                    if (alcoholDished[j].getPrice() == CheapestAlcoholPrice && alcoholDished[j].getId() < lastId &&
                        alcoholDished[j].getId() > CheapestAlcoholId) {
                        nextCheapest = alcoholDished[j].getPrice();
                        lastId = alcoholDished[j].getId();
                        samePrice = alcoholDished[j].getId();
                    }
                    if (alcoholDished[j].getPrice() > CheapestAlcoholPrice) {
                        //if(alcoholDished[j].getPrice() <Price){

                            nextCheapest = alcoholDished[j].getPrice();
                           // difPrice = alcoholDished[j].getId();
                            difId = alcoholDished[j].getId();
                        }


                }

                }//ENDOFLOOP


                if (samePrice ==-1&&difId!=INT32_MAX) {
                CheapestAlcoholId=difId;
                CheapestAlcoholPrice=nextCheapest;

                }
                else {

                    CheapestAlcoholId=samePrice;
                    CheapestAlcoholPrice=nextCheapest;


                }

                    output.push_back(CheapestAlcoholId);
                    orderTime++;
                    std::cout << getName() << " ordered " << menu[CheapestAlcoholId].getName() << std::endl;
                }
            }

        // alcoholDished.clear();
    return output;

}







std::string AlchoholicCustomer::toString() const {return this->getName()+",alc";}

AlchoholicCustomer* AlchoholicCustomer::clone() {
    AlchoholicCustomer *alchoholicCustomer=new  AlchoholicCustomer(this->getName(),this->getId());
    alchoholicCustomer->setAlcohol(this->orderTime,this->CheapestAlcoholPrice,this->nextCheapest,this->CheapestAlcoholId);
    return alchoholicCustomer;
}

void
AlchoholicCustomer::setAlcohol(int orderedTime, int CheapestAlcoholPrice, int nextCheapest, int CheapestAlcoholId) {
    this->orderTime=orderedTime;
    this->CheapestAlcoholId=CheapestAlcoholId;
    this->nextCheapest=nextCheapest;
    this->CheapestAlcoholPrice=CheapestAlcoholPrice;
    this->nextCheapestId=nextCheapestId;

}






