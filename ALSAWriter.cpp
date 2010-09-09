#include "ALSAWriter.h"

#include <alsa/asoundlib.h>
#include "ALSAWriterImpl.h"

ALSAWriter *ALSAWriter::CreateALSAWriter()
{
	return new ALSAWriterImpl();
}

