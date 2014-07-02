#ifndef __FILE_H__
#define __FILE_H__

#include "..\sys\sys_main_header.h"

enum FILE_STATE{
	FS_ERROR = -1,
	FS_CREATE,
	FS_OPEN,
	FS_CLOSE
};

class File
{
	static const size_t ReadBufferSize = ( 1 << 10 );
	static const size_t WriteBufferSize = ( 1 << 10 );

public:
	enum FLAGS {
		F_APPEND = O_APPEND,
		F_CREATE = O_CREAT,
		F_READ_ONLY = O_RDONLY,
		F_TRUNC = O_TRUNC,
		F_READ_WRITE = O_RDWR,
		F_WRITE_ONLY = O_WRONLY,
		F_BINARY = O_BINARY,
		F_TEXT = O_TEXT

	};

	enum ORIGIN {
		F_SEEK_SET = SEEK_SET,
		F_SEEK_CUR = SEEK_CUR,
		F_SEEK_END = SEEK_END 
	};
public:
	File( void );
	File( const char * name, int flag );
	~File( void );

	void Open( const char * name, int flag );
	void Close( void );

	int Read( byte * data, size_t size );
	int Write( const byte * data, size_t size );

	int GetChar( void );
	int PutChar( byte c );

	long Lseek( long offset, int origin );

	void Flush( void );

	bool IsOpen( void ) const { return ( FS_OPEN == state ); }

	static int Remove( const char * fileName );

private:
	const char * name;
	FILE_STATE state;
	int fileDesc;
	int flag;
};

#endif