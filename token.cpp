#include "token.h"
#include "tokenTyp.h"
using namespace std;


token::token() : tokenType(NUL), tokenDesc(""), lineNumber(0)
{
}

token::token(tokenTyp tkn) : tokenType(tkn), tokenDesc(""), lineNumber(0)
{
}

token::token(tokenTyp tokenTp, string tokenDes, int lineNumbe) : 
	tokenType(tokenTp), tokenDesc(tokenDes), lineNumber(lineNumbe), identifierName("")
{
}

token::token(enum tokenTyp tokenTp, string tokenDes, int lineNumbe, string ident) : 
	tokenType(tokenTp), tokenDesc(tokenDes), lineNumber(lineNumbe), identifierName(ident)
{
}

token::~token()
{
}
