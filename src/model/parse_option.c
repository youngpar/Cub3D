/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_option.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youngpar <youngseo321@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/28 20:33:09 by youngpar          #+#    #+#             */
/*   Updated: 2022/09/20 20:01:45 by kyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/structure.h"
#include "../../inc/parse.h"
#include "../../inc/util.h"
#include "../../libft/libft.h"

static t_options	set_opt(char *line)
{
	if (!ft_strncmp(line, "NO", 3))
		return (NORTH);
	else if (!ft_strncmp(line, "SO", 3))
		return (SOUTH);
	else if (!ft_strncmp(line, "WE", 3))
		return (WEST);
	else if (!ft_strncmp(line, "EA", 3))
		return (EAST);
	return (UNKNOWN);
}

static void	atocolor(char *str, int pos, t_option *opt)
{
	int		uc;

	if (pos < 0)
		return ;
	uc = 0;
	while (*str == ' ')
		str++;
	while (ft_isdigit(*str))
	{
		uc = uc * 10 + *str++ - '0';
		if (uc > 255)
			return ;
	}
	opt->color |= (uc << pos);
	while (ft_isspace(*str))
		str++;
	if (!pos && !*str)
		opt->valided = true;
	else if (*str == ',')
		atocolor(str + 1, pos - 8, opt);
}

void	set_color(char *line, t_option *opt)
{
	t_option	*op;

	if (!ft_strncmp(line, "C ", 2))
		op = &opt[CEIL];
	else if (!ft_strncmp(line, "F ", 2))
		op = &opt[FLOOR];
	else
		exit(1);
	if (op->parsed)
	{
		op->valided = false;
		return ;
	}
	op->parsed = true;
	if (*(++line) != ' ')
	{
		op->valided = false;
		return ;
	}
	atocolor(line, 16, op);
}

static	void	chk_file(t_option *opt, char *route, t_uint idx, void *mlx)
{
	int	fd;

	fd = open(route, O_RDONLY);
	if (fd < 0)
		exit_msg("invalid file path");
	close(fd);
	opt[idx].parsed = true;
	opt[idx].img.ptr = mlx_xpm_file_to_image(mlx, route,
			&opt[idx].width, &opt[idx].height);
	if (!opt[idx].img.ptr)
		exit_msg("invalid xpm file");
	opt[idx].img.data = mlx_get_data_addr(opt[idx].img.ptr,
			&opt[idx].img.bpp, &opt[idx].img.lsz, &opt[idx].img.endian);
}

void	set_path(char *line, t_option *opt, void *mlx)
{
	t_uint	sz;
	t_uint	idx;
	char	**arg;

	arg = ft_split(line, ' ');
	sz = 0;
	while (arg[sz])
		sz++;
	if (sz != 2 || (set_opt(arg[0]) == UNKNOWN))
		exit_msg("Wrong Identity");
	idx = set_opt(arg[0]);
	if (opt[idx].parsed)
		exit_msg("Redefined Resource Path");
	chk_file(opt, arg[1], idx, mlx);
	sz = -1;
	while (arg[++sz])
		free(arg[sz]);
	free(arg);
	if (opt[idx].img.ptr)
		opt[idx].valided = true;
}
