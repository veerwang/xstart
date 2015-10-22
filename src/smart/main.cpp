/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  本程序实现IPC数据的读取功能 
 *
 *        Version:  1.0
 *        Created:  10/22/2015 03:16:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  kevin (WangWei), kevin.wang2004@hotmail.com
 *        Company:  LiLin-Tech
 *   Organization:  GNU
 *
 * =====================================================================================
 */

#include "standard.h"
#include "globalkey.h"

#include "netclient.h"
#include "frame.h"

#define ONEMB  1024*1024*1			// 定义缓冲区大小为1MB

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  主函数 
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	std::cout<<"IPC get data program start"<<std::endl;
	unsigned char *data = new unsigned char[ONEMB];
	unsigned char *indata = new unsigned char[ONEMB];
	Frame fm;				// 报文发送模块
	int len = 0;				// 获得的报文长度
	char key = 0;				// 按键事件等待退出
	bool captureflag = false;		// 是否开始捕获数据标志

	FILE * pFile;
	pFile = fopen("Raw.h264", "wb+");

	WangV::InitKey();
	Netclient *cmdclient = NULL,*dataclient = NULL;	  			// 两路socket变量
	cmdclient = new Netclient;
	if ( cmdclient->Open_Socket() == false )
	{
		delete cmdclient;
		cmdclient = NULL;
		goto Exit;
	}

	if ( cmdclient ->Connect_Server() == true )
		std::cout<<"server ip: "<<cmdclient->Get_Remote_IP()<<std::endl;
	else
	{
		std::cout<<" Connect_Server failure"<<std::endl;
		cmdclient->Close_Socket();
		delete cmdclient;
		cmdclient = NULL;
		goto Exit;
	}

	/* 	注册报文发送   */
	fm.Register_Frame(data);						// 注册报文
	cmdclient->Send_Data(data,120);
LOOP0:
	if ( cmdclient->Poll_Socket_Status() == Netclient::DATAIN )
	{
		len = cmdclient->Get_Data(indata,ONEMB);			// 获得数据
		if ( len == 0 )
		{
			std::cout<<"1 register frame check fail"<<std::endl;
			cmdclient->Close_Socket();
			delete cmdclient;
			cmdclient = NULL;
			goto Exit;
		}
		printf ( "Register frame Return len = %d\n",len );
		fm.m_SessionID = indata[4];
	}
	else
		goto LOOP0;


	/* 	系统信息报文发送   */
	fm.System_Info_Frame(data);						// 获得系统信息报文
	cmdclient->Send_Data(data,74);
LOOP1:
	if ( cmdclient->Poll_Socket_Status() == Netclient::DATAIN )
	{
		len = cmdclient->Get_Data(indata,ONEMB);			// 获得数据
		if ( len == 0 )
		{
			std::cout<<"2 systeminfo frame check fail"<<std::endl;
			cmdclient->Close_Socket();
			delete cmdclient;
			cmdclient = NULL;
			goto Exit;
		}
		printf ( "system info frame Return len = %d\n",len );
		fm.m_SessionID = indata[4];
	}
	else
		goto LOOP1;


	/* 	系统时间设置报文发送   */
	fm.Set_Systemtime_Frame(data);						// 设置系统时间
	cmdclient->Send_Data(data,122);
LOOP2:
	if ( cmdclient->Poll_Socket_Status() == Netclient::DATAIN )
	{
		len = cmdclient->Get_Data(indata,ONEMB);				// 获得数据，阻塞方式
		if ( len == 0 )
		{
			std::cout<<"3 system datetime check fail"<<std::endl;
			cmdclient->Close_Socket();
			delete cmdclient;
			cmdclient = NULL;
			goto Exit;
		}
		printf ( "system time set len = %d\n",len );
	}
	else
		goto LOOP2;

	sleep(1);			// 暂时

	dataclient = new Netclient;
	if ( dataclient->Open_Socket() == false )
	{
		std::cout<<"can't open datasocket"<<std::endl;
		delete dataclient;
		dataclient = NULL;

		cmdclient->Close_Socket();
		delete cmdclient;
		cmdclient = NULL;
		goto Exit;
	}

	if ( dataclient->Connect_Server() == true )
		std::cout<<"datasocket server ip: "<<dataclient->Get_Remote_IP()<<std::endl;
	else
	{
		std::cout<<"can't connect server"<<std::endl;
		dataclient->Close_Socket();
		delete dataclient;
		dataclient = NULL;

		cmdclient->Close_Socket();
		delete cmdclient;
		cmdclient = NULL;
		goto Exit;
	}

	/*  数据socket开始进行数据的请求  */
	fm.Claim_Frame(data);
	dataclient->Send_Data(data,205);
LOOP3:
	if ( dataclient->Poll_Socket_Status() == Netclient::DATAIN )
	{
		len = dataclient->Get_Data(indata,ONEMB);				// 获得数据，阻塞方式
		if ( len == 0 )
		{
			std::cout<<"4 datasocket claim data check fail"<<std::endl;

			dataclient->Close_Socket();
			delete dataclient;
			dataclient = NULL;

			cmdclient->Close_Socket();
			delete cmdclient;
			cmdclient = NULL;
			goto Exit;
		}
		printf ( "dataclient socket len = %d\n",len );
	}
	else
		goto LOOP3;

	/*   主循环 */
	while( key != KEY_ESC )
	{
		key = WangV::GetPCKey();
		switch ( key ) 
		{
			case KEY_F1: 
				{
					std::cout<<"Start Capture IPC Data"<<std::endl;

					fm.Start_Frame(data);
					cmdclient->Send_Data(data,205);

MAINLOOP1:
					if ( cmdclient->Poll_Socket_Status() == Netclient::DATAIN )
					{
						len = cmdclient->Get_Data(indata,ONEMB);				// 获得数据
						if ( len == 0 )
						{
							std::cout<<"start frame fail"<<std::endl;
							dataclient->Close_Socket();
							delete dataclient;
							dataclient = NULL;

							cmdclient->Close_Socket();
							delete cmdclient;
							cmdclient = NULL;
							goto Exit;
						}
						printf ( "start frame len = %d\n",len );
						captureflag = true;
					}
					else
						goto MAINLOOP1;
				}
				break;

			case KEY_F2: 
				{
					std::cout<<"Stop Capture IPC Data"<<std::endl;

					fm.Stop_Frame(data);
					cmdclient->Send_Data(data,204);

MAINLOOP2:
					if ( cmdclient->Poll_Socket_Status() == Netclient::DATAIN )
					{
						len = cmdclient->Get_Data(indata,ONEMB);				// 获得数据，阻塞方式
						if ( len == 0 )
						{
							std::cout<<"stop frame fail"<<std::endl;
							dataclient->Close_Socket();
							delete dataclient;
							dataclient = NULL;

							cmdclient->Close_Socket();
							delete cmdclient;
							cmdclient = NULL;
							goto Exit;
						}
						else
						{
							if ( indata[1] == 0x01 && indata[8] == 0x01 && indata[14] == 0x83 && indata[15] == 0x05 )
							{
								printf ( "get stop frame\n" );				// 停止报文
								captureflag = false;

								usleep(2000);						// 等待传输彻底结束,当停止报文下发后，还有一些数据下行

								if ( dataclient->Poll_Socket_Status() == Netclient::DATAIN )
								{
									len = dataclient->Get_Data(indata,ONEMB);       // 获得数据，阻塞方式
									fwrite(indata,1,len,pFile);         		// 写入数据
									printf ( "capture data length = %d\n",len );
								}
							}
						}
					}
					else
						goto MAINLOOP2;
				}
				break;

			default:	
				break;
		}				/* -----  end switch  ----- */

		usleep(200);

		if ( captureflag == true )
		{
			if ( dataclient->Poll_Socket_Status() == Netclient::DATAIN )
			{
				len = dataclient->Get_Data(indata,ONEMB);			// 获得数据，阻塞方式
				fwrite(indata,1,len,pFile);         // 写入数据
				printf ( "capture data length = %d\n",len );
			}
		}
	}

	dataclient->Close_Socket();
	delete dataclient;
	
	cmdclient->Close_Socket();
	delete cmdclient;

Exit:
	std::cout<<"Bye ~"<<std::endl;
	fclose(pFile);

	/* 以下代码将保存的h264文件进行抽取 */


	FILE * pReadFile,*pWriteFile;							// 文件再处理
	pReadFile = fopen("Raw.h264", "r");
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
					fwrite(data,1,rlen,pWriteFile);
				}
				else
					printf ( "bad 2\n" );
			}
			else
				printf ( "bad 1\n" );
		}
		else
		{
			if ( feof(pReadFile) )
				break;
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
