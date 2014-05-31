#ifndef MY_H
# define MY_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>

# define OPSA	1
# define OPSB	2
# define OPSS	3
# define OPPA	4
# define OPPB	5
# define OPRA	6
# define OPRB	7
# define OPRR	13
# define OPRRA	8
# define OPRRB	9
# define OPRRR	17
# define OPTION_C(x) (x & (char) 1)
# define OPTION_V(x) (x & (char) 2)

typedef struct	s_list	t_list;
typedef struct	s_op	t_op;

struct		s_list
{
	int		content;
	t_list	*next;
};

struct		s_op
{
	int		opc;
	t_op	*next;
};

char	ft_mask(char *str, const char *ref);

#endif
