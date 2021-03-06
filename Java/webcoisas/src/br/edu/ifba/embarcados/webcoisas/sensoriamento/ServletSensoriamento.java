package br.edu.ifba.embarcados.webcoisas.sensoriamento;

import javax.servlet.ServletException;

import com.sun.jersey.spi.container.servlet.ServletContainer;
import com.sun.jersey.spi.container.servlet.WebConfig;

@SuppressWarnings("serial")
public class ServletSensoriamento extends ServletContainer {
	
	private LeitorSensoriamento leitor = null;
	private Thread tLeitor = null;
	
	@Override
	protected void init(WebConfig webConfig) throws ServletException {
		System.out.println("Iniciando servi�o web...");
		
		leitor = new LeitorSensoriamento();
		tLeitor = new Thread(leitor);
		tLeitor.start();
		
		super.init(webConfig);
	}

	@Override
	public void destroy() {
		System.out.println("Finalizando servi�o web...");
		
		leitor.parar();
		try {
			tLeitor.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		super.destroy();
	}
}
