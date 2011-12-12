//
//  Scales.h
//  melodizer
//
//  Created by Damien Di Fede on 12/9/11.
//  Copyright (c) 2011 Damien Di Fede. All rights reserved.
//

#ifndef melodizer_Scales_h
#define melodizer_Scales_h

#include "ofMain.h"

struct Scale
{
    Scale( const char* scaleName, int** scaleDegrees, int numScaleDegrees )
    : scaleName( scaleName )
    , scale( scaleDegrees )
    , scaleLength( numScaleDegrees )
    {
        
    }
    
    const char *  scaleName;
    int**         scale;
    int           scaleLength;  
    
    int length() const { return scaleLength; }
    
    int nextNoteIndex( int fromNote ) const
    {
        int  listLen = 1;
        int* nextNoteList = scale[fromNote];
        while( nextNoteList[listLen] != -1 )
        {
            ++listLen;
        }
        
        return nextNoteList[ (int)ofRandom(1, listLen) ];
    }
};

extern const Scale* Scales[]; 
extern const int    ScalesLength; 

#endif
