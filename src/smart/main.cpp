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

			unsigned char *data = new unsigned char[200];
			Frame fm;
			fm.Load_Frame(data);
			client->Send_Data(data,120);

//			int times = 0;
//LOOP:
//			if ( client->Poll_Socket_Status() == Netclient::DATAIN  )
//			{
//				unsigned char *indata = new unsigned char[200];
//
//				int len = client->Get_Data(indata,200);
//				for (int i=0;i<len;i++ )
//					printf ( "%x\n",indata[i] );
//
//				delete[] indata;
//				indata = NULL;
//			}
//			else
//				times++;
//			usleep(1000);
//			if ( times < 10 ) goto LOOP;

			unsigned char *indata = new unsigned char[200];

			int len = client->Get_Data(indata,200);
			for (int i=0;i<len;i++ )
				printf ( "%x\n",indata[i] );

			delete[] indata;
			indata = NULL;

			delete[] data;
			data = NULL;
		}
		else
			std::cout<<"connect to server fail"<<std::endl;

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




//	serverthread->Release();
//	serverthread->Stop();
//	delete serverthread;
//	serverthread = NULL;

	WangV::RestoreKey();
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
