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

#define ONEMB  1024*1024*1

#define  BUFNUM 800
#define  NBUF BUFNUM+1

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

	int times = 0;
	unsigned char *data = new unsigned char[ONEMB];
	unsigned char *indata = new unsigned char[ONEMB];
	Frame fm;
	FILE * pFile;
	pFile = fopen("TestRealPlay.h264", "wb+");

	WangV::InitKey();

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


			fm.Register_Frame(data);						// 注册报文
			client->Send_Data(data,120);

			int len = 0;

LOOP0:
			if ( client->Poll_Socket_Status() == Netclient::DATAIN )
			{
				len = client->Get_Data(indata,ONEMB);				// 获得数据
				if ( len == 0 )
				{
					std::cout<<"1 No read data"<<std::endl;
					goto Exit;
				}
				printf ( "Register Return len %d\n",len );
				fm.m_SessionID = indata[4];
			}
			else
				goto LOOP0;

			fm.System_Info_Frame(data);						// 获得系统信息报文
			client->Send_Data(data,74);

LOOP1:
			if ( client->Poll_Socket_Status() == Netclient::DATAIN )
			{
				len = client->Get_Data(indata,ONEMB);				// 获得数据
				if ( len == 0 )
				{
					std::cout<<"2 No read data"<<std::endl;
					goto Exit;
				}
				printf ( "system info len %d\n",len );
			}
			else
				goto LOOP1;

//			fm.Claim_Frame(data);							// 要求数据报文
//			client->Send_Data(data,205);
//LOOP2:
//			if ( client->Poll_Socket_Status() == Netclient::DATAIN )
//			{
//				len = client->Get_Data(indata,ONEMB);				// 获得数据，阻塞方式
//				if ( len == 0 )
//				{
//					std::cout<<"3 No read data"<<std::endl;
//					goto Exit;
//				}
//				printf ( "claim len %d\n",len );
//			}
//			else
//				goto LOOP2;

			fm.Set_Systemtime_Frame(data);						// 设置系统时间
			client->Send_Data(data,122);

LOOP3:
			if ( client->Poll_Socket_Status() == Netclient::DATAIN )
			{
				len = client->Get_Data(indata,ONEMB);				// 获得数据，阻塞方式
				if ( len == 0 )
				{
					std::cout<<"4 No read data"<<std::endl;
					goto Exit;
				}
				printf ( "system time len %d\n",len );
			}
			else
				goto LOOP3;

//			fm.Start_Frame(data);
//			client->Send_Data(data,205);
//LOOP4:
//			if ( client->Poll_Socket_Status() == Netclient::DATAIN )
//			{
//				len = client->Get_Data(indata,ONEMB);				// 获得数据，阻塞方式
//				if ( len == 0 )
//				{
//					std::cout<<"5 No read data"<<std::endl;
//					goto Exit;
//				}
//				printf ( "start len %d\n",len );
//			}
//			else
//				goto LOOP4;


//			while ( times )					                        // 是心跳包
//			{
//				fm.Heart_Frame(data);
//				client->Send_Data(data,87);
//
//				sleep(1);
//				times --;
//				printf ( "alive\n" );
//			}

//LOOP99:
//			if ( client->Poll_Socket_Status() == Netclient::DATAIN )
//			{
//				len = client->Get_Data(indata,ONEMB);
////				printf ( "read data length = %d\n",len );
//				if ( len == 0 )
//				{
//					std::cout<<"6 No read data"<<std::endl;
//					goto Exit;
//				}
//				else
//				{
//					fwrite(indata,1,len,pFile);         // 写入数据
//					times ++;
//					if ( times != BUFNUM )
//						goto LOOP99;
//				}
//			}
//			else
//				goto LOOP99;


//			fm.Stop_Frame(data);
//			client->Send_Data(data,204);
//LOOP100:
//			if ( client->Poll_Socket_Status() == Netclient::DATAIN )
//			{
//				len = client->Get_Data(indata,ONEMB);				// 获得数据，阻塞方式
//				if ( len == 0 )
//				{
//					std::cout<<"100 No read data"<<std::endl;
//					goto Exit;
//				}
//				else
//				{
//					printf ( "len == %d\n",len );
//					if ( indata[1] == 0x01 && indata[8] == 0x01 && indata[14] == 0x83 && indata[15] == 0x05 )
//					{
//						fclose(pFile);
//						goto Exit;         	 			// 截获结尾报文 结束                             
//					}
//					else
//					{
//						fwrite(indata,1,len,pFile);
//						times ++;
//						if ( times != NBUF )
//							goto LOOP100;					// 否则继续读取
//						else
//							fclose(pFile);
//					}
//				}
//			}
//			else
//				goto LOOP100;

			sleep(1);

			{

				Netclient *dataclient = new Netclient;
				if ( dataclient->Open_Socket() == false )
					std::cout<<"can't open datasocket"<<std::endl;
				else
				{
					if ( dataclient->Connect_Server() == true )
					{
						std::cout<<"datasocket server ip: "<<dataclient->Get_Remote_IP()<<std::endl;
						{
							fm.Claim_Frame(data);
							dataclient->Send_Data(data,205);


LOOP2:
							if ( dataclient->Poll_Socket_Status() == Netclient::DATAIN )
							{
								len = dataclient->Get_Data(indata,ONEMB);				// 获得数据，阻塞方式
								if ( len == 0 )
								{
									std::cout<<"aa bad No read data"<<std::endl;
									goto Exit1;
								}
								else
								{
									printf ( "clainm data len = %d\n",len );

									fm.Start_Frame(data);
									client->Send_Data(data,205);
LOOP4:
									if ( client->Poll_Socket_Status() == Netclient::DATAIN )
									{
										len = client->Get_Data(indata,ONEMB);				// 获得数据
										if ( len == 0 )
										{
											std::cout<<"5 No read data"<<std::endl;
											goto Exit;
										}
										printf ( "start len %d\n",len );
									}
									else
										goto LOOP4;

									sleep(4);
LOOP77:
									times++;
									if ( dataclient->Poll_Socket_Status() == Netclient::DATAIN )
									{
										len = dataclient->Get_Data(indata,ONEMB);			// 获得数据，阻塞方式
										fwrite(indata,1,len,pFile);         // 写入数据
										printf ( "%d\n",len );
										goto LOOP77;
									}
									else
									{
										if ( times < 200000 )
											goto LOOP77;
									}


									fm.Stop_Frame(data);
									client->Send_Data(data,204);

LOOP100:
									if ( client->Poll_Socket_Status() == Netclient::DATAIN )
									{
										len = client->Get_Data(indata,ONEMB);				// 获得数据，阻塞方式
										if ( len == 0 )
										{
											std::cout<<"100 No read data"<<std::endl;
											goto Exit;
										}
										else
										{
											printf ( "stop len %d\n",len );
											if ( indata[1] == 0x01 && indata[8] == 0x01 && indata[14] == 0x83 && indata[15] == 0x05 )
											{
												printf ( "get stop frame\n" );
											}		
										}
									}
									else
										goto LOOP100;


									times = 0;
LOOP78:
									times++;
									if ( dataclient->Poll_Socket_Status() == Netclient::DATAIN )
									{
										len = dataclient->Get_Data(indata,ONEMB);			// 获得数据，阻塞方式
										fwrite(indata,1,len,pFile);         // 写入数据
										printf ( "%d\n",len );
										goto LOOP77;
									}
									else
									{
										if ( times < 200000 )
											goto LOOP78;
									}


								}
							}
							else
								goto LOOP2;
						}
Exit1:
						dataclient->Close_Socket();
					}
					else
						std::cout<<"datasocket connect to server fail"<<std::endl;
			}
			delete dataclient;
			dataclient = NULL;
			}



Exit:
			printf("Exit\n");
		}
		else
			std::cout<<"connect to server fail"<<std::endl;

		printf ( "close socket\n" );
		client->Close_Socket();
	}

	char key = 0;								// 按键事件等待退出
	while( key != KEY_ESC )
	{
		key = WangV::GetPCKey();
		usleep(1000);
	}


	fclose(pFile);

	FILE * pReadFile,*pWriteFile;							// 文件再处理
	pReadFile = fopen("TestRealPlay.h264", "r");
	pWriteFile = fopen("good.h264","wb+");

	int rlen = 0;
	int ret = 0;
	while(1)
	{
		if ( fread(indata,1,0x10,pReadFile) != 0 )
		{
			if ( fread(indata,1,0x04,pReadFile) != 0 )
			{
				rlen = (  indata[1] << 8 ) + indata[0];

				if ( (ret = fread(data,1,rlen,pReadFile)) != 0 )
				{
					printf ( "%d\n",ret );
					fwrite(data,1,rlen,pWriteFile);
				}
			}

		}
	}


	fclose(pWriteFile);
	fclose(pReadFile);


	delete[] indata;
	indata = NULL;

	delete[] data;
	data = NULL;

	WangV::RestoreKey();

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
