#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <assert.h>

#include "reader.h"

bool Reader::OpenFile( const char *a_filename )
{
	assert( a_filename );
	inptr = fopen( a_filename, "r" );
	if (inptr == NULL )
	{
		return false;
	}
	line_number = 1;
	col_number = 1;
	strcpy( filename, a_filename );
}

Reader::Reader()
{
	strcpy( filename ,  "" );
	inptr = NULL;
}

Reader::~Reader()
{
	if( inptr != NULL )
	{
		fclose( inptr );
	}
}


bool Reader::isMonoWord( char val )
{
	switch (val )
	{
		case '!':
		case '£':
		case '$':
		case '%':
		case '^':
		case '&':
		case '*':
		case '(':
		case '{':
		case '}':
		case '[':
		case ']':
		case '=':
		case '-':
		case '+':
		case ',':
		//case '.':
		case '/':
		case '\\':
		case '#':
		case '~':
		case '@':
		case ';':
		case '?':
		case '|':
		return true;
	}
	return false;
}

bool Reader::isNewline( char val )
{
	if ( val == '\n' )
	{
		return true;
	}

	return false;
}

bool Reader::isWhitespace( char val )
{
	switch( val )
	{
		case ' ':
		case '\t':
		return true;
	}	
	return false;
}

void Reader::SkipTillNewline()
{
	while( true )	
	{
		char gotchar = (char)fgetc( inptr ) ;

		if( feof( inptr ) )
		{
			return;
		}
		
		if( isNewline( gotchar ) )
		{
			return;
		}
	}
}

char *Reader::NextWord() 
{
	assert( inptr );
	char *bp = &return_buffer[0];

	*bp = 0;
	while( true ) 
	{
		char gotchar = (char)fgetc( inptr );

		if (( col_number == 1 ) && (gotchar == '#') )	
		{
			// Comment line, skip till next newline and
			// continue;
			SkipTillNewline();
			continue;
		}

		if( strlen(return_buffer) == 0 )
		{	
			// new word
			if( isMonoWord(gotchar ) )
			{
				*bp = gotchar;
				bp++;
				*bp = 0;
				return &return_buffer[0];
			}

			if( isWhitespace( gotchar ) )
			{
				continue;
			}

			if( isNewline( gotchar ))
			{
				continue;
			}
		}


		// otherwise were in a word

		bool end=false;

		if( feof(inptr ) )
		{
			end = true;
		}
		else if( isNewline( gotchar ) )
		{
			line_number ++;
			col_number = 1;
			end=true;
		}
		else if( isWhitespace( gotchar ) )
		{	
			end=true;
		}
		else if( isMonoWord( gotchar ) )
		{
			end=true;
		}

		if( end==true )
		{
			// put back the ending char
			ungetc( (int)gotchar, inptr );

			return &return_buffer[0];
		}	
		else
		{
			// add the char to the return buffer
			*bp = gotchar;
			bp ++;
			*bp = 0;

			col_number ++;
		}
	}
}

int Reader::CurrentLine()
{
	return line_number;
}

#if 0
main()
{
	Reader my_reader;
	my_reader.OpenFile( "t.txt" );
	for( int i = 0 ; i < 40 ; i++)
	{
		printf("\"%s\"\n", my_reader.NextWord( ) );
	}
}
#endif
