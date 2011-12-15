//
//  Audio.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/7/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "Audio.h"
#include "App.h"

extern App* gApp;

Minim::AudioOutput& Out()
{
    return gApp->Out();
}

Minim::Summer& Melody()
{
    return gApp->Melody();
}

Minim::Summer& Bass()
{
    return gApp->Bass();
}

Minim::Summer& Drums()
{
    return gApp->Drums();
}