#include "monty.h"

/**
 * read_f - open and reads file
 * @file_name: file name
 * Return: file file content
 */

char *read_f(char *file_name)
{
	FILE *fp;
	char *buffer;
	long int buf_size;
	struct stat st;

	if (stat(file_name, &st) == 0)
		buf_size = st.st_size;

	buffer = malloc(sizeof(char) * buf_size);
	if (!buffer)
	{
		perror("Error: malloc failed");
		exit(EXIT_FAILURE);
	}

	fp = fopen(file_name, "r");
	if (!fp)
	{
		fprintf(stderr, "Error: Can't open file %s\n", file_name);
		exit(EXIT_FAILURE);
	}
	fread(buffer, sizeof(char), buf_size, fp);

	fclose(fp);
	return (buffer);
}

/**
 * parse_f - parse buffer
 * @file_name:file name
 * Return: file file content
 */

char **parse_f(char *file_name)
{
	char **commands;
	long int buf_size;
	char *buffer;
	struct stat st;
	int i = 0;

	if (stat(file_name, &st) == 0)
		buf_size = st.st_size;

	buffer = malloc(sizeof(char) * buf_size);
	commands = malloc(sizeof(char *) * buf_size);
	buffer = read_f(file_name);

	commands[i] = strtok(buffer, " \n");
	while (commands[i] != NULL)
	{
		i++;
		commands[i] = strtok(NULL, " \n");
	}
	free(buffer);
	return (commands);
}

/**
 * add_to_stack - change stack
 * @file_name: file name
 * Return: 1 at success
 */

int add_to_stack(char *file_name)
{
	stack_t **head;
	struct stat st;
	long int buf_size;
	char **commands;
	unsigned int i = 0;

	if (stat(file_name, &st) == 0)
		buf_size = st.st_size;

	head = malloc(sizeof(char) * buf_size);
	commands = malloc(sizeof(char *) * buf_size);
	commands = parse_f(file_name);
	while (commands[i] != NULL)
	{
		if (strcmp(commands[i], "push") == 0)
		{
			if (atoi(commands[i + 1]) != 0)
				push(head, atoi(commands[i + 1]));
			else
			{
				fprintf(stderr, "L%d: usage: push integer\n", i);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			get_op_func(commands[i], head, i);
		}
		i++;
	}
	free_dlistint(*head);
	free(commands);
	return (1);
}

/**
 * get_op_func - get functions
 * @s: command
 * @head: top of stack
 * @n: line_number
 */

void get_op_func(char *s, stack_t **head, unsigned int n)
{
	instruction_t ops[] = {
		{"pall", pall},
		{"pint", _pint},
		{NULL, NULL}
	};
	int i;

	i = 0;
	while (ops[i].opcode != NULL)
	{
		if (strcmp(ops[i].opcode, s) == 0)
			ops[i].f(head, n);
		i++;
	}
}
