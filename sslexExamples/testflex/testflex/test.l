/* scan.l - scanner for flex input */

%{
	unsigned wordCount = 0, charCount = 0, lineCount = 1;
%}

%x NL

word	[^ \t\n]+
eol		\n

%%

^{word}	wordCount++;
{word}	charCount++;



%%

main()
{
	yyin = fopen( "/home/sschmidt/Documents/scan2.l", "r" );
	char	*yytext;


	yylex();
	printf( "# of lines = %d, # of chars = %d, # of words = %d\n", lineCount, charCount, wordCount );
}

