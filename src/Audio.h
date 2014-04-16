//
//  Audio.h
//  melodizer
//
//  Created by Damien Di Fede on 12/7/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef melodizer_Audio_h
#define melodizer_Audio_h

namespace Minim 
{
    class AudioOutput;
    class Summer;
}

namespace Audio
{

    Minim::AudioOutput& Out();
    Minim::Summer&      Melody();
    Minim::Summer&      Bass();
    int                 CurrentTick();
    void                ToneBegan( Minim::Summer* onBus, const int onTick );
}

#endif
