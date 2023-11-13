#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#include "jailbreak.c"

typedef struct notify_request {
  char useless1[45];
  char message[3075];
} notify_request_t;

int sceKernelSendNotificationRequest(int, notify_request_t*, size_t, int);

// Send notify function
void sendNotification(const char *message) {
  notify_request_t req;
  bzero(&req, sizeof req);
  strncpy(req.message, message, sizeof req.message);
  sceKernelSendNotificationRequest(0, &req, sizeof req, 0);
}

// Copy function
void copy_file(char *sourcefile, char *destfile) {
  int src = open(sourcefile, O_RDONLY, 0);
  if (src != -1) {
    int out = open(destfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (out != -1) {
      char *buffer = malloc(4194304);
      if (buffer != NULL) { // Check if buffer is allocated correctly
        size_t bytes;
        while ((bytes = read(src, buffer, 4194304)) > 0) {
          write(out, buffer, bytes);
        }
        free(buffer);
      }
      close(out);
    }
    close(src);
  }
}

// Create empty file
void touch(const char *filename) {
    int file_descriptor = open(filename, O_CREAT, S_IRUSR | S_IWUSR);

    if (file_descriptor == -1) {
        perror("Error creating file");
        exit(EXIT_FAILURE);
    }

    close(file_descriptor);
}

// Stat check if exists
int file_exists(const char *filename) {
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}

int main() {
  // Escape Sandbox
  pid_t pid = getpid();
  jailbreak_process(pid, true);

  touch("/mnt/usb0/.probe");

  if (!file_exists("/mnt/usb0/.probe")) {
      touch("/mnt/usb1/.probe");
    if (!file_exists("/mnt/usb1/.probe")) {
      sendNotification("Insert USB for database backup");
    } else {
        sendNotification("Dumping to USB1");

        mkdir("/mnt/usb1/PS5", 0777);
        mkdir("/mnt/usb1/PS5/db-backup", 0777);

        copy_file("/system_data/priv/mms/app.db", "/mnt/usb1/PS5/db-backup/app.db");
        copy_file("/system_data/priv/mms/appinfo.db", "/mnt/usb1/PS5/db-backup/appinfo.db");
        copy_file("/system_data/priv/mms/addcontent.db", "/mnt/usb1/PS5/db-backup/addcontent.db");
        copy_file("/system_data/priv/mms/av_content_bg.db", "/mnt/usb1/PS5/db-backup/av_content_bg.db");
        
        sendNotification("Dump to USB1 done!");
    }
  } else {
    sendNotification("Dumping to USB0");

    mkdir("/mnt/usb0/PS5", 0777);
    mkdir("/mnt/usb0/PS5/db-backup", 0777);

    copy_file("/system_data/priv/mms/app.db", "/mnt/usb0/PS5/db-backup/app.db");
    copy_file("/system_data/priv/mms/appinfo.db", "/mnt/usb0/PS5/db-backup/appinfo.db");
    copy_file("/system_data/priv/mms/addcontent.db", "/mnt/usb0/PS5/db-backup/addcontent.db");
    copy_file("/system_data/priv/mms/av_content_bg.db", "/mnt/usb0/PS5/db-backup/av_content_bg.db");

    sendNotification("Dump to USB0 done!");
  }
  return EXIT_SUCCESS;
}
