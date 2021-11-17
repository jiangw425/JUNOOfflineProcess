#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper


def get_parser():

    import argparse
    parser = argparse.ArgumentParser(description='Run Atmospheric Simulation.')
    parser.add_argument("--evtmax", type=int, default=1, help='events to be processed')
    parser.add_argument("--detoption", default="Acrylic",
                                       choices=["Acrylic", "Balloon", "TT"],
                                       help="Det Option")
    parser.add_argument("--JOBA", type=int, default=0, help='JOBA')
    parser.add_argument("--JOBB", type=int, default=1, help='JOBB')
    parser.add_argument("--RfrIndxLS", type=float, default=1.5, help='RfrIndxLS')
    parser.add_argument("--RfrIndxWR", type=float, default=1.355, help='RfrIndxWR')
    parser.add_argument("--PMTtype", type=int, default=0, help='PMTtype')
    parser.add_argument("--filenum", type=int, default=10, help='Executing file number')
    parser.add_argument("--startseed", type=int, default=50000, help='Start seed')
    parser.add_argument("--enableAmC", action="store_true", help='enable AmC mode')
    parser.set_defaults(enableAmC=False)
    parser.add_argument("--SignalWindow", type=int, default=300, help='SignalWindow')
    parser.add_argument("--calibdir",  default="/junofs/users/zhangxt/20inch/rec/deconvolution/runs/20200504sample-J20v1r0-Pre0/phy/CLS/AmC", help='Dir of MapData')
    parser.add_argument("--usercalibdir",  default="/junofs/production/data-production/Pre-Releases/J20v2r0-Pre0/ACU+CLS/AmC", help='Dir of MapData')
    parser.add_argument("--posdir",  default="/junofs/users/huanggh/Data/ACU_CLS_3D_Pos", help='Dir of calibration position')
    parser.add_argument("--paradir",  default="/junofs/users/jiangw/give2lxj", help='Dir of calibration parameters')
    parser.add_argument("--calibsource",  default="AmC", help='Calibration source')
    parser.add_argument("--gdml", default="True", action="store_true", help="Use GDML.")
    parser.add_argument("--gdml-file", default="/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre0/offline/Examples/Tutorial/share/sample_detsim.root",
                                       help="the file contains the geometry info.")
    return parser


DEFAULT_GDML_OUTPUT = {"Acrylic": "sample_detsim.root",  #"geometry_acrylic.gdml"
                       "Balloon": "sample_detsim.root",
                       "TT": "sample_detsim.root",
                      }
parser = get_parser()
args = parser.parse_args()
    #geom_path_inroot = "JunoGeom"
    #if args.gdml:
    #    if args.gdml_file:
    #        gdml_filename = args.gdml_file
    #if gdml_filename.endswith("gdml"):
    #    geom_path_inroot = ""
    # === check the existance of the geometry file ===

gdml_filename = DEFAULT_GDML_OUTPUT[args.detoption]
geom_path_inroot = "JunoGeom"
if args.gdml:
    if args.gdml_file:
        gdml_filename = args.gdml_file
if gdml_filename.endswith("gdml"):
    geom_path_inroot = ""
# === check the existance of the geometry file ===
import os.path
if not gdml_filename.startswith("root://") and not os.path.exists(gdml_filename):
    import sys
    print("can't find the geometry file %s"%gdml_filename)
    sys.exit(-1)

task = Sniper.Task("CalibPDFtask")
#task.asTop()
task.setEvtMax(args.evtmax)
task.setLogLevel(0)

import BufferMemMgr
bufMgr = task.createSvc("BufferMemMgr")
#bufMgr.property("TimeWindow").set([-0.01, 0.01])

# == Create IO Svc ==
#import RootIOSvc
#inputsvc = task.createSvc("RootInputSvc/InputSvc")
#inputsvc.property("InputFile").set([args.input])

import Geometry
geom = task.createSvc("RecGeomSvc")
geom.property("GeomFile").set(gdml_filename)
geom.property("GeomPathInRoot").set(geom_path_inroot)
geom.property("FastInit").set(True)

import PMTCalibSvc
PMTCalibSvc = task.createSvc("PMTCalibSvc")    

# == Then use OMILREC to rec. the energy
#Sniper.loadDll("/afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/GenCalibPDFVertexAndEnergy/J20v2r0-Pre2_phy_zxt_decon/amd64_linux26/libGenCalibPDF.so")
Sniper.loadDll("../../amd64_linux26/libGenCalibPDF.so")
#import OMILREC
alg = task.createAlg("GenCalibPDF")

alg.property("jobA").set(args.JOBA)
alg.property("jobB").set(args.JOBB)
alg.property("RfrIndxLS").set(args.RfrIndxLS)
alg.property("RfrIndxWR").set(args.RfrIndxWR)
alg.property("PMTtype").set(args.PMTtype)
alg.property("filenum").set(args.filenum)
alg.property("startseed").set(args.startseed)
alg.property("SignalWindow").set(args.SignalWindow)
alg.property("enableAmC").set(args.enableAmC)
alg.property("calibdir").set(args.calibdir)
alg.property("usercalibdir").set(args.usercalibdir)
alg.property("posdir").set(args.posdir)
alg.property("paradir").set(args.paradir)
alg.property("calibsource").set(args.calibsource)
alg.setLogLevel(2)

task.show()
task.run()
