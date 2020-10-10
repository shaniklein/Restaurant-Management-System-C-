# SPL191-assignment1
My solution for assigmnet 1 in Systems Programming Course BGU University

### Assignment	Goals
The objective of this assignment is to design an object-oriented system and gain
implementation experience in C++ while using classes, standard data structures and unique
C++ properties such as the “Rule of 5”. In this assignment we learnt how to handle memory in C++ and avoid
memory leaks. 

### Assignment	Definition
In this assignment I wrote a C++ program that simulates a restaurant management
system. The program will open the restaurant, assign customers to tables, make orders,
provide bills to the tables, and other requests as described in the file [Assigment Specification](https://github.com/shaniklein/SPL191-assignment1/blob/main/Assignment%20Specifications.pdf)
.

The program will get a config file as an input, which includes all required information about the
restaurant opening - number of tables, number of available seats in each table, and details
about the dishes in the menu. The format of the input file is further described in part 3.5. in the  [Assigment Specification](https://github.com/shaniklein/SPL191-assignment1/blob/main/Assignment%20Specifications.pdf) file. 

There are 4 types of customers in this restaurant, each customer type has its own way of
ordering from the menu (an ordering strategy). An order may be taken from a table more than
once, in such cases some customers may order a different dish. (Ordering strategies are
described in part 3.3)

Each table in the restaurant has a limited amount of seats available (this info is provided in the
config file). The restaurant can’t connect tables together, nor accommodates more customers
than the number of seats available in a table. In this restaurant, it’s impossible to add new
customers to an open table, but it’s possible to move customers from one table to another.
A bill of a table is the total price of all dishes ordered for that table 

### How to run
You can find examples to configuration files to the restaurant and input output examples in [Input Output samples](https://github.com/shaniklein/SPL191-assignment1/tree/main/input%20output%20samples)
