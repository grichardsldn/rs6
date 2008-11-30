#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <stdio.h>
#define DEBUG 

#ident "$Id: setsound.c,v 1.2 2000/10/10 19:25:46 graham Exp graham $"
#define SMALL_FRAG (256)
#define SMALL_FRAG_PWR (9)

typedef signed short S16;

#warning stop received needs adding to this file
int stop_received = 0;

/* stuff to control soundcard */
int set_frag(int fd)
{
	int a;
	int retn;

	//a=0x20000|SMALL_FRAG_PWR;
	a=0x20000|12;
printf("Bollocks");
	retn=ioctl(fd, SNDCTL_DSP_SETFRAGMENT, &a);

	if (retn<0)
	{
		perror("Failed to set small fragment");
		return(0);
	}
	else
	{	
#ifdef DEBUG
		perror("Set fragment");
#endif
	}
	return(SMALL_FRAG);
}

int set_card(int cfd, int rate, int format, int channels)
{
	int a;
	int retn;
	int gotspeed;

	a=rate;
	

	retn=ioctl(cfd, SNDCTL_DSP_SPEED, &a);
	
	if (retn<0)
	{
#ifdef DEBUG 
		perror("set dsp speed");
#endif

		return(0);
	}
	gotspeed = a;
	a=channels-1;

	retn=ioctl(cfd, SNDCTL_DSP_STEREO, &a);
	
	if (retn<0)
	{
#ifdef DEBUG 
		perror("set dsp stereo");
#endif

		return(0);
	}
	a=format;

	retn=ioctl(cfd, SNDCTL_DSP_SETFMT, &a);
	
	if (retn<0)
	{
#ifdef DEBUG 
		perror("set dsp format");
#endif

		return(0);
	}
printf("card: gotspeed is %i\n", gotspeed);
	return(gotspeed);
}


/***************************************************************************
 *    FUNCTION: 
 *
 *  PARAMETERS: 
 *		
 *     RETURNS: 
 *
 * DESCRIPTION: 
 **************************************************************************/

int
play_data(S16 *data, int length, int samplerate, int width, int channels)
{
	int samples_left;
	int samples_played;
	int to_write;
	int written;

	FILE *dsp;

	if (channels != 1)
	{
		printf("play_data: Mono only supported.\n");
		return(0);
	}
			
	if (width != 16)
	{
		printf("play_data: 16bit only supported\n");
		return(0);
	}

	/* open sound device */
	if ((dsp=fopen("/dev/dsp", "w"))==NULL)	
	{
		printf("play_data: Cannot open sound device\n");
		return(0);
	}

	/* set up sound device */
	if (set_card(fileno(dsp), samplerate, width, channels) == 0)
	{
		printf("play_data: ioctl failed\n");
		fclose(dsp);
		return(0);
	}

	/* set up samples_left */
	samples_left = length;
	samples_played = 0;

	while (samples_left > 0)
	{
		if (samples_left > 1000)
		{
			to_write = 1000;
		}
		else
		{
			to_write = samples_left;
		}


		written = fwrite(data + samples_played, 2, to_write, dsp);

		if (written != to_write)
		{
			/* write failed */
			printf("card: write to device failed\n");	
			perror("fwrite");
			fclose(dsp);
			return(0);
		}

		if (stop_received == 1)
		{
			stop_received = 0;
			break;
		}
		
		/* update counts */
		samples_played += written;
		samples_left -= written;
	}

	/* close the device */
	fclose(dsp);

	

	/* return success */
	return(1);
}

FILE *
dsp_open(char *type,  int rate, int format, int channels)
{
	FILE *outptr;

	outptr = fopen("/dev/dsp", type);
	if (outptr == NULL) return NULL;
	
	set_card(fileno(outptr),rate, format,channels);

	return outptr;
}

#ifdef TESTING
main()
{
	FILE *outptr;
	char buffer[1024];
	int siz;

	outptr=fopen("/dev/dsp","w");
	if (outptr==NULL) printf("failed to open device\n");
	set_card(fileno(outptr), 4000, 8, 2);

/*	siz=set_frag(fileno(outptr)); */
	siz=1000;

	while (!feof(stdin))
	{
		fread(buffer, siz, 1, stdin);
		fwrite(buffer,siz,1,outptr);
	}
}
#endif
	
