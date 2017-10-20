
#include <vector>
std::vector<TH1D*> hs;

TH1D * plotOne( float sf, int iH, TH1D * h ){

}


void maxSignificance( TString fn = "mvaMPL_BDT_nTrain0.root", TString hn = "Method_BDT/BDT/MVA_BDT_rejBvsS" ){

    TFile *f = new TFile( fn );
    TH1 * h = (TH1*)f->Get( hn );
    TCanvas * can = new TCanvas( "can", "can", 16 * 100, 9 * 100 );
    // TH1D * hh = new TH1D( "hSig", "", h->GetNbinsX(), 0, 1 );
    // cout << "h="<<h<< endl;
    TLatex l1;
    l1.SetTextSize( 0.03 );
    gStyle->SetOptStat( 0 );
    int iH = 0;
    float sf = 0.1;
    for ( int iH = 0; iH < 101; iH++ ){
        
        TString hn = TString::Format( "%s_%d", "hSig", iH );
        // cout << "hn=" << hn << endl;
        TH1D * hh = new TH1D( hn, "; signal eff; S / #sqrt{S+B}", h->GetNbinsX(), 0, 1 );
        hs.push_back( hh );

        for ( int i = 1; i < h->GetNbinsX()+1; i++ ){
            double S = h->GetBinCenter( i );
            double B = (1.0 - h->GetBinContent( i )) * sf;

            // cout << "@S=" << S << ": S/sqrt(S+B) = " << (S/sqrt(S+B)) << endl;

            hh->SetBinContent( i, (S/sqrt(S+B)) );
        }

        int iMax = hh->GetMaximumBin();
        double maxX = hh->GetBinCenter(iMax);
        double maxY = hh->GetBinContent(iMax);

        hh->SetLineColor( kBlack );
        hh->SetLineWidth(1.0);

        if (0 == iH)
            hh->Draw();
        else 
            hh->Draw("same");

        gPad->SetTopMargin( 0.01 );
        gPad->SetRightMargin( 0.2 );

        TEllipse *el1 = new TEllipse(maxX,maxY, 0.01);
        el1->SetFillColor(2);
        el1->SetLineColor(2);
        el1->SetFillStyle(1001);
        el1->Draw();

        if ( 0 == iH || iH == 13 || iH == 49 || iH == 70 ){
            TString msg = TString::Format( "nBg = %.1f*nSignal", sf );
            l1.DrawLatexNDC( 0.81, maxY, msg );
        }
        cout << "iH["<<iH<<"]sf = " << sf << endl;
        sf *= 1.1;
    }


    l1.SetTextSize( 0.05 );
    l1.DrawLatexNDC( 0.15, 0.9, "Method: BDT " );
    l1.SetTextColor( kRed )
    l1.DrawLatexNDC( 0.15, 0.85, "#color[2]{Maximum shown in red}" );
    
    
    can->Print( "plot_MaxSignificance_BDT.pdf" );
    can->Print( "plot_MaxSignificance_BDT.png" );
    

}