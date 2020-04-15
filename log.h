#ifndef LOG_H
#define LOG_H
#include <cstdarg>
#include <iostream>
#include <time.h>
#include <direct.h>
#pragma warning(disable:4996)

struct comparison
{
	int year;
	int month;
	int day;
	int hour;
	comparison()
	{
		year = 0;
		month = 0;
		day = 0;
		hour = 0;
	}
};

//#define LOGFILE(
class logfile
{
public:
	logfile();
	~logfile();
	std::string GetTag(bool flag = false);
	void Open();
	void DebugPrintf(const char *p, ...);
	void Close();
private:
	FILE* m_fp;
	comparison temp_time_comp;	//对比时辰用的
};

logfile::logfile()
{
	m_fp = NULL;
	Open();
}

logfile::~logfile()
{
	if (m_fp != NULL)
	{
		fclose(m_fp);
	}
}

void logfile::Open()
{
	Close();
	std::string abc = "日志测试\\";
	mkdir(abc.c_str());
	char buf[256];
	memset(buf, 0, sizeof(buf));
	sprintf_s(buf, "%s%s.txt", abc.c_str(), GetTag().c_str());
	m_fp= fopen(buf, "a");
	if (m_fp == NULL)
		return;

}

void logfile::DebugPrintf(const char *p, ...)
{
	if (m_fp != NULL)
	{
		std::string temp_string;
		temp_string = GetTag(true);
		if (m_fp == NULL)//GetTag内有可能导致打开失败，特此
		{
			//尝试再打开一次
			Open();
			if (m_fp == NULL)
			{
				return;
			}
		}

		va_list arg;
		va_start(arg, p);
		fprintf(m_fp, temp_string.c_str());
		vfprintf(m_fp, p, arg);
		fprintf(m_fp, "\n");
		va_end(arg);
		fflush(m_fp);
	
	}
}
void logfile::Close()
{
	m_fp = NULL;
}

std::string logfile::GetTag(bool flag)
{
	time_t nowTs = time(NULL);
	tm time_data;
	localtime_s(&time_data, &nowTs);
	char timeStrBuf[256];
	memset(timeStrBuf, 0, sizeof(timeStrBuf));
	int tm_year = 1900 + time_data.tm_year;
	int tm_mon = time_data.tm_mon + 1;
	if (flag)	//日志内
	{
		//此处做个预防，防止日志都打印在同一个时辰文件中
		if (m_fp != NULL)
		{
			if (temp_time_comp.year == tm_year&&
				temp_time_comp.month == tm_mon&&
				temp_time_comp.day == time_data.tm_mday&&
				temp_time_comp.hour == time_data.tm_hour)
			{
				//不写
			}
			else
			{
				//当有一个条件对不上，则重打开一个文件
				fclose(m_fp);
				Open();

			}
		}


		sprintf_s(timeStrBuf, "%d-%d-%d [%d:%d:%d] ", tm_year,
			tm_mon,
			time_data.tm_mday,
			time_data.tm_hour,
			time_data.tm_min,
			time_data.tm_sec);
	}
	else //创建文件专用的
	{
		sprintf_s(timeStrBuf, "%d-%d-%d-%d", tm_year,
			tm_mon,
			time_data.tm_mday,
			time_data.tm_hour);

		temp_time_comp.year = tm_year;
		temp_time_comp.month = tm_mon;
		temp_time_comp.day = time_data.tm_mday;
		temp_time_comp.hour = time_data.tm_hour;
	}
	
	std::string ret;
	ret.insert(ret.end(), timeStrBuf, timeStrBuf + strlen(timeStrBuf));
	return ret;
}


#endif
