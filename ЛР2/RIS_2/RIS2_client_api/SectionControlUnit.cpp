#include <cstring>
#include <iostream>
#include "SectionControlUnit.h"
#pragma warning(disable : 4996)
CA InitCA(char* ipaddr, char* resource) {
    CA ca{};
    ca.status = CA::STATUS::INIT;
    ca.status = CA::STATUS::WAIT;
    strcpy(ca.ipaddr, ipaddr);
    strcpy(ca.resource, resource);
    std::cout << "Initializing a critical resource" << std::endl;
    return ca;
}
bool RequestAccess(CA& ca) {
    ca.status = CA::STATUS::WAIT;
    std::cout << "Request to log in to a critical resource" << std::endl;
    return true;
}

bool EnterCA(CA& ca) {
    ca.status = CA::STATUS::ENTER;
    std::cout << "Entering a critical resource" << std::endl;
    return true;
}

bool LeaveCA(CA& ca) {
    ca.status = CA::STATUS::LEAVE;

    std::cout << "Exiting a critical resource" << std::endl;
    return true;
}

bool CloseCA(CA& ca) {
    ca.status = CA::STATUS::NOINIT;
    std::cout << "Exiting a critical resource CA" << std::endl;
    return true;
}
