#ifndef MVA_H
#define MVA_H


#include "HistoAnalyzer.h"
#include "XmlRange.h"

#include "TMVA/Config.h"
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"

#include "vendor/loguru.h"

#include "TNamed.h"


template <>
TString XmlConfig::get<TString>( string path ) const {
	// LOG_F( INFO, "Getting TString( %s ) = %s", path.c_str(), getString( path ).c_str() );
	TString r( getString( path ) );
	return r;
}

template <>
TString XmlConfig::get<TString>( string path, TString dv ) const {
	if ( !exists( path ) )
		return dv;
	// LOG_F( INFO, "Getting TString( %s ) = %s", path.c_str(), getString( path ).c_str() );
	TString r( getString( path ) );
	return r;
}

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


		
		
		// factory->AddVariable( "dY := MtdPidTraits[Tracks.mMtdPidTraitsIndex].mDeltaY", "MTD DeltaY", "cm", 'F' );
		// factory->AddVariable( "dZ := MtdPidTraits[Tracks.mMtdPidTraitsIndex].mDeltaZ", "MTD DeltaZ", "cm", 'F' );
		// factory->AddVariable( "cell := (MtdPidTraits_mMtdHitChan )", "MTD cell", "", 'I' );
		
		// factory->AddVariable( "dY := ((65.0+Tracks_mCharge*MtdPidTraits_mDeltaY) + 130 * (MtdPidTraits_mCell))", "MTD DeltaY", "cm", 'F' );
		// factory->AddVariable( "dZ := (100.0+MtdPidTraits_mDeltaZ) + 200 * (MtdPidTraits_mCell)", "MTD DeltaZ", "cm", 'F' );


		// LIKELIHOOD
		// factory->AddVariable( "dY := (Tracks_mCharge*MtdPidTraits_mDeltaY)", "MTD DeltaY", "cm", 'F' );
		// factory->AddVariable( "dZ := MtdPidTraits_mDeltaZ", "MTD DeltaZ", "cm", 'F' );
		// factory->AddVariable( "dEdx := Tracks_mDedx", "dEdx", "KeV/cm", 'F');
		// factory->AddVariable( "nSigmaPi := Tracks_mNSigmaPion", "n #sigma #pi", "", 'F');
		// factory->AddVariable( "nh := Tracks_mNHitsFit", "N Hits Fit", "", 'I' );
		// factory->AddVariable( "dca := Tracks_mDCA", "DCA", "cm", 'F' );


		vector<string> active_vars = config.getStringVector( "vars" );
		LOG_F( INFO, "active variables = [%s]", vts( active_vars ).c_str() );

		// MLP
		if ( find( active_vars.begin(), active_vars.end(), "dY" ) != active_vars.end() )
			factory->AddVariable( "dY := (MtdPidTraits_mDeltaY * Tracks_mCharge)", "MTD DeltaY", "cm", 'F' );
		if ( find( active_vars.begin(), active_vars.end(), "dZ" ) != active_vars.end() )
			factory->AddVariable( "dZ := MtdPidTraits_mDeltaZ", "MTD DeltaZ", "cm", 'F' );
		if ( find( active_vars.begin(), active_vars.end(), "nSigmaPi" ) != active_vars.end() )
			factory->AddVariable( "nSigmaPi := Tracks_mNSigmaPion", "n #sigma #pi", "", 'F');
		// factory->AddVariable( "nh := Tracks_mNHitsFit", "N Hits Fit", "", 'I' );
		if ( find( active_vars.begin(), active_vars.end(), "dca" ) != active_vars.end() )
			factory->AddVariable( "dca := Tracks_mDCA", "DCA", "cm", 'F' );
		if ( find( active_vars.begin(), active_vars.end(), "Cell" ) != active_vars.end() )
			factory->AddVariable( "Cell := ((MtdPidTraits_mCell+1) * Tracks_mCharge)", "MTD Cell", "", 'I' );
		if ( find( active_vars.begin(), active_vars.end(), "Module" ) != active_vars.end() )
			factory->AddVariable( "Module := MtdPidTraits_mModule", "MTD Module", "", 'I' );
		if ( find( active_vars.begin(), active_vars.end(), "BL" ) != active_vars.end() )
			factory->AddVariable( "BL := MtdPidTraits_mBL", "MTD BL", "", 'I' );
		if ( find( active_vars.begin(), active_vars.end(), "pT" ) != active_vars.end() )
			factory->AddVariable( "pT := Tracks_mPt", "p_{T}", "GeV/c", 'F' );
		if ( find( active_vars.begin(), active_vars.end(), "charge" ) != active_vars.end() )
			factory->AddVariable( "charge := Tracks_mCharge", "q", "", 'I' );
		if ( find( active_vars.begin(), active_vars.end(), "dTOF" ) != active_vars.end() )
			factory->AddVariable( "dTOF := MtdPidTraits_mDeltaTOF", "MTD DeltaTOF", "ns", 'F' );
		

		// factory->AddVariable( "dY := ((65.0+Tracks_mCharge*MtdPidTraits_mDeltaY) + 130 * (MtdPidTraits_mCell))", "MTD DeltaY", "cm", 'F' );
		// factory->AddVariable( "dZ := (100.0+MtdPidTraits_mDeltaZ) + 200 * (MtdPidTraits_mCell)", "MTD DeltaZ", "cm", 'F' );
		
		

		// factory->AddVariable( "Cell := MtdPidTraits_mCell", "MTD Cell", "", 'I' );
		// factory->AddVariable( "Module := MtdPidTraits_mModule", "MTD Module", "", 'I' );
		// factory->AddVariable( "BL := MtdPidTraits_mBL", "MTD BL", "", 'I' );
		

		(TMVA::gConfig().GetVariablePlotting()).fNbins1D = 300.0;

		Double_t signalWeight     = 1.0;
   		Double_t backgroundWeight = 1.0;
		
		factory->AddSignalTree    ( signal_tree,     signalWeight     );
   		factory->AddBackgroundTree( pion_bg_tree, backgroundWeight );

   		LOG_F( INFO, "CUTS: %s", config.getString( "Prepare:cuts" ).c_str()  );


		TCut precuts = TCut( config.get<TString>( "Prepare:cuts", "" ) );
		TString preopts = config.get<TString>( "Prepare.opts" );
		
		LOG_F( INFO, "Prepare(\"%s\")", preopts.Data() );
		LOG_F( INFO, "Pre Cuts(\"%s\")", config.get<TString>( "Prepare:cuts", "" ).Data() );
		factory->PrepareTrainingAndTestTree( precuts, preopts );

		if ( config.getBool( "Methods.Likelihood:enable", true ) ){
			LOG_F( INFO, "Likelihood(\"%s\") ", config.get<TString>( "Methods.Likelihood:opts" ).Data() );
			factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood", config.get<TString>( "Methods.Likelihood:opts" ) );
		}

		if ( config.getBool( "Methods.MLP:enable", true ) ){
			LOG_F( INFO, "MLP(\"%s\") ", config.get<TString>( "Methods.MLP:opts" ).Data() );
			factory->BookMethod( TMVA::Types::kMLP, "MLP_" + config.getString( "mod" ), config.get<TString>( "Methods.MLP:opts" ) );
		}

		if ( config.getBool( "Methods.BDT:enable", true ) ){
			LOG_F( INFO, "BDT(\"%s\") ", config.get<TString>( "Methods.BDT:opts" ).Data() );
			factory->BookMethod( TMVA::Types::kBDT, "BDT_" + config.getString( "mod" ), config.get<TString>( "Methods.BDT:opts" ) );
		}

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