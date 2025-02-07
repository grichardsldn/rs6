#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "reader.h"

bool Reader2::OpenFile( const char *a_filename )
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

	return true;
}

Reader2::~Reader2()
{
	fclose(inptr );
}

bool Reader2::isMonoWord( char val )
{
	switch (val )
	{
		case '!':
		case '�':
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

bool Reader2::isNewline( char val )
{
	if ( val == '\n' )
	{
		return true;
	}

	return false;
}

bool Reader2::isWhitespace( char val )
{
	switch( val )
	{
		case ' ':
		case '\t':
		return true;
	}	
	return false;
}

void Reader2::SkipTillNewline()
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

char *Reader2::NextWord() 
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

int Reader2::CurrentLine()
{
	return line_number;
}
/*
main()
{
	Reader2 my_reader;
	my_reader.OpenFile( "t.txt" );
	for( int i = 0 ; i < 40 ; i++)
	{
		printf("\"%s\"\n", my_reader.NextWord( ) );
	}
}*/
