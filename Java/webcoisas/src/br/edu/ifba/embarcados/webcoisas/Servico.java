package br.edu.ifba.embarcados.webcoisas;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;

import br.edu.ifba.embarcados.webcoisas.sensoriamento.LeitorSensoriamento;

@Path("servico")
public class Servico {
		
	@GET
	@Path("/sensores")
	@Produces(MediaType.TEXT_PLAIN)
	public String getSensores() {
		return LeitorSensoriamento.getSensores().
				toString();
	}
	
	@GET
	@Path("/sensores/id")
	@Produces(MediaType.TEXT_PLAIN)
	public String getId() {
		return LeitorSensoriamento.getRFID() + "";
	}
	
	@GET
	@Path("/sensores/temperatura")
	@Produces(MediaType.TEXT_PLAIN)
	public String getTemperatura() {
		return LeitorSensoriamento.getTemperatura() + "";
	}
	
	@GET
	@Path("/sensores/umidade")
	@Produces(MediaType.TEXT_PLAIN)
	public String getUmidade() {
		return LeitorSensoriamento.getUmidade() + "";
	}
	
	@GET
	@Path("/sensores/luminosidade")
	@Produces(MediaType.TEXT_PLAIN)
	public String getLuminosidade() {
		return LeitorSensoriamento.getLuminosidade() + "";
	}
	
	@GET
	@Path("/sensores/som")
	@Produces(MediaType.TEXT_PLAIN)
	public String getSom() {
		return LeitorSensoriamento.getSom() + "";
	}
	
	@GET
	@Path("/sensores/leitura")
	@Produces(MediaType.TEXT_PLAIN)
	public String getLeitura() {
		return LeitorSensoriamento.getLeitura();
	}


}
