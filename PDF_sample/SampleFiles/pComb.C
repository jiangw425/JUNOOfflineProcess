
const int qBinN = 2;
const int dBinN = 200;
const int tBinN = 400;

const double qRange = 10.;
const double dRange = 40.;
const double tRange = 600.;

const int rBinN = 35;

const int CalibPosNum = 293;
const string source[2] = {"Ge68", "Laser0.05"}; //68Ge, Laser
const string pmttype[2] = {"dyn", "mcp"}; 
const string datapath = "..";
const string posfile  = "/junofs/users/huanggh/Data/ACU_CLS_3D_Pos/ACU-CLS-GT_XYZPos.txt";


void pComb()
{   

    for(int src=0;src<2;src++) {
        for(int pmt=0;pmt<2;pmt++) {
            TH2D* RsdTimeFuncRec[rBinN][qBinN];
            for(int rk=0;rk<rBinN;rk++) {
                for(int i=0;i<qBinN;i++) {
                    RsdTimeFuncRec[rk][i] = new TH2D(Form("RsdTimeFuncRec_%d_%d", rk, i), Form("RsdTimeFuncRec_%d_%d", rk, i), dBinN, 0., dRange, tBinN, 0., tRange);
                }
            }

            ifstream TimePdfIn;

            double BinTotalRec[rBinN][qBinN][dBinN];
            double* RECvsTRUTHRec = (double*)malloc(sizeof(double)*rBinN*qBinN*dBinN*tBinN);

            for(int rk=0;rk<rBinN;rk++) {
                for(int qk=0;qk<qBinN;qk++) {
                    for(int dk=0;dk<dBinN;dk++) {
                        BinTotalRec[rk][qk][dk] = 0.;
                        for(int tk=0;tk<tBinN;tk++) {
                            *(RECvsTRUTHRec + rk*qBinN*dBinN*tBinN  + qk*dBinN*tBinN + dk*tBinN + tk) = 0.;
                        }
                    }
                }
            }

            
            double R[CalibPosNum];
            double idtemp, xtemp, ytemp, ztemp;
            ifstream Rdatain;
            Rdatain.open(posfile.c_str(), ios::in);

            for(int i=0;i<CalibPosNum;i++) {
                Rdatain>>idtemp;
                Rdatain>>xtemp;
                Rdatain>>ytemp;
                Rdatain>>ztemp;
                R[i]= sqrt(xtemp*xtemp + ytemp*ytemp + ztemp*ztemp);
                cout<< R[i] << endl;
            }
            for(int ii=0;ii<CalibPosNum;ii += 1) { 

                double r3 = R[ii]*R[ii]*R[ii]/1.e9;
                //if(r3>5300.) continue;
                //int rBin =  int(r3/50.);
                int rBin = 0;
                if(r3<1000.) rBin = 0;
                else if(r3<2000.) rBin = 1;
                else if(r3<2500.) rBin = 2;
                else if(r3<3000.) rBin = 3;
                else if(r3<3500.) rBin = 4;
                else if(r3<3700.) rBin = 5;
                else if(r3<3900.) rBin = 6;
                else if(r3<3950.) rBin = 7;
                else if(r3<4000.) rBin = 8;
                else if(r3<4050.) rBin = 9;
                else if(r3<4100.) rBin = 10;
                else if(r3<4150.) rBin = 11;
                else if(r3<4200.) rBin = 12;
                else if(r3<4250.) rBin = 13;
                else if(r3<4300.) rBin = 14;
                else if(r3<4350.) rBin = 15;
                else if(r3<4400.) rBin = 16;
                else if(r3<4450.) rBin = 17;
                else if(r3<4500.) rBin = 18;
                else if(r3<4550.) rBin = 19;
                else if(r3<4600.) rBin = 20;
                else if(r3<4650.) rBin = 21;
                else if(r3<4700.) rBin = 22;
                else if(r3<4750.) rBin = 23;
                else if(r3<4800.) rBin = 24;
                else if(r3<4850.) rBin = 25;
                else if(r3<4900.) rBin = 26;
                else if(r3<4950.) rBin = 27;
                else if(r3<5000.) rBin = 28;
                else if(r3<5050.) rBin = 29;
                else if(r3<5100.) rBin = 30;
                else if(r3<5150.) rBin = 31;
                else if(r3<5200.) rBin = 32;
                else if(r3<5250.) rBin = 33;
                else if(r3<5300.) rBin = 34;
                if(r3>5300.) continue;


                std::cout<<source[src]<< '\t' << pmttype[pmt] << '\t' <<  ii<<'\t' << R[ii] << '\t' <<  rBin << std::endl;

                TimePdfIn.open(Form("%s/%s/T%s/T%s_pos%d.txt",  datapath.c_str(), source[src].c_str(), pmttype[pmt].c_str()), pmttype[pmt].c_str()), ii, std::ios::in);
                if(!TimePdfIn) {
                    cout<<Form("pos%d/T%s.txt does not exits!", ii, pmttype[pmt].c_str())<<endl;
                    continue;
                }

                double temp;
                for(int qk=0;qk<qBinN;qk++) {

                    for(int dk=0;dk<dBinN;dk++) {
                        
                        for(int tk=0;tk<tBinN;tk++) {

                            TimePdfIn>>temp; 
                            if(qk<qBinN) BinTotalRec[rBin][qk][dk] += temp; 
                            TimePdfIn>>temp; 
                            if(qk<qBinN) *(RECvsTRUTHRec + rBin*qBinN*dBinN*tBinN  + qk*dBinN*tBinN + dk*tBinN + tk) += temp;

                        }

                    }

                }
                TimePdfIn.close();

            }
            
            for(int rk=0;rk<rBinN;rk++) {
                for(int qk=0;qk<qBinN;qk++) {
                    for(int dk=0;dk<dBinN;dk++) {
                        for(int tk=0;tk<tBinN;tk++) {
                            if(BinTotalRec[rk][qk][dk]>1e-8) 
                                RsdTimeFuncRec[rk][qk]->SetBinContent(dk+1, tk+1, *(RECvsTRUTHRec + rk*qBinN*dBinN*tBinN  + qk*dBinN*tBinN + dk*tBinN + tk)*double(tBinN)/BinTotalRec[rk][qk][dk]);
                        }
                    }
                }
            }
            free(RECvsTRUTHRec);
            TFile* PerfmFile = TFile::Open(Form("TimePdfFile_%s_%s.root", source[src].c_str(), pmttype[pmt].c_str()), "RECREATE");

            /////////
            // Title 
            for(int rk=0;rk<rBinN;rk++) {
                for(int i=0;i<qBinN;i++) {

                    RsdTimeFuncRec[rk][i]->SetLineWidth(0);
                    RsdTimeFuncRec[rk][i]->GetXaxis()->SetTitle("d [m]");
                    RsdTimeFuncRec[rk][i]->GetYaxis()->SetTitle("t [ns]");
                    RsdTimeFuncRec[rk][i]->GetZaxis()->SetTitle("T");
                    RsdTimeFuncRec[rk][i]->GetXaxis()->SetLabelSize(0.05);
                    RsdTimeFuncRec[rk][i]->GetYaxis()->SetLabelSize(0.05);
                    RsdTimeFuncRec[rk][i]->GetZaxis()->SetLabelSize(0.05);
                    RsdTimeFuncRec[rk][i]->GetXaxis()->SetTitleSize(0.05);
                    RsdTimeFuncRec[rk][i]->GetYaxis()->SetTitleSize(0.05);
                    RsdTimeFuncRec[rk][i]->GetZaxis()->SetTitleSize(0.05);
                    RsdTimeFuncRec[rk][i]->GetXaxis()->SetTitleOffset(1.6);
                    RsdTimeFuncRec[rk][i]->GetYaxis()->SetTitleOffset(1.6);
                    RsdTimeFuncRec[rk][i]->Write();

                    delete RsdTimeFuncRec[rk][i];

                }
            }
        }
    }
}
