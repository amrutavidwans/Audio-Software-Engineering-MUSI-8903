//
//  MeterComponent.h
//  VibratoPlugin
//
//  Created by Amruta Vidwans on 4/22/16.
//
//

#ifndef MeterComponent_h
#define MeterComponent_h


class MeterComponent: public Component
{
public:
    void paint(Graphics& g);
    void setValue(float val);
    float getPeakValue();
    float getMaxValue();
    MeterComponent();
    ~MeterComponent();
    
private:
    float m_fPeakVal;
    float m_fMaxPeakVal;
};

#endif /* MeterComponent_h */
