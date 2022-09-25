#include "../includes/minishell.h"

char	*process_str(char *s1, char *s2, int mode)
{
	char	*tmp;
	char	*res;

	tmp = s1;
	res = NULL;
	if (mode == 0 || (mode == 1 && s1 == NULL))
		res = ft_strdup(s2);
	else if (mode == 1)
		res = ft_strjoin(s1, s2);
	free(tmp);
	return (res);
}

void	make_line(char **remain, char **res, char **n_pos, char **buf)
{
	*n_pos = ft_strchr(*buf, '\n');
	if (*n_pos)
	{
		**n_pos = '\0';
		*res = process_str(*res, *buf, 1);
		*res = process_str(*res, "\n", 1);
		*remain = process_str(*remain, ++(*n_pos), 0);
	}
	else
		*res = process_str(*res, *buf, 1);
	free(*buf);
}

void	check_remain(char **remain, char **res, char **n_pos)
{
	if (*remain == NULL)
		return ;
	*n_pos = ft_strchr(*remain, '\n');
	if (*n_pos)
	{
		**n_pos = '\0';
		*res = ft_strdup(*remain);
		*res = process_str(*res, "\n", 1);
		*remain = process_str(*remain, ++(*n_pos), 0);
	}
	else
	{
		if (ft_strlen(*remain))
			*res = ft_strdup(*remain);
		free(*remain);
		*remain = NULL;
	}
}

char	*read_file(int fd)
{
	char		*buf;
	int			read_count;

	buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buf)
		return (NULL);
	read_count = read(fd, buf, BUFFER_SIZE);
	if (read_count <= 0)
	{
		free(buf);
		return (NULL);
	}
	buf[read_count] = '\0';
	return (buf);
}

char	*get_next_line(int fd)
{
	char		*buf;
	char		*n_pos;
	char		*res;
	static char	*remain;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	res = NULL;
	n_pos = NULL;
	check_remain(&remain, &res, &n_pos);
	while (n_pos == NULL)
	{
		buf = read_file(fd);
		if (!buf)
			break ;
		make_line(&remain, &res, &n_pos, &buf);
	}
	return (res);
}
