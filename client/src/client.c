#include "client.h"

// Declaración de la función loggear_config
/*void loggear_config(t_config* config, t_log* logger, char* ip, char* puerto, char* valor);

void leer_config(t_config* config, char** ip, char** puerto, char** valor);*/

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	log_info(logger, "Soy un Log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	//leer_config(config, &ip, &puerto, &valor);
	//log_info(logger, "Config leido.");

	// Loggeamos el valor de config
	log_info(logger, "Valor de la config: %s", valor);

	//loggear_config(config, logger, ip, puerto, valor);
	//log_info(logger, "Config loggeado.");

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);
	//log_info(logger, "Consola leida.");

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);
	if (!conexion) {
			log_error(logger, "Error al crear la conexion");
			exit(EXIT_FAILURE);
	}

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	//log_info(logger, "Mensaje enviado al servidor.");

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("tp0.log", "client_log", 1, LOG_LEVEL_INFO);	
	if (nuevo_logger == NULL) {
		printf("Error al crear el logger.\n");
        exit(EXIT_FAILURE);
    }

	//log_info(nuevo_logger, "Soy un Log");

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("cliente.config");
    if (nuevo_config == NULL) {
		printf("Error al crear el config.\n");
        exit(EXIT_FAILURE);
    }

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// Primera linea
	leido = readline("> ");

	//log_info(logger, "%s", leido);

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
    while (strcmp(leido, "") != 0) {
		log_info(logger, "%s", leido);

		leido = readline("> ");
	}

	// Liberar las lineas antes de regresar
	free(leido);

	//return;
}

void paquete(int conexion)
{
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");

	paquete = crear_paquete();
	if (paquete == NULL) {
        printf("Error al crear el paquete.\n");
        exit(1);
    }

	agregar_a_paquete(paquete, leido, strlen(leido) + 1);

    while (strcmp(leido, "") != 0) {
		//printf("%s\n", leido);

		leido = readline("> ");

		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
	}

	enviar_paquete(paquete, conexion);

	// Liberar las líneas y el paquete antes de regresar
	free(leido);

	eliminar_paquete(paquete);

	//return;
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	
    // Destruir logger
    log_destroy(logger);

	// Destruir config
    config_destroy(config);

	//Liberar conexion
	liberar_conexion(conexion);
}

/*void leer_config(t_config* config, char** ip, char** puerto, char** valor) {
    *ip = config_get_string_value(config, "IP");
    if (*ip == NULL) {
        printf("Error al obtener el valor de IP del config.\n");
        exit(1);
    }

    *puerto = config_get_string_value(config, "PUERTO");
    if (*puerto == NULL) {
        printf("Error al obtener el valor de PUERTO del config.\n");
        exit(1);
    }

    *valor = config_get_string_value(config, "CLAVE");
    if (*valor == NULL) {
        printf("Error al obtener el valor de VALOR/CLAVE del config.\n");
        exit(1);
    }

	return;
}

void loggear_config(t_config* config, t_log* logger, char* ip, char* puerto, char* valor)
{
	char* clave = config_get_string_value(config, "CLAVE");
	if (clave == NULL) {
		log_error(logger, "Error al obtener el key CLAVE del config.");
        exit(1);
    }

	log_info(logger, "Valor de la key CLAVE: %s", valor);
	log_info(logger, "IP: %s", ip);
    log_info(logger, "Puerto: %s", puerto);
    log_info(logger, "Valor/Clave: %s", valor);

	return;
}*/