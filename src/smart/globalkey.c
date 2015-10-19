/*
 * =====================================================================================
 *
 *       Filename:  global.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年08月14日 13时05分52秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "cstandard.h"                                   /* 仅仅包含C的标准头文件 */

#include "keymap.h"

namespace WangV
{
static const int     FD_STDIN   =  0;

struct termios new_settings,store_settings;
int    m_keyflag;                   			 /* save the flag get from current environment */

void InitKey()
{
	tcgetattr(FD_STDIN,&store_settings);             /* save the current key environment variables in memory  */
	new_settings = store_settings;                   /* set the new environment variables in memory */
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_lflag &= (~ECHO);
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(FD_STDIN,TCSANOW,&new_settings);       /* enable the new value immediately */
	m_keyflag = fcntl(FD_STDIN,F_GETFL);             /* get the F_GETFL variable from environment  */
	fcntl(FD_STDIN,F_SETFL,m_keyflag | O_NONBLOCK ); /* set the read key type to be NONBLOCK */
}

void RestoreKey()
{
	fcntl(FD_STDIN,F_SETFL,m_keyflag);
	tcsetattr(FD_STDIN,TCSANOW,&store_settings);
}

char GetPCKey()
{
	int key;                                	 /* because the getchar function return int type  */
	key = getchar();
	if ( key == 27 )
	{
		key = getchar();
		if ( key == (char)(-1) ) return KEY_ESC;
		else if ( key == 91 )           	 /* function key value */
		{
			key = getchar();
			if ( key == 49 ) 
			{
				key = getchar();
				if ( key == 55 )
				{
					key = getchar();
					if ( key == 126 )
					{
						return KEY_F6; /* F6 key value */
					}
				}
			}
			else if ( key == 91 )
			{
				key = getchar();
				if ( key == 65 ) return KEY_F1;
				if ( key == 66 ) return KEY_F2;
				if ( key == 67 ) return KEY_F3;
				if ( key == 68 ) return KEY_F4;
			}
		}
	}
	else
	{
		return (char)key;                     /* Normal key return immediately */
	}
	return KEY_ESC;
}
}
