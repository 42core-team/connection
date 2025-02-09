#include "socket.h"
#include "parse_json.h"

int		socket_fd = -1;
bool	debug = false;
t_game	game;

bool	ft_receive_config()
{
	char	*msg = ft_read_socket_once(socket_fd);

	if (!msg)
	{
		printf("Something went very awry and there was no json received.\n");
		return false;
	}
	if (debug)
		printf("Received: %s\n", msg);
	ft_parse_json_config(msg);
	if (debug)
		ft_print_game_config(&game.config);
	free(msg);
	return true;
}

void	ft_init_game()
{
	game.cores = NULL;
	game.resources = NULL;
	game.units = NULL;
	game.walls = NULL;
	game.config.units = NULL;
}

/**
 * @brief Starts the connection to the server. This function should be called before any other function from this library.
 *
 * @param team_name The name of your team.
 * @param argc The argc from the main function.
 * @param argv The argv from the main function.
 */
void	ft_init_con(char *team_name, int *argc, char **argv)
{
	const char *env_ip = getenv("SERVER_IP");
	const char *env_port = getenv("SERVER_PORT");
	const int port = env_port ? atoi(env_port) : 4242;
	game.my_team_id = argv[1] ? atoi(argv[1]) : 0;

	socket_fd = ft_init_socket(ft_init_addr(env_ip ? env_ip : "127.0.0.1", port));

	char *login_msg = ft_create_login_msg(team_name, argc, argv);
	ft_send_socket(socket_fd, login_msg);
	free(login_msg);

	ft_init_game();
	ft_reset_actions();
	bool validConf = ft_receive_config();

	if (!validConf)
	{
		printf("Unable to initialize server connection, shutting down.\n");
		exit(1);
	}
	printf("Game started! Your id: %ld\n", game.my_team_id);
}

/// @brief Closes the connection to the server and frees all allocated memory.
void	ft_close_con()
{
	close(socket_fd);
	ft_free_all();
}

/// @brief Enables debug mode, which prints all sent and received messages.
void	ft_enable_debug()
{
	debug = true;
}

/**
 * @brief Starts the main loop that sends and receives messages from the server. This function should be called after ft_init_con.
 *
 * @param ft_init_func Your own function that is called once at the start of the game.
 * @param ft_user_loop Your own function that is called every time new data is received.
 * @param ptr A pointer that is passed to your functions.
 */
void	ft_loop(void (*ft_init_func)(void *ptr), void (*ft_user_loop)(void *ptr), void (*ft_on_exit)(void *ptr), void *ptr)
{
	char	*msg;
	char	*actions;

	while (game.status != STATUS_END)
	{
		actions = ft_all_action_json();
		ft_reset_actions();
		if (debug)
			printf("Actions: %s\n", actions);
		ft_send_socket(socket_fd, actions);
		free(actions);

		msg = ft_read_socket(socket_fd);
		if (!msg)
		{
			printf("Something went very awry and there was no json received.\n");
			continue;
		}
		if (debug)
		{
			json_node *node = string_to_json(msg);
			printf("Received: %s\n", json_to_formatted_string(node));
			free_json(node);
		}

		ft_parse_json_state(msg);
		free(msg);
		if (game.status == STATUS_END)
			break;

		if (ft_init_func != NULL) {
			ft_init_func(ptr);
			ft_init_func = NULL;
			continue;
		}

		if (ft_user_loop)
			ft_user_loop(ptr);
		else printf("No user loop function provided!\n");
	}

	if (ft_on_exit)
		ft_on_exit(ptr);

	if (ft_get_my_core())
		printf("Game over! You won!\n");
	else
		printf("Game over! You lost!\n");
}
