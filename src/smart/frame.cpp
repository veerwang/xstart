/*
 * =====================================================================================
 *
 *       Filename:  frame.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/20/2015 02:55:46 PM
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
#include "frame.h"

Frame::Frame ()
{
}

Frame::~Frame ()
{
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Load_Frame
 *  Description:  载入报文 
 * =====================================================================================
 */
bool Frame::Load_Frame(unsigned char* buf)
{
	buf[0] = 0xff; 
	buf[1] = 0x00; 
	buf[2] = 0x00; 
	buf[3] = 0x00;
	buf[4] = 0x00; 
	buf[5] = 0x00; 
	buf[6] = 0x00; 
	buf[7] = 0x00; 
	buf[8] = 0x00;  
	buf[9] = 0x00; 
	buf[10] = 0x00; 
	buf[11] = 0x00; 
	buf[12] = 0x00;  
	buf[13] = 0x00; 
	buf[14] = 0xe8; 
	buf[15] = 0x03;
                      
	sprintf((char*)(buf+16),"%s","d...{ \"EncryptType\" : \"MD5\", \"LoginType\" : \"DVRIP-Web\", \"PassWord\" : \"tlJwpbo6\", \"UserName\" : \"admin\" }.");

	buf[17]=0x00;
	buf[18]=0x00;
	buf[19]=0x00;

	buf[119] = 0x0a;
}
