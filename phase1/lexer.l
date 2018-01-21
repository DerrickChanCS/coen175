%option nounput noyywrap
%{
    #include <iostream>
    #include <string>
%}

digit   [0-9]
lettter [a-zA-Z]

%%
[ \t\n]+               { /* do nothing */}
"/*"                   {
                           register int c;
                           while(1){
                             c = yyinput();
                             if( c == '*'){
                               while ( c == '*') {
                                 c = yyinput();
                               } 
                               if( c == '/'){
                                 break;
                               }
                             }
                             if( c == EOF){
                               std::cout<<"EOF in comment\n";
                               break;
                             }
                           }
                       }
"auto"                 { std::cout<<"keyword "<<yytext<<"\n"; }
"break"                { std::cout<<"keyword "<<yytext<<"\n"; }
"case"                 { std::cout<<"keyword "<<yytext<<"\n"; }
"char"                 { std::cout<<"keyword "<<yytext<<"\n"; }
"const"                { std::cout<<"keyword "<<yytext<<"\n"; }
"continue"             { std::cout<<"keyword "<<yytext<<"\n"; }
"default"              { std::cout<<"keyword "<<yytext<<"\n"; }
"do"                   { std::cout<<"keyword "<<yytext<<"\n"; }
"double"               { std::cout<<"keyword "<<yytext<<"\n"; }
"else"                 { std::cout<<"keyword "<<yytext<<"\n"; }
"enum"                 { std::cout<<"keyword "<<yytext<<"\n"; }
"extern"               { std::cout<<"keyword "<<yytext<<"\n"; }
"float"                { std::cout<<"keyword "<<yytext<<"\n"; }
"for"                  { std::cout<<"keyword "<<yytext<<"\n"; }
"goto"                 { std::cout<<"keyword "<<yytext<<"\n"; }
"if"                   { std::cout<<"keyword "<<yytext<<"\n"; }
"int"                  { std::cout<<"keyword "<<yytext<<"\n"; }
"long"                 { std::cout<<"keyword "<<yytext<<"\n"; }
"register"             { std::cout<<"keyword "<<yytext<<"\n"; }
"return"               { std::cout<<"keyword "<<yytext<<"\n"; }
"short"                { std::cout<<"keyword "<<yytext<<"\n"; }
"signed"               { std::cout<<"keyword "<<yytext<<"\n"; }
"sizeof"               { std::cout<<"keyword "<<yytext<<"\n"; }
"static"               { std::cout<<"keyword "<<yytext<<"\n"; }
"struct"               { std::cout<<"keyword "<<yytext<<"\n"; }
"switch"               { std::cout<<"keyword "<<yytext<<"\n"; }
"typedef"              { std::cout<<"keyword "<<yytext<<"\n"; }
"union"                { std::cout<<"keyword "<<yytext<<"\n"; }
"unsigned"             { std::cout<<"keyword "<<yytext<<"\n"; }
"void"                 { std::cout<<"keyword "<<yytext<<"\n"; }
"volatile"             { std::cout<<"keyword "<<yytext<<"\n"; }
"while"                { std::cout<<"keyword "<<yytext<<"\n"; }
[a-zA-Z_][a-zA-Z_0-9]* { std::cout<<"identifier "<<yytext<<"\n"; }
0|[1-9][0-9]*          { std::cout<<"decimal "<<yytext<<"\n"; }
0[xX][0-9a-fA-F]+      { std::cout<<"hexadecimal "<<yytext<<"\n"; }
"\""                   { 
                            std::string out = "\"";
                            register int c;
                            while(1){
                             c = yyinput();
                             if( c == '\n'){
                                std::cout<<"new line in string\n";
                                break;
                             }
                             if( c == '"'){
                                char test = *out.rbegin();
                                if ( test == '\\'){}
                                else{
                                  std::cout<<"string "<<out<<"\"\n";
                                  break;
                                }
                             }
                             out = out + (char)c;
                                
                            }
                       }
"\'"                   { 
                            std::string out = "'";
                            register int c;
                            while(1){
                             c = yyinput();
                             if( c == EOF){
                                std::cout<<"Unterminated character\n";
                                break;

                             }
                             if( c == '\n'){
                                std::cout<<"new line in character\n";
                                break;
                             }
                             if( c == '\''){
                                char test = *out.rbegin();
                                if ( test == '\\'){}
                                else{
                                  if ( out.length() < 2){
                                    std::cout<<"Empty character \n";
                                    break;
                                  }
                                  std::cout<<"character "<<out<<"'\n";
                                  break;
                                }
                             }
                             out = out + (char)c;
                                
                            }
                       }
 /*\'(.|[^\\'(\n)])+\'    { std::cout<<"character "<<yytext<<"\n"; }*/
"="                    { std::cout<<"operator "<<yytext<<"\n"; }
"||"                   { std::cout<<"operator "<<yytext<<"\n"; }
"&&"                   { std::cout<<"operator "<<yytext<<"\n"; }
"|"                    { std::cout<<"operator "<<yytext<<"\n"; }
"=="                   { std::cout<<"operator "<<yytext<<"\n"; }
"!="                   { std::cout<<"operator "<<yytext<<"\n"; }
"<"                    { std::cout<<"operator "<<yytext<<"\n"; }
">"                    { std::cout<<"operator "<<yytext<<"\n"; }
"<="                   { std::cout<<"operator "<<yytext<<"\n"; }
">="                   { std::cout<<"operator "<<yytext<<"\n"; }
"+"                    { std::cout<<"operator "<<yytext<<"\n"; }
"-"                    { std::cout<<"operator "<<yytext<<"\n"; }
"*"                    { std::cout<<"operator "<<yytext<<"\n"; }
"/"                    { std::cout<<"operator "<<yytext<<"\n"; }
"%"                    { std::cout<<"operator "<<yytext<<"\n"; }
"&"                    { std::cout<<"operator "<<yytext<<"\n"; }
"!"                    { std::cout<<"operator "<<yytext<<"\n"; }
"++"                   { std::cout<<"operator "<<yytext<<"\n"; }
"--"                   { std::cout<<"operator "<<yytext<<"\n"; }
"->"                   { std::cout<<"operator "<<yytext<<"\n"; }
"("                    { std::cout<<"operator "<<yytext<<"\n"; }
")"                    { std::cout<<"operator "<<yytext<<"\n"; }
"["                    { std::cout<<"operator "<<yytext<<"\n"; }
"]"                    { std::cout<<"operator "<<yytext<<"\n"; }
"{"                    { std::cout<<"operator "<<yytext<<"\n"; }
"}"                    { std::cout<<"operator "<<yytext<<"\n"; }
";"                    { std::cout<<"operator "<<yytext<<"\n"; }
":"                    { std::cout<<"operator "<<yytext<<"\n"; }
"."                    { std::cout<<"operator "<<yytext<<"\n"; }
","                    { std::cout<<"operator "<<yytext<<"\n"; }
"..."                  { std::cout<<"operator "<<yytext<<"\n"; }
%%

int main(int argc, char *argv[]) {
    yylex();
}
