/*
  ==============================================================================

    AudioMixInstance.h
    Created: 4 May 2016 6:26:35am
    Author:  Milap Rane

  ==============================================================================
*/

#ifndef AUDIOMIXINSTANCE_H_INCLUDED
#define AUDIOMIXINSTANCE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class AudioMixInstance    : public MixerAudioSource
{
public:
    
    AudioMixInstance()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
    
        

    }

    ~AudioMixInstance()
    {
    }
    

    

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioMixInstance)
};


#endif  // AUDIOMIXINSTANCE_H_INCLUDED
