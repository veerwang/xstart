#if !defined(INCLUDED_GLOBALKEY_H)
#define INCLUDED_GLOBALKEY_H  1


/*
 * =====================================================================================
 *
 *       Filename:  global.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年08月14日 13时07分23秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "keymap.h"

namespace WangV
{
extern void InitKey();
extern void RestoreKey();
extern char GetPCKey();
}

#endif /* !defined(INCLUDED_GLOBALKEY_H) */
