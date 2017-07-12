#ifndef MVA_H
#define MVA_H


#include "HistoAnalyzer.h"
#include "XmlRange.h"

#include "TMVA/Factory.h"
#include "TMVA/Tools.h"

#include "vendor/loguru.h"



class MVA : public HistoAnalyzer
{
public:

	const int DEBUG = 1;
	MVA() {}
	~MVA() {}

	virtual void initialize(){
		HistoAnalyzer::initialize();
	}
protected:

	virtual void make() {

		TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", book->getOutputFile(), "!V:!Silent:Color:DrawProgressBar:Transformations=I:AnalysisType=Classification" );

		TTree * signal_tree = get<TTree>( "FemtoDst", "signal" );
		TTree * pion_bg_tree = get<TTree>( "FemtoDst", "pion_bg" );

		LOG_F( INFO, "signal tree = %p", signal_tree );
		LOG_F( INFO, "pion_bg tree = %p", pion_bg_tree );


		// factory->AddVariable( "pT := Tracks.mPt", "p_{T}", "GeV/c", 'F' );
		// factory->AddVariable( "charge := Tracks.mNHitsFit / abs( Tracks.mNHitsFit )", "q", "", 'I' );
		factory->AddVariable( "dY := MtdPidTraits[Tracks.mMtdPidTraitsIndex].mDeltaY", "MTD DeltaY", "cm", 'F' );
		factory->AddVariable( "dZ := MtdPidTraits[Tracks.mMtdPidTraitsIndex].mDeltaZ", "MTD DeltaZ", "cm", 'F' );
		factory->AddVariable( "cell := (MtdPidTraits.mMtdHitChan )", "MTD cell", "", 'I' );


		Double_t signalWeight     = 1.0;
   		Double_t backgroundWeight = 1.0;
		
		factory->AddSignalTree    ( signal_tree,     signalWeight     );
   		factory->AddBackgroundTree( pion_bg_tree, backgroundWeight );


		TCut preselect_signal = "Tracks.mPt > 1.0 && Tracks.mPt < 2.0";
		TCut preselect_bg = "Tracks.mPt > 1.0 && Tracks.mPt < 2.0";
		factory->PrepareTrainingAndTestTree( preselect_signal, preselect_bg, "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:V" );

		// factory->BookMethod( TMVA::Types::kCuts, "Cuts", "H:V" );

		factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood","H:!V:TransformOutput:PDFInterpol=Spline2:NAvEvtPerBin=1000:VarTransform=I" );
		// factory->BookMethod( TMVA::Types::kBDT, "BDT",
                        //    "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

		// Train MVAs using the set of training events
		factory->TrainAllMethods();

		// ---- Evaluate all MVAs using the set of test events
		factory->TestAllMethods();

		// ----- Evaluate and compare performance of all configured MVAs
		factory->EvaluateAllMethods();

	}

};


#endif