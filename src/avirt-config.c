/*
 * Application interface library for the AVIRT driver
 * 
 * avirt-config.c - Main AVIRT configuration via configfs
 *
 * Copyright (C) 2018 Fiberdyne Systems Pty Ltd
 * Author: Mark Farrugia <mark.farrugia@fiberdyne.com.au>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <avirt/avirt.h>

#include <stdbool.h>
#include <stdio.h>
#include <alsa/asoundlib.h>
#include <sys/stat.h>
#include <sys/mount.h>

#define AVIRT_CONFIGFS_PATH_STREAMS "/config/avirt/streams/"
#define AVIRT_CONFIGFS_PATH_MAXLEN 64

#define AVIRT_ERROR(errmsg) \
  fprintf(stderr, "AVIRT ERROR: %s\n", errmsg);
#define AVIRT_ERROR_V(fmt, args...) \
      fprintf(stderr, "AVIRT ERROR: " fmt "\n", ##args);

#define AVIRT_DEBUG_ON
#ifdef AVIRT_DEBUG_ON
# define AVIRT_DEBUG(debugmsg) \
  fprintf(stderr, "AVIRT DEBUG: %s\n", debugmsg);
# define AVIRT_DEBUG_V(fmt, args...) \
  fprintf(stderr, "[%s]: AVIRT DEBUG: " fmt "\n", __func__, ##args);
#endif

#define WRITE_TO_PATH(path, fmt, args...) \
  do { \
    FILE *fd = fopen(path, "w"); \
    if (!fd) { \
      AVIRT_ERROR_V("Failed to open file at '%s'", path); \
      return -EPERM; \
    } \
    fprintf(fd, fmt, ##args); \
    fclose(fd); \
  } while (0)

static bool configfs_mounted = false;
static bool card_sealed = false;

static int mount_configfs()
{
  int err;

  err = mount("none", "/config", "configfs", 0, NULL);
  if (!err)
    configfs_mounted = true;
  else
    AVIRT_ERROR("Failed to mount configfs filesystem!");

  return err;
}

int AVIRT_CreateStream(const char *name, unsigned int channels, int direction,
                       const char *map)
{
  int err;
  char path[AVIRT_CONFIGFS_PATH_MAXLEN];
  char path_attr[AVIRT_CONFIGFS_PATH_MAXLEN];
  FILE *fd;

  // Check whether the configfs filesystem is mounted
  if (!configfs_mounted)
  {
    err = mount_configfs();
    if (err < 0)
      return err;
  }

  // Check if card is already sealed
  if (card_sealed)
  {
    AVIRT_ERROR("Card is already sealed!");
    return -EPERM;
  }

  // This indicates to AVIRT the direction of the stream
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
    AVIRT_ERROR_V("Cannot create stream '%s' since name is too long!", name);
    return -ENOMEM;
  }

  strcat(path, name);
  err = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
  if (err < 0)
  {
    AVIRT_ERROR_V("Cannot create stream '%s' at directory '%s'", name, path);
    return err;
  }

  // Write channels
  strcpy(path_attr, path);
  strcat(path_attr, "/channels");
  WRITE_TO_PATH(path_attr, "%d", channels);

  if (map)
  {
    // Write mapping
    strcpy(path_attr, path);
    strcat(path_attr, "/map");
    WRITE_TO_PATH(path_attr, "%s", map);
  }

  AVIRT_DEBUG_V("Created stream: %s", name);

  return 0;
}

int AVIRT_SealCard()
{
  int err;
  char path_sealed[AVIRT_CONFIGFS_PATH_MAXLEN];
  FILE *fd;

  // Check if card is already sealed
  if (card_sealed)
  {
    AVIRT_ERROR("Card is already sealed!");
    return -EPERM;
  }

  if (!configfs_mounted)
  {
    err = mount_configfs();
    if (err < 0)
      return err;
  }

  strcpy(path_sealed, AVIRT_CONFIGFS_PATH_STREAMS);
  strcat(path_sealed, "/sealed");
  WRITE_TO_PATH(path_sealed, "%d", 1);

  AVIRT_DEBUG("Card sealed!");

  card_sealed = true;
}
