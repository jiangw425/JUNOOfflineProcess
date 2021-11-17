#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper

def get_parser():

    import argparse

    parser = argparse.ArgumentParser(description='Run Atmospheric Simulation.')
    #parser.add_argument("--user-output", default="sample_rec_user.root", help="output user file name")
    parser.add_argument("--evtmax", type=int, default=1, help='events to be processed')
    parser.add_argument("--JOBA", type=int, default=0, help='JOBA')
    parser.add_argument("--JOBB", type=int, default=1, help='JOBB')
    parser.add_argument("--runmode", type=int, default=1, help='runmode')
    parser.add_argument("--peRange", type=int, default=1, help='pe range')
    parser.add_argument('--enableHighMuFit', dest='enableHighMuFit', action='store_true')
    parser.add_argument('--enableUserSPEs', dest='enableUserSPEs', action='store_true')
    parser.add_argument("--userSPEFile", default="",help="")
    parser.add_argument("--outdir", default="Test",help="")


    return parser

parser = get_parser()
args = parser.parse_args()

task = Sniper.Task("rectask")
#task.asTop()
task.setEvtMax(args.evtmax)
task.setLogLevel(0)

# == Create Data Buffer Svc ==
import DataRegistritionSvc
drs = task.createSvc("DataRegistritionSvc")
drs.property("EventToPath").set({
        #"JM::SimEvent": "/Event/Sim"
    })

import BufferMemMgr
bufMgr = task.createSvc("BufferMemMgr")
bufMgr.property("TimeWindow").set([-0.01, 0.01])

import PMTCalibSvc
PMTCalibSvc = task.createSvc("PMTCalibSvc")

import os
Sniper.loadDll("../amd64_linux26/libGenQPDF.so")
#Sniper.loadDll("/afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/GenCalibPDFVertexAndEnergy/J21v1r0-Pre0/GenQPDF_v2/amd64_linux26/libGenQPDF.so")
#Sniper.loadDll("/junofs/users/huanggh/EnergyRec/GhRec/GenQPDF_batch/amd64_linux26/libGenQPDF.so")
alg = task.createAlg("GenQPDF")
alg.property("jobA").set(args.JOBA)
alg.property("jobB").set(args.JOBB)
alg.property("runmode").set(args.runmode)
alg.property("peRange").set(args.peRange)
alg.property("enableHighMuFit").set(args.enableHighMuFit)
alg.property("enableUserSPEs").set(args.enableUserSPEs)
alg.property("userSPEFile").set(args.userSPEFile)
alg.property("outdir").set(args.outdir)
alg.setLogLevel(2)


task.show()
task.run()
