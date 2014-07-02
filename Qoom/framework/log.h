#ifndef __LOG_H__
#define __LOG_H__

#include "..\sys\sys_main_header.h"
#include "..\file\file.h"

struct bad_log {
	int code;
};

#define LOGFILENAME "gameLog.log"
#define TIMEFORMAT "%d.%d.%d-%02d:%02d:%02d\n"
#define FATALERRORPREF "Fatal Error:\t"
#define ERRORPREF "Error:\t"
#define WARNINGPREF "Warning:\t"
#define INFOPREF "Info:\t"

#define ENDLINE	"\n"

class Logger
{
	static const size_t BufferSize = ( 1 << 10 );


public:
	Logger( void );
	~Logger( void );

	void Close( void ) { file.Close(); }

	void FatalError( const char * fmt, ... );
	void Error( const char * fmt, ... );

	void Warning( const char * fmt, ... );
	void Info( const char * fmt, ... );

private:
	File file;
	char * buffer;
};

#endif