#pragma once

#include "Result.h"
#include "ofPixels.h"

namespace ofxZxing {
    
	Result decode(ofPixels& pixels, bool adaptive);
    bool getCodeCorners(ofPixels& pixels, ofVec2f* points, ofVec2f* pointsTransformed);
    
}