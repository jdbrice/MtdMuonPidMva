


void extractHisto( TString fin, TString hn, TString nout="" ){

    TFile * f = new TFile( fin );
    TH1 * h = (TH1*)f->Get( hn );

    if ( h ){
        if ( "" == nout )
            nout = hn + ".root";
        TFile * fout = new TFile( nout, "RECREATE" );
        fout->cd();
        h->Write();
        fout->Close();
    }
}