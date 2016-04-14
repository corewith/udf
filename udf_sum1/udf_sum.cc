#define DEBUG 1

#include <memory>

#include <string.h>
#include <stdint.h>
#include <iostream>
#include <tree.h>


extern "C" {

typedef unsigned long long ulonglong;
typedef long long longlong;
// Use the standard mysql dev headers.
#include <mysql.h>
#include <ctype.h>

my_bool   udf_sum_init(UDF_INIT *const initid, UDF_ARGS *const args,
                           char *const message);
void      udf_sum_deinit(UDF_INIT *const initid);
void      udf_sum_clear(UDF_INIT *const initid, char *const is_null,
                            char *const error);
my_bool   udf_sum_add(UDF_INIT *const initid, UDF_ARGS *const args,
                          char *const is_null, char *const error);
char *    udf_sum(UDF_INIT *const initid, UDF_ARGS *const args,
                      char *const result, unsigned long *const length,
                      char *const is_null, char *const error);

} /* extern "C" */

struct agg_state {
    DoubleListTree p_sum;
    DoubleListTree m_sum;
    void *rbuf;
};

my_bool
udf_sum_init(UDF_INIT *const initid, UDF_ARGS *const args,
                 char *const message)
{
    if (args->arg_count != 1 ||
        args->arg_type[0] != STRING_RESULT)
    {
        strcpy(message, "Usage: udf_sum(string value)");
        return 1;
    }

    agg_state *const as = new agg_state();
    as->rbuf = malloc(256);
    initid->ptr = reinterpret_cast<char *>(as);
    initid->maybe_null = 1;
    return 0;
}

void
udf_sum_deinit(UDF_INIT *const initid)
{
    agg_state *const as = reinterpret_cast<agg_state *>(initid->ptr);
    free(as->rbuf);
    delete as;
}

void
udf_sum_clear(UDF_INIT *const initid, char *const is_null, char *const error)
{
    agg_state *const as = reinterpret_cast<agg_state *>(initid->ptr);
    as->p_sum = NULL;
    as->m_sum = NULL;
}

my_bool
udf_sum_add(UDF_INIT *const initid, UDF_ARGS *const args,
                char *const is_null, char *const error)
{
	std::cerr << "1. the value of args->args[0]:       " << args->args[0] << "\n";
    agg_state *const as = reinterpret_cast<agg_state *>(initid->ptr);   
    //
    DoubleListTree e;
    if (NULL == args->args[0]) {
        e = string_to_tree("193#2923656193063716479"); //0
        std::cerr << "there is a NULL value" << "\n";
    } else {
        e = string_to_tree(args->args[0]);
    }
    //std::cerr << "2. the value of tree_to_string: " << tree_to_string(e) << "\n";
    if(e->flag==0){  
	    if(NULL == as->p_sum){
	    	as->p_sum=e;
	    }else{
	    	as->p_sum=tree_add(as->p_sum,e);
		}
	}else{
		if(NULL == as->m_sum){
			as->m_sum=e;
		}else{
			as->m_sum=tree_add(as->m_sum,e);
		}
	}
    //std::cerr << "3. sum so far:                  " << tree_to_string(as->sum) << "\n";
    // memset(args->args[0],0,256);
    memset(args->args[0],0,*(args->lengths));
    //memset(args->args[0],0,sizeof(args->args[0]));
    return true;
}

char *
udf_sum(UDF_INIT *const initid, UDF_ARGS *const args, char *const result,
            unsigned long *const length, char *const is_null, char *const error)
{
    agg_state *const as = reinterpret_cast<agg_state *>(initid->ptr);
    int flag=0;
    if(as->p_sum!=NULL){
    	strcpy(static_cast<char *>(as->rbuf),tree_to_string(as->p_sum));	
    	strcpy(result,static_cast<char *>(as->rbuf));
    	flag=1;
    }
    std::cerr << "4. the as->rbuf:                " << static_cast<char *>(as->rbuf) << "\n";
    if(as->m_sum!=NULL){
    	if(flag==1){
    		// std::cerr << "5. the result:            " << result << "\n";
    		strcat(result,"&");	
    		memset(as->rbuf,0,256);
    		strcpy(static_cast<char*>(as->rbuf),tree_to_string(as->m_sum));
    		strcat(result,static_cast<char*>(as->rbuf));
    	}else{
    		strcpy(static_cast<char*>(as->rbuf),tree_to_string(as->m_sum));
    		strcpy(result,static_cast<char*>(as->rbuf));
    	}
    }
    std::cerr << "4. the as->rbuf:               " << static_cast<char *>(as->rbuf) << "\n\n\n";
    *length=strlen(result);
    return result;
}
