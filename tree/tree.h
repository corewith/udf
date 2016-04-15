#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bigint.h>

#define LEN_ALL 256
#define NSQUARE "7852129455018228769"

#ifdef	__cplusplus
extern "C" {
#endif
	typedef struct doublelist
	{
		char *value;
		struct doublelist *next;
		struct doublelist *preview;
	}*DoubleList;

	typedef struct doublelisttree
	{
		int value;
		int flag;
		DoubleList left;
		DoubleList right;
	}*DoubleListTree;

	DoubleListTree tree_init();
	void tree_split(const char *s, char c, char *strs[], int *len);
	DoubleListTree string_to_tree(const char *s);
	char* tree_to_string(DoubleListTree tree);
	DoubleListTree tree_mov(DoubleListTree b);
	char* string_mul_mod(const char *a,const char *b,const char *m);
	DoubleListTree tree_add(DoubleListTree first, DoubleListTree second);
#ifdef	__cplusplus
}
#endif
