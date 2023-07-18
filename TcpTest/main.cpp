#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

const int BUFFER_SIZE = 4096;

int main() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return -1;

	SOCKET clntSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clntSocket == INVALID_SOCKET)
		return -1;

	std::string serverIP = "127.0.0.1";  // ���� IP �ּ�
	int serverPort = 4321;  // ���� ��Ʈ ��ȣ

	sockaddr_in serverAddress{};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(serverPort);

	char buffer[BUFFER_SIZE];

	int errorCode = 0;
	int iResult;

	if (inet_pton(AF_INET, serverIP.data(), &(serverAddress.sin_addr)) <= 0)
		return -1;

	// ������ ����
	if (connect(clntSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR)
		return -1;

	// ========== shutdown(SD_RECEIVE) ���� ============
	//iResult = recv(sockfd, buffer, 4, 0); // ������ 5����Ʈ�� �۽�������, Ŭ���̾�Ʈ�� 4����Ʈ�� ����
	//std::cout << iResult << " bytes" << std::endl;

	//printf("Press any key to call shutdown");
	//getchar();

	//// ���� 1����Ʈ�� ���� ���ۿ� �ִ� ���¿��� shutdown ȣ��
	//shutdown(sockfd, SD_RECEIVE);
	//	iResult = recv(sockfd, buffer, 4, 0); // ������ 5����Ʈ�� �۽�������, Ŭ���̾�Ʈ�� 4����Ʈ�� ����
	//std::cout << iResult << " bytes" << std::endl;
	//printf("Press any key to call closesocket");
	//getchar();

	// ========== shutdown(SD_SEND) ���� ============
	errorCode = 0;

	const int DEFAULT_BUFLEN = 10; // 1024 * 1024 * 10;
	char* recvbuf = new char[DEFAULT_BUFLEN];

	memset(recvbuf, 0x63636363, DEFAULT_BUFLEN / 4);

	// 10MB�� ���� ������ �����ϰ�,
	iResult = send(clntSocket, recvbuf, DEFAULT_BUFLEN, 0); // DEFAULT_BUFLEN == 1024 * 1024 * 10
	errorCode = WSAGetLastError();
	printf("send: %d(%d)\n", iResult, errorCode);

	printf("Press any key to call shutdown");
	getchar();

	// Ű�� ������ shutdown(SD_SEND) ȣ��
	iResult = shutdown(clntSocket, SD_SEND);
	errorCode = WSAGetLastError();
	printf("shutdown: %d(%d)\n", iResult, errorCode);

	printf("Press any key to call closesocket");
	getchar();

	delete[] recvbuf;

	// Ű�� ������ closesocket ȣ��
	iResult = closesocket(clntSocket);
	errorCode = WSAGetLastError();
	printf("closesocket: %d(%d)\n", iResult, errorCode);

	//closesocket(clntSocket);
	WSACleanup();
	return 0;
}
