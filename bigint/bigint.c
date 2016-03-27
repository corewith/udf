#include "bigint.h"

/*
	bigint initialization
*/
BigInt_List bigint_init()
{
	BigInt_List list=(BigInt_List)(malloc(sizeof(struct bigint_list)));
	list->first = NULL;
	list->count = 0;
	//list->flag = 0;
	return list;
}
/*
	a=b;
*/
BigInt_List bigint_mov_to(BigInt_List b)
{
	BigInt_List a=bigint_init();
	BigInt_Node node = b->first;
	int i;
	for (i = 0; i < b->count; i++)
	{
		BigInt_Node temp = (BigInt_Node)malloc(sizeof(struct bigint_node));
		temp->value = node->value;
		if (a->first == NULL)
		{
			a->first = temp;
			temp->preview = temp;
			temp->next = temp;
		}
		else
		{
			BigInt_Node last = a->first->preview;
			last->next = temp;
			temp->preview = last;

			temp->next = a->first;
			a->first->preview = temp;
		}
		node = node->next;
	}
	a->count=b->count;
	return a;
}
void bigint_mov(BigInt_List a, BigInt_List b)
{
	// init
	a->first=NULL;
	a->count=0;

	BigInt_Node node = b->first;
	int i;
	for (i = 0; i < b->count; i++)
	{
		BigInt_Node temp = (BigInt_Node )malloc(sizeof(struct bigint_node));
		temp->value = node->value;
		if (a->first == NULL)
		{
			a->first = temp;
			temp->preview = temp;
			temp->next = temp;
		}
		else
		{
			BigInt_Node last = a->first->preview;
			last->next = temp;
			temp->preview = last;

			temp->next = a->first;
			a->first->preview = temp;
		}
		node = node->next;
	}
	a->count=b->count;
}
/*
	mov a value to list
*/
BigInt_List bigint_mov_value_to(int value)
{
	BigInt_List list=bigint_init();
	BigInt_Node node = (BigInt_Node)malloc(sizeof(struct bigint_node));
	node->value = value;
	node->preview = node;
	node->next = node;

	list->count = 1;
	list->first = node;
	return list;
}
void bigint_mov_value(BigInt_List list, int value)
{
	// init
	list->first=NULL;
	list->count=0;

	BigInt_Node node = (BigInt_Node)malloc(sizeof(struct bigint_node));
	node->value = value;
	node->preview = node;
	node->next = node;

	list->count = 1;
	list->first = node;
}
/*
 * input: char *s
 * output: BigInt_List
 */
BigInt_List string_to_list(const char *s)
{
	BigInt_List list=bigint_init();
	int i,k=N-1,len=strlen(s);
	char str[N+1];
	for(i=len-1;i>=-1;i--)
	{
		if(k==-1)
		{
			BigInt_Node node=(BigInt_Node)malloc(sizeof(struct bigint_node));
			node->value=atoi(str);
			if(list->first==NULL)
			{
				list->first=node;
				node->preview=node;
				node->next=node;
			}
			else
			{
				list->first->preview->next=node;
				node->preview=list->first->preview;

				node->next=list->first;
				list->first->preview=node;

				list->first=node;
			}
			list->count++;
			if(i==-1) break; // end
			k=N-1;
		}
		if(i<k){ k=i; str[k+1]='\0';}
		str[k--]=s[i];
	}
	return list;
}
/*
	input :BigInt_List
	output: char *
*/
char *list_to_string(BigInt_List list)
{
	char *s=(char *)malloc(256);
	int i,j,k=0;
	BigInt_Node node=list->first;
	for(i=0;i<list->count;i++)
	{
		char str[N+1];
		sprintf(str,"%d",node->value);
		int len=strlen(str);
		// if the length is less N(=4) bits, then supplement enough by '0', and do not consider the first one for it's MSB.
		if(i!=0 && len<N)
		{
			int t=0;
			char str_temp[N+1];
			for(j=0;j<N;j++)
			{
				if(j<N-len)	str_temp[j]='0';
				else str_temp[j]=str[t++];
			}
			for(j=0;j<N;j++) str[j]=str_temp[j];
			len=strlen(str);
		}
		for(j=0;j<len;j++)
		{
			s[k++]=str[j];
		}
		node=node->next;
	}
	s[k]='\0';
	return s;
}
/*
	compare
*/
int bigint_cmp(BigInt_List a, BigInt_List b)
{
	BigInt_Node node1 = a->first;
	BigInt_Node node2 = b->first;
	int i;

	if (a->count > b->count)	return 1;  // a>b
	if (a->count < b->count)	return -1; // a<b

	if (a->count == b->count)
	{
		for (i = 0; i < a->count; i++)
		{
			if (node1->value > node2->value)	return 1; // a>b
			if (node1->value < node2->value)	return -1;// a<b
			node1 = node1->next;
			node2 = node2->next;
		}
		return 0;
	}
	return 0;
}
/*
 * c=a+b
 */
BigInt_List bigint_add(BigInt_List a, BigInt_List b)
{
	int i, carry = 0;
	BigInt_List c=bigint_mov_to(a);
	// from end to begin
	BigInt_Node node_a = c->first->preview;
	BigInt_Node node_b = b->first->preview;
	for (i = 0; i < c->count && i < b->count; i++)
	{
		int value=node_a->value + node_b->value + carry;
		if(value<10000) carry=0;
		else
		{
			value-=10000;
			carry=1;
		}
		node_a->value=value;
		node_a = node_a->preview;
		node_b = node_b->preview;
	}
	// if c(also a) has more
	while (i < c->count && carry>0)
	{
		int value = node_a->value + carry;
		if (value < 10000) carry = 0;
		else
		{
			value-=10000;
			carry = 1;
		}
		node_a->value=value;
		node_a = node_a->preview;
		i++;
	}
	// if b has more
	while (i < b->count)
	{
		int value = node_b->value + carry;
		if (value < 10000) carry = 0;
		else
		{
			value -= 10000;
			carry = 1;
		}
		BigInt_Node node=(BigInt_Node)malloc(sizeof(struct bigint_node));
		node->value=value;

		c->first->preview->next=node;
		node->preview=c->first->preview;

		node->next=c->first;
		c->first->preview=node;

		c->first=node;
		c->count++;

		node_b = node_b->preview;
		i++;
	}
	if (carry == 1)
	{
		BigInt_Node node = (BigInt_Node)malloc(sizeof(struct bigint_node));
		node->value = carry;

		c->first->preview->next = node;
		node->preview = c->first->preview;

		node->next = c->first;
		c->first->preview = node;

		c->first = node;
		c->count++;
	}
	if (c->count > 1)
	{
		while (c->first->value == 0)
		{
			c->first->preview->next = c->first->next;
			c->first->next->preview = c->first->preview;

			c->first = c->first->next;
			c->count--;
			if (c->count == 1)
			{
				break;
			}
		}
	}
	return c;
}
/*
	c=a-b;
	note: need a>=b
*/
BigInt_List bigint_sub(BigInt_List a, BigInt_List b)
{
	int i, borrow = 0;
	BigInt_List c=bigint_mov_to(a);
	BigInt_Node node_a = c->first->preview;
	BigInt_Node node_b = b->first->preview;

	for (i = 0; i < c->count && i < b->count; i++)
	{
		int value = node_a->value - node_b->value - borrow;
		if (value >= 0) borrow = 0;
		else
		{
			value += 10000;
			borrow = 1;
		}
		node_a->value=value;
		node_a = node_a->preview;
		node_b = node_b->preview;
	}
	while (i < c->count && borrow>0)
	{
		int value = node_a->value - borrow;
		if (value >= 0) borrow = 0;
		else
		{
			value += 10000;
			borrow = 1;
		}
		node_a->value=value;
		node_a = node_a->preview;
		i++;
	}
	// those two would not happen because c>=b
	// while (i < b->count)
	//
	//if (borrow==1)

	if (c->count > 1)
	{
		while (c->first->value == 0)
		{
			c->first->preview->next = c->first->next;
			c->first->next->preview = c->first->preview;

			c->first = c->first->next;
			c->count--;
			if (c->count == 1)
			{
				break;
			}
		}
	}
	return c;
}
/*
	c=a*b;
*/
BigInt_List bigint_mul(BigInt_List a, int b)
{
	int i, carry = 0;
	BigInt_List c=bigint_mov_to(a);
	BigInt_Node node = c->first->preview;
	for (i = 0; i < c->count; i++)
	{
		int value= node->value * b + carry;
		node->value = value % 10000;
		carry = value / 10000;

		node = node->preview;
	}
	if (carry > 0)
	{
		while (carry > 0)
		{
			BigInt_Node temp = (BigInt_Node )malloc(sizeof(struct bigint_node));
			temp->value = carry % 10000;
			carry = carry / 10000;

			c->first->preview->next = temp;
			temp->preview = c->first->preview;

			temp->next = c->first;
			c->first->preview = temp;

			c->first = temp;
			c->count++;
		}
	}
	if (c->count > 1)
	{
		while (c->first->value == 0)
		{
			c->first->preview->next = c->first->next;
			c->first->next->preview = c->first->preview;

			c->first = c->first->next;
			c->count--;
			if (c->count == 1)
			{
				break;
			}
		}
	}
	return c;
}
BigInt_List bigint_multiply(BigInt_List a, BigInt_List b)
{
	int i, j, times = 0;
	BigInt_Node node_b = b->first->preview;
	BigInt_List result;
	BigInt_List temp;
	BigInt_List total;

	total=bigint_mov_value_to(0); // firstly total=0;

	for (i = 0; i < b->count; i++)
	{
		result=bigint_mul(a,node_b->value);
		if (i > 0)
		{
			times++;
		}
		for (j = 0; j < times; j++)
		{
			result=bigint_mul(result,10000);
		}
		total=bigint_add(total,result);

		node_b = node_b->preview;
	}
	if (total->count > 1)
	{
		while (total->first->value == 0)
		{
			total->first->preview->next = total->first->next;
			total->first->next->preview = total->first->preview;

			total->first = total->first->next;
			total->count--;
			if (total->count == 1)
			{
				break;
			}
		}
	}
	return total;
}
/*
	c=a/b;
*/
BigInt_List bigint_div(BigInt_List a, int b)
{
	int i, tmp, remain = 0;
	BigInt_List c=bigint_init();
	BigInt_Node node = a->first;
	for (i = 0; i < a->count; i++)
	{
		BigInt_Node temp = (BigInt_Node)malloc(sizeof(struct bigint_node));
		tmp = node->value + remain;
		temp->value = tmp / b;
		remain = (tmp%b) * 10000;
		if (c->first == NULL)
		{
			c->first = temp;
			temp->preview = temp;
			temp->next = temp;
		}
		else
		{
			BigInt_Node last = c->first->preview;
			last->next = temp;
			temp->preview = last;

			temp->next = c->first;
			c->first->preview = temp;
		}
		c->count++;
		node = node->next;
	}
	if (c->count > 1)
	{
		while (c->first->value == 0)
		{
			c->first->preview->next = c->first->next;
			c->first->next->preview = c->first->preview;

			c->first = c->first->next;
			c->count--;
			if (c->count == 1)
			{
				break;
			}
		}
	}
	return c;
}
void divide1(BigInt_List a, BigInt_List b, BigInt_List c, BigInt_List remain1)
{
	if (a->count < b->count) // a<b
	{		
		bigint_mov_value(c,0);
		bigint_mov(remain1,a);
	}
	else if (a->count == b->count)
	{
		BigInt_List remain;
		BigInt_List node;

		remain=bigint_mov_to(a);
		int flag=1,times=1,result=0;

		while(bigint_cmp(remain,b)>=0)
		{
			if(flag==1)
			{
				// get the times.
				node=bigint_mov_to(b);
				while (bigint_cmp(remain, node) > 0)
				{
					node=bigint_mul(b, times * 10);
					if (bigint_cmp(remain, node) <= 0)
					{
						break;
					}
					times *= 10;
				}
				flag++;
			}
			node=bigint_mul(b,times);
			int first=remain->first->value;
			int second=node->first->value;
			int quotient=first/second;

			BigInt_List temp=bigint_mul(node,quotient);
			while (bigint_cmp(remain, temp) < 0)
			{
				quotient -= 1;
				temp=bigint_mul(node, quotient);
			}
			int accumulate= times*quotient;
			result +=accumulate;
			// get the remainder
			node=bigint_mul(b,accumulate);
			remain=bigint_sub(remain,node);

			times /=10;
		}
		bigint_mov_value(c,result);
		// std::cout<< result << "\n";
		bigint_mov(remain1,remain);
	}
	else  // a->count > b->count
	{
		BigInt_List result;
		BigInt_List remain;
		BigInt_List times;

		result=bigint_mov_value_to(0);
		remain=bigint_mov_to(a);
		while(bigint_cmp(remain,b)>=0)
		{
			int i,flag=0;
			int len=remain->count-b->count;
			BigInt_List new_a=bigint_mov_to(remain);

			times=bigint_mov_value_to(1);
			for(i=0;i<len;i++)
			{
				times=bigint_mul(times,10000);
			}
			BigInt_List new_b=bigint_multiply(b,times);
			while (bigint_cmp(new_a, new_b) < 0)
			{
				new_b=bigint_div(new_b, 10);
				times=bigint_div(times, 10);
			}
			BigInt_List temp;
			while (bigint_cmp(new_a, new_b) > 0)
			{
				temp=bigint_mul(new_b,10);
				if (bigint_cmp(new_a, temp) < 0)
				{
					break;
				}
				new_b=bigint_mov_to(temp);
				times=bigint_mul(times, 10);
			}
			if(new_a->count!=new_b->count)
			{
				// a,b all multiply by 10
				flag=1;
				new_a=bigint_mul(new_a,10);
				new_b=bigint_mul(new_b,10);
			}
			// test
			// std::cout<< list_to_string(new_a) << "\n"; //6200123
			// std::cout<< list_to_string(new_b) << "\n"; //1000000

			BigInt_List new_c = bigint_init();
			divide1(new_a,new_b,new_c,remain);
			if(flag==1)
			{
				remain=bigint_div(remain,10);
				flag=0;
			}
			new_c=bigint_multiply(new_c,times);
			result=bigint_add(result,new_c);
			// if now a's length equals b's length, then no need to loop any more.
			if(remain->count==b->count)
			{
				new_a=bigint_mov_to(remain);
				divide1(new_a,b,new_c,remain);
				result=bigint_add(result,new_c);
				break;
			}
		}
		bigint_mov(c,result);
		bigint_mov(remain1,remain);
	}
}
BigInt_List bigint_divide(BigInt_List a, BigInt_List b)
{
	BigInt_List c =(BigInt_List)malloc(sizeof(struct bigint_list));
	BigInt_List remain =(BigInt_List)malloc(sizeof(struct bigint_list));
	divide1(a, b, c, remain);
	return c;
}
/*
 * c=a%b
 */
 BigInt_List bigint_mod(BigInt_List a,BigInt_List b)
 {
 	BigInt_List result;
 	BigInt_List node;
 	result=bigint_divide(a,b);
 	node=bigint_multiply(b,result);
 	result=bigint_sub(a,node);
 	return result;
 }
