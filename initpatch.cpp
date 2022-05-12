#include "daisy_patch_sm.h"
#include "daisysp.h"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM hw;

void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    hw.ProcessAllControls();
    for(size_t i = 0; i < size; i++)
    {
        OUT_L[i] = IN_L[i];
        OUT_R[i] = IN_R[i];
    }
}

int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(4); // number of samples handled per callback
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
    hw.StartAudio(AudioCallback);
    hw.StartLog(false);
    //Configure and initialize button
    Switch button1;
    hw.PrintLine("Trying to access button...");
    //Set button to pin 28, to be updated at a 1kHz  samplerate
    button1.Init(hw.GetPin(DaisyPatchSM::PinBank::B, 8), 1000);

    // Loop forever
    for(;;)
    {
        //Debounce the button
        button1.Debounce();
        //If the button is pressed, turn the LED on
        hw.SetLed(button1.Pressed());
        //wait 1 ms
        System::Delay(1);
    }
}
