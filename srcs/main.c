#include "my.h"

void	ft_putnbr(int x)
{
	if (x < 0)
	{
		write(1, "-", 1);
		x = -x;
	}
	if (x > 9)
	{
		ft_putnbr(x / 10);
		ft_putnbr(x % 10);
	}
	else
	{
		x = x + '0';
		write(1, &x, 1);
	}
}

int		ft_atoi(const char * const str)
{
	long int	ret;
	int			neg;
	int			count;

	count = 0;
	ret = 0;
	neg = 1;
	while (str[count] == '-')
	{
		neg *= -1;
		count++;
	}
	while (str[count] == '+')
	{
		neg = 1;
		count++;
	}
	while (str[count])
	{
		ret *= 10;
		ret += str[count] - '0';
		count++;
	}
	return ((ret > (INT_MAX)) ? -42: (ret * neg));
}

size_t	ft_strlen(const char * const str)
{
	size_t	count;

	count = -1;
	while (str[++count])
		;
	return (count);
}

void	ft_putstr(const char * const str)
{
	write(1, str, ft_strlen(str));
}

void	ft_error(const char * const msg)
{
	write(2, msg, ft_strlen(msg));
}

char	is_num(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

char	is_nums(char *str)
{
	size_t	count;

	count = 0;
	while (str[count] == '+' || str[count] == '-')
		count++;
	while (str[count])
	{
		if (!(is_num(str[count])))
			return (0);
		count++;
	}
	return (1);
}

char	is_good(char **av)
{
	size_t	count;

	count = 0;
	while (av[++count])
	{
		if (!(is_nums(&av[count][1])))
			return (0);
	}
	return (1);
}

t_list	*create_list(char **av)
{
	t_list	*a;
	size_t	count;
	t_list	*s;

	if (!(a = malloc(sizeof(*a))))
		return (0);
	count = 0;
	s = a;
	while (av[++count])
	{
		a->content = ft_atoi(av[count]);
		if (*av[count] == '-' && a->content > 0)
			return (NULL);
		//if (is_num(*av[count]) && a->content <= 0)
		//	return (NULL);
		if (av[count + 1])
		{
			if (!(a->next = malloc(sizeof(*a))))
				return (0);
			a = a->next;
		}
	}
	a->next = NULL;
	return (s);
}

void	free_list(t_list *a)
{
	t_list	*tmp;

	while (a)
	{
		tmp = a;
		a = a->next;
		free(tmp);
		tmp = NULL;
	}
	free(a);
	a = NULL;
}

void	print_list(t_list *a)
{
	t_list	*tmp;

	tmp = a;
	while (tmp)
	{
		ft_putnbr(tmp->content);
		write(1, (!(tmp->next)) ? "\n" : " ", 1);
		tmp = tmp->next;
	}
}

char	swap(t_list **a)
{
	t_list	*tmp;

	if (*a && (*a)->next)
	{
		tmp = (*a);
		(*a) = (*a)->next;
		tmp->next = (*a)->next;
		(*a)->next = tmp;
	}
	return (1);
}

char	sa(t_list **a, t_op **op)
{
	(*op)->opc = OPSA;
	return (swap(a));
}

char	sb(t_list **b, t_op **op)
{
	(*op)->opc = OPSB;
	return (swap(b));
}

char	ss(t_list **a, t_list **b, t_op **op)
{
	if (swap(a) && swap(b))
	{
		(*op)->opc = OPSS;
		return (1);
	}
	return (0);
}

char	push(t_list **a, t_list **b)
{
	t_list	*tmp;

	tmp = *a;
	*a = *b;
	*b = (*b)->next;
	(*a)->next = tmp;
	return (1);
}

char	pa(t_list **a, t_list **b, t_op **op)
{
	(*op)->opc = OPPA;
	return (push(a, b));
}

char	pb(t_list **a, t_list **b, t_op **op)
{
	(*op)->opc = OPPB;
	return (push(b, a));
}

char	to_end(t_list **a)
{
	t_list	*tmp;

	tmp = *a;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = (*a);
	*a = (*a)->next;
	tmp->next->next = NULL;
	return (1);
}

char	ra(t_list **a, t_op **op)
{
	(*op)->opc = OPRA;
	return (to_end(a));
}

char	rb(t_list **b, t_op **op)
{
	(*op)->opc = OPRB;
	return (to_end(b));
}

char	rr(t_list **a, t_list **b, t_op **op)
{
	if (rb(b, op) && ra(a, op))
	{
		(*op)->opc = OPRR;
		return (1);
	}
	return (0);
}

char	to_first(t_list **ptr)
{
	t_list	*tmp;
	t_list	*save;

	tmp = *ptr;
	save = *ptr;
	while (tmp->next && tmp->next->next)
		tmp = tmp->next;
	*ptr = tmp->next;
	(*ptr)->next = save;
	tmp->next = NULL;
	return (1);
}

char	rra(t_list **a, t_op **op)
{
	(*op)->opc = OPRRA;
	return (to_first(a));
}

char	rrb(t_list **b, t_op **op)
{
	(*op)->opc = OPRRB;
	return (to_first(b));
}

char	rrr(t_list **a, t_list **b, t_op **op)
{
	if (rra(a, op) && rrb(b, op))
	{
		(*op)->opc = OPRRR;
		return (1);
	}
	return (0);
}

void	print_op(t_op *op, char mask)
{
	const char			ops[][3] = {"", "sa", "sb", "ss", "pa", "pb", "ra", "rb", "rra", "rrb"};
	const char * const	col = "\033[0;32m";
	const char * const	end = "\033[0;00m";
	t_op				*tmp;

	tmp = op;
	while (tmp->next)
	{
		if (OPTION_C(mask))
			write(1, col, ft_strlen(col));
		write(1, ops[tmp->opc], ft_strlen(ops[tmp->opc]));
		write(1, (!(tmp->next->next) ? "\n" : " "), 1);
		tmp = tmp->next;
	}
	if (OPTION_C(mask))
		write(1, end, ft_strlen(end));
}

char	push_swap(t_list *a, t_list *b, char mask)
{
	t_list	*tmp;
	t_list	*min;
	t_op	*op;
	t_op	*s;

	if (!(op = malloc(sizeof(*op))))
		return (0);
	s = op;
	while (a)
	{
		tmp = a;
		min = tmp;
		while (tmp)
		{
			if (tmp->content < min->content)
				min = tmp;
			tmp = tmp->next;
		}
		while (a != min)
		{
			ra(&a, &op);
			if (!(op->next = malloc(sizeof(*op))))
				return (0);
			op = op->next;
		}
		pb(&a, &b, &op);
		if (!(op->next = malloc(sizeof(*op))))
			return (0);
		op = op->next;
	}
	while (b)
	{
		pa(&a, &b, &op);
		if (!(op->next = malloc(sizeof(*op))))
			return (0);
		op = op->next;
	}
	op->next = NULL;
	print_op(s, mask);
	return (1);
}

char	mask_arg(char **av, char *options)
{
	size_t	count;
	char	mask;

	mask = 0;
	count = -1;
	while (*(av + ++count))
	{
		if (*(*(av + count)) == '-')
			mask |= ft_mask(*(av + count) + 1, options);
	}
	if (mask == -1)
	{
		ft_error("Error\n");
		return (-1);
	}
	return (mask);
}

char	ft_mask(char *str, const char *ref)
{
	char ret;
	char mod;
	char count;
	char counter;

	ret = -1;
	count = -1;
	if ((is_nums(str)))
		return (0);
	else
		return (-1);
	while (*(str + ++count))
	{
		if (ret == -1)
			ret = 0;
		counter = -1;
		mod = 1;
		while (*(ref + ++counter) && *(ref + counter) != *(str + count))
			mod *= 2;
		if (!(*(ref + counter)))
			return (-1);
		ret |= mod;
	}
	return (ret);
}

int		main(int ac, char **av)
{
	t_list	*a;
	t_list	*b;
	char	mask;

	b = NULL;
	mask = 0;
	if (av[ac - 1][0] == '-' && av[ac - 1][1] == 'c' && av[ac - 1][2] == '\0')
	{
		mask |= 1;
		av[ac - 1] = NULL;
	}
	if (!(is_good(av)))
	{
		ft_error("Error\n");
		return (0);
	}
	if (ac == 1)
	{
		ft_error("Error\n");
		return (ac);
	}
	if (!(a = create_list(av)))
	{
		ft_error("Error\n");
		return (0);
	}
	if (!(push_swap(a, b, mask)))
		return (0);
	free_list(a);
	return (0);
}
