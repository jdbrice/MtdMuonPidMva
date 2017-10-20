

double findEffBFor( double sEff, TH1D * effS, TH1D * effB ){

    double vClosest = 100;
    int iClosest = -1;
    for ( int i = 1; i < effS->GetNbinsX()+1; i++ ){
        double v = effS->GetBinContent( i );
        // cout << "v=" << v << "bin = " << i << endl;
        if ( fabs( sEff - v ) < vClosest ){
            iClosest = i;
            vClosest = fabs( sEff - v );
        }
    }
    // cout << "@effS = " << sEff << " bin = " << iClosest << ", vClosest = " << vClosest << endl;
    if ( iClosest > 0 ){

        return effB->GetBinContent( iClosest );
    }
    return 1.0;

}

void SQR_rejBvsS( TString fn = "dSQR.root" ){
    TFile *f = new TFile( fn );

    TFile * fout = new TFile( "sqr_rejBvsS.root", "RECREATE" );

    TH1D * effS = (TH1D*)f->Get("effS");
    TH1D * effB = (TH1D*)f->Get("effB");

    TH1D * hrejBvsS = new TH1D( "hrejBvsS", ";Signal eff; Background rej (1 - eff)", 100, 0, 1 );
    for ( int i = 1; i < 101; i++ ){
        double eff = i / 100.0;

        hrejBvsS->SetBinContent( i, 1 - findEffBFor( eff, effS, effB ) );
    }

    hrejBvsS->Draw();

    fout->Write();
}