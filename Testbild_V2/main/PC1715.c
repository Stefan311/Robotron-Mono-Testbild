#include "main.h"

void IRAM_ATTR PC1715() {
	dedic_gpio_cpu_ll_write_all(5);
	XTHAL_SET_CCOUNT(0);
	int a = 0;

	// VSYNC-Periode
	for (int b=0;b<12;b++)
	{
		a += 13700;
		wait_until(a);
		CLR_GPIO(1)
		a += 1297;
		wait_until(a);
		SET_GPIO(1);
	}

	// Deformierter erster Zeilenimpuls
	a += 2644;
	wait_until(a);
	CLR_GPIO(1)
	a += 12359;
	wait_until(a);
	SET_GPIO(1);

	// Leere Zeilen zum Beginn
	for (int b=0;b<11;b++)
	{
		a += 1173;
		wait_until(a);
		CLR_GPIO(1)
		a += 13823;
		wait_until(a);
		SET_GPIO(1);
	}

	// horizontale helle Linie (obere Grenzlinie)
	a += 1173;
	wait_until(a);
	CLR_GPIO(1)
	wait_until(a+1527);
	CLR_GPIO(4)
	wait_until(a+12635);
	SET_GPIO(4)
	a += 13823;
	wait_until(a);
	SET_GPIO(1);

	// Gitterlinien
	for (int b=0;b<297;b++)
	{
		a += 1173;
		wait_until(a);
		CLR_GPIO(1)
		wait_until(a+1527);
		CLR_GPIO(4)
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

		// 1 helles Pixel (linke Grenzlinie)
		if ((b & 7) == 7)
		{
			// entweder normale linie (horizontale Gitterlinie)
			SET_GPIO(6)
		}
		else
		{
			// oder normale Pixel alle 8 Pixelpositionen (vertikale Gitterlinien)
			SET_GPIO(4)
			for (int c=1;c<80;c++)
			{
				// normales Pixel (vertikale Gitterlinie)
				wait_until(a+1527+((c*2221)>>4));
				SET_GPIO(2)
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
				CLR_GPIO(2)
			}
		}

		// 1 helles Pixel (rechte Grenzlinie)
		wait_until(a+12617);
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
		SET_GPIO(4)

		a += 13823;
		wait_until(a);
		SET_GPIO(1);
	}

	// horizontale helle Linie (untere Grenzlinie)
	a += 1173;
	wait_until(a);
	CLR_GPIO(1)
	wait_until(a+1527);
	CLR_GPIO(4)
	wait_until(a+12635);
	SET_GPIO(4)
	a += 13823;
	wait_until(a);
	SET_GPIO(1);

	// leere Zeile
	a += 1173;
	wait_until(a);
	CLR_GPIO(1)
	a += 13823;
	wait_until(a);
	SET_GPIO(1);
}
