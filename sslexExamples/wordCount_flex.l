

%{
%}

word	[^ \t\n]+
eol		\n



%%

^"%{".*	++bracelevel; yyless( 2 );	/* eat only %{ */
^"%}".*	--bracelevel; yyless( 2 );	/* eat only %} */


%%
