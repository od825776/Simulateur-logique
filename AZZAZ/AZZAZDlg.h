#pragma once
#include <vector>
#include <string>
#include <map>
#include "afxwin.h"
#include <gdiplus.h>
#include <shlwapi.h>
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "shlwapi.lib")

using namespace Gdiplus;

// Structure des signaux temporels
struct TimeStep {
    bool X;
    bool Y;
    bool Z;
    int duration;
};

class Node;

// Boîte de dialogue principale
class CAZZAZDlg : public CDialogEx
{
public:
    CAZZAZDlg(CWnd* pParent = nullptr);
    virtual ~CAZZAZDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_AZZAZ_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

    HICON m_hIcon;

private:
    // Chronogramme
    std::vector<TimeStep> timeline;
    std::vector<bool> Xbits, Ybits, Zbits, Sbits;

    // Arbre logique
    Node* arbreGlobal = nullptr;

    // GDI+
    ULONG_PTR gdiplusToken; // token pour GDI+

    // Fonctions utilitaires
    void LoadSignalFile(const CString& filePath);
    void DrawChronogram(CDC* pDC, int x, int y,
        const std::vector<bool>& values,
        const std::vector<int>& durations,
        CString name);

    void DrawLogicTree(CDC* pDC, Node* node, int x, int y, int& nextX);

    // Gestion des événements
    afx_msg void OnBnClickedButtonSimuler();
    afx_msg void OnPaint();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg HCURSOR OnQueryDragIcon();

    DECLARE_MESSAGE_MAP()
};

// Fonction pour dessiner un PNG
void DrawGatePNG(CDC* pDC, int x, int y, LPCWSTR filePath, int width, int height);
