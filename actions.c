#include "parse_json.h"

int	ft_travel_to_id(unsigned long id, unsigned long x, unsigned long y)
{
	t_action_travel	**actions = &game.actions.travels;
	unsigned int	*count = &game.actions.travels_count;

	if (!*actions)
	{
		*actions = malloc(sizeof(t_action_travel) * 2);
		if (!*actions)
		{
			LOG_ERR("Error allocating actions");
			return 1;
		}
		*count = 0;
	}
	else
	{
		*actions = realloc(*actions, sizeof(t_action_travel) * (*count + 2));
		if (!*actions)
		{
			LOG_ERR("Error reallocating actions");
			return 1;
		}
	}
	(*actions)[*count + 1].id = 0;

	(*actions)[*count].id = id;
	(*actions)[*count].is_vector = false;
	(*actions)[*count].x = x;
	(*actions)[*count].y = y;
	(*count)++;

	return 0;
}

int	ft_travel_to(t_obj *unit, unsigned long x, unsigned long y)
{
	if (unit == NULL)
	{
		LOG_ERR("Error: unit or target == NULL");
		return 1;
	}
	if (unit->type != OBJ_UNIT)
	{
		LOG_ERR("OBJ is not type of UNIT");
		return 1;
	}
	if (ft_travel_to_id(unit->id, x, y))
		return 1;
	return 0;
}

int	ft_travel_to_id_obj(unsigned long id, t_obj *target)
{
	if (ft_travel_to_id(id, target->x, target->y))
		return 1;

	return 0;
}

int	ft_travel_to_obj(t_obj *unit, t_obj *target)
{
	if (unit == NULL || target == NULL)
	{
		LOG_ERR("Error: unit or target == NULL");
		return 1;
	}
	if (unit->type != OBJ_UNIT)
	{
		LOG_ERR("OBJ is not type of UNIT");
		return 1;
	}
	if (ft_travel_to(unit, target->x, target->y))
		return 1;

	return 0;
}

int	ft_travel_dir_id(unsigned long id, double x, double y)
{
	t_action_travel	**actions = &game.actions.travels;
	unsigned int	*count = &game.actions.travels_count;

	long x_long = x;
	if (x < 0)
		x_long = 1000 * x;
	long y_long = y;
	if (y < 0)
		y_long = 1000 * y;

	if (!*actions)
	{
		*actions = malloc(sizeof(t_action_travel) * 2);
		if (!*actions)
		{
			LOG_ERR("Error allocating actions");
			return 1;
		}
		*count = 0;
	}
	else
	{
		*actions = realloc(*actions, sizeof(t_action_travel) * (*count + 2));
		if (!*actions)
		{
			LOG_ERR("Error reallocating actions");
			return 1;
		}
	}
	(*actions)[*count + 1].id = 0;

	(*actions)[*count].id = id;
	(*actions)[*count].is_vector = true;
	(*actions)[*count].x = x_long;
	(*actions)[*count].y = y_long;
	(*count)++;

	return 0;
}

int	ft_travel_dir(t_obj *unit, double x, double y)
{
	if (unit == NULL)
		return 1;
	if (unit->type != OBJ_UNIT)
	{
		LOG_ERR("OBJ is not type of UNIT");
		return 1;
	}
	if (ft_travel_dir_id(unit->id, x, y))
		return 1;

	return 0;
}

t_obj	*ft_create_unit(t_unit_type type_id)
{
	int unit_count = 0;
	t_action_create	**actions;
	unsigned int	*count;


	while ((int)game.config.units[unit_count].type_id != 0)
		unit_count++;
	if ((int)type_id < 1 || (int)type_id > unit_count)
		return NULL;
	if (game.config.units[type_id - 1].cost > ft_get_my_team()->balance)
		return NULL;

	actions = &game.actions.creates;
	count = &game.actions.creates_count;

	if (!*actions)
	{
		*actions = malloc(sizeof(t_action_create) * 2);
		if (!*actions)
		{
			LOG_ERR("Error allocating actions");
			return NULL;
		}
		*count = 0;
	}
	else
	{
		*actions = realloc(*actions, sizeof(t_action_create) * (*count + 2));
		if (!*actions)
		{
			LOG_ERR("Error reallocating actions");
			return NULL;
		}
	}
	(*actions)[*count + 1].type_id = 0;

	(*actions)[*count].type_id = type_id;
	(*count)++;

	t_obj *newUnit = malloc(sizeof(t_obj));

	if (!newUnit)
		return (NULL);
	newUnit->s_unit.type_id = type_id;
	newUnit->s_unit.team_id = game.my_team_id;
	newUnit->type = OBJ_UNIT;
	newUnit->id = 0;
	newUnit->state = STATE_UNINITIALIZED;
	newUnit->data = NULL;

	int unitsLen = 0;
	while (game.units[unitsLen])
		unitsLen++;
	game.units = realloc(game.units, sizeof(t_obj *) * (unitsLen + 2));
	if (!game.units)
	{
		free(newUnit);
		LOG_ERR("Error reallocating game.units");
		return NULL;
	}
	game.units[unitsLen] = newUnit;
	game.units[unitsLen + 1] = NULL;

	return newUnit;
}

int	ft_attack_id(unsigned long attacker_id, unsigned long target_id)
{
	t_action_attack	**actions = &game.actions.attacks;
	unsigned int	*count = &game.actions.attacks_count;

	if (!*actions)
	{
		*actions = malloc(sizeof(t_action_attack) * 2);
		if (!*actions)
		{
			LOG_ERR("Error allocating actions");
			return 1;
		}
		*count = 0;
	}
	else
	{
		*actions = realloc(*actions, sizeof(t_action_attack) * (*count + 2));
		if (!*actions)
		{
			LOG_ERR("Error reallocating actions");
			return 1;
		}

	}
	(*actions)[*count + 1].attacker_id = 0;

	(*actions)[*count].attacker_id = attacker_id;
	(*actions)[*count].target_id = target_id;
	(*count)++;

	return 0;
}

int	ft_attack(t_obj *attacker_unit, t_obj *target_obj)
{
	if (attacker_unit == NULL || target_obj == NULL)
		return 1;
	if (attacker_unit->type != OBJ_UNIT)
	{
		LOG_ERR("Attacker OBJ is not type of UNIT");
		return 1;

	}
	if (ft_attack_id(attacker_unit->id, target_obj->id))
		return 1;

	return 0;
}

int	ft_travel_attack(t_obj *attacker_unit, t_obj *attack_obj)
{
	if (ft_travel_to_obj(attacker_unit, attack_obj))
		return 1;
	if (ft_attack(attacker_unit, attack_obj))
		return 1;

	return 0;
}
