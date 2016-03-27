#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 4

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct bigint_node
{
	int value;                      // four bits between 0000 and 9999
	struct bigint_node *preview;
	struct bigint_node *next;
}*BigInt_Node;

typedef struct bigint_list
{
	BigInt_Node first;
	int count;          // the length of BigInt_Node
	//int flag;         // the sign flag, 0 represent plus, 1 represent minus.--Have not considered it at present.
}*BigInt_List;

BigInt_List bigint_init();
BigInt_List bigint_mov_to(BigInt_List b);
void bigint_mov(BigInt_List a, BigInt_List b);
BigInt_List bigint_mov_value_to(int value);
void bigint_mov_value(BigInt_List list, int value);
BigInt_List string_to_list(const char *s);
char *list_to_string(BigInt_List list);
int bigint_cmp(BigInt_List a, BigInt_List b);
BigInt_List bigint_add(BigInt_List a, BigInt_List b);
BigInt_List bigint_sub(BigInt_List a, BigInt_List b);
BigInt_List bigint_mul(BigInt_List a, int b);
BigInt_List bigint_multiply(BigInt_List a, BigInt_List b);
BigInt_List bigint_div(BigInt_List a, int b);
void divide1(BigInt_List a, BigInt_List b, BigInt_List c, BigInt_List remain1);
BigInt_List bigint_divide(BigInt_List a, BigInt_List b);
BigInt_List bigint_mod(BigInt_List a,BigInt_List b);

#ifdef	__cplusplus
}
#endif
