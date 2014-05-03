/*
History
=======

Notes
=======

*/

#include "../clib/stl_inc.h"
#include <iomanip>

#include "eprog.h"
namespace Pol {
  namespace Bscript {
	void EScriptProgram::dump( ostream& os )
	{
	  Token token;
	  unsigned PC;
	  if ( !exported_functions.empty() )
	  {
		os << "Exported Functions:" << endl;
		os << "   PC  Args  Name" << endl;
		for ( auto &elem : exported_functions )
		{
		  os << std::setw( 5 ) << elem.PC
			<< std::setw( 6 ) << elem.nargs
			<< "  " << elem.name
			<< endl;
		}
	  }
	  unsigned nLines = tokens.length() / sizeof( StoredToken );
	  for ( PC = 0; PC < nLines; PC++ )
	  {
		if ( _readToken( token, PC ) )
		{
		  return;
		}
		else
		{
		  if ( fileline.size() > PC && !fileline[PC].empty() )
			os << fileline[PC] << endl;
		  if ( program_PC == PC && !program_decl.empty() )
			os << program_decl << endl;
		  if ( function_decls.size() > PC && !function_decls[PC].empty() )
			os << function_decls[PC] << endl;
		  if ( sourcelines.size() > PC && !sourcelines[PC].empty() )
			os << sourcelines[PC] << endl;
		  os << PC << ": " << token << endl;
		  if ( token.id == INS_CASEJMP )
		  {
			dump_casejmp( os, token );
		  }
		}
	  }
	}

	void EScriptProgram::dump_casejmp( ostream& os, const Token& token )
	{
	  const unsigned char* dataptr = token.dataptr;
	  for ( ;; )
	  {
		unsigned short offset = *(const unsigned short*)dataptr;
		dataptr += 2;
		unsigned char type = *dataptr;
		dataptr += 1;
		if ( type == CASE_TYPE_LONG )
		{
		  unsigned int lval = *(const unsigned int*)dataptr;
		  dataptr += 4;
		  os << "\t" << lval << ": @" << offset << endl;
		}
		else if ( type == CASE_TYPE_DEFAULT )
		{
		  os << "\tdefault: @" << offset << endl;
		  break;
		}
		else
		{ // type is the length of the string, otherwise
		  os << "\t\"" << string( (const char*)dataptr, type ) << "\": @" << offset << endl;
		  dataptr += type;
		}
	  }
	}
  }
}
