#ifndef METRICS_H
#define METRICS_H

namespace casimiro {

class Metrics {
public:
    Metrics(int _position)
    {
        calculateMRR(_position);
        calculateSAt5(_position);
        calculateSAt10(_position);
    }
    
    virtual ~Metrics() {}
    
    virtual float MRR() const 
    {
        return m_MRR;
    }
    
    virtual float SAt5() const
    {
        return m_SAt5;
    }
    
    virtual float SAt10() const
    {
        return m_SAt10;
    }
    
private:
    float m_MRR;
    float m_SAt5;
    float m_SAt10;
    
    virtual void calculateMRR(int _position)
    {
        m_MRR = 1.0 / (_position);
    }
    
    virtual void calculateSAt5(int _position)
    {
        m_SAt5 = 0;
        if(_position <= 5)
            m_SAt5 = 1;
    }
    
    virtual void calculateSAt10(int _position)
    {
        m_SAt10 = 0;
        if(_position <= 10)
            m_SAt10 = 1;
    }
};

}
#endif // METRICS_H