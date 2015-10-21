/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  实现
 *
 *        Version:  1.0
 *        Created:  10/10/2015 03:04:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  kevin (WangWei), kevin.wang2004@hotmail.com
 *        Company:  LiLin-Tech
 *   Organization:  GNU
 *
 * =====================================================================================
 */

#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "standard.h"

#include "threadbase.h"
#include "serverpolicy.h"

#include "globalkey.h"

#include "netclient.h"
#include "frame.h"

#define ONEMB  1024*1024*5
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	std::cout<<"IPC client program running"<<std::endl;

	int times = 1;

	WangV::InitKey();

	ThreadHost<ServerPolicy>* 	serverthread = new ThreadHost<ServerPolicy>();
	Netclient *client = new Netclient;
	if ( client->Open_Socket() == false )
	{
		std::cout<<"can't open socket"<<std::endl;
	}
	else
	{
		if ( client->Connect_Server() == true )
		{
			std::cout<<"server ip: "<<client->Get_Remote_IP()<<std::endl;

			unsigned char *data = new unsigned char[1024];
			Frame fm;
			fm.Register_Frame(data);						// 注册报文
			client->Send_Data(data,120);

			unsigned char *indata = new unsigned char[ONEMB];
			int len = client->Get_Data(indata,ONEMB);				// 获得数据，阻塞方式
			if ( len == 0 )
			{
				std::cout<<"1 No read data"<<std::endl;
				goto Exit;
			}
			fm.m_SessionID = indata[4];

			fm.System_Info_Frame(data);						// 获得系统信息报文
			client->Send_Data(data,74);

			len = client->Get_Data(indata,ONEMB);					// 必须要读取数据，才能断开网络链接
			if ( len == 0 )
			{
				std::cout<<"2 No read data"<<std::endl;
				goto Exit;
			}

			fm.Claim_Frame(data);							// 要求数据报文
			client->Send_Data(data,205);

			len = client->Get_Data(indata,ONEMB);
			if ( len == 0 )
			{
				std::cout<<"3 No read data"<<std::endl;
				goto Exit;
			}
			printf ( "claim\n" );

			fm.Set_Systemtime_Frame(data);						// 设置系统时间
			client->Send_Data(data,122);

			len = client->Get_Data(indata,ONEMB);
			if ( len == 0 )
			{
				std::cout<<"4 No read data"<<std::endl;
				goto Exit;
			}
			printf ( "system time\n" );

			fm.Start_Frame(data);
			client->Send_Data(data,205);

//			len = client->Get_Data(indata,ONEMB);
//			if ( len == 0 )
//			{
//				std::cout<<"5 No read data"<<std::endl;
//				goto Exit;
//			}
//			printf ( "start frame %d\n",len );

//			while ( times )
//			{
//				fm.Heart_Frame(data);
//				client->Send_Data(data,87);
//
//				sleep(1);
//				times --;
//				printf ( "alive\n" );
//			}

			sleep(10);								// 等待数据的传输,只下是一口气进行传输
LOOP1:
			if ( client->Poll_Socket_Status() == Netclient::DATAIN )
			{
				len = client->Get_Data(indata,ONEMB);
				printf ( "read data length = %d\n",len );
				if ( len == 0 )
				{
					std::cout<<"6 No read data"<<std::endl;
					goto Exit;
				}
				else
				{
					FILE * pFile;
					pFile = fopen("TestRealPlay.h264", "wb+");
					fwrite(indata,1,len,pFile);
					fclose(pFile);
				}
				goto LOOP1;
			}

			fm.Stop_Frame(data);
			client->Send_Data(data,204);

			len = client->Get_Data(indata,ONEMB);
			if ( len == 0 )
			{
				std::cout<<"6 No read data"<<std::endl;
				goto Exit;
			}
			printf ( "stop frame %d\n",len );

Exit:
			delete[] indata;
			indata = NULL;

			delete[] data;
			data = NULL;
		}
		else
			std::cout<<"connect to server fail"<<std::endl;

		printf ( "close socket\n" );
		client->Close_Socket();
	}

	serverthread->Init(34567);
	serverthread->Set_Interval_Second(0);
	serverthread->Set_Interval_MilliSecond(300);
	serverthread->Start();

	char key = 0;	
	while( key != KEY_ESC )
	{
		key = WangV::GetPCKey();
		usleep(100);
	}


	serverthread->Release();
	serverthread->Stop();
	delete serverthread;
	serverthread = NULL;

	WangV::RestoreKey();
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
