//
//  Scales.h
//  melodizer
//
//  Created by Damien Di Fede on 12/9/11.
//  Copyright (c) 2011 Damien Di Fede. All rights reserved.
//

#ifndef melodizer_Scales_h
#define melodizer_Scales_h

struct Scale
{
    Scale( const char* scaleName, int** scaleDegrees, int numScaleDegrees )
    : scaleName( scaleName )
    , scale( scaleDegrees )
    , scaleLength( numScaleDegrees )
    {
        
    }
    
    const char *  scaleName;
    const int* const * const scale;
    const int scaleLength;   
};

extern const Scale* const Scales[]; 
extern const int		  ScalesLength; 

#endif
