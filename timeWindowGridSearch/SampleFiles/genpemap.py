
import ROOT
from ROOT import TTree, TFile, gROOT
from ROOT import TH2D, TH1D, TGraph, TGraph2D

ROOT.gROOT.Reset()

import time
import math 
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import seaborn as sns
import matplotlib.cm as cm
from scipy.interpolate import griddata

TotalPosNum = 311
CalibPosNum = 293
BadPoNum = 18

RBinN = 1770
TBinN = 18
PTBinN = 1440

RRange = 17700.
TRange = 180
PTRange = 180.


r = np.arange(0, RRange, RRange/RBinN)
theta = np.arange(0, TRange, TRange/TBinN)
grid_r, grid_theta = np.meshgrid(r, theta)



data = np.loadtxt("/junofs/users/huanggh/Data/ACU_CLS_3D_Pos/ACU-CLS-GT_XYZPos.txt")
badPos = np.loadtxt("/junofs/users/huanggh/EnergyRec/GenCalibData/ACU_CLS_MAP/nPEMap/J20v2r0-Pre0_phy_zxt_decon/share/LackPos.txt")

positionX = data[0:CalibPosNum, 1]
positionZ = data[0:CalibPosNum, 3]

positionX = np.array(positionX, dtype=float)
positionZ = np.array(positionZ, dtype=float)

EvtR = np.sqrt(positionX*positionX + positionZ*positionZ)

EvtTheta = np.arccos(positionZ/EvtR)*180./math.pi
EvtTheta[0] = 0.


EvtR = np.insert(EvtR, 0, 0)
EvtTheta = np.insert(EvtTheta, 1, 180)

#print(EvtR, EvtTheta)


LMu2D = np.zeros(shape=(PTBinN, CalibPosNum+1))
SMu2D = np.zeros(shape=(PTBinN, CalibPosNum+1))

s_LMu2D = np.zeros(shape=(CalibPosNum+1, PTBinN))
s_SMu2D = np.zeros(shape=(CalibPosNum+1, PTBinN))



j=-1
for jj in range(TotalPosNum):

    IsBadPos=False
    
    for jjj in range(BadPoNum):
        if jj==badPos[jjj]:
            print("BadPos:", badPos[jjj])
            IsBadPos=True
    if IsBadPos:
        print("Skip badPos")
        continue
    j = j + 1
    #f = TFile("/junofs/users/huanggh/EnergyRec/GenCalibData/ACU_CLS_MAP/nPEMap/J20v2r0-Pre2_phy_zxt_decon/share/Ge68/Dyn/PE-theta-distribution-%d.root"%(jj), "READ");
    f = TFile("../root/PE-theta-distribution-%d.root"%(jj), "READ");
    LGrTemp = f.Get("RecGraph");
    SGrTemp = f.Get("RecNSPMTGraph");
    print(j, LGrTemp.Eval(90.), SGrTemp.Eval(90.))
    
    
    LSmoothH = TH1D("LSmoothH%d"%(j), "LSmoothH%d"%(j), PTBinN + 1, 0.-0.125/2., PTRange + 0.125/2.);
    SSmoothH = TH1D("SSmoothH%d"%(j), "SSmoothH%d"%(j), PTBinN + 1, 0.-0.125/2., PTRange + 0.125/2.);

    for k in range(PTBinN):
        theta = k*0.125
        s_LMu2D[j][k] = LGrTemp.Eval(theta)
        s_SMu2D[j][k] = SGrTemp.Eval(theta)

        LSmoothH.SetBinContent(k+1, LGrTemp.Eval(theta))
        SSmoothH.SetBinContent(k+1, SGrTemp.Eval(theta))
    
    LSmoothH.Smooth()
    SSmoothH.Smooth()

    for k in range(PTBinN):
        theta = k*0.125
        #LPE = LGrTemp.Eval(theta);
        #SPE = SGrTemp.Eval(theta);
        LPE = LSmoothH.Interpolate(theta)
        SPE = SSmoothH.Interpolate(theta)
        if LPE<0.:
            npoint = LGrTemp.GetN();
            if theta<90.:
                LPE = LGrTemp.GetY()[0];
            else :
                LPE = LGrTemp.GetY()[npoint-1];
                
        if SPE<0.:
            npoint = SGrTemp.GetN();
            if theta<90.:
                SPE = SGrTemp.GetY()[0];
            else :
                SPE = SGrTemp.GetY()[npoint-1];

        LMu2D[k][j] = LPE;
        SMu2D[k][j] = SPE;


for j in range(CalibPosNum, 0, -1):
    for k in range(PTBinN):
        LMu2D[k][j] = LMu2D[k][j-1]
        SMu2D[k][j] = SMu2D[k][j-1]    
#np.insert(LMu2D, 1, LMu2D[:,0], axis=1)


RTheta = np.array((EvtR, EvtTheta))
RTheta = np.transpose(RTheta)
np.savetxt("rt.txt", RTheta)
np.savetxt("lmu_SOURCE.txt", LMu2D)
np.savetxt("smu_SOURCE.txt", SMu2D)


## Interpolation using python
quit()

LMu2DRT = TFile("LnPEMapFile_Test.root", "RECREATE");
for i in range(139, 140):

    grid_mu = griddata((EvtR, EvtTheta), LMu2D[i], (grid_r, grid_theta), method='cubic')
    
    
    np.savetxt("grid_mu.txt", grid_mu)
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    #ax.plot_surface(grid_r, grid_theta, grid_mu, rstride=1, cstride=1, cmap=cm.coolwarm)
    ax.plot_surface(grid_r, grid_theta, grid_mu)
    ax.set_xlabel('x')
    ax.set_ylabel('y')

    ax.set_xlim(0, RRange)
    ax.set_ylim(0, TRange)

    #plt.colorbar(, shrink=0.5, aspect=5)

    plt.show()

    #LGrMu2D = TGraph2D(RBinN*TBinN, grid_r.reshape(RBinN*TBinN), grid_theta.reshape(RBinN*TBinN), grid_mu);
    #LGrMu2D.SetName("LGrMu2D_%d"%(i));
    #LGrMu2D.Write();



 
