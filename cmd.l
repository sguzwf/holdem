%{
#include "cmd.tab.h"
%}
%option noinput
%option nounput
%%
"login"                     { return LOGIN; }
"logout"                    { return LOGOUT;}
"mkdir"                     { return MKDIR; }
"cd"                        { return CD;    }
"exit"                      { return EXIT;  }
"ls"                        { return LS;    }
"pwd"                       { return PWD;   }
"raise"                     { return RAISE; }
"call"                      { return CALL;  }
"fold"                      { return FOLD;  }
"check"                     { return CHECK; }
"yell"                      { return YELL;  }
"+"                         { return ADD;   }
"-"                         { return SUB;   }
"*"                         { return MUL;   }
"/"                         { return DIV;   }
"("                         { return OP;    }
")"                         { return CP;    }
\n                          { return EOL;   }
[0-9]+                      { yylval.intval = atoi(yytext); return NUMBER; }
0x[a-f0-9]+                 { yylval.intval = strtol(yytext, NULL, 16); return NUMBER; }
[a-zA-Z][a-zA-Z0-9]*        { yylval.strval = strdup(yytext); return SYMBOL; }
.
%%