#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable : 4996)
#include <string>
#include <iostream>
#include "Winsock2.h"


#include "Error.h"
#include "SectionControlUnit.h"

#pragma comment(lib, "WS2_32.lib")
#include <ctime>
#include <list>

#define PORT 2000
#define MAX_MESSAGE 50
#define IP "192.168.43.50"
//#define IP "127.0.0.1"
#define RESOURCE "//DESKTOP-B9763I0/Lab2_RIS/output.txt"

using namespace std;

int main()
{
    std::list<SOCKADDR_IN> list;
    WSADATA wsaData;
    SOCKET sock_server;
    SOCKADDR_IN serv_addr;
    SOCKADDR_IN client_addr;
    SOCKADDR_IN temp;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(IP);

    int size_sock_addr_client = sizeof(client_addr);
    char buffer[MAX_MESSAGE];
    int  length_buffer = 0;
    int count_received_packs = 0;

    CA ca{};

    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("startup: ", WSAGetLastError());

        if ((sock_server = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket: ", WSAGetLastError());

        if (bind(sock_server, (LPSOCKADDR)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
            throw SetErrorMsgText("bind: ", WSAGetLastError());
        cout << "Server UDP " << "port: " << PORT << " address: " << inet_ntoa(serv_addr.sin_addr) << endl;

        do
        {
            memset(&client_addr, 0, sizeof(client_addr));
            memset(buffer, 0, sizeof(buffer));

            length_buffer = recvfrom(sock_server, buffer, MAX_MESSAGE, 0, (sockaddr*)&client_addr, &size_sock_addr_client);
            if (length_buffer == SOCKET_ERROR) {
                if (WSAGetLastError() != 10054)
                    throw  SetErrorMsgText("recv:", WSAGetLastError());
                else {
                    std::cerr << "remote closed connection" << std::endl;
                    list.pop_front();

                    if (list.empty()) {
                        UnblockCA(ca);
                        cout << "size queue: " << list.size() << endl;
                        continue;
                    }

                    temp = *list.begin();

                    cout << "client (ip: " << inet_ntoa(temp.sin_addr) <<
                        ", port: " << temp.sin_port << "), message: " << buffer << ", size queue: " << list.size() << endl;
                    UnblockCA(ca);

                    length_buffer = sendto(sock_server, (char*)MESSAGE_SUCCESS, MAX_MESSAGE, 0, (sockaddr*)&temp, sizeof(temp));
                    if (length_buffer == SOCKET_ERROR)
                        throw SetErrorMsgText("send:", WSAGetLastError());

                    BlockCA(ca);
                    continue;
                }
            }

            else
            {
                if (strcmp(buffer, MESSAGE_ENTER) == 0) {
                    list.push_back(client_addr);

                    cout << "client (ip: " << inet_ntoa(client_addr.sin_addr) <<
                        ", port: " << client_addr.sin_port << "), message: " << buffer << ", size queue: " << list.size() << endl;

                    if (ca.status == CA::STATUS::UNBLOCKED) {
                        BlockCA(ca);
                        length_buffer = sendto(sock_server, (char*)MESSAGE_SUCCESS, MAX_MESSAGE, 0, (sockaddr*)&client_addr, size_sock_addr_client);
                        if (length_buffer == SOCKET_ERROR)
                            throw SetErrorMsgText("send:", WSAGetLastError());
                    }

                }
                else if (strcmp(buffer, MESSAGE_LEAVE) == 0 && !list.empty()) {
                    list.pop_front();

                    if (list.empty()) {
                        UnblockCA(ca);
                        cout << "size queue: " << list.size() << endl;
                        continue;
                    }

                    temp = *list.begin();

                    cout << "client (ip: " << inet_ntoa(temp.sin_addr) <<
                        ", port: " << temp.sin_port << "), message: " << buffer << ", size queue: " << list.size() << endl;
                    UnblockCA(ca);
                    length_buffer = sendto(sock_server, (char*)MESSAGE_SUCCESS, MAX_MESSAGE, 0, (sockaddr*)&temp, sizeof(temp));
                    if (length_buffer == SOCKET_ERROR)
                        throw SetErrorMsgText("send:", WSAGetLastError());
                    BlockCA(ca);

                }
                else {
                    length_buffer = sendto(sock_server, (char*)MESSAGE_NO_SUPPORT, MAX_MESSAGE, 0, (sockaddr*)&client_addr, size_sock_addr_client);
                    if (length_buffer == SOCKET_ERROR)
                        throw SetErrorMsgText("send:", WSAGetLastError());
                }
            }



            if (strcmp(buffer, "") == 0)
                break;

        } while (true);

        cout << "RECEIVED: " << count_received_packs << endl;
        if (closesocket(sock_server) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket: ", WSAGetLastError());

        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
    }

    catch (string errorMsgText)
    {
        cout << endl << errorMsgText << endl;
    }

    return 0;
}