#pragma once

#include "Result.h"
#include "ofPixels.h"

namespace ofxZxing {
    
    Result decode(const unsigned char **pixels, int width, int height, bool adaptive);
	Result decode(ofPixels& pixels, bool adaptive);
    bool getCodeCorners(ofPixels& pixels, ofVec2f* points, ofVec2f* pointsTransformed);
    
}