#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "WS2_32.lib")
#include <string>
#include <iostream>
#include "Winsock2.h"
#include "Error.h"
#include <fstream>
#include <synchapi.h>
#include "Error.h"
#include "Time.h"
#include "SectionControlUnit.h"

#define COUNT 5                             // quantity of packages for sending
#define LOCALHOST                           // use localhost
#define MAX_MESSAGE 50
#define PORT 2000                           // port
//#define LOCAL_ADDRESS "127.0.0.1"           // local address PC
#define LOCAL_ADDRESS "192.168.43.50"      // local address PC
#define RESOURCE "//DESKTOP-B9763I0/Lab2_RIS/output.txt"

#define MESSAGE_ENTER   "ENTER"
#define MESSAGE_LEAVE   "LEAVE"
#define MESSAGE_SUCCESS "SUCCESS"

using namespace std;

int main(int argc, char* argv[]) {
    WSADATA wsaData;
    SOCKET cS;

    SOCKADDR_IN clnt;
    memset(&clnt, 0, sizeof(clnt));   // mem zero

    clnt.sin_family = AF_INET;
    clnt.sin_port = htons(PORT);

    clnt.sin_addr.s_addr = inet_addr(LOCAL_ADDRESS);
    CA ca = InitCA((char*)LOCAL_ADDRESS, (char*)RESOURCE);

    int lc = sizeof(clnt);
    char ibuf[50];                              // buffer for input
    int lb = 0;                                // input buffer length
    char obuf[50];                              // buffer for output
    int lobuf = 0;                             // output buffer length
    char buffer[1024];                            // array for func concat (itoa)
    auto fout = new std::fstream;
    try {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup: ", WSAGetLastError());

        if ((cS = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket: ", WSAGetLastError());

        int change = 0;

        //////////////////////////////////////////////////////////


        do {

            system("pause");
            system("cls");
            std::cout << "1. Output of 10 lines on the screen\n"
                "2. Writing 10 lines to a file\n"
                "3. Exit\n";

            std::cout << "Change: ";
            std::cin >> change;

            switch (change) {
            case 1: {
                lobuf = sendto(cS, (char*)MESSAGE_ENTER, MAX_MESSAGE, 0, (sockaddr*)&clnt, lc);
                if (lobuf == SOCKET_ERROR)
                    throw SetErrorMsgText("send:", WSAGetLastError());

                RequestAccess(ca);

                lb = recvfrom(cS, ibuf, MAX_MESSAGE, 0, (sockaddr*)&clnt, &lc);
                if (lb == SOCKET_ERROR)
                    throw SetErrorMsgText("recv:", WSAGetLastError());

                EnterCA(ca);

                if (strcmp(ibuf, MESSAGE_SUCCESS) != 0)
                    return -1;

                fout->open(RESOURCE, std::ios::in);
                if (!fout->is_open()) {
                    std::cerr << "Error open file" << std::endl;
                    break;
                }
                std::string s;
                for (int i = 0; i < 10; ++i) {
                    getline(*fout, s);
                    std::cout << s << std::endl;
                    Sleep(1000);
                }

                fout->close();

                lobuf = sendto(cS, (char*)MESSAGE_LEAVE, MAX_MESSAGE, 0, (sockaddr*)&clnt, lc);
                if (lobuf == SOCKET_ERROR)
                    throw SetErrorMsgText("send:", WSAGetLastError());
                LeaveCA(ca);
                break;
            }
            case 2: {

                lobuf = sendto(cS, (char*)MESSAGE_ENTER, MAX_MESSAGE, 0, (sockaddr*)&clnt, lc);
                if (lobuf == SOCKET_ERROR)
                    throw SetErrorMsgText("send:", WSAGetLastError());

                RequestAccess(ca);

                lb = recvfrom(cS, ibuf, MAX_MESSAGE, 0, (sockaddr*)&clnt, &lc);
                if (lb == SOCKET_ERROR)
                    throw SetErrorMsgText("recv:", WSAGetLastError());

                EnterCA(ca);

                if (strcmp(ibuf, MESSAGE_SUCCESS) != 0)
                    return -1;

                fout->open(RESOURCE, std::ios::app);

                if (!fout->is_open()) {
                    std::cerr << "Error open file" << std::endl;
                    break;
                }

                for (int i = 0; i < 10; i++) {
                    const char* time = GetTime();
                    *fout << "Client A" << " " << time << std::endl;
                    std::cout << "Client A" << " " << time << std::endl;
                    Sleep(1000);
                }

                fout->close();
                std::cout << "Record resource " << RESOURCE << std::endl;

                lobuf = sendto(cS, (char*)MESSAGE_LEAVE, MAX_MESSAGE, 0, (sockaddr*)&clnt, lc);
                if (lobuf == SOCKET_ERROR)
                    throw SetErrorMsgText("send:", WSAGetLastError());
                LeaveCA(ca);
                break;
            }
            case 3:
            {
                std::cout << "Exit\n";
                break;
            }
            default: {
                std::cout << "Command not support" << std::endl;
                break;
            }
            }
            std::cout << std::endl;

        } while (change != 3);



        std::cout << "--------------------" << std::endl << std::endl;


        CloseCA(ca);

        if (closesocket(cS) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket: ", WSAGetLastError());

        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
        system("pause");
    }
    catch (string errorMsgText)
    {
        cout << endl << errorMsgText << endl;
        system("pause");
    }
    return 0;
}