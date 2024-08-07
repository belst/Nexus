///----------------------------------------------------------------------------------------------------
/// Copyright (c) Raidcore.GG - All rights reserved.
///
/// Name         :  CFileLogger.h
/// Description  :  Custom logger to print to a log file.
/// Authors      :  K. Bieniek
///----------------------------------------------------------------------------------------------------

#ifndef FILELOGGER_H
#define FILELOGGER_H

#include <fstream>
#include <filesystem>

#include "ILogger.h"

///----------------------------------------------------------------------------------------------------
/// CFileLogger Class
///----------------------------------------------------------------------------------------------------
class CFileLogger : public virtual ILogger
{
	public:
		///----------------------------------------------------------------------------------------------------
		/// ctor
		///----------------------------------------------------------------------------------------------------
		CFileLogger(ELogLevel aLogLevel, std::filesystem::path aPath);

		///----------------------------------------------------------------------------------------------------
		/// dtor
		///----------------------------------------------------------------------------------------------------
		~CFileLogger();

		///----------------------------------------------------------------------------------------------------
		/// LogMessage:
		/// 	Overriden function to write to a file.
		///----------------------------------------------------------------------------------------------------
		void LogMessage(LogEntry aLogEntry);

	private:
		void rotate(const std::filesystem::path& aPath);
		std::ofstream File;
};

#endif
