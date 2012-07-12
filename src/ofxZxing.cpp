#include "ofxZxing.h"

#include "ofPixelsBitmapSource.h"

#include <zxing/qrcode/QRCodeReader.h>
#include <zxing/common/GlobalHistogramBinarizer.h>
#include <zxing/common/HybridBinarizer.h>
#include <zxing/common/DetectorResult.h>
#include <zxing/common/EdgeDetector.h>
#include <zxing/qrcode/detector/QREdgeDetector.h>
#include <zxing/Exception.h>
#include <zxing/ReaderException.h>
#include <zxing/qrcode/detector/Detector.h>

namespace ofxZxing {
    
    using namespace std;
    using namespace zxing;
    using namespace zxing::qrcode;
    
    
    // this needs another method to simply grab corners fast once
    // we've already decoded with some smarts about the previous locations
    bool getCodeCorners(ofPixels& pixels, ofVec2f* points, ofVec2f* pointsTransformed)
    {
        
        Ref<ofPixelsBitmapSource> source(new ofPixelsBitmapSource(pixels));
        
        Ref<Binarizer> binarizer(NULL);
        binarizer = new HybridBinarizer(source);
        Ref<BinaryBitmap> image(new BinaryBitmap(binarizer));
        //Detector detector(image->getBlackMatrix());
        
        Ref<ResultPointCallback> r;
        
        DecodeHints hints;
        hints.addFormat(BarcodeFormat_QR_CODE);
        hints.setResultPointCallback(r);
        hints.setTryHarder(true);
        
        //Ref<DetectorResult> result(detector.detect(hints));
        //Ref<DetectorResult> result = reader.decode(image, hints);
        
        Detector detector(image->getBlackMatrix());
        
        //std::vector<Ref<ResultPoint> > rpv = result->getPoints();
        //Ref<PerspectiveTransform> pt = result->getTransform();
        
        //Ref<ResultPoint> ul, ur, lr, align;
        //std::vector<Ref<ResultPoint> > rpv;
        //detector.createTransform(ul, ur, lr, align, 3);
        
        try {
            Ref<DetectorResult> detectorResult(detector.detect(hints));
            
            for( int i = 0; i < detectorResult->getPoints().size(); i++) {
                points[i].x = detectorResult->getPoints()[i]->getX();
                points[i].y = detectorResult->getPoints()[i]->getY();
            }
            
            Ref<PerspectiveTransform> p = detectorResult->getTransform();
            
            float px1, py1, px2, py2, px3, py3, px4, py4;
            
            p->quadrilateralToQuadrilateral(points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y, points[3].x, points[3].y, pointsTransformed[0].x, pointsTransformed[0].y, pointsTransformed[1].x, pointsTransformed[1].y, pointsTransformed[2].x, pointsTransformed[2].y, pointsTransformed[3].x, pointsTransformed[3].y);
            
        } catch( ReaderException readEx ) {
            cout << readEx.what() << endl;
            return false;
        }
        
        //if(rpv.size() < 1) {
        //    return false; // can't find anything
        //}
        
        // now convert to OF
        //for(int i = 0; i < 4; i++) {
        //    if(points[i] != NULL) {
        //        points[i].x = rpv[i]->getX();
        //        points[i].y = rpv[i]->getY();
        //    }
        //}
        
        /*
         vector<float> ptsf; // to be the transformed points
         ptsf.resize(12); // we'll need 12 spots
         for(int i = 0; i < 4; i++) {
         ptsf[i*3] = pointsTransformed[i].x;
         ptsf[i*3+1] = pointsTransformed[i].y;
         }
         
         pt->transformPoints(ptsf);
         
         // now back to OF
         for(int i = 0; i < 4; i++) {
         pointsTransformed[i].set(ptsf[i*3],  ptsf[i*3+1]);
         }
         */
        return true;
        
    }
    
    Result decode(ofPixels& pixels, bool adaptive = true) {
        try {
            Ref<ofPixelsBitmapSource> source(new ofPixelsBitmapSource(pixels));
            
            Ref<Binarizer> binarizer(NULL);
            if(adaptive) {
                binarizer = new HybridBinarizer(source);
            } else {
                binarizer = new GlobalHistogramBinarizer(source);
            }
            
            Ref<BinaryBitmap> image(new BinaryBitmap(binarizer));
            QRCodeReader reader;
            DecodeHints hints;
            hints.addFormat(BarcodeFormat_QR_CODE);
            hints.setTryHarder(true);
            Ref<zxing::Result> result(reader.decode(image, hints));
            
            string text = result->getText()->getText();
            vector<ofVec2f> points;
            vector< Ref<ResultPoint> > resultPoints = result->getResultPoints();
            for(int i = 0; i < resultPoints.size(); i++) {
                Ref<ResultPoint> cur = resultPoints[i];
                points.push_back(ofVec2f(cur->getX(), cur->getY()));
            }
            return Result(text, points);
        } catch (zxing::Exception& e) {
            //cerr << "Error: " << e.what() << endl;
            return Result();
        }
    }
    
}