#include "stdafx.h"
#include <iostream>
#include <ctime>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Winsock2.h"
#include <cmath>
#pragma comment (lib, "ws2_32.lib")


struct NTP_packet	//пакет NTP
{
	CHAR head[4];
	DWORD32 RootDelay;
	DWORD32 RootDispersion;
	CHAR ReferenceIdentifier[4];	//идентификатор
	DWORD ReferenceTimestamp[2];	//32 бита  - секунды с 01.01.1900 00:00, 32 бита - доли секунды в 2^-32 единицах
	DWORD64 OriginateTimestamp;
	DWORD32 TransmitTimestamp[2];	//32 бита  - секунды с 01.01.1900 00:00, 32 бита - доли секунды в 2^-32 единицах
	DWORD32 KeyIdentifier;			//optional
	DWORD64 MessageDigest[2];		//optional
};


void UnixTimeToFileTime(time_t t, LPFILETIME pft)
{
	// Note that LONGLONG is a 64-bit value
	LONGLONG ll;

	ll = Int32x32To64(t, 10000000) + 116444736000000000;
	pft->dwLowDateTime = (DWORD)ll;
	pft->dwHighDateTime = ll >> 32;
}

void UnixTimeToSystemTime(time_t t, LPSYSTEMTIME pst)
{
	FILETIME ft;

	UnixTimeToFileTime(t, &ft);
	FileTimeToSystemTime(&ft, pst);
}

int main()
{
	int Tc = 10000;
	int h = CLOCKS_PER_SEC; //clock-тиков в сек
	clock_t t = clock(); //тики со старта
	long d = 613608 * 3600; //613608 * 3600 сек между 1.1.1900:00:00 и 1.1.1970:00:00
	time_t ttime;
	time(&ttime);

	WSAData wsaData;
	SOCKET s;
	SOCKADDR_IN server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("88.147.254.235");
	server.sin_port = htons(123);

	NTP_packet out_buf, in_buf;
	ZeroMemory(&out_buf, sizeof(out_buf));
	ZeroMemory(&in_buf, sizeof(in_buf));
	out_buf.head[0] = 0x1B;
	out_buf.head[1] = 0x00;
	out_buf.head[2] = 4;
	out_buf.head[3] = 0xEC;

	int averCor = 0;

	for (int i = 0; i < 10; i++) {
		try
		{
			if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
				throw WSAGetLastError();
			if ((s = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
				throw WSAGetLastError();
			int lenout = 0, lenin = 0, lensockaddr = sizeof(server);

			if ((lenout = sendto(s, (char*)&out_buf, sizeof(out_buf), NULL, (sockaddr*)&server, sizeof(server))) == SOCKET_ERROR)
				throw  WSAGetLastError();
			if ((lenin = recvfrom(s, (char*)&in_buf, sizeof(in_buf), NULL, (sockaddr*)&server, &lensockaddr)) == SOCKET_ERROR)
				throw  WSAGetLastError();

			in_buf.ReferenceTimestamp[0] = ntohl(in_buf.ReferenceTimestamp[0]) - d;
			in_buf.TransmitTimestamp[0] = ntohl(in_buf.TransmitTimestamp[0]) - d;
			in_buf.TransmitTimestamp[1] = ntohl(in_buf.TransmitTimestamp[1]);				//доли секунды in_buf.TransmitTimestamp[1]/2^-32
			int ms = (int)1000.0 * ((double)(in_buf.TransmitTimestamp[1]) / (double)0xffffffff);	//миллисекунды

			SYSTEMTIME sysTime;
			UnixTimeToSystemTime(in_buf.TransmitTimestamp[0], &sysTime);
			sysTime.wMilliseconds = ms;

			printf("ServerTime:\n");
			printf("%d-%02d-%02d %02d:%02d:%02d.%03d\n",
				sysTime.wYear,
				sysTime.wMonth,
				sysTime.wDay,
				sysTime.wHour,
				sysTime.wMinute,
				sysTime.wSecond,
				sysTime.wMilliseconds);
			printf("OSTime:\n");
			SYSTEMTIME st;

			GetLocalTime(&st);

			printf("%d-%02d-%02d %02d:%02d:%02d.%03d\n",
				st.wYear,
				st.wMonth,
				st.wDay,
				st.wHour,
				st.wMinute,
				st.wSecond,
				st.wMilliseconds);

			averCor += abs(sysTime.wMilliseconds - st.wMilliseconds);

			printf("Difference in milliseconds: %d\n\n", (sysTime.wMilliseconds - st.wMilliseconds));
			if (closesocket(s) == INVALID_SOCKET)
				throw WSAGetLastError();
			if (WSACleanup() == SOCKET_ERROR)
				throw WSAGetLastError();

			Sleep(Tc);
		}
		catch (int e)
		{
			printf("Error: \n", e);
		}
	}
	std::cout << "Average correction = " << averCor / 10 << std::endl;
	system("pause");
	return 0;
}
