/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgrochow <staafnet@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 00:29:06 by rgrochow          #+#    #+#             */
/*   Updated: 2024/09/20 01:17:02 by rgrochow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_value;

static void	check_args(int argc, char **argv)
{
	if (argc != 1)
	{
		ft_putstr_fd(E_USAGE, 2);
		ft_putstr_fd(argv[0], 2);
		ft_putchar_fd('\n', 2);
		exit(EC_ARG_E);
	}
}

void	show_status(t_shell *shell)
{
	if (shell->exit_status == 0)
		printf("\033[42m" "  " "\033[0m");
	else
		printf("\033[41m" "  " "\033[0m");
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	check_args(argc, argv);
	get_envp_cpy(&shell, envp);
	shell.exit_status = 0;
	while (1)
	{
		init_shell(&shell);
		signal_handlers();
		shell.prompt = build_prompt();
		show_status(&shell);
		shell.input = readline(shell.prompt);
		signal_handlers_interactive();
		free(shell.prompt);
		if (shell.input == NULL)
			handle_eof(&shell);
		if (shell.input && *shell.input)
			add_history(shell.input);
		if (parse_input(&shell, shell.input) == SUCCESS)
			exec_cmds(&shell);
		clean_shell_within_rl(&shell);
		free(shell.input);
	}
	return (0);
}
