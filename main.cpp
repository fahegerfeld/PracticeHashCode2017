#include <iostream>
#include "Pizza.h"

int main(int argc, char ** argv) {

    if(argc >= 2)
    {
        Pizza pizza(argv[1]);
        //pizza.print_cells();
        pizza.run_algorithm_falko();
        int score = pizza.get_score();
        std::cout << "Score: " << score << std::endl;
        pizza.write_submission_file("myoutputfile.txt");
        return 0;
    }

    return 1;
}