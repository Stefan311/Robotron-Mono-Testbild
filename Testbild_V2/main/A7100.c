#include "main.h"

void IRAM_ATTR A7100() 
{
	dedic_gpio_cpu_ll_write_all(7);
	XTHAL_SET_CCOUNT(0);
	int a = 0;

	// VSYNC-Periode
	for (int b=0;b<4;b++)
	{
		a += 10940;
		wait_until(a);
		CLR_GPIO(1)
		a += 104;
		wait_until(a);
		SET_GPIO(1);
	}

	// Leere Zeilen zum Beginn
	for (int b=0;b<28;b++)
	{
		a += 247;
		wait_until(a);
		CLR_GPIO(1)
		a += 10797;
		wait_until(a);
		SET_GPIO(1);
	}

	// horizontale helle Linie (obere Grenzlinie)
	a += 247;
	wait_until(a);
	CLR_GPIO(1)
	wait_until(a+1100);
	CLR_GPIO(6)
	wait_until(a+10715);
	SET_GPIO(6)
	a += 10797;
	wait_until(a);
	SET_GPIO(1);

	// Gitterlinien
	for (int b=0;b<398;b++)
	{
		a += 247;
		wait_until(a);
		CLR_GPIO(1)

		// 1 helles Pixel (linke Grenzlinie)
		wait_until(a+1100);
		CLR_GPIO(6)
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP

		if ((b & 7) == 7)
		{
			// entweder normale linie (horizontale Gitterlinie)
			SET_GPIO(4)
		}
		else
		{
			// oder normale Pixel alle 8 Pixelpositionen (vertikale Gitterlinien)
			SET_GPIO(6)
			for (int c=1;c<80;c++)
			{
				// normales Pixel (vertikale Gitterlinie)
				wait_until(a+1100+c*120);
				CLR_GPIO(2)
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				SET_GPIO(2)
			}
		}

		// 1 helles Pixel (rechte Grenzlinie)
		wait_until(a+10690);
		CLR_GPIO(6)
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		SET_GPIO(6)

		a += 10797;
		wait_until(a);
		SET_GPIO(1);
	}

	// horizontale helle Linie (untere Grenzlinie)
	a += 247;
	wait_until(a);
	CLR_GPIO(1)
	wait_until(a+1100);
	CLR_GPIO(6)
	wait_until(a+10715);
	SET_GPIO(6)
	a += 10797;
	wait_until(a);
	SET_GPIO(1);
}
