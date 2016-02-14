	.stack	1024

	.import label2

	.text


	bra.s	.label
.label:
	nop

	.data

.dc.b	10,30,10, 0xffa987
.string "Sven Schmidt"
