#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simple_json.h"
#include "simple_json_parse.h"
#include "simple_json_error.h"


long get_file_Size(FILE *file)
{
  long size;

  if(file != NULL){
    if( fseek(file, 0, SEEK_END) ){
      fclose(file);
      return -1;
    }

    size = ftell(file);
    rewind(file);
    return size;
  }

  return -1;
}

SJson *sj_load(const char *filename)
{
    FILE *file;
    SJson *json;
    long size,read;
    char *buffer = NULL;
    file = fopen(filename,"r");
    if (!file)
    {
        sj_set_error("sj_load: failed to open file");
        return NULL;
    }
    size = get_file_Size(file);
    if (size <= 0)
    {
        sj_set_error("sj_load: error with file size");
        fclose(file);
        return NULL;
    }
    printf("loaded file %s with a size of %li characters\n",filename,size);

    buffer = (char *)malloc(sizeof(char)*(size + 1));
    
    if (buffer == NULL)
    {
        sj_set_error("sj_load: failed to allocate character buffer for json file");
        fclose(file);
        return NULL;
    }
    memset(buffer,0,sizeof(char)*(size+1));
    
    if ((read = fread(buffer, sizeof(char), size, file)) != size)
    {
        printf("expected to read %li characters, but read %li instead\n",size,read);
    }
    else
    {
        printf("read %li characters of %li available",read,size);
    }
    printf("file contents:\n%s\n",buffer);
    fclose(file);
    
    json = sj_parse_buffer(buffer,read);
    
    free(buffer);
    
    return json;
}

SJson *sj_new()
{
    SJson *json;
    json = (SJson *)malloc(sizeof(SJson));
    if (!json)
    {
        sj_set_error("sj_new: failed to allocate space for new json object");
        return NULL;
    }
    memset(json,0,sizeof(SJson));
    return json;
}

void sj_free(SJson *json)
{
    if (!json)return;
    
    if (json->json_free)json->json_free(json);
}

void sj_save(SJson *json,char *filename)
{
    FILE *file;
    SJString *string;
    if ((!json) || (!json->get_string))return;
    string = json->get_string(json);
    if (!string)return;
    file = fopen(filename,"w");
    if (!file)
    {
        sj_string_free(string);
        return;
    }
    fputs(string->text,file);
    fclose(file);
}

void sj_echo(SJson *json)
{
    SJString *output;
    if ((!json) || (!json->get_string))return;
    output = json->get_string(json);
    if (!output)return;
    printf("%s\n",output->text);
    sj_string_free(output);
}

void sj_null_free(SJson *json)
{
    if (!json)return;
    free(json);
}

SJString *sj_value_to_json_string(SJson *json)
{
    if (!json)return NULL;
    if (!json->get_string)return NULL;
    return json->get_string(json);
}

SJString *sj_null_to_json_string(SJson *json)
{
    return sj_string_new_text("null");
}

SJson *sj_null_new()
{
    SJson *json;
    json = sj_new();
    if (!json)return NULL;
    json->sjtype = SJVT_NULL;
    json->json_free = sj_null_free;
    json->get_string = sj_null_to_json_string;
    return json;
}
/*eol@eof*/
