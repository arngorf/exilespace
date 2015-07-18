#include <iostream>

#include "esControl/ProgramController.hpp"

int main() {



    esControl::ProgramController* pc = esControl::ProgramController::Instance();

    pc->start();

    delete pc;

    return 0;
}