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

	std::string serverIP = "127.0.0.1";  // 서버 IP 주소
	int serverPort = 4321;  // 서버 포트 번호

	sockaddr_in serverAddress{};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(serverPort);

	char buffer[BUFFER_SIZE];

	int errorCode = 0;
	int iResult;

	if (inet_pton(AF_INET, serverIP.data(), &(serverAddress.sin_addr)) <= 0)
		return -1;

	// 서버에 연결
	if (connect(clntSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR)
		return -1;

	// ========== shutdown(SD_RECEIVE) 예제 ============
	//iResult = recv(sockfd, buffer, 4, 0); // 서버는 5바이트를 송신했지만, 클라이언트는 4바이트만 수신
	//std::cout << iResult << " bytes" << std::endl;

	//printf("Press any key to call shutdown");
	//getchar();

	//// 아직 1바이트가 수신 버퍼에 있는 상태에서 shutdown 호출
	//shutdown(sockfd, SD_RECEIVE);
	//	iResult = recv(sockfd, buffer, 4, 0); // 서버는 5바이트를 송신했지만, 클라이언트는 4바이트만 수신
	//std::cout << iResult << " bytes" << std::endl;
	//printf("Press any key to call closesocket");
	//getchar();

	// ========== shutdown(SD_SEND) 예제 ============
	errorCode = 0;

	const int DEFAULT_BUFLEN = 10; // 1024 * 1024 * 10;
	char* recvbuf = new char[DEFAULT_BUFLEN];

	memset(recvbuf, 0x63636363, DEFAULT_BUFLEN / 4);

	// 10MB를 서버 측으로 전송하고,
	iResult = send(clntSocket, recvbuf, DEFAULT_BUFLEN, 0); // DEFAULT_BUFLEN == 1024 * 1024 * 10
	errorCode = WSAGetLastError();
	printf("send: %d(%d)\n", iResult, errorCode);

	printf("Press any key to call shutdown");
	getchar();

	// 키가 눌리면 shutdown(SD_SEND) 호출
	iResult = shutdown(clntSocket, SD_SEND);
	errorCode = WSAGetLastError();
	printf("shutdown: %d(%d)\n", iResult, errorCode);

	printf("Press any key to call closesocket");
	getchar();

	delete[] recvbuf;

	// 키가 눌리면 closesocket 호출
	iResult = closesocket(clntSocket);
	errorCode = WSAGetLastError();
	printf("closesocket: %d(%d)\n", iResult, errorCode);

	//closesocket(clntSocket);
	WSACleanup();
	return 0;
}
