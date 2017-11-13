/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. */
/* See cxx source for full Copyright notice */
/* $Id$ */

#ifndef AliAnalysisTaskMyTask_H
#define AliAnalysisTaskMyTask_H

#include "AliAnalysisTaskSE.h"

class AliAnalysisTaskMyTask : public AliAnalysisTaskSE  
{
    public:
                                AliAnalysisTaskMyTask();
                                AliAnalysisTaskMyTask(const char *name);
        virtual                 ~AliAnalysisTaskMyTask();

        // these functions also exist in AliAnalysisTaskSE (SE=single event)
        // as we want to change the functions a litte, but dont want to change the 
        // original ones, we make them 'virtual' functions
        virtual void            UserCreateOutputObjects();
        virtual void            UserExec(Option_t* option);
        virtual void            Terminate(Option_t* option);

    private:
        AliAODEvent*            fAOD;           //! input event
        TList*                  fOutputList;    //! output list
        TH1F*                   fHistPt;        //! pt histogram
        TH1F*                   fHistEta;       //! eta 
        TH1F*                   fHistPrimVertx; //! primary vertex

        // tracking branches
        TTree*                  fTrackTree;     //! tracking Tree
        Double_t                fEta;
        Double_t                fPhi;
        Double_t                fPt;
        Double_t                fTPCsignal;
        Double_t                fITSsignal;
        Double_t                fTOFsignal;
        Double_t                fTRDsignal;
        Double_t                fHMPsignal;
        Double_t                fPrimVertx;
        Int_t                   fPdg;
        Double_t                fVx;
        Double_t                fVy;
        Double_t                fVz;
        Double_t                fNbTracks;
        // emcal tree
        TTree*                  fEMCalTree;
        Short_t                 fEMPos;
        Double_t                fEMCalAmp;
        // V0 tree
        TTree*                  fV0Tree;
        Short_t                 fV0Pos;
        Double_t                fV0Amp;
        // FMD tree
        TTree*                  fFMDTree;
        Short_t                 fFMDPos;
        Double_t                fFMDAmp;
        // AD tree
        TTree*                  fADTree;
        Short_t                 fADPos;
        Double_t                fADAmp;
        // not implemented but neccessary
        AliAnalysisTaskMyTask(const AliAnalysisTaskMyTask&); 
        AliAnalysisTaskMyTask& operator=(const AliAnalysisTaskMyTask&); 

        ClassDef(AliAnalysisTaskMyTask, 1);
};

#endif
