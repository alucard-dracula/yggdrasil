//flash_security_svr.cpp
#include <boost/asio.hpp>
#include <yggr/flash_security/security_protocol_def.h>
#include <yggr/flash_security/security_protocol.hpp>
#include <iostream>

typedef yggr::flash_security::security_protocol_def sp_def_type;
typedef yggr::flash_security::security_protocol<sp_def_type> sp_type;

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
    typedef SOCKET socket_type;
    typedef SOCKADDR_IN sockaddr_in_type;
    typedef SOCKADDR sockaddr_type;
#   define PROTOCOL_V4 AF_INET
#elif defined(YGGR_AT_LINUX)
#   include <sys/wait.h>
    typedef int socket_type;
    typedef sockaddr_in sockaddr_in_type;
    typedef sockaddr sockaddr_type;
#   define PROTOCOL_V4 PF_INET
#else
#   error "please add other os socket type def"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

int main(int argc, char* argv[])
{
	yggr::u16 port = 843;
	std::cin >> port;

	sp_type sp;
	sp.load_of_file("request.cfg", "post.cfg");

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
	WORD wVerRequested = MAKEWORD(1, 1);
	WSADATA wsaData = {0};
	int err = WSAStartup(wVerRequested, &wsaData);
	if(err != 0)
	{
		return 1;
	}

	if(LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return 1;
	}
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

    socket_type sockSrv = socket(PROTOCOL_V4, SOCK_STREAM, 0);
    sockaddr_in_type addrSrv;
    memset(&addrSrv, 0, sizeof(sockaddr_in_type));
    addrSrv.sin_family = PROTOCOL_V4;
    //addrSrv.sin_port = htons(sp_def_type::link_port());
	addrSrv.sin_port = htons(port);

    if(sockSrv < 0)
    {
        //std::cerr << "socket create err" << std::endl;
        return -1;
    }

    if(bind(sockSrv, (sockaddr_type*)&addrSrv, sizeof(sockaddr_type)) < 0)
	{
	    std::cerr << "socket bind err" << std::endl;
        perror("socket bind err");
	    return -1;
	}

	if(listen(sockSrv, 20) < 0)
	{
	    std::cerr << "socket listen err" << std::endl;
	    return -1;
	}

	std::string post(sp.get_post_str().size(), 0);
	memcpy(&post[0], &(sp.get_post_str()[0]), sp.get_post_str().size());

    sockaddr_in_type addrClient;

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
	int len = sizeof(sockaddr_type);
#elif defined(YGGR_AT_LINUX)
	unsigned int len = sizeof(sockaddr_type);
#else
#	error "please add other os len def!!!"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

	unsigned long long count = 0;
	for(;1;)
	{
		socket_type sockConn = (socket_type)0;
		sockConn = accept(sockSrv, (sockaddr_type*)&addrClient, &len);

		if(sockConn < 0)
		{
		    continue;
		}

		std::string recv_buf(sp_def_type::request_length(), 0);
		if(recv(sockConn, &recv_buf[0], sp_def_type::request_length(), 0) < 0)
		{
		    continue;
		}

		if(recv_buf == sp.get_request_str())
		{
			if(send(sockConn, &post[0], post.size(), 0) < 1)
			{
				std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!error!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl; 
			    continue;
			}
			std::cout << post << " posted" << std::endl;
		}

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		closesocket(sockConn);
#elif defined(YGGR_AT_LINUX)
        close(sockConn);
        waitpid(-1, 0, WNOHANG);
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		 ++count;
		 std::cout << "count = " << count << std::endl;
	}
	return 0;
}
