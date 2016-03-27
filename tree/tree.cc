/*
 * tree.c
 *
 *  Created on: Mar 24, 2016
 *      Author: insider
 */
#include "tree.h"
/*
DoubleListTree initializaiton
*/
DoubleListTree tree_init()
{
	DoubleListTree tree=(DoubleListTree )malloc(sizeof(struct doublelisttree));
	tree->value = 0;
	tree->left = NULL;
	tree->right = NULL;
	tree->flag = 0;
	return tree;
}
/*
split a string to an string array
*/
void tree_split(const char *s, char c, char *strs[], int *len)
{
	const char *temp = s;
	char *temp1;
	int i = 0;
	strs[0] = (char *)malloc(sizeof(char) * LEN_ALL);
	memset(strs[0],0,LEN_ALL);
	temp1 = strs[0];
	while (*temp != '\0')
	{
		if (*temp == '#')
		{
			*temp1 = '\0';
			i++;
			strs[i] = (char *)malloc(sizeof(char) * LEN_ALL);
			memset(strs[i],0,LEN_ALL);
			temp1 = strs[i];
			temp++;
			continue;
		}
		*temp1 = *temp;
		temp1++;
		temp++;
	}
	//*len = i;
	*len=i+1;
}
/*
convert a string to a tree
*/
DoubleListTree string_to_tree(const char *s)
{
	DoubleListTree tree=tree_init();
	int point = 0, *len, root, i;
	char *strs[100], *temp;
	len = (int *)malloc(sizeof(int));

	tree_split(s, '#', strs, len);

	root = atoi(strs[0]);
	tree->value = root;
	if (root >= 193)
	{
		point = root - 193 +1;
		tree->flag = 0;
	}
	if (root <= 62)
	{
		point = 62 - root +1;
		tree->flag = 1;
	}
	for (i = 1; i <= point; i++)
	{
		temp = (char *)malloc(sizeof(char) * LEN_ALL);
		memset(temp,0,LEN_ALL);
		strcpy(temp, strs[i]);
		DoubleList node = (DoubleList )malloc(sizeof(struct doublelist));
		node->value = temp;
		if (tree->left == NULL)
		{
			tree->left = node;
			node->preview = node;
			node->next = node;
		}
		else
		{
			DoubleList last = tree->left->preview;
			last->next = node;
			node->preview = last;

			node->next = tree->left;
			tree->left->preview = node;

			tree->left = node;
		}
	}
	for (i = point + 1; i < *len; i++)
	{
		temp = (char *)malloc(sizeof(char) * LEN_ALL);
		memset(temp,0,LEN_ALL);
		strcpy(temp, strs[i]);
		DoubleList node = (DoubleList )malloc(sizeof(struct doublelist));
		node->value = temp;
		if (tree->right == NULL)
		{
			tree->right = node;
			node->preview = node;
			node->next = node;
		}
		else
		{
			DoubleList last = tree->right->preview;
			last->next = node;
			node->preview = last;

			node->next = tree->right;
			tree->right->preview = node;
		}
	}
	return tree;
}
/*
convert a tree to a string
*/
char* tree_to_string(DoubleListTree tree)
{
	char *s=(char *)malloc(sizeof(char) * LEN_ALL);
	// init
	memset(s,0,LEN_ALL);
	char *root, *str;
	root = (char *)malloc(sizeof(char) * LEN_ALL);
	// init
	memset(root,0,LEN_ALL);
	sprintf(root, "%03d", tree->value);
	strcpy(s, root);
	strcat(s, "#");
	DoubleList node = tree->left->preview;
	do
	{
		str = node->value;
		strcat(s, str);
		strcat(s, "#");
		node = node->preview;
	} while (node != tree->left->preview);
	node = tree->right;
	if(node != NULL)
	{
		do
		{
			str = node->value;
			strcat(s, str);
			strcat(s, "#");
			node = node->next;
		} while (node != tree->right);
	}
	// remove the last "#"
	memset(root,0,LEN_ALL);
	strncpy(root,s,strlen(s)-1);
	return root;
}
/*
a=b;
*/
DoubleListTree tree_mov(DoubleListTree b)
{
	DoubleListTree a=tree_init();
	DoubleList node, temp;
	a->value = b->value;
	a->flag = b->flag;
	temp = b->left;
	do
	{
		node = (DoubleList )malloc(sizeof(struct doublelist));
		node->value = temp->value;

		if (temp == b->left)
		{
			a->left = node;
			node->preview = node;
			node->next = node;
		}
		else
		{
			DoubleList last = a->left->preview;
			last->next = node;
			node->preview = last;

			node->next = a->left;
			a->left->preview = node;
		}
		temp = temp->next;
	} while (temp != b->left);
	temp = b->right;
	if(temp != NULL)
	{
		do
		{
			node = (DoubleList )malloc(sizeof(struct doublelist));
			node->value = temp->value;
			if (temp == b->right)
			{
				a->right = node;
				node->next = node;
				node->preview = node;
			}
			else
			{
				DoubleList last = a->right->preview;
				last->next = node;
				node->preview = last;

				node->next = a->right;
				a->right->preview = node;
			}
			temp = temp->next;
		} while (temp != b->right);
	}
	return a;
}
/*
 * c=(a*b)%m
 */
char* string_mul_mod(const char *a,const char *b, const char *m)
{
	BigInt_List l_a=string_to_list(a);
	BigInt_List l_b=string_to_list(b);
	BigInt_List l_m=string_to_list(m);

	l_a=bigint_multiply(l_a,l_b); // a=a*b
	l_a=bigint_mod(l_a,l_m); // a=a%m

	char *result=list_to_string(l_a);
	return result;
}
/*
c=a+b;
*/
DoubleListTree tree_add(DoubleListTree first, DoubleListTree second)
{
	DoubleListTree result = tree_mov(first);
	int root_first = result->value;
	int root_second = second->value;
	if (second->flag == 0)
	{
		result->value = root_first >= root_second ? root_first : root_second;
	}
	else
	{
		result->value = root_first <= root_second ? root_first : root_second;
	}

	DoubleList node1 = result->right;
	DoubleList node2 = second->right;
	if(node1 != NULL && node2 != NULL)
	{
		do
		{
			const char *nsquare = NSQUARE;
			char *temp_first = node1->value;
			char *temp_second = node2->value;
			char *temp_result =string_mul_mod(temp_first, temp_second, nsquare);
			node1->value = temp_result;

			node1 = node1->next;
			node2 = node2->next;
		} while (node1 != result->right && node2 != second->right);
		if (node2 != second->right)
		{
			DoubleList last = result->right->preview;
			last->next = node2;
			node2->preview = last;

			result->right->preview = second->right->preview;
			second->right->preview->next = result->right;
		}
	}
	else if(node1 == NULL && node2 != NULL)
	{
		result->right=second->right;
	}
	// do nothing for those two situations below
//	else if(node1 != NULL && node2 == NULL)
//	{
//		// do nothing
//	}
//	else
//	{
//		// do nothing
//	}
	node1 = result->left;
	node2 = second->left;
	do
	{
		const char *nsquare = NSQUARE;
		char *temp_first = node1->value;
		char *temp_second = node2->value;
		char *temp_result = string_mul_mod(temp_first, temp_second, nsquare);
		node1->value = temp_result;

		node1 = node1->next;
		node2 = node2->next;
	} while (node1 != result->left && node2 != second->left);
	if (node2 != second->left)
	{
		DoubleList last = result->left->preview;
		last->next = node2;
		node2->preview = last;

		result->left->preview = second->left->preview;
		second->left->preview->next = result->left;
	}
	return result;
}
//void udf_add(char *a, char *b, char *c)
//{
//	DoubleListTree tree_a, *tree_b, *tree_c;
//	tree_a = (DoubleListTree )malloc(sizeof(DoubleListTree));
//	tree_b = (DoubleListTree )malloc(sizeof(DoubleListTree));
//	tree_c = (DoubleListTree )malloc(sizeof(DoubleListTree));
//	tree_init(tree_a);
//	tree_init(tree_b);
//	tree_init(tree_c);
//
//	string_to_tree(tree_a, a);
//	string_to_tree(tree_b, b);
//
//	tree_add(tree_a, tree_b, tree_c);
//	tree_to_string(c, tree_c);
//}



