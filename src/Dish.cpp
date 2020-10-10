#include <string>
#include <Dish.h>

#include "../include/Dish.h"

   Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type):
   id(d_id),name(d_name),price(d_price),type(d_type)   {
   }

    int Dish::getId() const{
        return id;
    }
    std::string Dish::getName() const{
        return name;
    }
    int Dish::getPrice() const{
        return price;
    }
    DishType Dish::getType() const{
        return type;
    }


std::string Dish::toString() {
    if (type == DishType::ALC)
        return getName() + " ALC " + std::to_string(getPrice());
    else if (type == DishType::BVG)
        return getName() + " BVG " + std::to_string(getPrice());
    else if (type == DishType::SPC)
        return getName() + " SPC " + std::to_string(getPrice());
    else //(type == DishType::VEG)
        return getName() + " VEG " + std::to_string(getPrice());

}




