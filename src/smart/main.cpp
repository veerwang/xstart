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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	std::cout<<"hello the world"<<std::endl;

	WangV::InitKey();

	ThreadHost<ServerPolicy>* 	serverthread = new ThreadHost<ServerPolicy>();

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
