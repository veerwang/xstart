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
 *         Name:  Register_Frame
 *  Description:  载入报文 
 * =====================================================================================
 */
bool Frame::Register_Frame(unsigned char* buf)
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

	return true;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  System_Info_Frame
 *  Description:   
 * =====================================================================================
 */
bool Frame::System_Info_Frame(unsigned char* buf)
{
	buf[0] = 0xff;
	buf[1] = 0x00;
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = m_SessionID;
	buf[5] = 0x00;
	buf[6] = 0x00;
	buf[7] = 0x00;
	buf[8] = 0x00;
	buf[9] = 0x00;
	buf[10] = 0x00;
	buf[11] = 0x00;
	buf[12] = 0x00;
	buf[13] = 0x00;
	buf[14] = 0xfc;
	buf[15] = 0x03;

	buf[16] = 0x36;
	buf[17] = 0x00;
	buf[18] = 0x00;
	buf[19] = 0x00;

	sprintf((char*)(buf+20),"{ \"Name\" : \"SystemInfo\", \"SessionID\" : \"0x%08X\" }.",m_SessionID);

	buf[20+strlen((const char*)(buf+20))-1] = 0x0a;

	return true;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Claim_Frame
 *  Description:  
 * =====================================================================================
 */
bool Frame::Claim_Frame(unsigned char* buf)
{
	buf[0] = 0xff;
	buf[1] = 0x00;
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = m_SessionID;
	buf[5] = 0x00;
	buf[6] = 0x00;
	buf[7] = 0x00;
	buf[8] = 0x00;
	buf[9] = 0x00;
	buf[10] = 0x00;
	buf[11] = 0x00;
	buf[12] = 0x00;
	buf[13] = 0x00;
	buf[14] = 0x85;
	buf[15] = 0x05;

	buf[16] = 0xb9;
	buf[17] = 0x00;
	buf[18] = 0x00;
	buf[19] = 0x00;

	sprintf((char*)(buf+20),"{ \"Name\" : \"OPMonitor\", \"OPMonitor\" : { \"Action\" : \"Claim\", \"Parameter\" : { \"Channel\" : 0, \"CombinMode\" : \"NONE\", \"StreamType\" : \"Main\", \"TransMode\" : \"TCP\" } }, \"SessionID\" : \"0x%02x\" }.",m_SessionID);

	buf[20+strlen((const char*)(buf+20))-1] = 0x0a;
	
	return true;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Set_Systemtime_Frame
 *  Description:  
 * =====================================================================================
 */
bool Frame::Set_Systemtime_Frame(unsigned char* buf)
{
	buf[0] = 0xff;
	buf[1] = 0x00;
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = m_SessionID;
	buf[5] = 0x00;
	buf[6] = 0x00;
	buf[7] = 0x00;
	buf[8] = 0x01;
	buf[9] = 0x00;
	buf[10] = 0x00;
	buf[11] = 0x00;
	buf[12] = 0x00;
	buf[13] = 0x00;
	buf[14] = 0x36;
	buf[15] = 0x06;

	buf[16] = 0x65;						// 有疑问
	buf[17] = 0x00;
	buf[18] = 0x00;
	buf[19] = 0x00;

	sprintf((char*)(buf+20),"{ \"Name\" : \"OPTimeSettingNoRTC\", \"OPTimeSettingNoRTC\" : \"2015-10-21 16:16:53\", \"SessionID\" : \"0x%02x\" }.",m_SessionID);

	buf[20+strlen((const char*)(buf+20))-1] = 0x0a;

	return true;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Start_Frame
 *  Description:  
 * =====================================================================================
 */
bool Frame::Start_Frame(unsigned char* buf)
{
	buf[0] = 0xff;
	buf[1] = 0x00;
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = m_SessionID;
	buf[5] = 0x00;
	buf[6] = 0x00;
	buf[7] = 0x00;
	buf[8] = 0x00;
	buf[9] = 0x00;
	buf[10] = 0x00;
	buf[11] = 0x00;
	buf[12] = 0x00;
	buf[13] = 0x00;
	buf[14] = 0x82;
	buf[15] = 0x05;

	buf[16] = 0xb9;
	buf[17] = 0x00;
	buf[18] = 0x00;
	buf[19] = 0x00;

	sprintf((char*)(buf+20),"{ \"Name\" : \"OPMonitor\", \"OPMonitor\" : { \"Action\" : \"Start\", \"Parameter\" : { \"Channel\" : 0, \"CombinMode\" : \"NONE\", \"StreamType\" : \"Main\", \"TransMode\" : \"TCP\" } }, \"SessionID\" : \"0x%02x\" }.",m_SessionID);

	buf[20+strlen((const char*)(buf+20))-1] = 0x0a;

	return true;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Heart_Frame
 *  Description:  心跳包 
 * =====================================================================================
 */
bool Frame::Heart_Frame(unsigned char* buf)
{
	buf[0] = 0xff;
	buf[1] = 0x01;
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = m_SessionID;
	buf[5] = 0x00;
	buf[6] = 0x00;
	buf[7] = 0x00;
	buf[8] = 0x00;
	buf[9] = 0x00;
	buf[10] = 0x00;
	buf[11] = 0x00;
	buf[12] = 0x00;
	buf[13] = 0x00;
	buf[14] = 0xef;
	buf[15] = 0x03;

	buf[16] = 0x43;
	buf[17] = 0x00;
	buf[18] = 0x00;
	buf[19] = 0x00;

	sprintf((char*)(buf+20),"{ \"Name\" : \"KeepAlive\", \"Ret\" : 100, \"SessionID\" : \"0x%08X\" }.",m_SessionID);

	buf[20+strlen((const char*)(buf+20))-1] = 0x0a;

	return true;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Stop_Frame
 *  Description:  
 * =====================================================================================
 */
bool Frame::Stop_Frame(unsigned char* buf)
{
	buf[0] = 0xff;
	buf[1] = 0x00;
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = m_SessionID;
	buf[5] = 0x00;
	buf[6] = 0x00;
	buf[7] = 0x00;
	buf[8] = 0x00;
	buf[9] = 0x00;
	buf[10] = 0x00;
	buf[11] = 0x00;
	buf[12] = 0x00;
	buf[13] = 0x00;
	buf[14] = 0x82;
	buf[15] = 0x05;

	buf[16] = 0xb8;
	buf[17] = 0x00;
	buf[18] = 0x00;
	buf[19] = 0x00;

	sprintf((char*)(buf+20),"{ \"Name\" : \"OPMonitor\", \"OPMonitor\" : { \"Action\" : \"Stop\", \"Parameter\" : { \"Channel\" : 0, \"CombinMode\" : \"NONE\", \"StreamType\" : \"Main\", \"TransMode\" : \"TCP\" } }, \"SessionID\" : \"0x%02x\" }.",m_SessionID);

	buf[20+strlen((const char*)(buf+20))-1] = 0x0a;

	return true;
}
