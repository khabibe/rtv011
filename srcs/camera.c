/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera->c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikrkharb <ikrkharb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/02 20:15:26 by ikrkharb          #+#    #+#             */
/*   Updated: 2020/01/02 23:07:34 by ikrkharb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/rtv1.h"

float		get_dist(t_vec eye, t_vec look_at)
{
	float 	result;
	float	a;
	float	b;
	float	c;

	a = pow((look_at.x - eye.x), 2);
	b = pow((look_at.y - eye.y), 2);
	c = pow((look_at.z - eye.z), 2);
	result = sqrtf(a + b + c);
	return (result);
}

t_camera	ft_create_cam(t_vec eye, t_vec look_at, float fov)
{
	t_camera 	camera;
	t_vec		up;
	float		dist;

	dist 					= get_dist(look_at, eye);
	up 						= (t_vec){0.0, 1.0, 0.0};
	camera.fov 				= DEG_TO_RAD(fov);
	camera.eye 				= eye;
	camera.look_at			= look_at;
	camera.view_dir 		= vec_normalize(vec_sub(look_at, eye));
	camera.u 				= vec_normalize(vec_cross(camera.view_dir, up));
	camera.u 				= vec_kscale(-1,camera.u);
	camera.v 				= vec_normalize(vec_cross(camera.u, camera.view_dir));
	camera.v 				= vec_kscale(-1,camera.v);
	camera.h_height 		= tan(camera.fov / 2.0) * 2.0 * dist;
	camera.aspect_ratio 	= (float)WIDTH / (float)HEIGHT;
	camera.h_width 			= camera.aspect_ratio * camera.h_height;
	return (camera);
}

t_ray generate_ray(t_camera *camera, int i, int j)
{
    t_ray ray;
    float deltax;
    float deltay;
    float x;
    float y;

    ray.origin = camera->eye;
    deltax = camera->h_height / HEIGHT;
    deltay = deltax;
    if (i >= 0 && i <= HEIGHT / 2)
        x = (i - HEIGHT / 2) * deltax;
    if (i > HEIGHT / 2) 
		x = -(camera->h_height / 2) + i * deltax;
    if (j >= 0 && j <= HEIGHT / 2)
		y = -(camera->h_height / 2) + j * deltay;
    if (j > HEIGHT / 2)
        y = (j - HEIGHT / 2) * deltay;
    ray.dir = vec_sum(vec_kscale(x, camera->u), vec_kscale(y, camera->v));
    ray.dir = vec_sum(ray.dir, camera->view_dir);
    ray.dir = vec_sum(ray.dir, camera->look_at);
	ray.dir = vec_sub(ray.dir, ray.origin);
    ray.dir = vec_normalize(ray.dir);
    return(ray);
}