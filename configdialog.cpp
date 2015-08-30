/*
 *  configdialog.cpp
 *  PHD Guiding
 *
 *  Created by Bret McKee
 *  Copyright (c) 2012 Bret McKee
 *  All rights reserved.
 *
 *  This source code is distributed under the following "BSD" license
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *    Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *    Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *    Neither the name of Craig Stark, Stark Labs nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "phd.h"

ConfigDialogPane::ConfigDialogPane(const wxString& heading, wxWindow *pParent)
    : wxStaticBoxSizer(new wxStaticBox(pParent, wxID_ANY, heading), wxVERTICAL)
{
    m_pParent = pParent;
}

void ConfigDialogPane::DoAdd(wxSizer *pSizer)
{
    this->Add(pSizer, wxSizerFlags().Expand().Border(wxALL,3));
}

void ConfigDialogPane::DoAdd(wxWindow *pWindow)
{
    this->Add(pWindow, wxSizerFlags().Expand().Border(wxALL,3));
}

void ConfigDialogPane::DoAdd(wxWindow *pWindow, const wxString& toolTip)
{
    pWindow->SetToolTip(toolTip);
    DoAdd(pWindow);
}

wxSizer *ConfigDialogPane::MakeLabeledControl(const wxString& label, wxWindow *pControl, const wxString& toolTip, wxWindow *pControl2)
{
    wxStaticText *pLabel = new wxStaticText(m_pParent, wxID_ANY, label + _(": "));
    pControl->SetToolTip(toolTip);

    wxBoxSizer *pSizer = new wxBoxSizer(wxHORIZONTAL);
    pSizer->Add(pLabel, wxSizerFlags().Align(wxALIGN_CENTER_VERTICAL));
    pSizer->Add(pControl, wxSizerFlags().Align(wxALIGN_CENTER_VERTICAL));
    if (pControl2)
        pSizer->Add(pControl2, wxSizerFlags().Align(wxALIGN_CENTER_VERTICAL));

    return pSizer;
}

void ConfigDialogPane::DoAdd(const wxString& label, wxWindow *pControl, const wxString& toolTip, wxWindow *pControl2)
{
    DoAdd(MakeLabeledControl(label, pControl, toolTip, pControl2));
}

int ConfigDialogPane::StringWidth(const wxString& string)
{
    int width, height;

    m_pParent->GetTextExtent(string, &width, &height);

    return width;
}

int ConfigDialogPane::StringArrayWidth(wxString string[], int nElements)
{
    int width = 0;

    for(int i=0;i<nElements;i++)
    {
        int thisWidth = StringWidth(string[i]);

        if (thisWidth > width)
        {
            width = thisWidth;
        }
    }

    return width;
}

// Default implementation does nothing because most config dialogs don't need an 'undo' function - simply not calling 'Unload' prevents any pending changes from
// being saved.  But if non-scalar objects are involved - as in guide algorithms - a specific Undo may be required
void ConfigDialogPane::Undo(void)
{
}

wxWindow* ConfigDialogPane::GetSingleCtrl(std::map <BRAIN_CTRL_IDS, BrainCtrlInfo> & CtrlMap, BRAIN_CTRL_IDS id)
{
    wxWindow* ctrl = NULL;
    try
    {
        ctrl = (wxWindow*)(CtrlMap.at(id).panelCtrl);
        if (ctrl != NULL)
            CtrlMap[id].isPositioned = true;
    }
    catch (...)
    {

    }
    return (ctrl);           // May return null, won't add entry in CtrlMap
}

wxSizer* ConfigDialogPane::GetSizerCtrl(std::map <BRAIN_CTRL_IDS, BrainCtrlInfo> & CtrlMap, BRAIN_CTRL_IDS id)
{
    wxSizer* ctrl = (wxSizer*)(CtrlMap.at(id).panelCtrl);
    if (ctrl != NULL)
        CtrlMap[id].isPositioned = true;
    return (ctrl);           // May return null, won't add entry in CtrlMap
}

ConfigDialogCtrlSet::ConfigDialogCtrlSet(wxWindow *pParent, AdvancedDialog* pAdvancedDialog, std::map <BRAIN_CTRL_IDS, BrainCtrlInfo>& CtrlMap)
{
    m_pParent = pParent;
    m_pAdvDlg = pAdvancedDialog;
}

int ConfigDialogCtrlSet::StringWidth(const wxString& string)
{
    int width, height;

    m_pParent->GetTextExtent(string, &width, &height);

    return width;
}

int ConfigDialogCtrlSet::StringArrayWidth(wxString string[], int nElements)
{
    int width = 0;

    for (int i = 0; i<nElements; i++)
    {
        int thisWidth = StringWidth(string[i]);

        if (thisWidth > width)
        {
            width = thisWidth;
        }
    }

    return width;
}

wxWindow* ConfigDialogCtrlSet::GetParentWindow(BRAIN_CTRL_IDS id) 
{ 
    return m_pAdvDlg->GetTabLocation(id); 
}

// Base method for adding map elements
void ConfigDialogCtrlSet::AddMapElement(std::map <BRAIN_CTRL_IDS, BrainCtrlInfo> & CtrlMap, BRAIN_CTRL_IDS id, wxObject *pElem)
{
    CtrlMap[id] = BrainCtrlInfo(id, pElem);
}

// Simple control
void ConfigDialogCtrlSet::AddCtrl(std::map <BRAIN_CTRL_IDS, BrainCtrlInfo> & CtrlMap, BRAIN_CTRL_IDS id, wxControl *pCtrl)
{
    AddMapElement(CtrlMap, id, pCtrl);
}
// Control with tooltip
void ConfigDialogCtrlSet::AddCtrl(std::map <BRAIN_CTRL_IDS, BrainCtrlInfo> & CtrlMap, BRAIN_CTRL_IDS id, wxControl *pCtrl, const wxString& toolTip)
{
    pCtrl->SetToolTip(toolTip);
    AddMapElement(CtrlMap, id, pCtrl);
}
// Control group via sizer
void ConfigDialogCtrlSet::AddGroup(std::map <BRAIN_CTRL_IDS, BrainCtrlInfo> & CtrlMap, BRAIN_CTRL_IDS id, wxSizer *pSizer)
{
    AddMapElement(CtrlMap, id, pSizer);
}

wxSizer *ConfigDialogCtrlSet::MakeLabeledControl(BRAIN_CTRL_IDS id, const wxString& label, wxWindow *pControl, const wxString& toolTip)
{
    wxStaticText *pLabel = new wxStaticText(GetParentWindow(id), wxID_ANY, label + _(": "));
    pControl->SetToolTip(toolTip);

    wxBoxSizer *pSizer = new wxBoxSizer(wxHORIZONTAL);
    pSizer->Add(pLabel, wxSizerFlags().Align(wxALIGN_CENTER_VERTICAL));
    pSizer->Add(pControl, wxSizerFlags().Align(wxALIGN_CENTER_VERTICAL));

    return pSizer;
}
void ConfigDialogCtrlSet::AddLabeledCtrl(std::map <BRAIN_CTRL_IDS, BrainCtrlInfo> & CtrlMap, BRAIN_CTRL_IDS id, const wxString& Label, wxControl *pCtrl, const wxString& toolTip)
{
    AddGroup(CtrlMap, id, MakeLabeledControl(id, Label, pCtrl, toolTip));
}
