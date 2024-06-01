#include "main.h"

void IRAM_ATTR EC1834() 
{
	dedic_gpio_cpu_ll_write_all(7);
	XTHAL_SET_CCOUNT(0);
	int a = 0;

	// VSYNC-Periode
	for (int b=0;b<6;b++)
	{
		a += 11294;
		wait_until(a);
		CLR_GPIO(1)
		a += 226;
		wait_until(a);
		SET_GPIO(1);
	}

	// Leere Zeilen zum Beginn
	for (int b=0;b<12;b++)
	{
		a += 482;
		wait_until(a);
		CLR_GPIO(1)
		a += 11038;
		wait_until(a);
		SET_GPIO(1);
	}

	// horizontale helle Linie (obere Grenzlinie)
	a += 482;
	wait_until(a);
	CLR_GPIO(1)
	wait_until(a+952);
	CLR_GPIO(6)
	wait_until(a+9500);
	SET_GPIO(6)
	a += 11038;
	wait_until(a);
	SET_GPIO(1);

	// Gitterlinien
	for (int b=0;b<348;b++)
	{
		a += 482;
		wait_until(a);
		CLR_GPIO(1)

		// 1 helles Pixel (linke Grenzlinie)
		wait_until(a+952);
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
				wait_until(a+952+c*106+(c>>1)+(c>>3));
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
				SET_GPIO(2)
			}
		}

		// 1 helles Pixel (rechte Grenzlinie)
		wait_until(a+9485);
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
		SET_GPIO(6)

		a += 11038;
		wait_until(a);
		SET_GPIO(1);
	}

	// horizontale helle Linie (untere Grenzlinie)
	a += 482;
	wait_until(a);
	CLR_GPIO(1)
	wait_until(a+952);
	CLR_GPIO(6)
	wait_until(a+9500);
	SET_GPIO(6)
	a += 11038;
	wait_until(a);
	SET_GPIO(1);

	// leere Zeile
	a += 482;
	wait_until(a);
	CLR_GPIO(1)
	a += 11038;
	wait_until(a);
	SET_GPIO(1);
}
