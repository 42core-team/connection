#include "con_lib.h"

t_obj	*ft_get_nearest_resource(t_obj *unit)
{
	int		ind = 0;
	long	min_dist = __LONG_MAX__;
	long	dist;
	t_obj	*nearest = NULL;

	if (!game.resources)
		return (NULL);

	while (game.resources[ind] != NULL)
	{
		if (game.resources[ind]->state != STATE_ALIVE)
		{
			ind++;
			continue;
		}

		dist = ft_distance(unit, game.resources[ind]);

		if (dist < min_dist)
		{
			nearest = game.resources[ind];
			min_dist = dist;
		}
		ind++;
	}

	return (nearest);
}

t_obj **ft_get_resources(void)
{
	// loop through and count alive resources, then put them all in a new array
	int		ind = 0;
	int		count = 0;

	if (!game.resources)
		return (NULL);
	while (game.resources[ind] != NULL)
	{
		if (game.resources[ind]->state == STATE_ALIVE)
			count++;
		ind++;
	}
	t_obj **alive_resources = malloc(sizeof(t_obj *) * (count + 1));
	if (!alive_resources)
		return (NULL);
	alive_resources[count] = NULL;
	ind = 0;
	count = 0;
	while (game.resources[ind] != NULL)
	{
		if (game.resources[ind]->state == STATE_ALIVE)
		{
			alive_resources[count] = game.resources[ind];
			count++;
		}
		ind++;
	}
	return (alive_resources);
}
