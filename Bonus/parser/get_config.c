/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_config.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moichou <moichou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 21:32:15 by moichou           #+#    #+#             */
/*   Updated: 2024/09/02 20:49:27 by moichou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

static char	**get_map(int fd, char *line, t_config **config)
{
	char	*map_str;
	char	**map;
	int		map_row;
	int		map_colmun;
	int		i;

	i = 0;
	map_row = -1;
	map_colmun = 0;
	map_str = NULL;
	while (line && ft_isemptystr(line))
		line = get_next_line(fd);
	while (line)
	{
		if (line && ft_isemptystr(line))
			return (ft_printerror(MAP_ERR), NULL);
		map_str = ft_strjoin(map_str, line);
		map_colmun++;
		if (map_row < ft_strlen(line))
			map_row = ft_strlen(line);
		line = get_next_line(fd);
		i++;
	}
	(*config)->mapx = map_row;
	(*config)->mapy = map_colmun;
	map = make_map(map_str, map_row, map_colmun);
	return (map);
}

static int	set_arg(t_config *config, char *line, int args_count)
{
	char	**args;

	args = ft_divide_str(line);
	if (args && args[0] && args[1] && ft_strcmp(args[0], "NO") == 0
		&& args_count == 1)
		config->no = args[1];
	else if (args && args[0] && args[1] && ft_strcmp(args[0], "SO") == 0
		&& args_count == 2)
		config->so = args[1];
	else if (args && args[0] && args[1] && ft_strcmp(args[0], "WE") == 0
		&& args_count == 3)
		config->we = args[1];
	else if (args && args[0] && args[1] && ft_strcmp(args[0], "EA") == 0
		&& args_count == 4)
		config->ea = args[1];
	else if (args && args[0] && args[1] && ft_strcmp(args[0], "F") == 0
		&& args_count == 5)
		config->f = ft_split(args[1], ',');
	else if (args && args[0] && args[1] && ft_strcmp(args[0], "C") == 0
		&& args_count == 6)
		config->c = ft_split(args[1], ',');
	else
		return (ft_printerror("invalid args\n"), -1);
	return (1);
}

static t_config	*get_args(t_config *config, int fd)
{ 
	char	*line;
	int		args_count;

	config = g_malloc(sizeof(t_config), ALLOCATE);
	if (!config)
		return (NULL);
	line = get_next_line(fd);
	args_count = 1;
	while (line)
	{
		if (line && !ft_isemptystr(line))
		{
			if (set_arg(config, ft_trim_spaces(line), args_count) == -1)
				return (NULL);
			args_count++;
		}
		line = get_next_line(fd);
		if (args_count == 7)
			break ;
	}
	config->map = get_map(fd, line, &config);
	if (!config->map)
		return (NULL);
	return (config);
}


/*
	=> parsing args
	=> making config
	checks file args and return a struct
	if a problem acurred it returns NULL
*/
t_config	*make_config(char *filename)
{
	int			fd;
	t_config	*config;

	config = NULL;
	fd = open(filename, O_RDONLY, 777);
	if (fd == -1)
		return (ft_printerror(OPEN_ERR), NULL);
	config = get_args(config, fd);
	if (!config || parse_map(config->map) == -1)
		return (close(fd), NULL);
	return (close(fd), config);
}
