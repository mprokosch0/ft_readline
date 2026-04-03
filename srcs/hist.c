#include "ft_readline.h"

void	static_infos(hist **head, bool mode)
{
	static hist **shead = NULL;

	if (mode)
	{
		shead = head;
		return ;
	}
	if (*shead)
		delete_hist(shead);
}

hist *new_node(void)
{
	hist	*new = calloc(sizeof(hist), 1);
	if (!new)
		return NULL;
	new->buffer = calloc(SIZE, 1);
	if (!new->buffer)
	{
		free(new);
		return NULL;
	}
	new->size = SIZE;
	new->prev = NULL;
	new->next = NULL;
	return new;
}

void	delete_hist(hist **head)
{
	while ((*head))
	{
		hist *tmp = (*head);
		(*head) = (*head)->next;
		if (tmp->buffer)
			free(tmp->buffer);
		free(tmp);
	}
}

bool	hist_add_back(hist **head, hist **tail, hist *new)
{
	if (!new)
		return 0;
	if (!(*head))
		(*head) = (*tail) = new;
	else
	{	
		(*tail)->next = new;
		new->prev = (*tail);
		(*tail) = new;
	}
	return 1;
}

bool	hist_add_front(hist **head, hist **tail, hist *new)
{
	if (!new)
		return 0;
	if (!(*head))
		(*head) = (*tail) = new;
	else
	{
		(*head)->prev = new;
		new->next = (*head);
		(*head) = new;
	}
	return 1;
}

void	move_to_last(hist **head, hist **tail, hist *node)
{
	if (node == *tail)
		return ;
	if (node == (*head))
	{
		(*head) = node->next;
		(*head)->prev = NULL;
	}
	else
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}
	(*tail)->next = node;
	node->prev = (*tail);
	node->next = NULL;
	(*tail) = node;
}

void	delete_node(hist **head, hist **tail, hist *node)
{
	if (node->buffer)
	{
		free(node->buffer);
		node->buffer = NULL;
	}
	hist *prev = node->prev, *next = node->next;
	if (prev)
	{
		if (next)
		{
			prev->next = next;
			next->prev = prev;
		}
		else
		{
			prev->next = NULL;
			(*tail) = prev;
		}
	}
	else if (next)
	{
		next->prev = NULL;
		(*head) = next;
	}
	else
	{
		(*head) = NULL;
		(*tail) = NULL;
	}
	free(node);
	node = NULL;
}