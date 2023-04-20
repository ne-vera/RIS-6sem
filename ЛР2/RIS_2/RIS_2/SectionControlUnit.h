#define MESSAGE_ENTER "ENTER"
#define MESSAGE_LEAVE "LEAVE"
#define MESSAGE_SUCCESS "SUCCESS"
#define MESSAGE_NO_SUPPORT "NO SUPPORT"

struct CA {
    enum STATUS {
        UNBLOCKED,
        BLOCKED,
    } status;
};

bool BlockCA(CA& ca);
bool UnblockCA(CA& ca);


