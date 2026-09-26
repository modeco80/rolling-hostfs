#ifndef UTILS_LOG_HPP
#define UTILS_LOG_HPP

enum LogLevel {
	LogInfo,
	LogWarn,
	LogErr
};

#define LOG_ENABLED

#ifdef LOG_ENABLED

void utilLog(LogLevel level, const char* pszMessage);
void utilLogf(LogLevel level, const char* pszMessage, ...);

#else

#define utilLog(level, m) (void)0
#define utilLogf(level, m, ...) (void)0

#endif

#endif
