/*
 * Copyright (C) 2018 alex943
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>


#define CODE_OPEN_CAMERA 1
#define PORT 8080 
#define BUFSIZE 24
#define EXIT_FAILURE -1


char* handle_business(char* business_code) 
{
  if(strcmp(business_code, "video-start") == 0) 
  {
    #ifdef ANDROID
      system("/system/bin/sh -c \"am start -a android.media.action.STILL_IMAGE_CAMERA\"");
    #else
      printf("command: video-start\n");
    #endif     
  } else if(strcmp(business_code, "video-stop") == 0) 
  {
    #ifdef ANDROID
      system("/system/bin/sh -c \"am force-stop com.android.camera2\"");
    #else
      printf("command: video-stop\n");
    #endif
  } else 
  {
    printf("unknown command: %s\n", business_code);
    return "Unknown command";
  }
  return "Executed command";
}


int client_start() 
{
  struct sockaddr_in address;
  int sock = 0, recv_size;
  struct sockaddr_in serv_addr;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
      printf("\n Socket creation error \n");
      return -1;
  }
  
  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
    
  #ifdef ANDROID
    if(inet_pton(AF_INET, "10.0.2.2", &serv_addr.sin_addr)<=0) 
  #else
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
  #endif    
  {
      printf("\nInvalid address/ Address not supported \n");
      return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
      printf("\nConnection Failed \n");
      return -1;
  }

  printf("start to read.\n");
  while(1) 
  {
    char buffer[BUFSIZE] = {0};
    char* resp = NULL;
    recv_size = recv(sock, buffer, BUFSIZE, 0);

    if(recv_size == 0 || strcmp(buffer, "quit") == 0) 
    {
      resp = "client quit";
      send(sock , resp , strlen(resp) , 0 );
      break;
    } 
    else 
    {
      resp = handle_business(buffer);
      send(sock , resp , strlen(resp) , 0 );
    }
  }
  
  return 0;
}


int main() 
{
  client_start();
  return 0;
}