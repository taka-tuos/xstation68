OUTPUT_ARCH(m68k)

MEMORY {
	rom : ORIGIN = 0x00000000, LENGTH = 128k
	ram : ORIGIN = 0x00020000, LENGTH = 128k
}

SECTIONS
{
	.text	0x00000000 : AT(0)
	{
		__s_text = . ;
		*(.text)
		CONSTRUCTORS
		__e_text = . ;
	} > rom

	.data	0x00000000 + SIZEOF(.text) : AT(SIZEOF(.text))
	{
		__s_data = . ;
		*(.data)
		__e_data = . ;
	} > rom

	.rodata	0x00000000 + SIZEOF(.text) + SIZEOF(.data) : AT(SIZEOF(.text) + SIZEOF(.data))
	{
		__s_rodata = . ;
		*(.rodata)
		__e_rodata = . ;
	} > rom

	.bss	0x00020000 :
	{
		__s_bss = . ;
		*(.bss)
		*(COMMON)
		__e_bss = . ;
	} > ram
}

