#ifndef LOG_H
#define LOG_H
#include <cstdarg>
#include <iostream>
#include <time.h>
#include <direct.h>
#pragma warning(disable:4996)

//#define LOGFILE(
class logfile
{
public:
	logfile();
	~logfile();
	std::string GetTag(bool flag=false);
	void Open();
	void PrintLog(const char *p, ...);
	void Close();
private:
	FILE* m_fp;
};

logfile::logfile()
{
	m_fp = NULL;
	Open();
}

logfile::~logfile()
{
	fclose(m_fp);
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

void logfile::PrintLog(const char *p, ...)
{
	if (m_fp != NULL)
	{
		va_list arg;
		va_start(arg, p);
		fprintf(m_fp, GetTag(true).c_str());
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
	if (flag)
	{
		sprintf_s(timeStrBuf, "%d-%d-%d [%d:%d:%d] ", 1900 + time_data.tm_year,
			time_data.tm_mon + 1,
			time_data.tm_mday,
			time_data.tm_hour,
			time_data.tm_min,
			time_data.tm_sec);
	}
	else
	{
		sprintf_s(timeStrBuf, "%d-%d-%d-%d", 1900 + time_data.tm_year,
			time_data.tm_mon + 1,
			time_data.tm_mday,
			time_data.tm_hour);
	}
	
	std::string ret;
	ret.insert(ret.end(), timeStrBuf, timeStrBuf + strlen(timeStrBuf));
	return ret;
}


#endif
