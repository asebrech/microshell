#include "microshell.h"

int	ft_strlen(char *str)
{
	int	i = 0;

	while (str[i])
		i++;
	return (i);
}

void	ft_fatal()
{
	write(2, "error: fatal\n", 13);
	exit (1);
}

void	ft_error(char *str)
{
		write(2, "error: cannot execute ", 22);
		write(2, str, ft_strlen(str));
		write(2, "\n", 1);
		exit (1);
}

void	ft_cd(char **av, int i, int j)
{
	av[i] = NULL;
	if (av[j + 1] == NULL || av[j + 2] != NULL)
		write(2, "error: cd: bad arguments\n", 25);
	else
	{
		if(chdir(av[j + 1]) == -1)
		{
			write(2, "error: cd: cannot change directory to ", 38);
			write(2, av[j + 1], ft_strlen(av[j + 1]));
			write(2, "\n", 1);
		}
	}
}

void	ft_point(char **av, char **env, int fdin, int i, int j)
{
	pid_t	pid;

	if (!(strcmp("cd", av[j])))
		ft_cd(av, i, j);
	else if ((pid = fork()) == 0)
	{
		if (fdin != 0)
		{
			dup2(fdin, 0);
			close(fdin);
		}
		av[i] = NULL;
		if (execve(av[j], &av[j], env) == -1);
			ft_error(av[j]);
	}
	else if (pid == -1)
		ft_fatal();
	waitpid(pid, NULL, 0);
}

void	ft_pipe(char **av, char **env, int fdin, int i, int j, int *fd)
{
	pid_t	pid;

	if ((pid = fork()) == 0)
	{
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		if (fdin != 0)
		{
			dup2(fdin, 0);
			close(fdin);
		}
		av[i] = NULL;
		if (execve(av[j], &av[j], env) == -1);
			ft_error(av[j]);
	}
	else if (pid == -1)
		ft_fatal();
	waitpid(pid, NULL, 0);
}

int	main(int ac, char **av, char **env)
{
	int	fd[2];
	int	fdin = 0;
	int	i = 1;
	int	j = 1;

	while (i < ac)
	{
		if (!(strcmp(";", av[i])))
		{
			ft_point(av, env, fdin, i, j);
			if (fdin != 0)
				close(fdin);
			fdin = 0;
			j = i + 1;
		}
		else if(!(strcmp("|", av[i])))
		{
			if (pipe(fd) == -1)
				ft_fatal();
			ft_pipe(av, env, fdin, i, j, fd);
			close(fd[1]);
			if (fdin != 0)
				close(fdin);
			fdin = fd[0];
			j = i + 1;
		}
		i++;
	}
	ft_point(av, env, fdin, i, j);
	if (fdin != 0)
		close(fdin);
	return (0);
}
