#include "file.h"


File::File( void )
{
	this->name = NULL;
	this->state = FS_CREATE;
	this->fileDesc = 0;
	this->flag = 0;
}


File::File( const char * _name, int _flag )
{
	Open( _name, _flag ); 
}

File::~File( void )
{
	Close();
	name = NULL;
	flag = NULL;
}

void File::Open( const char * _name, int _flag )
{
	name = _name;
	flag = _flag;
	if ( flag & File::F_CREATE ) {
		fileDesc = open( name, flag, S_IWRITE );
	} else {
		fileDesc = open( name, flag );
	}
	if ( fileDesc == -1 ) {
		state = FS_ERROR;
	} else {
		state = FS_OPEN;
	}
}

void File::Close( void )
{
	if ( state == FS_OPEN ) {
		close( fileDesc );
		fileDesc = 0;
		state = FS_CLOSE;
	}
}

void File::Flush( void )
{

}

int File::Read( byte * buffer, size_t size )
{
	int ret = read( fileDesc, buffer, size );
	if ( ret < 0 ) {
		state = FS_ERROR;
	}
	return ret;
}

int File::Write( const byte * buffer, size_t size )
{
	int ret = write( fileDesc, buffer, size );
	if ( ret < 0 ) {
		state = FS_ERROR;
	}

	return ret;
}

int File::GetChar( void )
{
	byte c;
	Read( &c, sizeof( c ) );
	return c;
}

int File::PutChar( byte c )
{
	return Write( &c, sizeof( c ) );
}

long File::Lseek( long offset, int origin )
{
	return lseek( fileDesc, offset, origin );
}

int File::Remove( const char * fileName )
{
	return remove( fileName );
}