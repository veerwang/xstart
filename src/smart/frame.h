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

		bool Load_Frame(unsigned char* buf);
	protected:

	private:

}; /* -----  end of class Frame  ----- */

#endif /* !defined(INCLUDED_FRAME_H) */
