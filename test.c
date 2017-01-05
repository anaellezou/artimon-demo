#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include "artimon.h"

# define N_SENSORS 6 
# define BUFF_SIZE 1024

enum	e_sensor
{
	GLOBAL_TIME,
	HEART_RATE,
	BODY_TEMP,
	GLUCOSE,
	HYDRATION,
	FLEXION,	
};

typedef struct			s_artimon
{
	int			nb_atoms;
	int			nb_sig;
	int			nb_main;
	int			nb_warn;
	unsigned int 		sigtab[2];
	double			*hazard_detect_table;
	double			*main_reset;
	double			*warn_value_tab;
}				t_artimon;

typedef struct			s_socket
{
	int			socket_desc;
	int			client_socket_desc;
	int			bytes_read;
	socklen_t		addr_len;
	struct sockaddr_in	server;
}				t_socket;

typedef struct			s_readtools
{
	char			buffer[BUFF_SIZE];
	char			*token[N_SENSORS];
	int			bytes_read;
}				t_readtools;

// a container to treate sensor array data as first class memory
typedef struct			s_sensor
{
	double			array[N_SENSORS];
}				t_sensor;

void	error_exit(const char *message, int line)
{
	printf("%d : %s\n", line, message);
	exit(1);
}

void	set_socket(t_socket *sock, const char *host)
{
	sock->socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (sock->socket_desc == -1)
		error_exit("Could not open socket", __LINE__);
	sock->server.sin_addr.s_addr = inet_addr(host);
	sock->server.sin_family = AF_INET;
	sock->server.sin_port = htons(50001);
	if (bind(sock->socket_desc, (struct sockaddr *)&sock->server,
	sizeof(sock->server)) < 0)
		error_exit("Could not bind port", __LINE__);
	listen(sock->socket_desc, 1);
	printf("Waiting ...\n");
	sock->addr_len = sizeof(struct sockaddr_in);
	sock->client_socket_desc =
	accept(sock->socket_desc, (struct sockaddr *)&sock->server, &sock->addr_len);
	if (sock->client_socket_desc < 0)
		error_exit("Could not accept client", __LINE__);
}

void		init_artimon(t_artimon *a)
{
	if (init_artimon_embedded_4_42(	&a->nb_atoms,
					&a->nb_sig,
					1,
					&a->nb_main,
					&a->hazard_detect_table,
					&a->main_reset,
					&a->nb_warn,
					&a->warn_value_tab))
		{
			error_exit("Could not initialize artimon", __LINE__);
		}
}

void	tokenize(t_readtools *reader)
{
	int i = 0;
	while (i < N_SENSORS)
	{
		if (i == 0)
			reader->token[i] = strtok(reader->buffer, " ");
		else
			reader->token[i] = strtok(NULL, " ");
		i++;
	}
}

void		unity_string_to_sensor_data(t_readtools *reader, t_sensor *sensor)
{
	int i = 0;
	while (i < N_SENSORS)
	{
		sensor->array[i] = (double)atof(reader->token[i]);
		i++;
	}
}

void		sensor_data_to_artimon(t_sensor *sensor, t_artimon *artimon)
{
	//global time not refreshed here
	int i = 1;
	while (i < N_SENSORS)
	{
		artimon->sigtab[0] = (* ((unsigned *) &sensor->array[i]));
		artimon->sigtab[1] = (*(((unsigned *) &sensor->array[i]) + 1));
		artimon_refresh_signal(i - 1, artimon->sigtab);
		i++;
	}
}

void		check_alerts(t_artimon *artimon)
{
	int i = 0;
	double	*warning_table = artimon->warn_value_tab;
	
	while (i < artimon->nb_warn)
	{
		if (warning_table[i])
		{
			printf("Warning #%d Active\n", i);
		}
		i++;	
	}	
}

int		main(void)
{
	t_socket		listen_unity;
	t_artimon		artimon;
	t_sensor		sensor;
	t_readtools		reader;

	set_socket(&listen_unity, "172.20.10.6");
	init_artimon(&artimon);
	while (1)
	{
		check_alerts(&artimon);
		reader.bytes_read = recv(listen_unity.client_socket_desc, reader.buffer, BUFF_SIZE, 0);
		if (reader.bytes_read > 0)
		{
			reader.buffer[reader.bytes_read] = '\0';
			tokenize(&reader);
			unity_string_to_sensor_data(&reader, &sensor);
			sensor_data_to_artimon(&sensor, &artimon);	
			artimon_refresh_time(sensor.array[GLOBAL_TIME]);
		
			printf("Global Time: %f, Heart Rate: %f\n",
			sensor.array[GLOBAL_TIME],
			sensor.array[HEART_RATE]);
		}
	}
	return (0);
}
