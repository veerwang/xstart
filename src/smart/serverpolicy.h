#if !defined(INCLUDED_SERVERPOLICY_HPP)
#define INCLUDED_SERVERPOLICY_HPP  1

/*
 * =====================================================================================
 *
 *       Filename:  serverpolicy.hpp
 *
 *    Description:  服务器的policy类
 *
 *        Version:  1.0
 *        Created:  06/30/2015 04:41:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "standard.h"

//#include "protocolpolicy.hpp"
//#include "framereflectpolicy.hpp"

template<typename T>
class ServerPolicy
{
	public:
		const char* m_clocalip = "0.0.0.0";		       			       // 本机的IP地址 // 注意 127.0.0.1是不行的
		void Init(int port)
		{
			//	if ( (m_listenfd = socket(AF_INET,SOCK_DGRAM,0)) == -1 )       // UDP   // 或出现not support 字样
			if ( (m_listenfd = socket(AF_INET,SOCK_STREAM,0)) == -1 )      // TCP
			{
				perror("listen created fail");
				return;
			}

			bzero(&m_serversocket,sizeof(m_serversocket));
			m_serversocket.sin_family 	 = AF_INET;
			m_serversocket.sin_port   	 = htons(port);

			if ( inet_aton(m_clocalip,(struct in_addr*)&(m_serversocket.sin_addr.s_addr)) == 0 )
			{
				perror("inet_aton");
				return;
			}

			fcntl(m_listenfd, F_SETFL, O_NONBLOCK);

			epfd = epoll_create(256);
			if ( epfd == -1 )
			{
				perror("creat epoll fail");
				return;
			}

			ev.data.fd = m_listenfd;
			ev.events = EPOLLIN|EPOLLET;
			epoll_ctl(epfd,EPOLL_CTL_ADD,m_listenfd,&ev);

			int on = 1;
			if ( setsockopt(m_listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) == -1 )
			{
				perror("setsockopt fail");
				close(m_listenfd);
				return;
			}

			if ( bind(m_listenfd,(struct sockaddr*)&m_serversocket,sizeof(struct sockaddr)) == -1 )
			{
				perror("bind fail");
				close(m_listenfd);
				return;
			}

			if ( listen(m_listenfd,20) == -1 )
			{
				perror(" listen fail");
				close(m_listenfd);
				return;
			}
			m_clientlength = sizeof(struct sockaddr_in);				// 注意这里很重要，如果不初始化，无法进行telnet连接

			welmsg = "Welcom to connect Server\n";
		}

		void Release()
		{
			if ( m_connectlist.size() != 0 )
				printf ( "BAD connect list size\n" );
			close(m_listenfd);
		}

		void Run()
		{
			int nfds = epoll_wait(epfd,events,MAX_EVENTS_COUNT,0);
			int fdtemp;
//			CDTProtocolPolicy<> dp;

			for ( int i = 0; i<nfds; i++ )
			{
				if      ( events[i].data.fd == m_listenfd )
				{
					fdtemp = accept(m_listenfd,(struct sockaddr*)&m_clientsocket,&m_clientlength);
					if ( fdtemp < 0 )
					{
						perror("connect fail");
						exit(0);
					}

					char *str = inet_ntoa(m_clientsocket.sin_addr); 
					printf ( "connect frome %s\n",str );

					write(fdtemp,welmsg.c_str(),welmsg.length());

					std::pair<int,struct sockaddr_in> pair;
					pair.first  = fdtemp;
					pair.second = m_clientsocket;
					m_connectlist.insert(pair);

					ev.data.fd = fdtemp;
					ev.events = EPOLLIN|EPOLLET;
					epoll_ctl(epfd,EPOLL_CTL_ADD,fdtemp,&ev);
				}
				else if ( events[i].events&EPOLLIN )
				{
					if ( (fdtemp = events[i].data.fd) < 0 )
						continue;
					int n = read(fdtemp,recmsg,1024);
					if ( n > 0 )
					{
						recmsg[n] = 0;
//						dp.Store_Raw_Data_Into_DB((unsigned char*)recmsg,n);

//						if ( dp.Get_Length() != 0 )
//						{
//							FrameReflect<> fr;
//							fr.Process_Frame(dp.Analyze(),fdtemp);
//						}
					}
					else if ( n == 0 )
					{
						std::map<int,struct sockaddr_in>::iterator p = m_connectlist.find(fdtemp);

						char *str = inet_ntoa(p->second.sin_addr); 
						printf ( "client %s close socket\n", str);
						m_connectlist.erase(p);
						close(fdtemp);
						events[i].data.fd = -1;
					}
				}
				else
				{
					printf ( "else\n" );
				}
			}
		}

	protected:
		int m_listenfd;
		struct sockaddr_in m_serversocket;
		struct sockaddr_in m_clientsocket;
		socklen_t m_clientlength;

		static const int MAX_EVENTS_COUNT = 20;
		int epfd;
		struct epoll_event ev,events[MAX_EVENTS_COUNT];

		char recmsg[1024];
		std::string welmsg;	

		std::map<int,struct sockaddr_in>	m_connectlist;		//  保存网络句柄 网络地址的配对
};

#endif /* !defined(INCLUDED_SERVERPOLICY_HPP) */
