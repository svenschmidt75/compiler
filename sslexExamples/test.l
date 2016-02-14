
%definition


word	[^ \t\n]+
eol		\n



%specification


^{word}	wordCount++;
{word}	charCount++;
{eol}	;

