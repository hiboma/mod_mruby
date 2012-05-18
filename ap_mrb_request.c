#include "mod_mruby.h"
#include "ap_mrb_request.h"
#include "json.h"

static char *ap_mrb_string_check(apr_pool_t *p, char *str);

request_rec *mrb_request_rec_state = NULL;

struct mrb_data_type request_rec_type = {
  "request_rec", 0,
};


int ap_mrb_push_request(request_rec *r)
{
    mrb_request_rec_state = (request_rec *)apr_pcalloc(r->pool, sizeof (*mrb_request_rec_state));
    mrb_request_rec_state = r;
    return OK;
}

request_rec *ap_mrb_get_request()
{
    return mrb_request_rec_state;
}

mrb_value ap_mrb_init_request(mrb_state *mrb, mrb_value str)
{
    str = mrb_class_new_instance(mrb, 0, NULL, class_request);
    mrb_iv_set(mrb
        , str
        , mrb_intern(mrb, "request_rec")
        , mrb_obj_value(Data_Wrap_Struct(mrb
            , mrb->object_class
            , &request_rec_type
            , ap_mrb_get_request())
        )
    );

    ap_log_error(APLOG_MARK
        , APLOG_WARNING
        , 0
        , NULL
        , "%s ERROR %s: Initialied."
        , MODULE_NAME
        , __func__
    );

    return str;
}

mrb_value ap_mrb_get_request_rec(mrb_state *mrb, const char *member)
{
    char *val;
    request_rec *r = ap_mrb_get_request();

    if (strcmp(member, "filename") == 0)
        val = apr_pstrdup(r->pool, r->filename);
    else if (strcmp(member, "uri") == 0)
        val = apr_pstrdup(r->pool, r->uri);
    else if (strcmp(member, "user") == 0)
        val = apr_pstrdup(r->pool, r->user);
    else if (strcmp(member, "content_type") == 0)
        val = apr_pstrdup(r->pool, r->content_type);
    else if (strcmp(member, "the_request") == 0)
        val = apr_pstrdup(r->pool, r->the_request);
    else if (strcmp(member, "protocol") == 0)
        val = apr_pstrdup(r->pool, r->protocol);
    else if (strcmp(member, "vlist_validator") == 0)
        val = apr_pstrdup(r->pool, r->vlist_validator);
    else if (strcmp(member, "ap_auth_type") == 0)
        val = apr_pstrdup(r->pool, r->ap_auth_type);
    else if (strcmp(member, "unparsed_uri") == 0)
        val = apr_pstrdup(r->pool, r->unparsed_uri);
    else if (strcmp(member, "canonical_filename") == 0)
        val = apr_pstrdup(r->pool, r->canonical_filename);
    else if (strcmp(member, "path_info") == 0)
        val = apr_pstrdup(r->pool, r->path_info);
    else if (strcmp(member, "hostname") == 0)
        val = apr_pstrdup(r->pool, r->hostname);

    if (val == NULL)
        val = apr_pstrdup(r->pool, "(null)");
    
    return mrb_str_new(mrb, val, strlen(val));
}


static char *ap_mrb_string_check(apr_pool_t *p, char *str)
{
    if (str == NULL)
        str = apr_pstrdup(p, "null");

    return str;
}

mrb_value ap_mrb_get_request_rec_json(mrb_state *mrb, const char *member)
{
    char *val;
    request_rec *r = ap_mrb_get_request();
    json_object *my_object;

    my_object = json_object_new_object();
    json_object_object_add(my_object, "filename", json_object_new_string(ap_mrb_string_check(r->pool, r->filename)));
    json_object_object_add(my_object, "uri", json_object_new_string(ap_mrb_string_check(r->pool, r->uri)));
    json_object_object_add(my_object, "user", json_object_new_string(ap_mrb_string_check(r->pool, r->user)));
    json_object_object_add(my_object, "content_type", json_object_new_string(ap_mrb_string_check(r->pool, r->content_type)));
    json_object_object_add(my_object, "protocol", json_object_new_string(ap_mrb_string_check(r->pool, r->protocol)));
    json_object_object_add(my_object, "vlist_validator", json_object_new_string(ap_mrb_string_check(r->pool, r->vlist_validator)));
    json_object_object_add(my_object, "ap_auth_type", json_object_new_string(ap_mrb_string_check(r->pool, r->ap_auth_type)));
    json_object_object_add(my_object, "unparsed_uri", json_object_new_string(ap_mrb_string_check(r->pool, r->unparsed_uri)));
    json_object_object_add(my_object, "canonical_filename", json_object_new_string(ap_mrb_string_check(r->pool, r->canonical_filename)));
    json_object_object_add(my_object, "path_info", json_object_new_string(ap_mrb_string_check(r->pool, r->path_info)));
    json_object_object_add(my_object, "hostname", json_object_new_string(ap_mrb_string_check(r->pool, r->hostname)));

    val = json_object_to_json_string(my_object);

    if (val == NULL)
        val = apr_pstrdup(r->pool, "(null)");
    
    return mrb_str_new(mrb, val, strlen(val));


}

mrb_value ap_mrb_set_request_rec(mrb_state *mrb, const char *member, mrb_value str)
{
    mrb_value val;

    request_rec *r = ap_mrb_get_request();
    mrb_get_args(mrb, "o", &val);

    if (strcmp(member, "filename") == 0)
        r->filename = apr_pstrdup(r->pool, RSTRING_PTR(val));
    else if (strcmp(member, "uri") == 0)
        r->uri = apr_pstrdup(r->pool, RSTRING_PTR(val));
    else if (strcmp(member, "user") == 0)
        r->user = apr_pstrdup(r->pool, RSTRING_PTR(val));
    else if (strcmp(member, "content_type") == 0)
        r->content_type = apr_pstrdup(r->pool, RSTRING_PTR(val));
    else if (strcmp(member, "the_request") == 0)
        r->the_request = apr_pstrdup(r->pool, RSTRING_PTR(val));
    else if (strcmp(member, "protocol") == 0)
        r->protocol = apr_pstrdup(r->pool, RSTRING_PTR(val));
    else if (strcmp(member, "vlist_validator") == 0)
        r->vlist_validator = apr_pstrdup(r->pool, RSTRING_PTR(val));
    else if (strcmp(member, "ap_auth_type") == 0)
        r->ap_auth_type = apr_pstrdup(r->pool, RSTRING_PTR(val));
    else if (strcmp(member, "unparsed_uri") == 0)
        r->unparsed_uri = apr_pstrdup(r->pool, RSTRING_PTR(val));
    else if (strcmp(member, "canonical_filename") == 0)
        r->canonical_filename = apr_pstrdup(r->pool, RSTRING_PTR(val));
    else if (strcmp(member, "path_info") == 0)
        r->path_info = apr_pstrdup(r->pool, RSTRING_PTR(val));

    return val;
}

mrb_value ap_mrb_get_request_filename(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_get_request_rec(mrb, "filename");
}

mrb_value ap_mrb_get_request_uri(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_get_request_rec(mrb, "uri");
}

mrb_value ap_mrb_get_request_user(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_get_request_rec(mrb, "user");
}

mrb_value ap_mrb_get_request_content_type(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_get_request_rec(mrb, "content_type");
}

mrb_value ap_mrb_get_request_the_request(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_get_request_rec(mrb, "the_request");
}

mrb_value ap_mrb_get_request_protocol(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_get_request_rec(mrb, "protocol");
}

mrb_value ap_mrb_get_request_vlist_validator(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_get_request_rec(mrb, "vlist_validator");
}

mrb_value ap_mrb_get_request_ap_auth_type(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_get_request_rec(mrb, "ap_auth_type");
}

mrb_value ap_mrb_get_request_unparsed_uri(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_get_request_rec(mrb, "unparsed_uri");
}

mrb_value ap_mrb_get_request_canonical_filename(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_get_request_rec(mrb, "canonical_filename");
}

mrb_value ap_mrb_get_request_path_info(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_get_request_rec(mrb, "path_info");
}

mrb_value ap_mrb_get_request_hostname(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_get_request_rec(mrb, "hostname");
}

mrb_value ap_mrb_set_request_filename(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_set_request_rec(mrb, "filename", str);
}

mrb_value ap_mrb_set_request_uri(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_set_request_rec(mrb, "uri", str);
}

mrb_value ap_mrb_set_request_user(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_set_request_rec(mrb, "user", str);
}

mrb_value ap_mrb_set_request_content_type(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_set_request_rec(mrb, "content_type", str);
}

mrb_value ap_mrb_set_request_the_request(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_set_request_rec(mrb, "the_request", str);
}

mrb_value ap_mrb_set_request_protocol(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_set_request_rec(mrb, "protocol", str);
}

mrb_value ap_mrb_set_request_vlist_validator(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_set_request_rec(mrb, "vlist_validator", str);
}

mrb_value ap_mrb_set_request_ap_auth_type(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_set_request_rec(mrb, "ap_auth_type", str);
}

mrb_value ap_mrb_set_request_unparsed_uri(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_set_request_rec(mrb, "unparsed_uri", str);
}

mrb_value ap_mrb_set_request_canonical_filename(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_set_request_rec(mrb, "canonical_filename", str);
}

mrb_value ap_mrb_set_request_path_info(mrb_state *mrb, mrb_value str)
{
    return ap_mrb_set_request_rec(mrb, "path_info", str);
}

mrb_value ap_mrb_write_request(mrb_state *mrb, mrb_value str)
{   

    struct RProc *b;
    mrb_value argc, *argv;
    char *member, *value;
    request_rec *r = ap_mrb_get_request();

    mrb_get_args(mrb, "b*", &b, &argv, &argc);
    if (mrb_fixnum(argc) != 2) {
        ap_log_error(APLOG_MARK
            , APLOG_WARNING
            , 0
            , NULL
            , "%s ERROR %s: argument is not 2"
            , MODULE_NAME
            , __func__
        );
        return str;
    }

    member = RSTRING_PTR(argv[0]);
    value  = RSTRING_PTR(argv[1]);

    if (strcmp(member, "filename") == 0)
        r->filename = apr_pstrdup(r->pool, value);
    else if (strcmp(member, "uri") == 0)
        r->uri = apr_pstrdup(r->pool, value);

    return str;
}
