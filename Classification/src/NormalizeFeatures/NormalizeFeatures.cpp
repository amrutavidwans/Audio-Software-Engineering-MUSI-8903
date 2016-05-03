#include "NormalizeFeatures.h"

CNormalizeFeatures::CNormalizeFeatures(){
    
}
CNormalizeFeatures::~CNormalizeFeatures(){
    
}

void CNormalizeFeatures::createInstance(CNormalizeFeatures *&CNorm){
    CNorm = new CNormalizeFeatures();
}

void CNormalizeFeatures::destroyInstance(CNormalizeFeatures *&CNorm){
    delete CNorm;
    CNorm = 0;
}

void CNormalizeFeatures::normalize(float &featVal, SelectFeat_t kFeat){
    switch (kFeat) {
        case kTimeRMS:
            featVal = (featVal - m_fTimeRMSMin)/(m_fTimeRMSMax - m_fTimeRMSMin);
            break;
        case kSpecCentroid:
            featVal = (featVal - m_fSpecCenMin)/(m_fSpecCenMax - m_fSpecCenMin);
            break;
        default:
            break;
    }
}