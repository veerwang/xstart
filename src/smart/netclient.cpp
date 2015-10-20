/*
 * =====================================================================================
 *
 *       Filename:  Netclient.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年10月19日 15时17分13秒
 *       Revision:  none
 *       Compiler:  gcc-arm
 *
 *         Author:  kevin.wang (), kevin.wang2004@gmail.com
 *        Company:  eddysun (XIAMEN)
 *
 * =====================================================================================
 */

#include	"netclient.h"

Netclient::Netclient ()
{
	m_Port = 34567;
	m_ServerIP = new char[100];
	m_ClientIP = new char[100];

	sprintf ( m_ServerIP,"%s","192.168.10.10" );
	Getip(m_ClientIP);
//	sprintf(m_ClientIP,"192.168.10.5");
	printf ( "%s\n",m_ClientIP );

	m_tv.tv_sec  = 0;
	m_tv.tv_usec = 1;
}

Netclient::~Netclient ()
{
	delete[] m_ServerIP;
	m_ServerIP = NULL;
	delete[] m_ClientIP;
	m_ClientIP = NULL;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Open_Client
 *  Description:  
 * =====================================================================================
 */
bool Netclient::Open_Socket()
{
	m_Socketfd = socket(AF_INET,SOCK_STREAM,0);
	if ( m_Socketfd == -1 ) { perror("Can't Init a socket!"); return false; }

	bzero(&m_Locale,sizeof(m_Locale));
	m_Locale.sin_family = AF_INET;           /* IPV4 version */
	m_Locale.sin_port = htons(m_Port);
	if ( inet_aton(m_ClientIP,(struct in_addr *)&m_Locale.sin_addr.s_addr) == 0 )
	{
		perror("Can't Init a local socket!"); 
		close(m_Socketfd);	
		return false;
	}

	bzero(&m_Remote,sizeof(m_Remote));
	m_Remote.sin_family = AF_INET;
	m_Remote.sin_port = htons(m_Port);       /* Remote port */
	if ( inet_aton(m_ServerIP,(struct in_addr *)&m_Remote.sin_addr.s_addr) == 0 )
	{
		perror("Can't Init a remote socket!"); 
		close(m_Socketfd);	
		return false;
	}

	int32_t on = 1;                            /* Enable the Port can't be reused */
	if ( setsockopt(m_Socketfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) == -1 )
	{
		perror("Set Sockopt error");
		close(m_Socketfd);
		return false;
	}

//	if ( setsockopt(m_Socketfd,SOL_SOCKET,SO_RCVTIMEO,(int8_t *)&m_tv,sizeof(struct timeval)) == -1 )   // 对于IPC来说不可用
//	{
//		perror("Set Sockopt error 1");
//		close(m_Socketfd);
//		return false;
//	}

	int rcvbuf = 1024*128;
	if ( setsockopt(m_Socketfd,SOL_SOCKET,SO_RCVBUF,(const char *)&rcvbuf,sizeof(int) ) == -1 ) 
	{
		perror("Set Sockopt error 2");
		close(m_Socketfd);
		return false;
	}

	int sndbuf = 1024*128;
	if ( setsockopt(m_Socketfd,SOL_SOCKET,SO_SNDBUF,(const char *)&sndbuf,sizeof(int) ) == -1 ) 
	{
		perror("Set Sockopt error 3");
		close(m_Socketfd);
		return false;
	}

//	int flags;
//	flags = fcntl(m_Socketfd,F_GETFL,0);
//	fcntl(m_Socketfd,F_SETFL,flags|O_NONBLOCK);		// 不能使用非阻塞模式，对于IPC

//	if ( setsockopt(m_Socketfd,SOL_SOCKET,SO_SNDTIMEO,(int8_t *)&m_tv,sizeof(struct timeval)) == -1 )
//	{
//		perror("Set Sockopt error 2");
//		close(m_Socketfd);
//		return false;
//	}

//	if ( bind(m_Socketfd,(struct sockaddr *)&m_Locale,sizeof(struct sockaddr) ) == -1 )
//	{
//		perror("Can't bind");
//		close(m_Socketfd);
//		return false;
//	}

	return true;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Close_Socket
 *  Description:  
 * =====================================================================================
 */
void Netclient::Close_Socket()
{
	close(m_Socketfd);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Connect_Server
 *  Description:   
 * =====================================================================================
 */
bool Netclient::Connect_Server()
{
	bool flag = false;
	if ( connect(m_Socketfd,(struct sockaddr*)&m_Remote,sizeof(m_Remote)) != 0 )
	{
		flag = false;
	}
	else
	{
		flag = true;
	}
	return flag;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Get_Remote_IP
 *  Description:  
 * =====================================================================================
 */
const char* Netclient::Get_Remote_IP()
{
	return inet_ntoa(m_Remote.sin_addr);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Poll_Socket_Status
 *  Description:  
 * =====================================================================================
 */
Netclient::NETSTA Netclient::Poll_Socket_Status()
{
	FD_ZERO(&m_RDfd);
	FD_SET(m_Socketfd,&m_RDfd);

	if ( select(m_Socketfd+1,&m_RDfd,NULL,NULL,&m_tv) <= 0 )
		return Netclient::NETNONE;
	else
	{
		if ( !FD_ISSET(m_Socketfd,&m_RDfd) )
		{
			return Netclient::NETNONE;
		}
		else
		{
			return Netclient::DATAIN;
		}
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Get_Data
 *  Description:  Get the Data from socket 
 * =====================================================================================
 */
int32_t Netclient::Get_Data(unsigned char* data,int32_t len)
{
	return recv(m_Socketfd,data,len,0);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Send_Data
 *  Description:  Send Data into Net port 
 * =====================================================================================
 */
bool Netclient::Send_Data(unsigned char* data,int32_t len)
{
	FD_ZERO(&m_WRfd);
	FD_SET(m_Socketfd,&m_WRfd);

	if ( select(m_Socketfd+1,NULL,&m_WRfd,NULL,&m_tv) <= 0 )
		return false; 
	else
	{
		if ( !FD_ISSET(m_Socketfd,&m_WRfd) )
		{
			printf ( "test\n" );
			return false; 
		}
		else
		{
			if ( send(m_Socketfd,data,len,0) != len )
			{
				printf ( "send data length != wanted send data length\n" );
				return false;
			}
			else
				return true;
		}
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Getip
 *  Description:  
 * =====================================================================================
 */
void Netclient::Getip(char *ip)
{
	int fd;
	struct ifreq ifr;
	struct sockaddr_in *sin;
	fd = socket(AF_INET,SOCK_DGRAM,0);
	memset(&ifr,0,sizeof(ifr));
	strcpy(ifr.ifr_name,ETH_NAME);
	ioctl(fd,SIOCGIFADDR,&ifr);
	sin = (struct sockaddr_in*)&ifr.ifr_addr;
	strcpy((char *)ip,(char *)inet_ntoa(sin->sin_addr));
	close(fd);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Getmac
 *  Description:  
 * =====================================================================================
 */
void Netclient::Getmac(char *mac)
{
	int fd;
	unsigned char addr[6];
	struct ifreq ifr;
	struct sockaddr_in *sin;
	
	fd = socket(AF_INET,SOCK_DGRAM,0);
	memset(&ifr,0,sizeof(ifr));
	strcpy(ifr.ifr_name,ETH_NAME);
	ioctl(fd,SIOCGIFHWADDR,&ifr);
	sin = (struct sockaddr_in*)&ifr.ifr_addr;
	memset(addr,0,6);
	memcpy(addr,ifr.ifr_hwaddr.sa_data,6);
	sprintf((char *)mac,"%02x:%02x:%02x:%02x:%02x:%02x",
			addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);
	close(fd);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Test_Ether_Connect
 *  Description:  Test the Ether Connect is OK
 * =====================================================================================
 */
bool Netclient::Test_Ether_Connect()
{
	int fd = -1;
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strcpy(ifr.ifr_name, ETH_NAME);  

	fd = socket(AF_INET, SOCK_DGRAM, 0);  
	if 	( fd < 0 )
	{
		perror("Cannot get control socket");  
		return false;  
	}

	if 	( ioctl(fd, SIOCGIFFLAGS, &ifr) < 0 )
	{
		perror("Cannot get socket flags");  
		return false;
	}

	if	( ifr.ifr_flags & IFF_RUNNING )
		return true;
	else 
	{
		perror("socket not running status");  
		return false;
	}
}
