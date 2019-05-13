#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pcs_json.h"

char __attribute__((section (".myBufSection"))) json_buffer_g[JSON_BUFFER_SIZE];  // TODO: protect with mutex, because we may access this driver from different tasks

static int json_level = 0;
static char json_level_delimiter[] = "\t";
static char *json_marker = json_buffer_g;
static char json_numeric_buffer[12];

void JSON_Start ()
{
   memset(json_buffer_g, 0, sizeof(json_buffer_g));
   json_marker = json_buffer_g;
   json_level = 1;
   JSON_Append("{\n");
}

void JSON_Delimit ()
{
   int i = 0;
   for (i = 0; i < json_level; i++)
   {
      JSON_Append (json_level_delimiter);
   }
}

void JSON_Append (char *str)
{
   int len = strlen(str);
   strncpy(json_marker, str, len);
   json_marker += len;
}

void JSON_Start_Object(char* object_name)
{
   JSON_Delimit();
   JSON_Append("\"");
   JSON_Append(object_name);
   JSON_Append("\": {\n");
   json_level++;
}

void JSON_Add_Field_Integer(char* object_name, int object_value)
{
   JSON_Delimit();
   JSON_Append("\"");
   JSON_Append(object_name);
   JSON_Append("\": ");

   sprintf(json_numeric_buffer, "%d", object_value);
   JSON_Append(json_numeric_buffer);
   JSON_Append(",\n");

}

void JSON_Add_Field_Double(char* object_name, double object_value)
{
   JSON_Delimit();
   JSON_Append("\"");
   JSON_Append(object_name);
   JSON_Append("\": ");

   sprintf(json_numeric_buffer, "%.1f", object_value);
   JSON_Append(json_numeric_buffer);
   JSON_Append(",\n");

}

void JSON_Add_Field_String(char* object_name, char* object_value)
{
   JSON_Delimit();
   JSON_Append("\"");
   JSON_Append(object_name);
   JSON_Append("\": \"");
   JSON_Append(object_value);
   JSON_Append("\",\n");

}

void JSON_End_Object()
{
   //Remove the last newline and comma
   json_marker -= 2;

   JSON_Append("\n");
   json_level--;
   JSON_Delimit();
   JSON_Append("},\n");

}

char *JSON_End()
{
   //Remove the last newline and comma
   json_marker -= 2;
   memset(json_marker, '\0', 2);

   JSON_Append("\n");
   json_level--;
   JSON_Delimit();
   JSON_Append("}");
   return json_buffer_g;
}

void JSON_Start_Array(char* object_name)
{
   JSON_Delimit();
   JSON_Append("\"");
   JSON_Append(object_name);
   JSON_Append("\": [\n");
   json_level++;
}

void JSON_End_Array()
{
   //Remove the last newline and comma
   json_marker -= 2;

   JSON_Append("\n");
   json_level--;
   JSON_Delimit();
   JSON_Append("],\n");
}

void JSON_Start_ArrayObject()
{
   JSON_Delimit();
   JSON_Append("{\n");
   json_level++;
}

//Functions below are for extracting data from JSON strings


int JSON_Extract_String(char *json_string, char *target_object, char *return_buffer, size_t return_buffer_size)
{
   char *objPtr = strstr(json_string, target_object);
   if (objPtr != NULL)
   {
       objPtr += strlen(target_object) + 4;
       char c = *(objPtr);

       int inc = 0;
       while (c != '"' && c != '\n' && inc != return_buffer_size)
       {
           *(return_buffer + inc) = c;
           inc++;
           c = *(objPtr + inc);
       }
       *(return_buffer + inc) = '\0';
       return 1;
   }

   return 0;
}

int JSON_Extract_Integer(char *json_string, char *target_object, int *return_int)
{
   char *objPtr = strstr(json_string, target_object);
   if (objPtr != NULL)
   {
       objPtr += strlen(target_object) + 2;
       *return_int = atoi(objPtr);
       return 1;
   }
   return 0;
}

int JSON_Extract_Double(char *json_string, char *target_object, double *return_double)
{
    char*objPtr = strstr(json_string, target_object);
    if (objPtr != NULL)
    {
        objPtr += strlen(target_object) + 3;

        char doubleBuf[30];
        int inc = 0;
        char c = *(objPtr);
        while(c != ',' && c != '\n' && inc != 30)
        {
            doubleBuf[inc] = c;
            inc++;
            c = *(objPtr + inc);
        }

        *return_double = strtod(doubleBuf, NULL);
    }
    return 0;
}

int JSON_Extract_Object(char *json_string, char *target_object, char *return_object)
{
    char* objPtr = strstr(json_string, target_object);
    objPtr += strlen(target_object) + 3;
    if(objPtr != NULL)
    {
        char c = *(objPtr);
        int inc = 0;
        int objNum = 0;
        while (c != '}' || objNum != 0)
        {
            if (c == '{' && inc != 0)
            {
                objNum++;
            }
            if (c == '}')
            {
                objNum--;
            }
            *(return_object + inc) = c;
            inc ++;
            c = *(objPtr + inc);
        }
        *(return_object + inc) = c;
        return 1;
    }
    return 0;
}

/* Note: Object numeration starts from 1. return_object - is data pointer to target object start
 * This function has limitation - it can't handle objects inside other objects.
 * It must be used only for simple object arrays */
int JSON_Extract_Object_From_Arr(char *json_string, char *target_arr, char **return_object, unsigned char obj_num)
{
  char* objArr = strstr(json_string, target_arr);

  if(objArr != NULL)
  {
    objArr = strchr(objArr, '[');
    if (NULL == objArr)
    {
      return 0;
    }

    for (unsigned char i = 0; i < obj_num; i++)
    {
      objArr = strchr(objArr, '{');
      if (NULL == objArr)
      {
        return 0;
      }
      objArr++;
      /* Check if object has closing bracket */
      if (NULL == strchr(objArr, '}'))
      {
        return 0;
      }
    }

    *return_object = objArr;

    return 1;
  }
  return 0;
}
