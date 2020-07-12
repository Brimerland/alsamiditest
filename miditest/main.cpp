#include <stdio.h>
#include <alsa/asoundlib.h>

int main (int argc, char ** argv)
{
    printf("midi test start\n");
    snd_rawmidi_t *handle_midi_in = nullptr;
    snd_rawmidi_t *handle_midi_out = nullptr;


    auto err = snd_rawmidi_open(&handle_midi_in, &handle_midi_out, "hw:3,0,0", 0 );
    printf("Err: %i\n", err);

    bool cmd = false;
    int count = 0;
    unsigned char *buffer = new unsigned char[1024];
    int writeOffset = 0;
    int readOffset = 0;
    while (1)
    {
        writeOffset = writeOffset % 1024;
        readOffset = readOffset % 1024;
        unsigned char ch;
        snd_rawmidi_read(handle_midi_in, &ch, 1);
        if (ch == 0xfe)
          continue;
        
        cmd = ch > 0x7f;
        if (cmd && count > 0)
        {
            printf("\n");
            count = 0;
        }
        
        count++;
          

        printf("%x ", ch);
        if (count>2)
        {
          printf("\n");
          count = 0;
        }

        *(buffer + writeOffset) = ch;
        writeOffset++;
        if (abs(writeOffset - readOffset) >= 3*0)
        {
            snd_rawmidi_write(handle_midi_out, buffer + readOffset, 1);
            readOffset++;
        }

    }

    printf("End.\n");
    return 0;
}