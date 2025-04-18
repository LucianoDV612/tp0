#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	size_t bytes;
	int32_t handshake = 1;
	int32_t result;
	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger,"Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	// Loggeamos el valor de config
	
	if(config_has_property(config, "IP")){
		ip = config_get_string_value(config,"IP");
	}
	if(config_has_property(config, "PUERTO")){
		puerto = config_get_string_value(config, "PUERTO");
	}
	if(config_has_property(config,"VALOR")){
		valor = config_get_string_value(config,"VALOR");
		log_info(logger,"Valor leido de la config: %s", valor);
	}

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

		bytes = send(conexion, &handshake, sizeof(int32_t), 0);
		bytes = recv(conexion, &result, sizeof(int32_t), MSG_WAITALL);

		if (result == 0) {
    		log_info(logger,"Comunicacion con el servidor establecida");
		} else {
    		perror("No se pude establecer la comunicacion con el servidor");
			exit(EXIT_FAILURE);
		}

	// Enviamos al servidor el valor de CLAVE como mensaje

	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete

	paquete(conexion);


	terminar_programa(conexion, logger, config);

	return 0;
	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}


t_log* iniciar_logger(void){

	t_log* nuevo_logger = log_create("/home/utnso/Desktop/tp0/client/tp0.log","CLIENTE.LOGGER",true,LOG_LEVEL_INFO);
	if(nuevo_logger == NULL){
		perror("No se pudo crear o encontrar el archivo .log");
		exit(EXIT_FAILURE);
	}
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("/home/utnso/Desktop/tp0/client/cliente.config");
	if(nuevo_config == NULL){
		perror("No se pudo crear el config.");
		exit(EXIT_FAILURE);
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	leido = readline("> ");
    log_info(logger,">> %s", leido);

	while (strcmp(leido, "") != 0){
		free(leido);
		leido = readline("> ");
		log_info(logger,">> %s", leido);
	} 
	free(leido);
	
	// La primera te la dejo de yapa
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	// ¡No te olvides de liberar las lineas antes de regresar!
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();
	
	// Leemos y esta vez agregamos las lineas al paquete

	leido = readline("> ");
	while(strcmp(leido, "") != 0){
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		free(leido);
		leido = readline("> ");
	}
	enviar_paquete(paquete,conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
