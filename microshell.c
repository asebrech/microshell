#include "microshell.h"

int	nb_pipe(char **av)
{
	int	i = 0;
	int pipe = 0;

	while (av[i] != NULL)
	{
		if (!(strcmp(av[i], "|")))
			pipe++;
		if (pipe == 2)
			return (2);
		i++;
	}
	return (pipe);
}

int	ft_exec(char **av, char **env)
{
	int	pid;

	if (!(pid = fork()))
	{
		execve(av[0], av, env);
	}
	return (pid);
}

void	parsing(char **av, char **env, int fd0)
{
	int		i = 0;
	pid_t	pid;
	int		fd[2];

	dup2(fd0, 0);
	if (fd0 != 0)
		close(fd0);
	while (av[i] != NULL)
	{
		if (!(strcmp(av[i], ";")))
		{
			av[i] = NULL;
			if (!(pid = fork()))
				execve(av[0], av, env);
			else
				waitpid(pid, NULL, 0);
			parsing(&av[i + 1], env, 0);
		}
		else if (!(strcmp(av[i], "|")))
		{
			pipe(fd);
			av[i] = NULL;
			if (!(pid = fork()))
			{
				dup2(fd[1], 1);
				close(fd[1]);
				execve(av[0], av, env);
			}
			else
				waitpid(pid, NULL, 0);
			parsing(&av[i + 1], env, fd[0]);
			close(fd[0]);
			close(fd[1]);
		}
		i++;
	}
	execve(av[0], av, env);
}

int	main(int ac, char **av, char **env)
{
	parsing(&av[1], env, 0);
	return (0);
}
