//
//  Audio.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/7/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "Audio.h"
#include "App.h"

Minim::AudioOutput& Audio::Out()
{
    return App::Instance().Out();
}

Minim::Summer& Audio::Melody()
{
    return App::Instance().Melody();
}

Minim::Summer& Audio::Bass()
{
    return App::Instance().Bass();
}

int Audio::CurrentTick()
{
    return App::Instance().currentTick();
}

void Audio::ToneBegan( Minim::Summer* onBus, const int onTick )
{
    App::Instance().toneBegan(onBus, onTick);
}