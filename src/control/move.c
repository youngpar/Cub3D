#include "../inc/cub3d.h"
#include "../mlxdir/mlx.h"

bool	is_wall(t_cub *cub, double nx, double ny)
{
	if ((nx < 0) || (nx >= cub->parse.map.width)
		|| (ny < 0) || (ny >= cub->parse.map.height))
		return (true);
	return (cub->parse.map.map[(int)round(ny)][(int)round(nx)] == '1');
}

void	move(t_cub *cub)
{
	double	nx;
	double	ny;
	double	mx;
	double	my;

	nx = cub->user.x;
	mx = cos(cub->user.radian) * (ROTANGLE);// * SPEED;
	ny = cub->user.y;
	my = sin(cub->user.radian) * (ROTANGLE);// * SPEED;
	if (cub->key.w)
	{
		ny -= my;
		nx -= mx;
	}
	if (cub->key.s)
	{
		ny += my;
		nx += mx;
	}
	if (cub->key.a)
		nx -= mx;
	if (cub->key.d)
		nx += mx;
	if (!is_wall(cub, nx, ny))
	{
		cub->user.x = nx;
		cub->user.y = ny;
	}
	else if (!is_wall(cub, nx, cub->user.y))
		cub->user.x = nx;
	else if (!is_wall(cub, cub->user.x, ny))
		cub->user.y = ny;
	printf("%lf %lf\n", mx, my);	
}

void	rotate(t_cub *cub)
{
	if (cub->key.larw)
		cub->user.radian -= ROTANGLE;
	if (cub->key.rarw)
		cub->user.radian += ROTANGLE;
	if (cub->user.radian < 0)
		cub->user.radian += M_PI * 2;
	if (cub->user.radian > M_PI * 2)
		cub->user.radian -= M_PI * 2;
}