/**************************************
 * Study AHDC waveform
 * 
 * @author ftouchte
 * @date September 05, 2025
 * **********************************/

#include "gui.h"
#include "fAxis.h"
#include "fCanvas.h"
#include "fColorPalette.h"
#include "AhdcMapping.h"

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <functional>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "TString.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TFile.h"

/**
 * @brief Convert (sector, layer, component) to a unqiue wire id (number betwwen 0 and 575)
 * 
 * @param sector (not used)
 * @param layer 
 * @param component 
 * @return unique wire id
 */
int slc2wire(int sector, int layer, int component) {
    if (layer == 11) {
        return component - 1;
    } 
    else if (layer == 21) {
        return 47 + component - 1;
    } 
    else if (layer == 22) {
        return 47 + 56 + component - 1;
    } 
    else if (layer == 31) {
        return 47 + 56 + 56 + component - 1;
    } 
    else if (layer == 32) {
        return 47 + 56 + 56 + 72 + component - 1;
    } 
    else if (layer == 41) {
        return 47 + 56 + 56 + 72 + 72 + component - 1;
    } 
    else if (layer == 42) {
        return 47 + 56 + 56 + 72 + 72 + 87 + component - 1;
    } 
    else if (layer == 51) {
        return 47 + 56 + 56 + 72 + 72 + 87 + 87 + component - 1;
    } else {
        return -1; // not a ahdc wire
    }
}

int layer2number(int digit) {
    if      (digit == 11) {
        return 1;
    } 
    else if (digit == 21) {
        return 2;
    } 
    else if (digit == 22) {
        return 3;
    } 
    else if (digit == 31) {
        return 4;
    } 
    else if (digit == 32) {
        return 5;
    } 
    else if (digit == 41) {
        return 6;
    } 
    else if (digit == 42) {
        return 7;
    } 
    else if (digit == 51) {
        return 8;
    } else {
        return 0; // not a layer, can encode all layers
    }
}

TGraph* getZPhiLine(AhdcWire* wire, const char * name) {
    double ztop = wire->top.z;
    double zbot = wire->bot.z;
    int N = 100;
    TGraph* gr = new TGraph(N+1);
    gr->SetName(name);
    for (int n = 0; n <= N; n++) {
        double z = ztop + (zbot-ztop)*n/N;
        wire->set_z(z);
        double x = wire->x;
        double y = wire->y;
        double phi = atan2(y,x);
        if (phi < 0) phi += 2*M_PI;
        phi = phi*180/M_PI;
        gr->SetPoint(n, z, phi);
    }
    // reset z pos
    wire->set_z(ztop);
    // return graph
    return gr;
}

int main (int argc, char * argv[]) {
	
    AhdcDetector* ahdc = new AhdcDetector();

    AhdcWire* wire = ahdc->GetSector(0)->GetSuperLayer(0)->GetLayer(0)->GetWire(20);

    AhdcLayer* layer = ahdc->GetSector(0)->GetSuperLayer(1)->GetLayer(0);

    TCanvas* c = new TCanvas();

    TMultiGraph* mg = new TMultiGraph();

    TGraph * gr0 = getZPhiLine(wire, "L11W20");
    gr0->SetLineColor(kRed);
    mg->Add(gr0, "l");

    for (int i = 0; i < layer->GetNumberOfWires(); i++) {
        AhdcWire* w = layer->GetWire(i);
        TGraph * gr = getZPhiLine(w, TString::Format("L%2dW%2d", 21, i+1));
        mg->Add(gr, "l");
    }

    mg->Draw("apl");

    TFile* file = new TFile("./output/wf-study.root", "RECREATE");
    c->Write("phi-versus-z");

    file->Close();

    printf("Task finished : file ./output/wf-study.root created\n");


	return 0;
}