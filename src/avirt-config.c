/*
 * AVIRT User-space library
 * 
 * avirt-config.c - Main AVIRT configuration via configfs
 *
 * Copyright (C) 2018 Fiberdyne Systems Pty Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "avirt-config.h"

#include <stdbool.h>
#include <alsa/asoundlib.h>
#include <sys/stat.h>
#include <sys/mount.h>

#define AVIRT_CONFIGFS_PATH_STREAMS "/config/avirt/streams/"
#define AVIRT_CONFIGFS_PATH_MAXLEN 64

static bool configfs_mounted = false;

static int mount_configfs()
{
  int err;

  err = mount("none", "/config", "configfs", 0, NULL);
  if (!err)
    configfs_mounted = true;
  else
    printf("AVIRT ERROR: Failed to mount configfs filesystem!");

  return err;
}

int AVIRT_CreateStream(const char *name, unsigned int channels, int direction)
{
  int err;
  char path[AVIRT_CONFIGFS_PATH_MAXLEN];
  char path_chans[AVIRT_CONFIGFS_PATH_MAXLEN];
  FILE *fd;

  if (!configfs_mounted) {
    err = mount_configfs();
    if (err < 0)
      return err;
  }

  strcpy(path, AVIRT_CONFIGFS_PATH_STREAMS);
  switch (direction) {
    case SND_PCM_STREAM_PLAYBACK:
      strcat(path, "playback_");
      break;
    case SND_PCM_STREAM_CAPTURE:
      strcat(path, "capture_");
      break;
    default:
      return -EINVAL;
  }

  if ((AVIRT_CONFIGFS_PATH_MAXLEN - strlen(path)) < strlen(name))
  {
    printf("AVIRT ERROR: Cannot create stream '%s' since name is too long!", name);
    return -ENOMEM;
  }

  strcat(path, name);
  err = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
  if (err < 0)
  {
    printf("AVIRT ERROR: Cannot create stream '%s' at directory '%s'\n", name, path);
    return err;
  }

  strcpy(path_chans, path);
  strcat(path_chans, "/channels");
  fd = fopen(path_chans, "w");
  if (!fd)
  {
    printf("AVIRT ERROR: Failed to open file at '%s'", path_chans);
    return -1;
  }

  fprintf(fd, "%d", channels);

  return 0;
}

int AVIRT_SealCard()
{
  int err;

  if (!configfs_mounted) {
    err = mount_configfs();
    if (err < 0)
      return err;
  }

  strcpy(path_sealed, AVIRT_);
  strcat(path_chans, "/sealed");
  fd = fopen(path_chans, "w");
  if (!fd)
  {
    printf("AVIRT ERROR: Failed to open file at '%s'", path_chans);
    return -1;
  }

  fprintf(fd, "%d", 1);
}
