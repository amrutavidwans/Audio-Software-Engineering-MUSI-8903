#if !defined(__NormalizeFeatures_hdr__)
#define __NormalizeFeatures_hdr__

class CNormalizeFeatures{
    
public:
    enum SelectFeat_t{
        kTimeRMS,
        kSpecCentroid,
        
        kNumFeat
    };
    
    void normalize(float &featVal, SelectFeat_t kFeat);
    
    static void createInstance(CNormalizeFeatures *&CNorm);
    static void destroyInstance(CNormalizeFeatures *&CNorm);
    
protected:
    CNormalizeFeatures();
    virtual ~CNormalizeFeatures();
  
private:
    float m_fTimeRMSMin = 0.00025602;
    float m_fTimeRMSMax = 21692.0;
    float m_fSpecCenMin = 0.00000067435;
    float m_fSpecCenMax = 0.3791;
    
};

#endif