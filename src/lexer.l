%{
#include "parser.tab.hh"
#include "driver.h"

#include <string>

extern Driver* g_driver;
%}

%option noyywrap nounput noinput

%%

[ \t\r\n]+                  ;
"-"?([0-9]+|[0-9]+\.[0-9]*|\.[0-9]+)([eE][-+]?[0-9]+)? {
                                yylval.text = new std::string(yytext);
                                return NUMBER;
                            }
"table"                     { return TABLE; }
q\([^)]*\)                  {
                                std::string value(yytext);
                                value = value.substr(2, value.size() - 3);
                                yylval.text = new std::string(value);
                                return STRING;
                            }
"#{"[_a-zA-Z][_a-zA-Z0-9]*"}" {
                                std::string value(yytext);
                                value = value.substr(2, value.size() - 3);
                                yylval.text = new std::string(value);
                                return CONST_REF;
                            }
"<-"                        { return CONST_DECL; }
[_a-zA-Z][_a-zA-Z0-9]*      {
                                yylval.text = new std::string(yytext);
                                return IDENT;
                            }
[(),\[\]=;]                 { return yytext[0]; }
.                           {
                                std::string message = "Unexpected character: ";
                                message += yytext;
                                g_driver->error(message);
                            }

%%
