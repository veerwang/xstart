#if !defined(INCLUDED_FRAME_H)
#define INCLUDED_FRAME_H  1

/*
 * =====================================================================================
 *
 *       Filename:  frame.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/20/2015 02:55:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  kevin (WangWei), kevin.wang2004@hotmail.com
 *        Company:  LiLin-Tech
 *   Organization:  GNU
 *
 * =====================================================================================
 */

class Frame
{
	public:
		Frame ();                                       /* constructor */
		~Frame ();                                      /* destructor  */

		bool Register_Frame(unsigned char* buf);
		bool System_Info_Frame(unsigned char* buf);
		bool Claim_Frame(unsigned char* buf);
		bool Set_Systemtime_Frame(unsigned char* buf);

		bool Start_Frame(unsigned char* buf);
		bool Heart_Frame(unsigned char* buf);
		bool Stop_Frame(unsigned char* buf);

		unsigned char m_SessionID;
	protected:

	private:

}; /* -----  end of class Frame  ----- */

#endif /* !defined(INCLUDED_FRAME_H) */
