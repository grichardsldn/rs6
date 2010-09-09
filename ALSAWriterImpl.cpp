#include <stdio.h>

#include "ALSAWriter.h"

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>

#include "ALSAWriterImpl.h"

int ALSAWriterImpl::Init( int samplerate, int channels, int num_frames)
{
  /* Open PCM device for playback. */
  rc = snd_pcm_open(&handle, "default",
                    SND_PCM_STREAM_PLAYBACK, 0);
  if (rc < 0) {
    fprintf(stderr,
            "unable to open pcm device: %s\n",
            snd_strerror(rc));
  }

printf("GDR: opened sound\n");
  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(handle, params,
                      SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle, params,
                              SND_PCM_FORMAT_S16_LE);

  /* Two channels (stereo) */
  snd_pcm_hw_params_set_channels(handle, params, 2);

  unsigned int val = samplerate;
  snd_pcm_hw_params_set_rate_near(handle, params,
                                  &val, &dir);

  /* Set period size to 32 frames. */
  frames = 512; // GDR should be num_frames
  snd_pcm_hw_params_set_period_size_near(handle,
                              params, &frames, &dir);

  //snd_pcm_uframes_t bsize = 128;
  ////snd_pcm_hw_params_set_buffer_size_near( handle, 
//	params, &bsize);

  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0) {
    fprintf(stderr,
            "unable to set hw parameters: %s\n",
            snd_strerror(rc));
    exit(1);
  }

  
  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params, &frames,
                                    &dir);
  size = frames * 4; /* 2 bytes/sample, 2 channels */
 fprintf(stderr, "GDR: buffer size in periods=%d\n", frames );
  return frames;
}

// ignore num_frames, always one frame only
int ALSAWriterImpl::Write( char * data, int num_frames )
{
    rc = snd_pcm_writei(handle, data, frames);
    if (rc == -EPIPE) {
      /* EPIPE means underrun */
      //fprintf(stderr, "underrun occurred\n");
      snd_pcm_prepare(handle);
    } else if (rc < 0) {
      fprintf(stderr,
              "error from writei: %s\n",
              snd_strerror(rc));
    }  else if (rc != (int)frames) {
      fprintf(stderr,
              "short write, write %d frames\n", rc);
    }

  //snd_pcm_drain(handle);
  //snd_pcm_close(handle);

  return rc;
}
