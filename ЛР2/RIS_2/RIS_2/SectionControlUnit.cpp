#include <iostream>
#include "SectionControlUnit.h"

bool BlockCA(CA& ca) {
    ca.status = CA::STATUS::BLOCKED;
    std::cout << "---------- BLOCKED ----------\n";
    return true;
}

bool UnblockCA(CA& ca) {
    ca.status = CA::STATUS::UNBLOCKED;
    std::cout << "----------  UNBLOCKED  ----------\n";
    return true;
}
