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

		TTree * signal_tree = get<TTree>( "MvaDst", "signal" );
		TTree * pion_bg_tree = get<TTree>( "MvaDst", "pion_bg" );

		LOG_F( INFO, "signal tree = %p", signal_tree );
		LOG_F( INFO, "pion_bg tree = %p", pion_bg_tree );


		// factory->AddVariable( "pT := Tracks.mPt", "p_{T}", "GeV/c", 'F' );
		// factory->AddVariable( "charge := Tracks.mNHitsFit / abs( Tracks.mNHitsFit )", "q", "", 'I' );
		// factory->AddVariable( "dY := MtdPidTraits[Tracks.mMtdPidTraitsIndex].mDeltaY", "MTD DeltaY", "cm", 'F' );
		// factory->AddVariable( "dZ := MtdPidTraits[Tracks.mMtdPidTraitsIndex].mDeltaZ", "MTD DeltaZ", "cm", 'F' );
		// factory->AddVariable( "cell := (MtdPidTraits.mMtdHitChan )", "MTD cell", "", 'I' );

		factory->AddVariable( "dY := Tracks_mCharge * ((65.0+MtdPidTraits_mDeltaY) + 130 * (MtdPidTraits_mCell))", "MTD DeltaY", "cm", 'F' );
		// factory->AddVariable( "dZ := (100.0+MtdPidTraits_mDeltaZ) + 200 * (MtdPidTraits_mCell)", "MTD DeltaZ", "cm", 'F' );
		// factory->AddVariable( "dZ := MtdPidTraits_mDeltaZ", "MTD DeltaZ", "cm", 'F' );
		// factory->AddVariable( "Cell := MtdPidTraits_mCell", "MTD Cell", "", 'I' );
		// factory->AddVariable( "Module := MtdPidTraits_mDeltaZ", "MTD Module", "", 'I' );
		// factory->AddVariable( "BL := MtdPidTraits_mBL", "MTD BL", "", 'I' );
		


		Double_t signalWeight     = 1.0;
   		Double_t backgroundWeight = 1.0;
		
		factory->AddSignalTree    ( signal_tree,     signalWeight     );
   		factory->AddBackgroundTree( pion_bg_tree, backgroundWeight );


		TCut precuts = "";
		// TCut preselect_signal = "Tracks.mPt > 1.0 && Tracks.mPt < 2.0";
		// TCut preselect_bg = "Tracks.mPt > 1.0 && Tracks.mPt < 2.0";
		int nTrain = config.get<int>( "Prepare.nTrain" );
		int nTest = config.get<int>( "Prepare.nTest" );
		TString splitMode = config.get<string>( "Prepare.SplitMode", "Random" );
		TString normMode = config.get<string>( "Prepare.NormMode", "NumEvents" );

		TString preopts = "nTrain_Signal=" + ts(nTrain) + ":nTrain_Background=" + ts( nTrain ) + ":SplitMode=" + splitMode + ":NormMode=" + normMode + ":V";
		preopts = config.get<TString>( "Prepare.opts", preopts );
		LOG_F( INFO, "Prepare(\"%s\")", preopts.Data() );
		factory->PrepareTrainingAndTestTree( precuts, preopts );

		if ( config.exists( "Methods.Likelihood" ) ){
			LOG_F( INFO, "Likelihood(\"%s\") ", config.get<TString>( "Methods.Likelihood:opts" ).Data() );
			factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood", config.get<TString>( "Methods.Likelihood:opts" ) );
		}



		// factory->BookMethod( TMVA::Types::kCuts, "Cuts", "H:V" );
		// factory->BookMethod( TMVA::Types::kPDERS, "PDERS", "H:VolumeRangeMode=Unscaled" );

		
		// factory->BookMethod( TMVA::Types::kBDT, "BDT",
        //    "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
		// factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );

		// very poorly : factory->BookMethod( TMVA::Types::kLD, "LD", "H:!V:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );
		// not working : factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoamBoost","!H:!V:Boost_Num=30:Boost_Transform=linear:SigBgSeparate=F:MaxDepth=4:UseYesNoCell=T:DTLogic=MisClassificationError:FillFoamWithOrigWeights=F:TailCut=0:nActiveCells=500:nBin=20:Nmin=400:Kernel=None:Compress=T" );
		// very poorly : factory->BookMethod( TMVA::Types::kKNN, "KNN","H:nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );
		// very poorly : factory->BookMethod( TMVA::Types::kFisher, "BoostedFisher", "H:!V:Boost_Num=20:Boost_Transform=log:Boost_Type=AdaBoost:Boost_AdaBoostBeta=0.2:!Boost_DetailedMonitoring" );
		
		
		// Train MVAs using the set of training events
		factory->TrainAllMethods();

		// ---- Evaluate all MVAs using the set of test events
		factory->TestAllMethods();

		// ----- Evaluate and compare performance of all configured MVAs
		factory->EvaluateAllMethods();

	}

};


#endif