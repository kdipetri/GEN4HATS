# 

#!/usr/bin/env python

import os,sys
from string import *
from ROOT import *

def draw(h):

    hmax = 0
    for i in range(len(h)):
        h[i].SetStats(0)
        h[i].Sumw2()
        h[i].Scale(1./h[i].Integral())
        if h[i].GetMaximum() > hmax:
            hmax = h[i].GetMaximum()

    # Set line and fill style for each histogram:
    h[0].SetLineColor(kBlue)
    h[1].SetLineColor(kRed)

    hmax = hmax*1.3

    for i in range(len(h)):
    # Set attributes of the 1st histogram in the list
        h[i].SetMaximum(hmax)
        h[i].SetMinimum(0)
        h[i].SetTitle("")
        h[i].GetXaxis().SetTitle(h[i].GetName())
        h[i].GetXaxis().SetTitleOffset(1.25)
        h[i].GetXaxis().SetTitleSize(0.055)
        h[i].GetXaxis().SetTitleFont(42)
        h[i].GetXaxis().SetLabelOffset(0.012)
        h[i].GetXaxis().SetLabelSize(0.050)
        h[i].GetXaxis().SetLabelFont(42)
        #h[i].GetXaxis().SetNdivisions(6, 5, 0)
        h[i].GetYaxis().SetTitle("events (normalized to 1)")
        h[i].GetYaxis().SetTitleOffset(0.62)
        h[i].GetYaxis().SetTitleSize(0.055)
        h[i].GetYaxis().SetTitleFont(42)
        h[i].GetYaxis().SetNdivisions(0, 0, 0)
        # Draw
        if i == 0:
            h[i].Draw()
        else:
            h[i].Draw('same')

    # Make the histogram legend
    lxmn = 0.53; lxmx = 0.88; lymn = 0.75; lymx = 0.87
    l = TLegend(lxmn, lymn, lxmx, lymx)
    l.SetBorderSize(0)
    l.SetFillStyle(0000)
    l.SetTextSize(0.042)
    l.SetTextFont(42)
    l.AddEntry(h[1],'Pythia', 'l')
    l.AddEntry(h[0],'MadGraph+Pyth', 'l')
    l.Draw("same")
    # Make the legend visible (annoying pyROOT-specific feature)
    SetOwnership(l, 0 )

    # Kolmogorov-Smirnov test
    ks = h[0].KolmogorovTest(h[1])
    t = TLatex(0.13, 0.82, "KS: "+str(ks))
    t.SetTextSize(0.042)
    t.SetTextFont(42)
    t.SetNDC()
    t.Draw("same")
    SetOwnership(t, 0 )


def main():

    if len(sys.argv) < 3:
        print 'run as compare.py <file1> <file2>'
        sys.exit(0)

    # Open the files
    f = []
    f.append(TFile(sys.argv[1]))
    f.append(TFile(sys.argv[2]))

    # Make a canvas
    c = TCanvas('c', 'c', 450, 440)
    c.SetBottomMargin(0.15)
    # Get the histograms
    keys = f[0].GetListOfKeys()
    print keys
    for k1 in keys:
        k1name = k1.GetName()
        keys2 = f[0].Get(k1name).GetListOfKeys()
        for k2 in keys2:
            h = []
            k2name = k2.GetName()
            for i in range(len(f)):
                h.append(f[i].Get(k1name).Get(k2name))
            # Draw and print the histograms
            draw(h)
            c.Print('compare.ps(')

    c.Print('compare.ps)')

main()

