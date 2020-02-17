/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oderkaou <oderkaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/03 20:28:39 by ikrkharb          #+#    #+#             */
/*   Updated: 2020/02/16 22:30:56 by oderkaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rtv1.h"


t_obj_type	*ramplir(t_camera camera, t_list *objects, int i, int j, t_ray *ray)
{
	t_obj_type		*obj1;
	t_list			*tmp;

	camera = ft_create_cam(camera.eye, camera.look_at, camera.fov);
	*ray = generate_ray(&camera, i, j);
	tmp = objects;
	((t_obj_type *)tmp->content)->t = FALSE;
	obj1 = (t_obj_type *)tmp->content;
	while (tmp)
	{
		if (!(ft_strcmp(((t_obj_type*)tmp->content)->name, "sphere")))
			((t_obj_type*)tmp->content)->t = sphere(ray,(t_obj_type*)tmp->content);
		else if (!(ft_strcmp(((t_obj_type*)tmp->content)->name, "plane")))
			((t_obj_type*)tmp->content)->t = plane(ray, (t_obj_type*)tmp->content);
		else if (!(ft_strcmp(((t_obj_type*)tmp->content)->name, "cone")))
			((t_obj_type*)tmp->content)->t = cone(ray, (t_obj_type*)tmp->content);
		else if (!(ft_strcmp(((t_obj_type*)tmp->content)->name, "cylinder")))
			((t_obj_type*)tmp->content)->t = cylinder(ray, (t_obj_type*)tmp->content);
		else
			((t_obj_type *)tmp->content)->t = FALSE;
		if (((t_obj_type*)tmp->content)->t < obj1->t)
			obj1 = (t_obj_type *)tmp->content;
		tmp = tmp->next;
	}
	if (obj1->t == FALSE)
		return (NULL);
	return (obj1);
}

int		shadows(t_ray *ray, t_list *objects,t_list *lights,t_obj_type *obj)
{
	t_list			*tmp;
	t_light			*light;
	t_point 		p;
	t_ray			shadow_ray;
	t_vec			n;
	
	light = (t_light *)lights->content;
	p = vec_sum(ray->origin, vec_kscale(obj->t, ray->dir));
	shadow_ray.dir = vec_normalize(vec_sub(light->origin, p));
	n = vec_normalize(vec_sub(obj->center,p));
	shadow_ray.origin = vec_sum(p, vec_kscale(MIN_D, shadow_ray.dir));
	tmp = objects;
	while (tmp)
	{
		if (!(ft_strcmp(((t_obj_type*)tmp->content)->name, "sphere")))
			((t_obj_type*)tmp->content)->tsh = sphere(&shadow_ray,(t_obj_type*)tmp->content);
		else if (!(ft_strcmp(((t_obj_type*)tmp->content)->name, "plane")))
			((t_obj_type*)tmp->content)->tsh = plane(&shadow_ray, (t_obj_type*)tmp->content);
		else if (!(ft_strcmp(((t_obj_type*)tmp->content)->name, "cone")))
			((t_obj_type*)tmp->content)->tsh = cone(&shadow_ray, (t_obj_type*)tmp->content);
		else if (!(ft_strcmp(((t_obj_type*)tmp->content)->name, "cylinder")))
			((t_obj_type*)tmp->content)->tsh = cylinder(&shadow_ray, (t_obj_type*)tmp->content);
		if (((t_obj_type*)tmp->content)->tsh != FALSE)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

void	draw(t_mlx *mlx, t_camera cam,t_list *objects, t_list *lights)
{
	int			i;
	int			j;
	t_obj_type	*obj;
	t_ray 		ray;
	
	i = -1;
	while (++i < WIDTH)
	{
		j = -1;
		while (++j < HEIGHT)
		{
			obj = ramplir(cam, objects, i, j, &ray);
			if (obj == NULL)
			{	
				ft_mlx_pixel_put(mlx, i, j,0x00019);
				continue ;
			}
			obj->k = shadows(&ray, objects, lights, obj);
			ft_mlx_pixel_put(mlx, i, j, obj->k * phone_model(obj, &ray, lights));
			//ft_mlx_pixel_put(mlx, i, j, obj->color);
		}
	}
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
}

void create_actual_objs(t_mlx *mlx, t_list *camera, t_list *lights, t_list *objects)
{
	t_camera *cam;
	cam = (t_camera*)camera->content;
	draw(mlx, *cam, objects, lights);
}