
int main(void)
{
	volatile unsigned long v;

	v = *(volatile unsigned long *)0xE000ED00;
	(void)v;
	v = *(volatile unsigned long *)0xE002ED00;
	(void)v;

	*(unsigned long *)(0x40004828) = '*';

	while(1);
	return(0);
}
/* EOF */
