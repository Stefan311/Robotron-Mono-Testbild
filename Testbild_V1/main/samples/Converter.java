import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;

/*
 * Dieses Programm wandelt CSV-Export-Dateien des KingstVIS Logikanalyzers in Binärdateien um, die mit einem ESP32
 * wieder abgespielt werden können.
 * 
 * Binärformat:
 * 16-Bit Samples, 
 * - Bit 0-3	: Bits der Binärsignale
 * - Bit 4-15	: Samplezeit in CPU-Takte bei 240MHz
 * Bei Samplezeit = 0 wird im ESP32 die Berechnung der CPU-Takte, und die Warteschleife übersprungen. 
 * Dadurch kann eine höhere Bitrate für die Ausgabe erreicht werden.
 *   
 */


public class Converter {

	private static void processfile(String imp, String exp) throws IOException
	{
		System.out.print("Konveriere "+imp+" in "+exp+"...");
		BufferedReader rd = new BufferedReader(new FileReader(new File(imp)));
		FileOutputStream wt = new FileOutputStream(exp);

		String s[] = rd.readLine().split(", ");	// Kopfzeile einlesen / überspringen
		int oldState = 0;	// Signale letztes Sample
		int newState = 0;	// Signale dieses Sample
		int oldTime = 0;	// Zeit letztes Sample (in ESP32 CPU-Takte)
		int newTime = 0;	// CPU-Takte dieses Sample
		double startTime = -1.0d; // offset Zeit zum Dateianfang der CSV-Datei (in Sekunden)

		while (rd.ready())
		{
			s = rd.readLine().split(", ");
			if (startTime == -1.0d)
			{
				 startTime = Double.parseDouble(s[0]);
			}
			// Sample-Zeit in CPU-Takte umrechnen, der ESP32 läuft mit 240MHz
			newTime = (int)((Double.parseDouble(s[0])-startTime) * 240000000.0d);
			
			// Sample-Bits einlesen
			newState = 0;
			for (int a=1;a<s.length;a++)
			{
				newState |= (s[a].equals("0")) ? 1<<(a-1) : 0;
			}

			// die ESP32-Samples gehen maximal 4092 CPU-Takte, wenn die Import-Samples länger sind, 
			// dann müssen wir die aufteilen 
			while (newTime-oldTime > 0xfff)
			{
				wt.write(oldState | 0xf0);
				wt.write(0xff);
				oldTime +=  0xfff;
			}
			
			int t = (newTime - oldTime) << 4;
			t = t | newState;
		    oldTime = newTime;
		    oldState = newState;
		    wt.write(t & 0xff);
		    wt.write((t >> 8) & 0xff);
		}		
		wt.close();
		rd.close();
		System.out.println("fertig");
	}
	
	public static void main(String[] args) 
	{
		try 
		{
			processfile("A7100.csv", "A7100.bin");
			processfile("PC1715.csv", "PC1715.bin");
			processfile("EC1834.csv", "EC1834.bin");
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		}
	}

}
