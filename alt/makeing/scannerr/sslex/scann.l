/* Test-Scanner-File */

%incl_sc
{
	SCA SCB
}

%def_char
{
	'a'-'d','z'
	'A'-'D','Z'
/*
	'a', 'b'
	/*, 'c'*/
	'a'-'z'
	\a
	0-10
	\x10-20
	'b','c','d'
*/
}

/*
%excl_sc
{
	SCC SCD
}

%predef_nea
{
	DELIM		[a-z]
}



<SCA,SCB>(a|b)+c

test{DELIM}
*/

/*
<SCA>abc
<SCA>def
(a|b)*abb
ab
abc
(a|b)*abb
*/


([a-cA-C])+

"DBZz"

[^A-D]