#ifndef COMPUTED_RANGE_H
#define COMPUTED_RANGE_H

#include "XmlRange.h"




class ComputedRange : public XmlRange {

protected:
    
    
    
    // input
    double interval_min, interval_max;
    double interval_zero;
    double t;

    public:
    ComputedRange() {
        min = 0;
        max = 0;
        interval_min = 0;
        interval_max = 0;
        interval_zero = 0;
        t = 0;

    }

    virtual void set_interval( double _interval_zero, double _interval_min, double _interval_max ){
        this->interval_zero = _interval_zero;
        this->interval_min  = _interval_min;
        this->interval_max  = _interval_max;
        update();
    }

    virtual void set_t( double _t ){
        this->t = _t;
        update();
    }
    protected:

    void update(){
        this->min = this->interval_zero + this->t * (this->interval_min - this->interval_zero);
        this->max = this->interval_zero + this->t * (this->interval_max - this->interval_zero);
    }

};

template <>
const ComputedRange XmlConfig::get( string path ) const {
    ComputedRange cr;
    cr.set_interval( get<double>( path + ":zero", 0.0 ), get<double>( path + ":min", -1.0 ), get<double>( path + ":max", 1.0 ) );
    cr.set_t( get<double>( path+":t", 1.0 ) );
    return cr;
}


#endif