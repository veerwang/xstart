#if !defined(INCLUDED_THREADBASE_HPP)
#define INCLUDED_THREADBASE_HPP  1

/*
 * =====================================================================================
 *
 *       Filename:  threadbase.hpp
 *
 *    Description:  线程类的模板实现形式
 *
 *        Version:  1.0
 *        Created:  06/30/2015 01:39:41 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "standard.h"

template<typename T>
class DefaultPolicy
{
	public:
		void Run()
		{
//			std::cout<<"... "<<std::endl;
		}
};

struct PImple
{
	std::thread*		thread_class1;
	std::atomic<bool>       runflag;
	std::atomic<bool>       stopflag;
	std::atomic<bool>       pauseflag;
	std::mutex  		lock;

	int                     interval_second;          // 在 RUN 中调度的间隔参数
	int			interval_millisecond;
};

template<template <typename T > class baseclass=DefaultPolicy>
class ThreadHost : public baseclass<class T>
{
	public:
		ThreadHost() : m_Imple( new struct PImple )
		{
			m_Imple->thread_class1 	      = new std::thread(std::bind(&ThreadHost::ThreadLoop,this));
			m_Imple->runflag              = false;
			m_Imple->stopflag             = false;
			m_Imple->pauseflag            = false;
			m_Imple->interval_second      = 0;
			m_Imple->interval_millisecond = 0;
		}

		~ThreadHost()
		{
			if ( m_Imple->thread_class1->joinable() )
			{
				m_Imple->thread_class1->join();
				delete m_Imple->thread_class1;
			}
		}

		void Start()
		{
			m_Imple->runflag = true;
		}

		void Stop()
		{
			m_Imple->stopflag = true;
		}

		void Pause()
		{
			m_Imple->pauseflag = true;
		}

		void Resume()
		{
			m_Imple->pauseflag = false;
		}

		void Set_Interval_Second(const int& v)
		{
			m_Imple->interval_second = v;
		}

		void Set_Interval_MilliSecond(const int& v)
		{
			m_Imple->interval_millisecond = v;
		}

	protected:
		void ThreadLoop()
		{
			while ( !m_Imple->runflag )
				std::this_thread::yield();

			while ( !m_Imple->stopflag )
			{
				while ( m_Imple->pauseflag )
					std::this_thread::yield();

				this->Run();			// 注意此地的用法

				std::this_thread::sleep_for(std::chrono::seconds(m_Imple->interval_second)+std::chrono::milliseconds(m_Imple->interval_millisecond));
			}
		}

	protected:
		friend struct PImple;
		std::unique_ptr<struct PImple> m_Imple;
};

#endif /* !defined(INCLUDED_THREADBASE_HPP) */
