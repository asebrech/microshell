#ifndef MICROSHELL_H
# define MICROSHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>

typedef struct	s_info
{
	int 	fd1[2];
	int		fd2[2];
	char	**av;
	char	**env;
}				t_info;

#endif
