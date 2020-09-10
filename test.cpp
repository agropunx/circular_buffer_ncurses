#include "cirBuff.h"
#include <iostream>

int main(void)
{
	cirBuff<uint32_t> circle(8);
	printf("\n ===*** C++ CIRCULAR BUFFER TEST CHECK ***===\n");
	printf("Buffer Size: %zu, Buffer Capacity: %zu\n", circle.size(), circle.capacity());

	std::cout << "---------------------------------------------------------------------\n"
	          << "TEST1: ------------------- PUSH BACK FILL CHECK ---------------------\n"
		  << "---------------------------------------------------------------------"
		  << "\n\n";
	
	for(size_t i = 0; i < circle.size() + 10; i++)
        {
                circle.push_back(i);
                circle.print_buffer();
		std::cout << " size  = " << circle.size() << "\n";
		std::cout << " is full ? : " << circle.isfull() << " || " << " is empty ? : " << circle.isempty() << "\n\n";

        }
	
	std::cout << "---------------------------------------------------------------------\n"
                  << "TEST2: ------------------- BUFFER RESET CHECK -----------------------\n"
                  << "---------------------------------------------------------------------"
                  << "\n\n";

	circle.reset_buffer();
	std::cout << "\n\n" << " ... buffer reset called ... " << "\n\n";
	std::cout << " size = " << circle.size() << "\n";
	std::cout << " is full ? : " << circle.isfull() << " || " << " is empty ? : " << circle.isempty() << "\n\n";


	std::cout << "---------------------------------------------------------------------\n"
                  << "TEST3: ------------------- PUSH FRONT FILL CHECK --------------------\n"
                  << "---------------------------------------------------------------------"
                  << "\n\n";

	for(size_t i = 0; i < circle.size() + 10; i++)
        {
                circle.push_front(i);
                circle.print_buffer();
		std::cout << " size = " << circle.size() << "\n";
		std::cout << " is full ? : " << circle.isfull() << " || " << " is empty ? : " << circle.isempty() << "\n\n";
        }

	std::cout << "---------------------------------------------------------------------\n"
                  << "TEST4: ------------------- OPERATOR[] READ CHECK --------------------\n"
                  << "---------------------------------------------------------------------"
                  << "\n\n";

	std::cout << " Current ";
        circle.print_buffer();

	for(size_t i = 0; i < circle.size(); i++)
        {
	        std::cout << " reading index " << i << " buffer element  = " << circle[i] << "\n";
        }

        std::cout << "---------------------------------------------------------------------\n"
		  << "TEST5: ------------------ OPERATOR[] WRITE CHECK --------------------\n"
                  << "---------------------------------------------------------------------"
		  << "\n\n";

        std::cout << " Current ";
        circle.print_buffer();
	
        
        for(size_t i = 0; i < circle.size(); i++)
        {
		circle[i] = circle[i] + 10;
        }
	std::cout << "\n\n" << " ************ editing buffer by adding to each element value ************ " << "\n\n"; 
        std::cout << "  Edited ";
       	circle.print_buffer();
        std::cout << "\n\n";

	std::cout << "---------------------------------------------------------------------\n"
                  << "TEST5: --------------------- DESTRUCTOR CHECK -----------------------\n"
                  << "---------------------------------------------------------------------"
                  << "\n\n";

        std::cout << " Current ";
        circle.print_buffer();
	
        std::cout << "\n\n" << " ************ deleting buffer ************ " << "\n\n";
	circle.~cirBuff();
	
	return 0;
}
