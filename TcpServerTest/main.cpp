#include <WinSock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int main() {
	SOCKET clntSocket, ListenSocket;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		return -1;

	ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ListenSocket == INVALID_SOCKET)
		return -1;

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(4321);
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(ListenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		return -1;

	if (listen(ListenSocket, 10) == SOCKET_ERROR)
		return -1;


	
	// ======== shutdown(SD_RECEIVE) ���� ==============
	//char buffer[10] = { 0, };
	//FD_SET fds;
	//FD_ZERO(&fds);

	//clntSocket = accept(ListenSocket, NULL, NULL);
	//FD_SET(clntSocket, &fds);
	//FD_SET copyFds = fds;

	//send(clntSocket, "hello", 3, 0);    // 5����Ʈ �۽�

	//select(0, &copyFds, nullptr, nullptr, nullptr);     // select �Լ��� ����
	//printf("return select\n");

	//int recvSize = recv(clntSocket, buffer, sizeof(buffer) - 1, 0);
	//printf("recvSize : %d\n", recvSize);

	//closesocket(clntSocket);
	//FD_CLR(clntSocket, &fds);

	// =========== shutdown(SD_SEND) ���� ===============
	int errorCode = 0;

	int iResult;
	const int DEFAULT_BUFLEN = 1000;
	char recvbuf[DEFAULT_BUFLEN];

	clntSocket = accept(ListenSocket, NULL, NULL);

	// ���� ���� ��, Ű�� ������ Ŭ���̾�Ʈ�κ��� ������ �����͸� ��� ����
	printf("Press any key to call recv");
	getchar();

	int totalReceive = 0;
	while (true)
	{
		iResult = recv(clntSocket, recvbuf, DEFAULT_BUFLEN, 0);
		if (iResult == -1 || iResult == 0)
		{
			errorCode = WSAGetLastError();
			printf("recv: %d(%d)\n", iResult, errorCode);
			break;
		}

		totalReceive += iResult;
		printf("recvLen: %d (total: %d)\n", iResult, totalReceive);
	}
	
	// ���� �Ϸ� �� FIN ����
	iResult = closesocket(clntSocket);
	errorCode = WSAGetLastError();
	printf("closesocket: %d(%d), totalRecv = %d\n", iResult, errorCode, totalReceive);


	//closesocket(ListenSocket);
	WSACleanup();
	return 0;
}