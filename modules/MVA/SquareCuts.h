#ifndef SQUARE_CUTS_H
#define SQUARE_CUTS_H

#include "TreeAnalyzer.h"
#include "ComputedRange.h"

#include "FemtoDstFormat/BranchReader.h"
#include "MvaDstFormat/TrackHeap.h"

#define LOGURU_IMPLEMENTATION 0
#include "vendor/loguru.h"



class SquareCuts : public TreeAnalyzer {

    protected:
    BranchReader<TrackHeap> tr;

    ComputedRange cutRange_dY;
    ComputedRange cutRange_dZ;
    ComputedRange cutRange_nSigmaPi;
    ComputedRange cutRange_nHitsFit;
    ComputedRange cutRange_dca;
    public:
        SquareCuts() {}
        ~SquareCuts() {}


        virtual void initialize(  ){
            TreeAnalyzer::initialize();
            
            tr.setup( chain, "" );

            cutRange_dY       = config.get<ComputedRange>( "CutRanges.DeltaY" );
            cutRange_dZ       = config.get<ComputedRange>( "CutRanges.DeltaZ" );
            cutRange_nSigmaPi = config.get<ComputedRange>( "CutRanges.nSigmaPion" );
            cutRange_nHitsFit = config.get<ComputedRange>( "CutRanges.nHitsFit" );
            cutRange_dca      = config.get<ComputedRange>( "CutRanges.DCA" );
            
            LOG_F( INFO, "DeltaY Range = %s", cutRange_dY.toString().c_str() );
            LOG_F( INFO, "DeltaZ Range = %s", cutRange_dZ.toString().c_str() );
            LOG_F( INFO, "nSigmaPi Range = %s", cutRange_nSigmaPi.toString().c_str() );
            LOG_F( INFO, "nHitsFit Range = %s", cutRange_nHitsFit.toString().c_str() );
            LOG_F( INFO, "DCA Range = %s", cutRange_dca.toString().c_str() );

        }
    protected:

    virtual void preEventLoop(){
        TreeAnalyzer::preEventLoop();

        LOG_F( INFO, "iEventLoop=%d", iEventLoop );

        cutRange_dY.set_t( (iEventLoop / 100.0) );
        cutRange_dZ.set_t( (iEventLoop / 100.0) );
        cutRange_nSigmaPi.set_t( (iEventLoop / 100.0) );
        cutRange_nHitsFit.set_t( (iEventLoop / 100.0) );
        cutRange_dca.set_t( (iEventLoop / 100.0) );

        LOG_F( INFO, "DeltaY Range = %s", cutRange_dY.toString().c_str() );
        LOG_F( INFO, "DeltaZ Range = %s", cutRange_dZ.toString().c_str() );
        LOG_F( INFO, "nSigmaPi Range = %s", cutRange_nSigmaPi.toString().c_str() );
        LOG_F( INFO, "nHitsFit Range = %s", cutRange_nHitsFit.toString().c_str() );
        LOG_F( INFO, "DCA Range = %s", cutRange_dca.toString().c_str() );

    }
    virtual void analyzeEvent(){
        
        TrackHeap * th = tr.get();
        // LOG_F( INFO, "mPt=%f", th->Tracks_mPt );


        book->fill( "TotalEvents", iEventLoop );

        if ( !cutRange_dY.inInclusiveRange( th->MtdPidTraits_mDeltaY ) )
            return;
        if ( !cutRange_dZ.inInclusiveRange( th->MtdPidTraits_mDeltaZ ) )
            return;

        book->fill( "PassEvents", iEventLoop );

    }

};


#endif