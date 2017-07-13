#ifndef MVA_H
#define MVA_H


#include "HistoAnalyzer.h"
#include "XmlRange.h"

#include "TMVA/Factory.h"
#include "TMVA/Tools.h"

#include "vendor/loguru.h"

#include "TNamed.h"

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

		TTree * signal_tree = get<TTree>( "MvaDst", "signal" );
		TTree * pion_bg_tree = get<TTree>( "MvaDst", "pion_bg" );

		LOG_F( INFO, "signal tree = %p", signal_tree );
		LOG_F( INFO, "pion_bg tree = %p", pion_bg_tree );


		// factory->AddVariable( "pT := Tracks.mPt", "p_{T}", "GeV/c", 'F' );
		// factory->AddVariable( "charge := Tracks.mNHitsFit / abs( Tracks.mNHitsFit )", "q", "", 'I' );
		// factory->AddVariable( "dY := MtdPidTraits[Tracks.mMtdPidTraitsIndex].mDeltaY", "MTD DeltaY", "cm", 'F' );
		// factory->AddVariable( "dZ := MtdPidTraits[Tracks.mMtdPidTraitsIndex].mDeltaZ", "MTD DeltaZ", "cm", 'F' );
		// factory->AddVariable( "cell := (MtdPidTraits_mMtdHitChan )", "MTD cell", "", 'I' );
		
		factory->AddVariable( "dY := ((65.0+Tracks_mCharge*MtdPidTraits_mDeltaY) + 130 * (MtdPidTraits_mCell))", "MTD DeltaY", "cm", 'F' );
		factory->AddVariable( "dZ := (100.0+MtdPidTraits_mDeltaZ) + 200 * (MtdPidTraits_mCell)", "MTD DeltaZ", "cm", 'F' );
		factory->AddVariable( "nh := Tracks_mNHitsFit", "N Hits Fit", "", 'I' );
		factory->AddVariable( "dca := Tracks_mDCA", "DCA", "cm", 'F' );
		// factory->AddVariable( "dZ := MtdPidTraits_mDeltaZ", "MTD DeltaZ", "cm", 'F' );
		// factory->AddVariable( "Cell := MtdPidTraits_mCell", "MTD Cell", "", 'I' );
		// factory->AddVariable( "Module := MtdPidTraits_mDeltaZ", "MTD Module", "", 'I' );
		// factory->AddVariable( "BL := MtdPidTraits_mBL", "MTD BL", "", 'I' );
		


		Double_t signalWeight     = 1.0;
   		Double_t backgroundWeight = 1.0;
		
		factory->AddSignalTree    ( signal_tree,     signalWeight     );
   		factory->AddBackgroundTree( pion_bg_tree, backgroundWeight );


		TCut precuts = TCut( config.get<TString>( "Prepare:cuts", "" ) );
		TString preopts = config.get<TString>( "Prepare.opts" );
		
		LOG_F( INFO, "Prepare(\"%s\")", preopts.Data() );
		LOG_F( INFO, "Pre Cuts(\"%s\")", config.get<TString>( "Prepare:cuts", "" ).Data() );
		factory->PrepareTrainingAndTestTree( precuts, preopts );

		if ( config.exists( "Methods.Likelihood" ) ){
			LOG_F( INFO, "Likelihood(\"%s\") ", config.get<TString>( "Methods.Likelihood:opts" ).Data() );
			factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood", config.get<TString>( "Methods.Likelihood:opts" ) );
		}

		factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );

		// Train MVAs using the set of training events
		factory->TrainAllMethods();

		// ---- Evaluate all MVAs using the set of test events
		factory->TestAllMethods();

		// ----- Evaluate and compare performance of all configured MVAs
		factory->EvaluateAllMethods();



		TNamed config_str( "config", config.toXml() );
		config_str.Write();

	}

};


#endif