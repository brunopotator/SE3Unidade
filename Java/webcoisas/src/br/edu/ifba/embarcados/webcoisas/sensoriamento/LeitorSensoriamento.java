package br.edu.ifba.embarcados.webcoisas.sensoriamento;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;

public class LeitorSensoriamento implements 
	Runnable {
	
	private static final int DESLOCAMENTO_RFID = 22;
	private static final int DESLOCAMENTO_TEMP = 15;
	private static final int DESLOCAMENTO_UMID = 8;
	private static final int DESLOCAMENTO_LDR = 4;
	private static final int DESLOCAMENTO_SOM = 0;

	private static final String ARQUIVO_PIPE = 
			"/home/arfilho/workspacecpp/incubadorap/incubadora";
	
	private RandomAccessFile pipe = null;
	private boolean continuar = true;
	
	private static Integer sensores = 0;

	@Override
	public void run() {
		try {
			pipe = new RandomAccessFile(ARQUIVO_PIPE,
					"r");
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			
			return;
		}
		
		continuar = true;
		while (continuar) {
			try {
				String s = pipe.readLine();
				
				if ((s != null) && (!s.equals(""))) {
					System.out.println("Sensores lidos: " + s);
					
					
					synchronized(sensores) {
						sensores = Integer.parseInt(s);
					}
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
			
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	public void parar() {
		continuar = false;
	}
	
	public static Integer getSensores() {
		synchronized (sensores) {
			
			return sensores;
		}
	}
	
	public static int getRFID() {
		int rfid = getSensores();
		
		rfid = rfid >> DESLOCAMENTO_RFID;
		
		return rfid;
	}
	
	public static int getTemperatura() {
		int temp = getSensores();
		
		temp = (temp & 4161536) >> DESLOCAMENTO_TEMP;
		
		return temp;
	}
	
	public static int getUmidade() {
		int umid = getSensores();
		
		umid = (umid & 32512) >> DESLOCAMENTO_UMID;
		
		return umid;
	}
	
	public static int getLuminosidade() {
		int ldr = getSensores();
		
		ldr = (ldr & 240) >> DESLOCAMENTO_LDR;
		
		return ldr;
	}
	
	public static int getSom() {
		int som = getSensores();
		
		som = (som & 15);
		
		return som;
	}
	
}
