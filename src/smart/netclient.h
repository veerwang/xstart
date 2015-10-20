/*
 * =====================================================================================
 *
 *       Filename:  Netclient.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011年10月19日 15时14分10秒
 *       Revision:  none
 *       Compiler:  gcc-arm
 *
 *         Author:  kevin.wang (), kevin.wang2004@gmail.com
 *        Company:  eddysun (XIAMEN) eddysun (XIAMEN)
 *
 * =====================================================================================
 */

#ifndef  _NETCLIENT_INC
#define  _NETCLIENT_INC

/*
 * =====================================================================================
 *        Class:  Netclient
 *  Description:  
 * =====================================================================================
 */
/*-----------------------------------------------------------------------------
 *  Net program used head file
 *-----------------------------------------------------------------------------*/

#include 	"standard.h"
#define 	ETH_NAME "eth0"

class Netclient
{
	public:
		enum NETSTA { NETNONE  = 0,
			      DATAIN   = 1 };
		Netclient ();                             /* constructor */
		~Netclient ();                            /* destructor  */
		bool Open_Socket();
		void Close_Socket();
		bool Connect_Server();
		const char* Get_Remote_IP();
		NETSTA Poll_Socket_Status();
		int32_t Get_Data(unsigned char* data,int32_t len);
		bool Send_Data(unsigned char* data,int32_t len);
		void Getip(char *ip);
		void Getmac(char *mac);

		bool Test_Ether_Connect();
	protected:

	private:
		int32_t			m_Port;
		char*			m_ServerIP;
		char*			m_ClientIP;
		int32_t			m_Socketfd;
		struct sockaddr_in	m_Remote;
		struct sockaddr_in	m_Locale;
		struct timeval          m_tv;
		fd_set			m_WRfd;
		fd_set			m_RDfd;
}; /* -----  end of class Netclient  ----- */
#endif   /* ----- #ifndef _NETCLIENT_INC  ----- */
