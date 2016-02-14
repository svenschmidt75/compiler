
%definition

%x NL

word	[^ \t\n]+
eol		\n



%specification

^"int"	cout << yyText << "\n";

{word}	{ wordCount++; charCount += yyLeng;
											}

{eol}	{ BEGIN(NL); charCount++; lineCount++;
											}
.		charCount++;


<NL>.	{
		cout << "I'am at the beginning of a line!\n";
		BEGIN( INITIAL );
		}

