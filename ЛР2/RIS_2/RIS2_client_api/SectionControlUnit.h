#define MESSAGE_ENTER "ENTER"
#define MESSAGE_LEAVE "LEAVE"
#define MESSAGE_SUCCESS "SUCCESS"
#define MESSAGE_NO_SUPPORT "NO SUPPORT"

struct CA {
    char ipaddr[15];                // id address
    char resource[20];              // name of resource
    enum STATUS {
        NOINIT,
        INIT,
        ENTER,
        LEAVE,
        WAIT
    } status;
};

CA InitCA(char ipaddr[15], char resource[20]);
bool RequestAccess(CA& ca);
bool EnterCA(CA& ca);
bool LeaveCA(CA& ca);
bool CloseCA(CA& ca);