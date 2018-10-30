/*
 * Application interface library for the AVIRT driver
 * 
 * avirt.h - AVIRT Application interface library header
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

#ifndef _AVIRT_H_
#define _AVIRT_H_

/**
 * snd_avirt_stream_new - Create a stream in AVIRT
 * @name: The name of the stream
 * @channels: The number of channels for the stream
 * @direction: The stream direction (SND_PCM_STREAM_PLAYBACK or SND_PCM_STREAM_CAPTURE)
 * @map: The audio path to map this stream to
 * @return: 0 on success, negative ERRNO otherwise
 * 
 * Each stream creates a PCM device for the AVIRT sound card.
 * Streams will not appear to the user-space until `snd_avirt_card_seal()` is called.
 * NOTE: Once `snd_avirt_card_seal` is called, no more streams may be added.
 */
int snd_avirt_stream_new(const char *name, unsigned int channels, int direction,
                       const char *map);

/**
 * snd_avirt_card_seal - Finalize AVIRT stream creation and register sound card
 * @return: 0 on success, negative ERRNO otherwise
 * 
 * This should be called once all streams have been created via `snd_avirt_stream_new()`.
 * Calling this function will register the AVIRT sound card to the user-space, and will
 * configure all mapped lower-level Audio Paths for the given stream configuration.
 * configuration.
 * Once this function is called, no more streams may be created.
 */
int snd_avirt_card_seal();

#endif // _AVIRT_H_
