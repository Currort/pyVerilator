OUTPUT_ARCH( "riscv" )
ENTRY( _start )

MEMORY
{
	/* Fake ROM area */
    /* BASE_ADDRESS replaced with the real value by the pysoc. */
	rom (rxa)  		: ORIGIN = BASE_ADDRESS       ,       LENGTH = 32K   		/*只读存储器*/
	peripheral(wxa) : ORIGIN = BASE_ADDRESS + 32K,       LENGTH = PERIPHERAL_LEN           /*读写存储器*/
	ram (wxa)  		: ORIGIN = BASE_ADDRESS + PERIPHERAL_LEN + 32K,       LENGTH = 32K	        /*读写存储器*/
}

SECTIONS
{
	.init :
	{
		_text = .;
		KEEP (*(SORT_NONE(.init)))
	} >rom AT>rom
	
	.text :
	{
		*(.text.unlikely .text.unlikely.*)
		*(.text.startup .text.startup.*)
		*(.text .text.*)
		*(.gnu.linkonce.t.*)
	} >rom AT>rom
	
	.fini :
	{
		KEEP (*(SORT_NONE(.fini)))
		_etext = .;
	} >rom AT>rom

	.rodata.align :
	{
		. = ALIGN(4);
		_rodata = .;
	} >rom AT>rom

	.rodata.start :
	{
		_rodata_lma = LOADADDR(.rodata.start);
	} >rom AT>rom

	.rodata :
	{
		*(.rdata)
		*(.rodata .rodata.*)
		*(.gnu.linkonce.r.*)

		. = ALIGN(4);
		_erodata = .;
	} >rom AT>rom

	.data.align :
	{
		. = ALIGN(4);
		_data = .;
	} >ram AT>rom

	.data.start :
	{
		_data_lma = LOADADDR(.data.start);
	} >ram AT>rom

	.data :
	{
		*(.data .data.*)
		*(.gnu.linkonce.d.*)
		. = ALIGN(8);
		PROVIDE( __global_pointer$ = . + 0x800 );
		*(.sdata .sdata.*)
		*(.sdata2 .sdata2.*)
		*(.gnu.linkonce.s.*)
		. = ALIGN(8);
		*(.srodata.cst16)
		*(.srodata.cst8)
		*(.srodata.cst4)
		*(.srodata.cst2)
		*(.srodata .srodata.*)

		. = ALIGN(4);
		_edata = .;
	} >ram AT>rom

	.bss.align :
	{
		. = ALIGN(4);
		_bss = .;
	} >ram AT>rom

	.bss.start :
	{
		_bss_lma = LOADADDR(.bss.start);
	} >ram AT>rom

	.bss :
	{
		*(.sbss*)
		*(.gnu.linkonce.sb.*)
		*(.bss .bss.*)
		*(.gnu.linkonce.b.*)
		*(COMMON)

		. = ALIGN(4);
		_ebss = .;
	} >ram AT>rom

	. = ALIGN(8);
	_end = .;

	.stack :
	{
		. = ALIGN(16);
		//. += __stack_size;
		_stack_top = .;
		
	} >ram AT>ram

	.uart :
	{
		* (.uart*)
		. = ALIGN(8);
		PROVIDE( _uart = .);
	} >peripheral

}
