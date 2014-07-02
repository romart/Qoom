#include "log.h"

Logger logger;

Logger::Logger( void )
{
	File::Remove( LOGFILENAME );
	file.Open( LOGFILENAME, File::F_CREATE | File::F_WRITE_ONLY );
	if ( !file.IsOpen() ) {
		exit(0);
	}
	SYSTEMTIME wnd_time;
	GetLocalTime( &wnd_time );

	buffer = new char[ Logger::BufferSize ];
	int len = sprintf( buffer, TIMEFORMAT, (int)wnd_time.wDay, (int)wnd_time.wMonth, (int)wnd_time.wYear, (int)wnd_time.wHour, (int)wnd_time.wMinute, (int)wnd_time.wSecond );
	file.Write( ( byte* )buffer, len );

	buffer[0] = '\0';
}

Logger::~Logger( void )
{
	file.Close();
	delete[] buffer;
}

void Logger::Info( const char * fmt, ... )
{
	strcat( buffer, INFOPREF );
	strcat( buffer, fmt );
	strcat( buffer, ENDLINE );

	va_list argptr;

	char text[ 2 * Logger::BufferSize ];

	va_start( argptr, fmt );
	int len = vsnprintf( text, sizeof( text ) - 1, buffer, argptr );
	va_end( argptr );

	file.Write( ( byte* )text, len );
	buffer[0] = '\0';
}

void Logger::Warning( const char * fmt, ... )
{
	//size_t length = strlen( fmt );
	strcat( buffer, WARNINGPREF );
	strcat( buffer, fmt );
	strcat( buffer, ENDLINE );

	va_list argptr;

	char text[ 2 * Logger::BufferSize ];

	va_start( argptr, fmt );
	int len = vsnprintf( text, sizeof( text ) - 1, buffer, argptr );
	va_end( argptr );

	file.Write( ( byte* )text, len );
	buffer[0] = '\0';
}

void Logger::Error( const char * fmt, ... )
{
	//size_t length = strlen( fmt );
	strcat( buffer, ERRORPREF );
	strcat( buffer, fmt );
	strcat( buffer, ENDLINE );

	va_list argptr;

	char text[ 2 * Logger::BufferSize ];

	va_start( argptr, fmt );
	int len = vsnprintf( text, sizeof( text ) - 1, buffer, argptr );
	va_end( argptr );

	file.Write( ( byte* )text, len );
	buffer[0] = '\0';
}

void Logger::FatalError( const char * fmt, ... )
{
	//size_t length = strlen( fmt );
	strcat( buffer, FATALERRORPREF );
	strcat( buffer, fmt );
	strcat( buffer, ENDLINE );

	va_list argptr;

	char text[ 2 * Logger::BufferSize ];

	va_start( argptr, fmt );
	int len = vsnprintf( text, sizeof( text ) - 1, buffer, argptr );
	va_end( argptr );

	file.Write( ( byte* )text, len );
	buffer[0] = '\0';
}
