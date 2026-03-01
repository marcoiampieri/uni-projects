void IV(){
    gStyle->SetPadGridX(true);
    gStyle->SetPadGridY(true);
    gStyle->SetPadTickX(true);
    gStyle->SetPadTickY(true);
    TGraphErrors* gr = new TGraphErrors("I_b_200muA.dat","%lg %lg %lg %lg");
    gr->SetTitle("Caratteristica I-V Transistor emettitore comune e I_{B}=200#muA;V_{CE} [mV]; I_{C} [mA]");
    gr->Draw("APE");
    

  
    

    double s1{0},sx{0},sy{0},sxx{0},sxy{0},d{0},min{100},max{-1};
    int NPoints{gr->GetN()};

    for(int i{0};i<NPoints-6;++i){
        double e = gr->GetErrorX(i);
        double y = gr->GetX()[i];
        double x = gr->GetY()[i];

        s1+=1/(e*e);
        sx+=x/(e*e);
        sy+=y/(e*e);
        sxx+=x*x/(e*e);
        sxy+=x*y/(e*e);

        if(x<min)min=x;
        if(x>max)max=x;

    }

    d=s1*sxx-sx*sx;

    TF1* fl = new TF1("fitl","-[0]/[1] + x/[1]",0,5000);
    fl->SetParameters((sy*sxx-sx*sxy)/d,(s1*sxy-sx*sy)/d);
    fl->SetLineColor(2);
    fl->Draw("SAME");
    //gr->Draw("SAMEPE");
    
    TLegend* leg = new TLegend(0.78, 0.85, .99, .99);
    leg->AddEntry(gr,"Punti sperimentali","lep");
    leg->AddEntry(fl,"Fit","l");
    leg->Draw("SAME");

    std::fstream in;
    in.open("fit200muA.txt",std::fstream::out);
    in<<"Linear fit method results:\n VA="<<(sy*sxx-sx*sxy)/d<<"+/-"<<sqrt(sxx/d)<<"mV   VCE/IC="<<(s1*sxy-sx*sy)/d<<"+/-"<<sqrt(s1/d)<<" V/A\ng="
    <<d/(s1*sxy-sx*sy)<<"+/-"<<d/(s1*sxy-sx*sy)*d/(s1*sxy-sx*sy)*sqrt(s1/d)<<"\n";
}