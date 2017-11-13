/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* AliAnaysisTaskMyTask
 *
 * empty task which can serve as a starting point for building an analysis
 * as an example, one histogram is filled
 */

#include "TChain.h"
#include "TH1F.h"
#include "TList.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliAnalysisTaskMyTask.h"

class AliAnalysisTaskMyTask;    // your analysis class

using namespace std;            // std namespace: so you can do things like 'cout'

ClassImp(AliAnalysisTaskMyTask) // classimp: necessary for root

AliAnalysisTaskMyTask::AliAnalysisTaskMyTask() : AliAnalysisTaskSE(), 
    // we initilize all private mem-vars pointing to 0, so it does not
    // get randomly allocated
    fAOD(0), fOutputList(0), fHistPt(0), fHistPrimVertx(0),
    // Tracking tree
    fTrackTree(0), fEta(0), fPhi(0), fPt(0), fTPCsignal(0), fITSsignal(0), fTOFsignal(0), fPdg(0),
    fVx(0), fVy(0), fVz(0), fPrimVertx(0),
    // emcal tree
    fEMCalTree(0), fEMPos(0), fEMCalAmp(0),
    // V0 tree
    fV0Tree(0), fV0Pos(0), fV0Amp(0),
    // FMD tree
    fFMDTree(0), fFMDPos(0), fFMDAmp(0),
    // AD tree
    fADTree(0), fADPos(0), fADAmp(0),
{
    // default constructor, don't allocate memory here!
    // this is used by root for IO purposes, it needs to remain empty
}
//_____________________________________________________________________________
AliAnalysisTaskMyTask::AliAnalysisTaskMyTask(const char* name) : AliAnalysisTaskSE(name),
    fAOD(0), fOutputList(0), fHistPt(0), fHistPrimVertx(0), fHistCentr(0), fHistIsInRange(0)
{
    // constructor
    DefineInput(0, TChain::Class());    // define the input of the analysis: in this case we take a 'chain' of events
                                        // this chain is created by the analysis manager, so no need to worry about it, 
                                        // it does its work automatically
    DefineOutput(1, TList::Class());    // define the ouptut of the analysis: in this case it's a list of histograms 
                                        // you can add more output objects by calling DefineOutput(2, classname::Class())
                    
    // tracking tree
    DefineOutput(2, TTree::Class());    
    // EMCal tree
    DefineOutput(3, TTree::Class());    
    // V0 tree
    DefineOutput(4, TTree::Class());    
    // FMD tree
    DefineOutput(5, TTree::Class());    
    // AD tree
    DefineOutput(6, TTree::Class());    
}
//_____________________________________________________________________________
AliAnalysisTaskMyTask::~AliAnalysisTaskMyTask()
{
    // destructor
    if(fOutputList) {
        delete fOutputList;     // at the end of your task, it is deleted from memory by calling this function
    }
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::UserCreateOutputObjects()
{
    // create output objects
    //
    // this function is called ONCE at the start of your analysis (RUNTIME)
    // here you ceate the histograms that you want to use 
    //
    // the histograms are in this case added to a tlist, this list is in the end saved
    // to an output file
    //
    fOutputList = new TList();          // this is a list which will contain all of your histograms
                                        // at the end of the analysis, the contents of this list are written
                                        // to the output file
    fOutputList->SetOwner(kTRUE);       // memory stuff: the list is owner of all objects it contains and 
                                        // will delete them
                                        // if requested (dont worry about this now)

    fHistPt  = new TH1F("fHistPt",  "fHistPt",  100,   0, 10);       
    fHistEta = new TH1F("fHistEta", "fHistEta", 100, -15, 15);
    fHistPrimVertx = new TH1F("fHistPrimVertx", "fHistPrimVertx", 100, -7, 7);
    fOutputList->Add(fHistPt);          // don't forget to add it to the list! the list will be written to file, 
                                        // so if you want
                                        // your histogram in the output file, add it to the list!
    fOutputList->Add(fHistEta);

    fTrackTree = new TTree("particles", "particle tree");
    fTrackTree->Branch("fEta",          &fEta);
    fTrackTree->Branch("fPhi",          &fPhi);
    fTrackTree->Branch("fPt",           &fPt);
    fTrackTree->Branch("fTPCsignal",    &fTPCsignal);
    fTrackTree->Branch("fITSsignal",    &fITSsignal);
    fTrackTree->Branch("fTOFsignal",    &fTOFsignal);
    fTrackTree->Branch("fPrimVertx",    &fPrimVertx);
    fTrackTree->Branch("fVx",           &fVx);
    fTrackTree->Branch("fVy",           &fVy);
    fTrackTree->Branch("fVz",           &fVz);
    // emcal tree
    fEMCalTree = new TTree("emcalClusters", "EMCal cluster information");
    fEMCalTree->Branch("fEMCalAmp",     &fEMCalAmp);
    fEMCalTree->Branch("fEMPos",        &fEMpos);
    // V0 tree
    fV0Tree = new TTree("V0clusters", "V0 cluster information");
    fV0Tree->Branch("fV0Amp",           &fV0Amp);
    fV0Tree->Branch("fV0Pos",           &fV0pos);

    fFMDTree = new TTree("FMDclusters", "FMD cluster information");
    fFMDTree->Branch("fFMDAmp",         &fFMDAmp);
    fFMDTree->Branch("fFMDPos",         &fFMDpos);
    // AD tree
    fADTree = new TTree("FMDclusters", "FMD cluster information");
    fADTree->Branch("fADAmp",           &fADAmp);
    fADTree->Branch("fADPos",           &fADpos);

    PostData(1, fOutputList);           // postdata will notify the analysis manager of changes / updates to the 
                                        // fOutputList object. the manager will in the end 
                                        // take care of writing your output to file
                                        // so it needs to know what's in the output
    PostData(2, fTrackTree);
    PostData(3, fEMCalTree);
    PostData(4, fV0Tree);
    PostData(5, fFMDTree);
    PostData(6, fADTree);
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::UserExec(Option_t *)
{
    // user exec
    // this function is called once for each event
    // the manager will take care of reading the events from file, and with the static function InputEvent() you 
    // have access to the current event. 
    // once you return from the UserExec function, the manager will retrieve the next event from the chain
    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());    // get an event (called fAOD) from the input file
                                                        // there's another event format (ESD) which works in a similar wya
                                                        // but is more cpu/memory unfriendly. for now, we'll stick with aod's
    if(!fAOD) return;                                   // if the pointer to the event is empty (getting it failed) skip this event
    fHistPrimVertx->Fill( fAOD->GetPrimaryVertex()->GetZ() );
    Int_t iTracks(fAOD->GetNumberOfTracks());           
    for(Int_t i(0); i < iTracks; i++) {                 
        AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(i));  // get a track (type AliAODTrack) from the event
        if(!track || !track->TestFilterBit(1)) continue; // if we failed, skip this track
        fPt        = track->Pt();
        fEta       = track->Eta();
        fPhi       = track->Phi();
        fPdg       = track->PDG();
        fVx        = track->Xv();
        fVy        = track->Yv();
        fVz        = track->Zv();
        fTPCsignal = track->GetTPCsignal();
        fITSsignal = track->GetITSsignal();
        fTOFsignal = track->GetTOFsignal();
        fTRDsignal = track->GetTRDsignal();
        fHMPsignal = track->GetHMPIDsignal();
        fNbTracks  = iTracks;

        fHistPt->Fill(fPt);
        fTrackTree->Fill();
    }                                                   
    Int_t iCaloClusters(fAOD->GetNumberOfCaloClusters());
    for(Int_t i(0); i < iCaloClusters; i++){
        AliVCluster* cluster = static_cast<AliVCluster*>(fAOD->GetCaloCluster(i)); 
        Int_t iCells(cluster->GetNumberOfCells());
        for(Int_t j(0); j < iCells; j++){
            fEMCalInfo = cluster->GetCellAmplitude(j);
            fPos = cluster->GetCellPosition(j);
            fEMCal_tree->Fill();
        }
    
    }

    PostData(1, fOutputList);                           // stream the results the analysis of this event to
                                                        // the output manager which will take care of writing
                                                        // it to a file
    PostData(2, fTrackTree);
    PostData(3, fEMCalTree);
    PostData(4, fV0Tree);
    PostData(5, fFMDTree);
    PostData(6, fADTree);
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::Terminate(Option_t *)
{
    // terminate
    // called at the END of the analysis (when all events are processed)
}
//_____________________________________________________________________________
