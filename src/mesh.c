#include "mesh.h"
#include "array.h"
#include "vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

mesh_t mesh = {.vertices = NULL, .faces = NULL, .rotation = {0, 0, 0}};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
	{.x = -1, .y = -1, .z = -1}, {.x = -1, .y = 1, .z = -1},
	{.x = 1, .y = 1, .z = -1},	 {.x = 1, .y = -1, .z = -1},
	{.x = 1, .y = 1, .z = 1},	 {.x = 1, .y = -1, .z = 1},
	{.x = -1, .y = 1, .z = 1},	 {.x = -1, .y = -1, .z = 1}};

face_t cube_faces[N_CUBE_FACES] = {
	{.a = 1, .b = 2, .c = 3}, {.a = 1, .b = 3, .c = 4},

	{.a = 4, .b = 3, .c = 5}, {.a = 4, .b = 5, .c = 6},

	{.a = 6, .b = 5, .c = 7}, {.a = 6, .b = 7, .c = 8},

	{.a = 8, .b = 7, .c = 2}, {.a = 8, .b = 2, .c = 1},

	{.a = 2, .b = 7, .c = 5}, {.a = 2, .b = 5, .c = 3},

	{.a = 6, .b = 8, .c = 1}, {.a = 6, .b = 1, .c = 4}};

void load_cube_mesh_data(void)
{
	for (int i = 0; i < N_CUBE_VERTICES; i++)
	{
		vec3_t cube_vertex = cube_vertices[i];
		array_push(mesh.vertices, cube_vertex);
	}
	for (int i = 0; i < N_CUBE_FACES; i++)
	{
		face_t cube_face = cube_faces[i];
		array_push(mesh.faces, cube_face);
	}
}

/*
 * Got this from
 * https://stackoverflow.com/questions/29576799/reading-an-unknown-length-line-from-stdin-in-c-with-fgets/29577105#29577105
 *
 * batch read if files get too big
 */
char *get_line(FILE *f)
{
	size_t size = 0;
	size_t len = 0;
	size_t last = 0;
	char *buf = NULL;

	do
	{
		size += BUFSIZ;
		buf = realloc(buf, size);

		if (buf == NULL)
		{
			return NULL;
		}

		fgets(buf + last, BUFSIZ, f);
		len = strlen(buf);
		last = len - 1;
	} while (!feof(f) && buf[last] != '\n');

	return buf;
}

void load_obj_file_data(char *filename)
{
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)

	{
		printf("Error: could not open file with filename %s\n", filename);
		return;
	}

	// we are going to read a file line by line, parsing as we go
	char *str = NULL;
	while (!feof(fp))
	{
		str = get_line(fp);
		if (str == NULL)
		{
			printf("get_line() failed");
			fclose(fp);
			exit(EXIT_FAILURE);
		}

		if (strncmp(str, "v ", 2) == 0)
		{
			vec3_t vertex;
			sscanf(str, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			array_push(mesh.vertices, vertex);
		}
		else if (strncmp(str, "f ", 2) == 0)
		{
			face_t face_vertex_idx;
			face_t face_texture_idx;
			face_t face_normal_idx;

			sscanf(str, "f %d/%d/%d %d/%d/%d %d/%d/%d", &face_vertex_idx.a,
				   &face_texture_idx.a, &face_normal_idx.a, &face_vertex_idx.b,
				   &face_texture_idx.b, &face_normal_idx.b, &face_vertex_idx.c,
				   &face_texture_idx.c, &face_normal_idx.c);
			array_push(mesh.faces, face_vertex_idx);
		}
		free(str);
	}

	fclose(fp);
}
